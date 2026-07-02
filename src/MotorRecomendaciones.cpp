// MotorRecomendaciones.cpp
#include "MotorRecomendaciones.h"

#include <algorithm>
#include <future>
#include <set>
#include <thread>

namespace streaming {

MotorRecomendaciones::MotorRecomendaciones(const std::vector<Pelicula>& catalogo,
                                             std::shared_ptr<IEstrategiaSimilitud> estrategia,
                                             unsigned numHilos)
    : catalogo_(catalogo), estrategia_(std::move(estrategia)) {
    numHilos_ = (numHilos == 0) ? std::max(1u, std::thread::hardware_concurrency()) : numHilos;
}

void MotorRecomendaciones::alRecibirLike(IdPelicula id) {
    {
        std::lock_guard<std::mutex> lock(mutex_);
        if (std::find(likes_.begin(), likes_.end(), id) == likes_.end()) {
            likes_.push_back(id);
        }
    }
    recalcular();
}

void MotorRecomendaciones::recalcularDesde(const std::vector<IdPelicula>& likesActuales) {
    {
        std::lock_guard<std::mutex> lock(mutex_);
        likes_ = likesActuales;
    }
    recalcular();
}

void MotorRecomendaciones::recalcular() {
    std::vector<IdPelicula> likesCopia;
    {
        std::lock_guard<std::mutex> lock(mutex_);
        likesCopia = likes_;
    }

    if (likesCopia.empty()) {
        std::lock_guard<std::mutex> lock(mutex_);
        recomendacionesCache_.clear();
        return;
    }

    std::set<IdPelicula> likesSet(likesCopia.begin(), likesCopia.end());
    std::vector<const Pelicula*> peliculasConLike;
    for (IdPelicula id : likesCopia) {
        if (id >= 0 && static_cast<std::size_t>(id) < catalogo_.size()) {
            peliculasConLike.push_back(&catalogo_[static_cast<std::size_t>(id)]);
        }
    }

    using Candidato = std::pair<double, IdPelicula>; // <similitud, id>

    std::size_t total = catalogo_.size();
    std::size_t numHilos = std::min<std::size_t>(numHilos_, std::max<std::size_t>(1, total));
    std::size_t tamanoBloque = (total + numHilos - 1) / std::max<std::size_t>(1, numHilos);

    std::vector<std::future<std::vector<Candidato>>> futuros;

    for (std::size_t h = 0; h < numHilos; ++h) {
        std::size_t inicio = h * tamanoBloque;
        std::size_t fin = std::min(total, inicio + tamanoBloque);
        if (inicio >= fin) {
            break;
        }

        futuros.push_back(std::async(std::launch::async, [this, &peliculasConLike, &likesSet, inicio, fin]() {
            std::vector<Candidato> local;
            local.reserve(fin - inicio);

            for (std::size_t i = inicio; i < fin; ++i) {
                IdPelicula idCandidata = static_cast<IdPelicula>(i);
                if (likesSet.count(idCandidata) > 0) {
                    continue; // no recomendar algo que ya le gusto
                }

                double mejorSimilitud = 0.0;
                for (const Pelicula* base : peliculasConLike) {
                    double s = estrategia_->calcular(*base, catalogo_[i]);
                    mejorSimilitud = std::max(mejorSimilitud, s);
                }

                if (mejorSimilitud > 0.0) {
                    local.emplace_back(mejorSimilitud, idCandidata);
                }
            }

            // Top-K LOCAL de este bloque: solo viajan al hilo principal, como
            // maximo, MAX_RECOMENDACIONES candidatos por bloque (no la lista
            // completa), minimizando el costo de la fase de combinacion.
            std::size_t k = std::min(local.size(), MAX_RECOMENDACIONES);
            std::partial_sort(local.begin(), local.begin() + k, local.end(),
                               [](const Candidato& a, const Candidato& b) { return a.first > b.first; });
            if (local.size() > k) {
                local.resize(k);
            }
            return local;
        }));
    }

    std::vector<Candidato> combinado;
    for (auto& f : futuros) {
        std::vector<Candidato> parcial = f.get();
        combinado.insert(combinado.end(), parcial.begin(), parcial.end());
    }

    std::size_t kFinal = std::min(combinado.size(), MAX_RECOMENDACIONES);
    std::partial_sort(combinado.begin(), combinado.begin() + kFinal, combinado.end(),
                       [](const Candidato& a, const Candidato& b) { return a.first > b.first; });
    if (combinado.size() > kFinal) {
        combinado.resize(kFinal);
    }

    std::vector<IdPelicula> resultado;
    resultado.reserve(combinado.size());
    for (const auto& [similitud, id] : combinado) {
        resultado.push_back(id);
    }

    std::lock_guard<std::mutex> lock(mutex_);
    recomendacionesCache_ = std::move(resultado);
}

std::vector<IdPelicula> MotorRecomendaciones::obtenerRecomendaciones() const {
    std::lock_guard<std::mutex> lock(mutex_);
    return recomendacionesCache_;
}

}

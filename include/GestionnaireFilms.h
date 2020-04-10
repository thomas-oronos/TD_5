/// Gestionnaire de films.
/// \author Misha Krieger-Raynauld
/// \date 2020-01-12

#ifndef GESTIONNAIREFILMS_H
#define GESTIONNAIREFILMS_H

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>
#include "Film.h"

/// Classe qui gère les informations de tous les films et qui conserve des filtres pour les rechercher rapidement.
class GestionnaireFilms
{
public:
    // Fonctions membres spéciales
    GestionnaireFilms() = default;
    GestionnaireFilms(const GestionnaireFilms& other);
    GestionnaireFilms(GestionnaireFilms&&) = default;
    GestionnaireFilms& operator=(GestionnaireFilms other);

    // Surcharges d'opérateurs
    friend std::ostream& operator<<(std::ostream& outputStream, const GestionnaireFilms& gestionnaireFilms);

    // Opérations d'ajout et de suppression
    bool chargerDepuisFichier(const std::string& nomFichier);
    bool ajouterFilm(const Film& film);
    bool supprimerFilm(const std::string& nomFilm);

    // Getters
    std::size_t getNombreFilms() const;
    const Film* getFilmParNom(const std::string& nom) const;
    std::vector<const Film*> getFilmsParGenre(Film::Genre genre) const;
    std::vector<const Film*> getFilmsParPays(Pays pays) const;
    std::vector<const Film*> getFilmsEntreAnnees(int anneeDebut, int anneeFin);

private:
    std::vector<std::unique_ptr<Film>> films_; // Vecteur de pointeurs pour ne pas que les éléments des filtres
                                               // deviennent invalidés lors d'un resize du vecteur

    std::unordered_map<std::string, const Film*> filtreNomFilms_;
    std::unordered_map<Film::Genre, std::vector<const Film*>> filtreGenreFilms_;
    std::unordered_map<Pays, std::vector<const Film*>> filtrePaysFilms_;
};

#endif // GESTIONNAIREFILMS_H

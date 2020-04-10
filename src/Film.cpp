/// Fonctions auxiliaires à la struct pour les films.
/// \author Misha Krieger-Raynauld
/// \date 2020-01-12

#include "Film.h"
#include <unordered_map>

/// Convertit la valeur du enum Film::Genre en string.
/// \param genre    Le genre à convertir.
/// \return         String représentant le enum.
std::string getGenreString(Film::Genre genre)
{
    static const std::unordered_map<Film::Genre, std::string> genreStrings = {{Film::Genre::Action, "Action"},
                                                                              {Film::Genre::Aventure, "Aventure"},
                                                                              {Film::Genre::Comedie, "Comédie"},
                                                                              {Film::Genre::Documentaire,
                                                                               "Documentaire"},
                                                                              {Film::Genre::Drame, "Drame"},
                                                                              {Film::Genre::Fantastique, "Fantastique"},
                                                                              {Film::Genre::Horreur, "Horreur"},
                                                                              {Film::Genre::Romance, "Romance"},
                                                                              {Film::Genre::ScienceFiction,
                                                                               "Science-fiction"}};

    auto it = genreStrings.find(genre);
    if (it != genreStrings.cend())
    {
        return it->second;
    }
    return "Erreur";
}

/// Affiche les informations d'un film à la sortie du stream donné.
/// \param outputStream Le stream auquel écrire les informations du film.
/// \param film         Le film à afficher au stream.
/// \return             Une référence au stream.
std::ostream& operator<<(std::ostream& outputStream, const Film& film)
{
    outputStream << "Nom: " << film.nom << " | Genre: " << getGenreString(film.genre)
                 << " | Pays: " << getPaysString(film.pays) << " | Réalisateur: " << film.realisateur
                 << " | Année: " << film.annee;
    return outputStream;
}

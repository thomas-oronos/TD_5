/// Struct pour les films.
/// \author Misha Krieger-Raynauld
/// \date 2020-01-12

#ifndef FILM_H
#define FILM_H

#include <iostream>
#include <string>
#include "Pays.h"

/// Struct contenant les caractéristiques pour un film.
struct Film
{
    /// Enum pour le genre (catégorie) d'un film.
    enum class Genre
    {
        Action,
        Aventure,
        Comedie,
        Documentaire,
        Drame,
        Fantastique,
        Horreur,
        Romance,
        ScienceFiction
    };

    std::string nom;
    Genre genre;
    Pays pays;
    std::string realisateur;
    int annee;
};

std::string getGenreString(Film::Genre genre);
std::ostream& operator<<(std::ostream& outputStream, const Film& film);

#endif // FILM_H

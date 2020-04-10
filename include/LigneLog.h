/// Struct pour les lignes de log.
/// \author Misha Krieger-Raynauld
/// \date 2020-01-12

#ifndef LIGNELOG_H
#define LIGNELOG_H

#include <string>
#include "Film.h"
#include "Utilisateur.h"

/// Struct contenant les informations traduites d'une ligne du log.
struct LigneLog
{
    std::string timestamp;
    const Utilisateur* utilisateur;
    const Film* film;
};

#endif // LIGNELOG_H

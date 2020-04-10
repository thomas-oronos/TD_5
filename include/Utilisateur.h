/// Struct pour les utilisateurs.
/// \author Misha Krieger-Raynauld
/// \date 2020-01-12

#ifndef UTILISATEUR_H
#define UTILISATEUR_H

#include <iostream>
#include <string>
#include "Pays.h"

/// Struct contenant les informations pour un utilisateur.
struct Utilisateur
{
    std::string id;
    std::string nom;
    int age;
    Pays pays;
};

std::ostream& operator<<(std::ostream& outputStream, const Utilisateur& utilisateur);

#endif // UTILISATEUR_H

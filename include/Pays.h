/// Enum pour les pays.
/// \author Misha Krieger-Raynauld
/// \date 2020-01-12

#ifndef PAYS_H
#define PAYS_H

#include <string>

/// Enum pour les différents pays.
enum class Pays
{
    Bresil,
    Canada,
    Chine,
    EtatsUnis,
    France,
    Japon,
    RoyaumeUni,
    Russie,
    Mexique
};

std::string getPaysString(Pays pays);

#endif // PAYS_H

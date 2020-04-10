/// Fonctions auxiliaires à l'enum pour les pays.
/// \author Misha Krieger-Raynauld
/// \date 2020-01-12

#include "Pays.h"
#include <string>
#include <unordered_map>

/// Convertit la valeur du enum Pays en string.
/// \param pays Le pays à convertir.
/// \return     String représentant le enum.
std::string getPaysString(Pays pays)
{
    static const std::unordered_map<Pays, std::string> paysStrings = {{Pays::Bresil, "Brésil"},
                                                                      {Pays::Canada, "Canada"},
                                                                      {Pays::Chine, "Chine"},
                                                                      {Pays::EtatsUnis, "États-Unis"},
                                                                      {Pays::France, "France"},
                                                                      {Pays::Japon, "Japon"},
                                                                      {Pays::RoyaumeUni, "Royaume-Uni"},
                                                                      {Pays::Russie, "Russie"},
                                                                      {Pays::Mexique, "Mexique"}};

    auto it = paysStrings.find(pays);
    if (it != paysStrings.cend())
    {
        return it->second;
    }
    return "Erreur";
}

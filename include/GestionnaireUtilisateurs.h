/// Gestionnaire d'utilisateurs.
/// \author Misha Krieger-Raynauld
/// \date 2020-01-12

#ifndef GESTIONNAIREUTILISATEURS_H
#define GESTIONNAIREUTILISATEURS_H

#include <string>
#include <unordered_map>
#include "Utilisateur.h"

/// Classe qui gère les informations de tous les utilisateurs.
class GestionnaireUtilisateurs
{
public:
    // Surcharges d'opérateurs
    friend std::ostream& operator<<(std::ostream& outputStream,
                                    const GestionnaireUtilisateurs& gestionnaireUtilisateurs);

    // Opérations d'ajout et de suppression
    bool chargerDepuisFichier(const std::string& nomFichier);
    bool ajouterUtilisateur(const Utilisateur& utilisateur);
    bool supprimerUtilisateur(const std::string& idUtilisateur);

    // Getters
    std::size_t getNombreUtilisateurs() const;
    const Utilisateur* getUtilisateurParId(const std::string& id) const;

private:
    std::unordered_map<std::string, Utilisateur> utilisateurs_;
};

#endif // GESTIONNAIREUTILISATEURS_H

/// Analyseur de statistiques grâce aux logs.
/// \author Misha Krieger-Raynauld
/// \date 2020-01-12

#ifndef ANALYSEURLOGS_H
#define ANALYSEURLOGS_H

#include <string>
#include <vector>
#include "GestionnaireFilms.h"
#include "GestionnaireUtilisateurs.h"
#include "LigneLog.h"
#include "Tests.h"

/// Classe contenant la liste des entrées du log pour en analyser les tendances pertinentes.
class AnalyseurLogs
{
public:
    // Opérations d'ajout de logs
    bool chargerDepuisFichier(const std::string& nomFichier, GestionnaireUtilisateurs& gestionnaireUtilisateurs,
                              GestionnaireFilms& gestionnaireFilms);
    bool creerLigneLog(const std::string& timestamp, const std::string& idUtilisateur, const std::string& nomFilm,
                       GestionnaireUtilisateurs& gestionnaireUtilisateurs, GestionnaireFilms& gestionnaireFilms);
    void ajouterLigneLog(const LigneLog& ligneLog);

    // Statistiques
    int getNombreVuesFilm(const Film* film) const;
    const Film* getFilmPlusPopulaire() const;
    std::vector<std::pair<const Film*, int>> getNFilmsPlusPopulaires(std::size_t nombre) const;
    int getNombreVuesPourUtilisateur(const Utilisateur* utilisateur) const;
    std::vector<const Film*> getFilmsVusParUtilisateur(const Utilisateur* utilisateur) const;

private:
    std::vector<LigneLog> logs_;
    std::unordered_map<const Film*, int> vuesFilms_;

    friend double Tests::testAnalyseurLogs(); // Pour les tests
};

#endif // ANALYSEURLOGS_H

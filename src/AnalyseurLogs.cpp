/// Analyseur de statistiques grâce aux logs.
/// \author Misha Krieger-Raynauld
/// \date 2020-01-12

#include "AnalyseurLogs.h"
#include <algorithm>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <unordered_set>
#include "Foncteurs.h"

/// Ajoute les lignes de log en ordre chronologique à partir d'un fichier de logs.
/// \param nomFichier               Le fichier à partir duquel lire les logs.
/// \param gestionnaireUtilisateurs Référence au gestionnaire des utilisateurs pour lier un utilisateur à un log.
/// \param gestionnaireFilms        Référence au gestionnaire des films pour pour lier un film à un log.
/// \return                         True si tout le chargement s'est effectué avec succès, false sinon.
bool AnalyseurLogs::chargerDepuisFichier(const std::string& nomFichier,
                                         GestionnaireUtilisateurs& gestionnaireUtilisateurs,
                                         GestionnaireFilms& gestionnaireFilms)
{
    std::ifstream fichier(nomFichier);
    if (fichier)
    {
        logs_.clear();
        vuesFilms_.clear();

        bool succesParsing = true;

        std::string ligne;
        while (std::getline(fichier, ligne))
        {
            std::istringstream stream(ligne);

            std::string timestamp;
            std::string idUtilisateur;
            std::string nomFilm;

            if (stream >> timestamp >> idUtilisateur >> std::quoted(nomFilm))
            {
                // TODO: Uncomment une fois que la fonction creerLigneLog est écrite
                creerLigneLog(timestamp, idUtilisateur, nomFilm, gestionnaireUtilisateurs, gestionnaireFilms);
            }
            else
            {
                std::cerr << "Erreur AnalyseurLogs: la ligne " << ligne
                          << " n'a pas pu être interprétée correctement\n";
                succesParsing = false;
            }
        }
        return succesParsing;
    }
    std::cerr << "Erreur AnalyseurLogs: le fichier " << nomFichier << " n'a pas pu être ouvert\n";
    return false;
}

/// Cree une ligne log et l'ajoute au vecteur de logs
/// \param timesamp                     La date a laquelle le filmest regarde
/// \param idUtilisateur                L'id de l'utilisateur qui regarde le film
/// \param nomFilm                      Le nom du film regarde
/// \param gestionnaireUtilisateurs     Reference au gestionnaire des utilisateurs
/// \param gestionnaireFilms            Reference au gestionnairede films
/// \return                             true si la ligne a bien ete cree, false sinon 
bool AnalyseurLogs::creerLigneLog(const std::string& timestamp, const std::string& idUtilisateur, const std::string& nomFilm,
                       GestionnaireUtilisateurs& gestionnaireUtilisateurs, GestionnaireFilms& gestionnaireFilms)
{
    LigneLog ligneLog{timestamp, gestionnaireUtilisateurs.getUtilisateurParId(idUtilisateur), gestionnaireFilms.getFilmParNom(nomFilm)};
    if(ligneLog.film == nullptr || ligneLog.utilisateur == nullptr)
    {
        return false;
    }
    ajouterLigneLog(ligneLog);

    return std::binary_search(logs_.begin(), logs_.end(), ligneLog, ComparateurLog());
}

/// Ajoute une ligne log passe en parametre au vecteur de logs
/// \param ligneLog     La ligne log a ajouter
void AnalyseurLogs::ajouterLigneLog(const LigneLog& ligneLog)
{
    auto position = std::lower_bound(logs_.begin(), logs_.end(), ligneLog, ComparateurLog());
    logs_.emplace(position, ligneLog);
    vuesFilms_[ligneLog.film]++;
}

/// Retourne le nombre de vues d'un film passe en parametre
/// \param film     Le film dont on veut le nombre de vues
int AnalyseurLogs::getNombreVuesFilm(const Film* film) const
{
    if(vuesFilms_.find(film) == vuesFilms_.end())
    {
        return 0;
    }
    return vuesFilms_.at(film);
}

/// Retourne le film le plus populaires du vecteur de vues films.
/// \return         Un pointeur vers le film le plus populaire ou nullptr si il n'y a aucun film
const Film* AnalyseurLogs::getFilmPlusPopulaire() const
{
    if(logs_.size() == 0)
    {
        return nullptr;
    }
    return std::max_element(vuesFilms_.begin(), vuesFilms_.end(), ComparateurSecondElementPaire<const Film*, int>())->first;
}

/// Retourne un vecteur contenant les n films les plus populaires
/// \param nombre      Le nombre de films a retourner
/// \return            Le vecteur contenant les films les plus populaires
std::vector<std::pair<const Film*, int>> AnalyseurLogs::getNFilmsPlusPopulaires(std::size_t nombre) const
{
    std::vector<std::pair<const Film*, int>>nFilmsPlusPopulaires(std::min(vuesFilms_.size(), nombre));
    std::partial_sort_copy(vuesFilms_.begin(), vuesFilms_.end(), nFilmsPlusPopulaires.begin(), nFilmsPlusPopulaires.end(),
        [](const std::pair<const Film*, int>& film1, const std::pair<const Film*, int>& film2) {return film1.second > film2.second;});
    return nFilmsPlusPopulaires;
}

/// Retourne le nombre de vues total pour un utilisateur
/// \param utilisateur      L'utilisateur dont on veut savoirlenombre de vues
/// \return                 un int contenant le nombre de vues pour l'utilisateur
int AnalyseurLogs::getNombreVuesPourUtilisateur(const Utilisateur* utilisateur) const
{
    return static_cast<int>(std::count_if(logs_.begin(), logs_.end(), [&utilisateur](const LigneLog& ligneLog){return ligneLog.utilisateur == utilisateur;}));
}

/// Retourne un vecteur contenangt les films vus par l'utilisateur passe en parametres
/// \param utilisateur      L'utilisateur dont on veut avoir les films vus.
/// \return                 Un vecteur contenant les films vus par l'utilisateur passe en parametres 
std::vector<const Film*> AnalyseurLogs::getFilmsVusParUtilisateur(const Utilisateur* utilisateur) const
{
    std::unordered_set<const Film*> filmsVus;
    for (const LigneLog& i : logs_)
    {
        if(utilisateur == i.utilisateur)
        {
            filmsVus.insert(i.film);
        }
    }
    // std::copy_if(logs_.begin(), logs_.end(), filmsVus,[&utilisateur](LigneLog& ligneLog){return ligneLog.utilisateur == utilisateur;};
    std::vector<const Film*> films;
    std::copy(filmsVus.begin(), filmsVus.end(), std::back_inserter(films));

    return films;
}



/// Gestionnaire de films.
/// \author Misha Krieger-Raynauld
/// \date 2020-01-12

#include "GestionnaireFilms.h"
#include <algorithm>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include "Foncteurs.h"
#include "RawPointerBackInserter.h"

/// Constructeur par copie.
/// \param other    Le gestionnaire de films à partir duquel copier la classe.
GestionnaireFilms::GestionnaireFilms(const GestionnaireFilms& other)
{
    films_.reserve(other.films_.size());
    filtreNomFilms_.reserve(other.filtreNomFilms_.size());
    filtreGenreFilms_.reserve(other.filtreGenreFilms_.size());
    filtrePaysFilms_.reserve(other.filtrePaysFilms_.size());

    for (const auto& film : other.films_)
    {
        // TODO: Uncomment une fois que la fonction ajouterFilm est écrite
        ajouterFilm(*film);
    }
}

/// Opérateur d'assignation par copie utilisant le copy-and-swap idiom.
/// \param other    Le gestionnaire de films à partir duquel copier la classe.
/// \return         Référence à l'objet actuel.
GestionnaireFilms& GestionnaireFilms::operator=(GestionnaireFilms other)
{
    std::swap(films_, other.films_);
    std::swap(filtreNomFilms_, other.filtreNomFilms_);
    std::swap(filtreGenreFilms_, other.filtreGenreFilms_);
    std::swap(filtrePaysFilms_, other.filtrePaysFilms_);
    return *this;
}

/// Affiche les informations des films gérés par le gestionnaire de films à la sortie du stream donné.
/// \param outputStream         Le stream auquel écrire les informations des films.
/// \param gestionnaireFilms    Le gestionnaire de films à afficher au stream.
/// \return                     Une référence au stream.
std::ostream& operator<<(std::ostream& outputStream, const GestionnaireFilms& gestionnaireFilms)
{
    // TODO: Uncomment une fois que la fonction getNombreFilms est écrite
    outputStream << "Le gestionnaire de films contient " << gestionnaireFilms.getNombreFilms() << " films.\n"
                 << "Affichage par catégories:\n";

    // TODO: Réécrire l'implémentation avec des range-based for et structured bindings (voir énoncé du TP)
    for (const auto& [key, value] : gestionnaireFilms.filtreGenreFilms_)
    {
        Film::Genre genre = key;
        std::vector<const Film*> listeFilms = value;
        outputStream << "Genre: " << getGenreString(genre) << " (" << listeFilms.size() << " films):\n";
        for (auto & element : listeFilms)
        {
            outputStream << '\t' << *element << '\n';
        }
    }
    return outputStream;
}

/// Ajoute les films à partir d'un fichier de description des films.
/// \param nomFichier   Le fichier à partir duquel lire les informations des films.
/// \return             True si tout le chargement s'est effectué avec succès, false sinon.
bool GestionnaireFilms::chargerDepuisFichier(const std::string& nomFichier)
{
    std::ifstream fichier(nomFichier);
    if (fichier)
    {
        films_.clear();
        filtreNomFilms_.clear();
        filtreGenreFilms_.clear();
        filtrePaysFilms_.clear();

        bool succesParsing = true;

        std::string ligne;
        while (std::getline(fichier, ligne))
        {
            std::istringstream stream(ligne);

            std::string nom;
            int genre;
            int pays;
            std::string realisateur;
            int annee;

            if (stream >> std::quoted(nom) >> genre >> pays >> std::quoted(realisateur) >> annee)
            {
                // TODO: Uncomment une fois que la fonction ajouterFilm est écrite
                ajouterFilm(Film{nom, static_cast<Film::Genre>(genre), static_cast<Pays>(pays), realisateur, annee});
            }
            else
            {
                std::cerr << "Erreur GestionnaireFilms: la ligne " << ligne
                          << " n'a pas pu être interprétée correctement\n";
                succesParsing = false;
            }
        }
        return succesParsing;
    }
    std::cerr << "Erreur GestionnaireFilms: le fichier " << nomFichier << " n'a pas pu être ouvert\n";
    return false;
}

bool GestionnaireFilms::ajouterFilm(const Film& film)
{
    if(getFilmParNom(film.nom) != nullptr)
        return false;
    std::unique_ptr<Film> ptrFilm = std::make_unique<Film>(film);
    films_.push_back(std::move(ptrFilm));
    filtreNomFilms_.emplace(film.nom, ptrFilm.get());
    filtreGenreFilms_[film.genre].push_back(ptrFilm.get());
    filtrePaysFilms_[film.pays].push_back(ptrFilm.get());

    return true; 
}

bool GestionnaireFilms::supprimerFilm(const std::string& nomFilm)
{
    std::vector<std::unique_ptr<Film>>::iterator trouve = find_if(films_.begin(), films_.end(), [&nomFilm](std::unique_ptr<Film>& film){return film->nom == nomFilm;});
    if(trouve == films_.end())
        return false;
    Film film = *(trouve->get());
    filtreNomFilms_.erase(nomFilm);
    std::vector<const Film*>& vecteurPays = filtrePaysFilms_[film.pays];
    std::vector<const Film*>& vecteurGenre = filtreGenreFilms_[film.genre];
    
    vecteurPays.erase(std::remove(vecteurPays.begin(), vecteurPays.end(), trouve->get()), vecteurPays.end());
    vecteurGenre.erase(std::remove(vecteurGenre.begin(), vecteurGenre.end(),trouve->get()), vecteurGenre.end());

    films_.erase(trouve);
}

std::size_t GestionnaireFilms::getNombreFilms() const
{
    return films_.size();
}

const Film* GestionnaireFilms::getFilmParNom(const std::string& nom) const
{
    auto film = filtreNomFilms_.find(nom);
    if(film == filtreNomFilms_.end())
        return nullptr;
    return film->second;
}

std::vector<const Film*> GestionnaireFilms::getFilmsParGenre(Film::Genre genre) const
{
    auto it = filtreGenreFilms_.find(genre);
    if(it == filtreGenreFilms_.end())
        return std::vector<const Film*>();
    return filtreGenreFilms_.at(genre);
}

std::vector<const Film*> GestionnaireFilms::getFilmsParPays(Pays pays) const
{
    auto it = filtrePaysFilms_.find(pays);
    if(it == filtrePaysFilms_.end())
        return std::vector<const Film*>();
    return filtrePaysFilms_.at(pays);
}

std::vector<const Film*> GestionnaireFilms::getFilmsEntreAnnees(int anneeDebut, int anneeFin)
{
    std::vector<const Film*> filmsEntreAnnees;
    copy_if(films_.begin(), films_.end(), RawPointerBackInserter(filmsEntreAnnees), EstDansIntervalleDatesFilm(anneeDebut, anneeFin));
    return filmsEntreAnnees;
}



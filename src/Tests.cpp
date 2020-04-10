/// Tests automatisés.
/// \author Misha Krieger-Raynauld
/// \date 2020-03-04

#include "Tests.h"
#include <algorithm>
#include <array>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <vector>
#include "AnalyseurLogs.h"
#include "Foncteurs.h"
#include "GestionnaireFilms.h"
#include "GestionnaireUtilisateurs.h"

namespace
{
    /// Affiche un header pour chaque section de tests à l'écran.
    /// \param nomSectionTest   Le nom de la section de tests.
    void afficherHeaderTest(const std::string& nomSectionTest)
    {
        std::cout << "\nTests pour " + nomSectionTest + ":\n--------\n";
    }

    /// Affiche un footer avec les points pour chaque section de tests à l'écran.
    /// \param totalPoints  Le nombre de points obtenus à la section de tests.
    /// \param maxPoints    Le nombre de points maximal alloués à la section de tests.
    void afficherFooterTest(double totalPoints, double maxPoints)
    {
        std::cout << "--------\nTotal pour la section: " << totalPoints << "/" << maxPoints << '\n';
    }

    /// Affiche le nom d'un test ainsi que son état de passage à l'écran.
    /// \param index        L'index du test.
    /// \param nom          Le nom du test.
    /// \param estReussi    L'état de passage du test.
    void afficherResultatTest(int index, const std::string& nom, bool estReussi)
    {
        static constexpr int largeurNumeroTest = 2;
        static constexpr int largeurNomTest = 50;
        std::cout << "Test " << std::right << std::setw(largeurNumeroTest) << index << ": " << std::left
                  << std::setw(largeurNomTest) << nom << ": " << (estReussi ? "OK" : "FAILED") << '\n';
    }
} // namespace

namespace Tests
{
    /// Appelle tous les tests et affiche la somme de ceux-ci à l'écran.
    void testAll()
    {
        static constexpr double maxPointsAll = 6.0;

        double totalPointsAll = 0.0;
        totalPointsAll += testGestionnaireUtilisateurs();
        totalPointsAll += testFoncteurs();
        totalPointsAll += testGestionnaireFilms();
        totalPointsAll += testAnalyseurLogs();

        std::cout << "\nTotal pour tous les tests: " << totalPointsAll << '/' << maxPointsAll << '\n';
    }

    /// Teste la classe GestionnaireUtilisateurs.
    /// \return Le nombre de points obtenus aux tests.
    double testGestionnaireUtilisateurs()
    {
        afficherHeaderTest("GestionnaireUtilisateurs");
        static constexpr double maxPointsSection = 1.0;

#if TEST_GESTIONNAIRE_UTILISATEURS_ACTIF
        std::vector<bool> tests;

        GestionnaireUtilisateurs gestionnaireUtilisateurs;

        // Test 1
        Utilisateur utilisateur1{"prénom.nom.1@email.com", "Prénom Nom", 20, Pays::Canada};
        Utilisateur utilisateur2{"prénom.nom.2@email.com", "Prénom Nom", 20, Pays::Canada};
        bool ajout1 = gestionnaireUtilisateurs.ajouterUtilisateur(utilisateur1);
        bool ajout2 = gestionnaireUtilisateurs.ajouterUtilisateur(utilisateur1);
        bool ajout3 = gestionnaireUtilisateurs.ajouterUtilisateur(utilisateur2);
        tests.push_back(ajout1 && !ajout2 && ajout3);
        afficherResultatTest(1, "GestionnaireUtilisateurs::ajouterUtilisateur", tests.back());

        // Test 2
        Utilisateur utilisateur3{"prénom.nom.3@email.com", "Prénom Nom", 20, Pays::Canada};
        bool suppression1 = gestionnaireUtilisateurs.supprimerUtilisateur(utilisateur1.id);
        bool suppression2 = gestionnaireUtilisateurs.supprimerUtilisateur(utilisateur1.id);
        bool suppression3 = gestionnaireUtilisateurs.supprimerUtilisateur(utilisateur2.id);
        bool suppression4 = gestionnaireUtilisateurs.supprimerUtilisateur(utilisateur3.id);
        tests.push_back(suppression1 && !suppression2 && suppression3 && !suppression4);
        afficherResultatTest(2, "GestionnaireUtilisateurs::supprimerUtilisateur", tests.back());

        // Test 3
        std::size_t nombre1 = gestionnaireUtilisateurs.getNombreUtilisateurs();
        gestionnaireUtilisateurs.ajouterUtilisateur(utilisateur1);
        gestionnaireUtilisateurs.ajouterUtilisateur(utilisateur2);
        gestionnaireUtilisateurs.ajouterUtilisateur(utilisateur3);
        std::size_t nombre2 = gestionnaireUtilisateurs.getNombreUtilisateurs();
        tests.push_back(nombre1 == 0 && nombre2 == 3);
        afficherResultatTest(3, "GestionnaireUtilisateurs::getNombreUtilisateurs", tests.back());

        // Test 4
        Utilisateur utilisateur4{"prénom.nom.4@email.com", "PrénomUnique NomUnique", 30, Pays::EtatsUnis};
        Utilisateur utilisateur5{"prénom.nom.5@email.com", "Prénom Nom", 20, Pays::Canada};
        gestionnaireUtilisateurs.ajouterUtilisateur(utilisateur4);
        const Utilisateur* utilisateurTrouve1 = gestionnaireUtilisateurs.getUtilisateurParId(utilisateur4.id);
        bool utilisateurTrouve1EstIdentique =
            utilisateurTrouve1 != nullptr && utilisateurTrouve1->id == utilisateur4.id &&
            utilisateurTrouve1->nom == utilisateur4.nom && utilisateurTrouve1->age == utilisateur4.age &&
            utilisateurTrouve1->pays == utilisateur4.pays;
        const Utilisateur* utilisateurTrouve2 = gestionnaireUtilisateurs.getUtilisateurParId(utilisateur5.id);
        gestionnaireUtilisateurs.supprimerUtilisateur(utilisateur4.id);
        const Utilisateur* utilisateurTrouve3 = gestionnaireUtilisateurs.getUtilisateurParId(utilisateur4.id);
        tests.push_back(utilisateurTrouve1EstIdentique && utilisateurTrouve2 == nullptr &&
                        utilisateurTrouve3 == nullptr);
        afficherResultatTest(4, "GestionnaireUtilisateurs::getUtilisateurParId", tests.back());

        // Test 5
        gestionnaireUtilisateurs.chargerDepuisFichier("utilisateurs.txt");
        std::stringstream stream;
        stream << gestionnaireUtilisateurs;
        std::vector<std::string> lignes;
        std::string ligneStream;
        while (std::getline(stream, ligneStream))
        {
            lignes.push_back(std::move(ligneStream));
        }
        std::sort(std::next(lignes.begin(), 1), lignes.end());
        std::string sortieRecue;
        for (const auto& ligne : lignes)
        {
            sortieRecue += ligne + '\n';
        }
        static const std::string sortieAttendue =
            "Le gestionnaire d'utilisateurs contient 100 utilisateurs:\n"
            "\tIdentifiant: akoblin@optonline.net | Nom: Marnie Melvin | Âge: 37 | Pays: Chine\n"
            "\tIdentifiant: alhajj@mac.com | Nom: Toccara Patino | Âge: 65 | Pays: Canada\n"
            "\tIdentifiant: andrei@hotmail.com | Nom: Armanda Hooper | Âge: 11 | Pays: Royaume-Uni\n"
            "\tIdentifiant: animats@sbcglobal.net | Nom: Nickole Montero | Âge: 91 | Pays: Mexique\n"
            "\tIdentifiant: augusto@verizon.net | Nom: Rhett Sargent | Âge: 15 | Pays: Mexique\n"
            "\tIdentifiant: barjam@sbcglobal.net | Nom: Kennith Doan | Âge: 69 | Pays: États-Unis\n"
            "\tIdentifiant: bartak@outlook.com | Nom: Eli Burdette | Âge: 86 | Pays: Russie\n"
            "\tIdentifiant: bebing@mac.com | Nom: Rupert Irvine | Âge: 9 | Pays: Mexique\n"
            "\tIdentifiant: bester@optonline.net | Nom: Margherita Noland | Âge: 50 | Pays: États-Unis\n"
            "\tIdentifiant: bsikdar@gmail.com | Nom: Emerald Stubbs | Âge: 5 | Pays: France\n"
            "\tIdentifiant: cantu@optonline.net | Nom: Amberly Bohannon | Âge: 99 | Pays: Brésil\n"
            "\tIdentifiant: carreras@mac.com | Nom: Roma Greco | Âge: 29 | Pays: Mexique\n"
            "\tIdentifiant: chaffar@live.com | Nom: Babara Guerin | Âge: 52 | Pays: Russie\n"
            "\tIdentifiant: claypool@aol.com | Nom: Hilton Coles | Âge: 90 | Pays: Mexique\n"
            "\tIdentifiant: codex@mac.com | Nom: Stan Durand | Âge: 3 | Pays: Royaume-Uni\n"
            "\tIdentifiant: cosimo@live.com | Nom: Ilana Cervantes | Âge: 71 | Pays: Canada\n"
            "\tIdentifiant: cremonini@verizon.net | Nom: Truman Sawyer | Âge: 20 | Pays: Mexique\n"
            "\tIdentifiant: danzigism@att.net | Nom: Masako Shipp | Âge: 44 | Pays: Chine\n"
            "\tIdentifiant: dawnsong@icloud.com | Nom: Stephine Jamison | Âge: 16 | Pays: Canada\n"
            "\tIdentifiant: denton@me.com | Nom: Vashti Snipes | Âge: 80 | Pays: Canada\n"
            "\tIdentifiant: dinther@me.com | Nom: Deetta Edmond | Âge: 94 | Pays: Chine\n"
            "\tIdentifiant: dkrishna@att.net | Nom: Shirely Kenney | Âge: 100 | Pays: Canada\n"
            "\tIdentifiant: dmouse@mac.com | Nom: Marlin Mcgregor | Âge: 51 | Pays: Chine\n"
            "\tIdentifiant: dogdude@icloud.com | Nom: Renetta Camarillo | Âge: 8 | Pays: Japon\n"
            "\tIdentifiant: dsowsy@yahoo.com | Nom: Angla Corrigan | Âge: 37 | Pays: Mexique\n"
            "\tIdentifiant: dvdotnet@yahoo.ca | Nom: Shirlee Keating | Âge: 82 | Pays: Canada\n"
            "\tIdentifiant: facet@verizon.net | Nom: Charisse Beckman | Âge: 73 | Pays: France\n"
            "\tIdentifiant: falcao@verizon.net | Nom: Breanne Hawley | Âge: 86 | Pays: Brésil\n"
            "\tIdentifiant: fatelk@comcast.net | Nom: Gay Shuler | Âge: 71 | Pays: États-Unis\n"
            "\tIdentifiant: frode@sbcglobal.net | Nom: Lane Britton | Âge: 25 | Pays: Brésil\n"
            "\tIdentifiant: froodian@mac.com | Nom: Florrie Carmichael | Âge: 50 | Pays: Brésil\n"
            "\tIdentifiant: fwitness@gmail.com | Nom: Arlinda Murillo | Âge: 62 | Pays: Japon\n"
            "\tIdentifiant: gomor@optonline.net | Nom: Earlie Portillo | Âge: 57 | Pays: États-Unis\n"
            "\tIdentifiant: gravyface@live.com | Nom: Euna Spinks | Âge: 49 | Pays: Canada\n"
            "\tIdentifiant: grdschl@icloud.com | Nom: Lavenia Beach | Âge: 10 | Pays: Mexique\n"
            "\tIdentifiant: gward@sbcglobal.net | Nom: Rocco Lacroix | Âge: 76 | Pays: Russie\n"
            "\tIdentifiant: ijackson@gmail.com | Nom: Seymour Knudson | Âge: 28 | Pays: Japon\n"
            "\tIdentifiant: ivoibs@yahoo.ca | Nom: Louanne Bellamy | Âge: 83 | Pays: Russie\n"
            "\tIdentifiant: ivoibs@yahoo.com | Nom: Jenise Oates | Âge: 45 | Pays: Russie\n"
            "\tIdentifiant: jdhedden@mac.com | Nom: Meghann Cope | Âge: 80 | Pays: Royaume-Uni\n"
            "\tIdentifiant: jespley@me.com | Nom: Marylouise Otero | Âge: 26 | Pays: France\n"
            "\tIdentifiant: jfinke@comcast.net | Nom: Mara Yarbrough | Âge: 68 | Pays: Royaume-Uni\n"
            "\tIdentifiant: jkegl@verizon.net | Nom: Retta Wheatley | Âge: 91 | Pays: Brésil\n"
            "\tIdentifiant: kalpol@icloud.com | Nom: Chaya Massie | Âge: 21 | Pays: Mexique\n"
            "\tIdentifiant: karasik@hotmail.com | Nom: Luis Noe | Âge: 18 | Pays: États-Unis\n"
            "\tIdentifiant: karasik@msn.com | Nom: Carman Gipson | Âge: 97 | Pays: Russie\n"
            "\tIdentifiant: klaudon@hotmail.com | Nom: Salena Christy | Âge: 39 | Pays: Royaume-Uni\n"
            "\tIdentifiant: kobayasi@yahoo.com | Nom: Lindsay Lamar | Âge: 44 | Pays: France\n"
            "\tIdentifiant: kourai@verizon.net | Nom: Kendall Dias | Âge: 84 | Pays: Chine\n"
            "\tIdentifiant: kramulous@aol.com | Nom: Shizuko Beltran | Âge: 48 | Pays: Brésil\n"
            "\tIdentifiant: kwilliams@msn.com | Nom: Emmanuel Mcwhorter | Âge: 31 | Pays: Japon\n"
            "\tIdentifiant: leakin@yahoo.ca | Nom: Gita Stitt | Âge: 72 | Pays: Russie\n"
            "\tIdentifiant: linuxhack@yahoo.com | Nom: Lennie Hatley | Âge: 43 | Pays: Royaume-Uni\n"
            "\tIdentifiant: mastinfo@live.com | Nom: Echo Shumate | Âge: 41 | Pays: États-Unis\n"
            "\tIdentifiant: mbswan@optonline.net | Nom: Pattie Delgadillo | Âge: 84 | Pays: Canada\n"
            "\tIdentifiant: mcraigw@verizon.net | Nom: Anjanette Seitz | Âge: 58 | Pays: France\n"
            "\tIdentifiant: mcsporran@yahoo.ca | Nom: Anya Baez | Âge: 73 | Pays: Canada\n"
            "\tIdentifiant: melnik@aol.com | Nom: Isiah Peralta | Âge: 50 | Pays: Canada\n"
            "\tIdentifiant: mfburgo@live.com | Nom: Jaimee Chester | Âge: 36 | Pays: Royaume-Uni\n"
            "\tIdentifiant: mglee@outlook.com | Nom: Sharron Parson | Âge: 67 | Pays: Royaume-Uni\n"
            "\tIdentifiant: mlewan@hotmail.com | Nom: Tambra Wilbanks | Âge: 59 | Pays: Russie\n"
            "\tIdentifiant: moonlapse@msn.com | Nom: Verdell Reichert | Âge: 25 | Pays: Chine\n"
            "\tIdentifiant: mrobshaw@outlook.com | Nom: Joella Sparkman | Âge: 43 | Pays: Brésil\n"
            "\tIdentifiant: mschwartz@verizon.net | Nom: Lilly Valles | Âge: 84 | Pays: Brésil\n"
            "\tIdentifiant: mxiao@verizon.net | Nom: Marla Dempsey | Âge: 39 | Pays: États-Unis\n"
            "\tIdentifiant: notaprguy@verizon.net | Nom: Kacie Heflin | Âge: 80 | Pays: Canada\n"
            "\tIdentifiant: noticias@optonline.net | Nom: Adelaide Blank | Âge: 10 | Pays: Canada\n"
            "\tIdentifiant: pajas@comcast.net | Nom: Eldora Schaffer | Âge: 66 | Pays: Royaume-Uni\n"
            "\tIdentifiant: pakaste@gmail.com | Nom: Corazon Toliver | Âge: 2 | Pays: France\n"
            "\tIdentifiant: paley@msn.com | Nom: Magan Messer | Âge: 57 | Pays: France\n"
            "\tIdentifiant: parksh@msn.com | Nom: Alden Tyner | Âge: 21 | Pays: Chine\n"
            "\tIdentifiant: pdbaby@msn.com | Nom: Chas Spain | Âge: 27 | Pays: Royaume-Uni\n"
            "\tIdentifiant: philb@yahoo.ca | Nom: Hildred Fernandes | Âge: 89 | Pays: Chine\n"
            "\tIdentifiant: philen@msn.com | Nom: Gaston Hundley | Âge: 2 | Pays: Chine\n"
            "\tIdentifiant: plover@outlook.com | Nom: Julianne Sasser | Âge: 87 | Pays: Brésil\n"
            "\tIdentifiant: policies@yahoo.com | Nom: Fatimah Teal | Âge: 2 | Pays: Brésil\n"
            "\tIdentifiant: quinn@comcast.net | Nom: Lashell Bower | Âge: 54 | Pays: Russie\n"
            "\tIdentifiant: rasca@msn.com | Nom: Donnette Crawley | Âge: 76 | Pays: Canada\n"
            "\tIdentifiant: rddesign@me.com | Nom: Chanel Arevalo | Âge: 30 | Pays: Mexique\n"
            "\tIdentifiant: rgarton@att.net | Nom: Christiana Alves | Âge: 82 | Pays: France\n"
            "\tIdentifiant: rgiersig@live.com | Nom: Pierre Mcnabb | Âge: 28 | Pays: Chine\n"
            "\tIdentifiant: scottlee@att.net | Nom: Kiley Tang | Âge: 1 | Pays: Japon\n"
            "\tIdentifiant: sharon@icloud.com | Nom: Karine Herrington | Âge: 97 | Pays: Canada\n"
            "\tIdentifiant: singh@me.com | Nom: Tanika Weston | Âge: 76 | Pays: Canada\n"
            "\tIdentifiant: snunez@optonline.net | Nom: Mitchel Brandt | Âge: 2 | Pays: États-Unis\n"
            "\tIdentifiant: sopwith@hotmail.com | Nom: Russ Humphries | Âge: 98 | Pays: France\n"
            "\tIdentifiant: srour@me.com | Nom: Johnetta Adler | Âge: 72 | Pays: Chine\n"
            "\tIdentifiant: srour@verizon.net | Nom: Cyndi Marino | Âge: 42 | Pays: Russie\n"
            "\tIdentifiant: staikos@optonline.net | Nom: Christinia Rousseau | Âge: 14 | Pays: Mexique\n"
            "\tIdentifiant: temmink@att.net | Nom: Tracee Boyer | Âge: 97 | Pays: États-Unis\n"
            "\tIdentifiant: thrymm@aol.com | Nom: Olin Staton | Âge: 58 | Pays: Chine\n"
            "\tIdentifiant: tlinden@outlook.com | Nom: Thu Doughty | Âge: 22 | Pays: Canada\n"
            "\tIdentifiant: trieuvan@att.net | Nom: Mariana Pfeiffer | Âge: 85 | Pays: Chine\n"
            "\tIdentifiant: tubajon@live.com | Nom: Irish Gilmore | Âge: 21 | Pays: États-Unis\n"
            "\tIdentifiant: unreal@aol.com | Nom: Necole Johansen | Âge: 80 | Pays: Royaume-Uni\n"
            "\tIdentifiant: wainwrig@aol.com | Nom: Danyelle Craddock | Âge: 16 | Pays: Chine\n"
            "\tIdentifiant: wetter@yahoo.com | Nom: Mignon Haag | Âge: 69 | Pays: Canada\n"
            "\tIdentifiant: wikinerd@icloud.com | Nom: Georgetta Aquino | Âge: 10 | Pays: Japon\n"
            "\tIdentifiant: wortmanj@me.com | Nom: Bernie Napier | Âge: 1 | Pays: France\n"
            "\tIdentifiant: zyghom@yahoo.com | Nom: Easter Findley | Âge: 74 | Pays: Japon\n";
        tests.push_back(sortieRecue == sortieAttendue);
        afficherResultatTest(5, "GestionnaireUtilisateurs::operator<<", tests.back());

        int nombreTestsReussis = static_cast<int>(std::count(tests.begin(), tests.end(), true));
        double totalPointsSection =
            static_cast<double>(nombreTestsReussis) / static_cast<double>(tests.size()) * maxPointsSection;
#else
        std::cout << "[Tests désactivés]\n";
        double totalPointsSection = 0.0;
#endif
        afficherFooterTest(totalPointsSection, maxPointsSection);
        return totalPointsSection;
    }

    /// Teste les foncteurs du fichier Foncteurs.h.
    /// \return Le nombre de points obtenus aux tests.
    double testFoncteurs()
    {
        afficherHeaderTest("Foncteurs");
        static constexpr double maxPointsSection = 1.0;

#if TEST_FONCTEURS_ACTIF
        std::vector<bool> tests;

        // Test 1
        auto film1 =
            std::make_unique<Film>(Film{"Nom", Film::Genre::Documentaire, Pays::RoyaumeUni, "Réalisateur", 1970});
        auto film2 =
            std::make_unique<Film>(Film{"Nom", Film::Genre::Documentaire, Pays::RoyaumeUni, "Réalisateur", 1980});
        auto film3 =
            std::make_unique<Film>(Film{"Nom", Film::Genre::Documentaire, Pays::RoyaumeUni, "Réalisateur", 1981});
        auto film4 =
            std::make_unique<Film>(Film{"Nom", Film::Genre::Documentaire, Pays::RoyaumeUni, "Réalisateur", 1990});
        auto film5 =
            std::make_unique<Film>(Film{"Nom", Film::Genre::Documentaire, Pays::RoyaumeUni, "Réalisateur", 1999});
        auto film6 =
            std::make_unique<Film>(Film{"Nom", Film::Genre::Documentaire, Pays::RoyaumeUni, "Réalisateur", 2000});
        auto film7 =
            std::make_unique<Film>(Film{"Nom", Film::Genre::Documentaire, Pays::RoyaumeUni, "Réalisateur", 2010});
        const auto film8 =
            std::make_unique<Film>(Film{"Nom", Film::Genre::Documentaire, Pays::RoyaumeUni, "Réalisateur", 1995});
        int anneeInferieure = 1980;
        int anneeSuperieure = 2000;
        EstDansIntervalleDatesFilm foncteurIntervalle(anneeInferieure, anneeSuperieure);
        bool estDansIntervalle1 = foncteurIntervalle(film1);
        bool estDansIntervalle2 = foncteurIntervalle(film2);
        bool estDansIntervalle3 = foncteurIntervalle(film3);
        bool estDansIntervalle4 = foncteurIntervalle(film4);
        bool estDansIntervalle5 = foncteurIntervalle(film5);
        bool estDansIntervalle6 = foncteurIntervalle(film6);
        bool estDansIntervalle7 = foncteurIntervalle(film7);
        bool estDansIntervalle8 = foncteurIntervalle(film8);
        tests.push_back(!estDansIntervalle1 && estDansIntervalle2 && estDansIntervalle3 && estDansIntervalle4 &&
                        estDansIntervalle5 && estDansIntervalle6 && !estDansIntervalle7 && estDansIntervalle8);
        afficherResultatTest(1, "Foncteur EstDansIntervalleDatesFilm", tests.back());

        // Test 2
        Utilisateur utilisateur{"prénom.nom.@email.com", "Prénom Nom", 20, Pays::Canada};
        Film film{"Nom", Film::Genre::Documentaire, Pays::RoyaumeUni, "Réalisateur", 1970};
        LigneLog ligneLog1{"2018-01-01T14:54:19Z", &utilisateur, &film};
        LigneLog ligneLog2{"2018-04-01T14:54:19Z", &utilisateur, &film};
        const LigneLog ligneLog3{"2018-06-01T14:54:19Z", &utilisateur, &film};
        ComparateurLog foncteurComparateurLog;
        auto comparaisonLog1 = foncteurComparateurLog(ligneLog1, ligneLog2);
        auto comparaisonLog2 = foncteurComparateurLog(ligneLog2, ligneLog1);
        auto comparaisonLog3 = foncteurComparateurLog(ligneLog3, ligneLog3);
        tests.push_back(comparaisonLog1 && !comparaisonLog2 && !comparaisonLog3);
        afficherResultatTest(2, "Foncteur ComparateurLog", tests.back());

        // Test 3
        std::pair<std::string, int> pair1("First", 2);
        std::pair<std::string, int> pair2("First", 3);
        const std::pair<std::string, int> pair3("First", 4);
        ComparateurSecondElementPaire<std::string, int> foncteurComparateurSecondElementPaire;
        auto comparaisonPaire1 = foncteurComparateurSecondElementPaire(pair1, pair2);
        auto comparaisonPaire2 = foncteurComparateurSecondElementPaire(pair2, pair1);
        auto comparaisonPaire3 = foncteurComparateurSecondElementPaire(pair3, pair3);
        tests.push_back(comparaisonPaire1 && !comparaisonPaire2 && !comparaisonPaire3);
        afficherResultatTest(3, "Foncteur ComparateurSecondElementPaire", tests.back());

        int nombreTestsReussis = static_cast<int>(std::count(tests.begin(), tests.end(), true));
        double totalPointsSection =
            static_cast<double>(nombreTestsReussis) / static_cast<double>(tests.size()) * maxPointsSection;
#else
        std::cout << "[Tests désactivés]\n";
        double totalPointsSection = 0.0;
#endif
        afficherFooterTest(totalPointsSection, maxPointsSection);
        return totalPointsSection;
    }

    /// Teste la classe GestionnaireFilms.
    /// \return Le nombre de points obtenus aux tests.
    double testGestionnaireFilms()
    {
        afficherHeaderTest("GestionnaireFilms");
        static constexpr double maxPointsSection = 2.0;

#if TEST_GESTIONNAIRE_FILMS_ACTIF
        std::vector<bool> tests;

        GestionnaireFilms gestionnaireFilms;

        // Test 1
        Film film1{"Nom1", Film::Genre::Documentaire, Pays::RoyaumeUni, "Réalisateur", 1970};
        Film film2{"Nom2", Film::Genre::Documentaire, Pays::RoyaumeUni, "Réalisateur", 1970};
        bool ajout1 = gestionnaireFilms.ajouterFilm(film1);
        bool ajout2 = gestionnaireFilms.ajouterFilm(film1);
        bool ajout3 = gestionnaireFilms.ajouterFilm(film2);
        tests.push_back(ajout1 && !ajout2 && ajout3);
        afficherResultatTest(1, "GestionnaireFilms::ajouterFilm", tests.back());

        // Test 2
        Film film3{"Nom3", Film::Genre::Documentaire, Pays::RoyaumeUni, "Réalisateur", 1970};
        bool suppression1 = gestionnaireFilms.supprimerFilm(film1.nom);
        bool suppression2 = gestionnaireFilms.supprimerFilm(film1.nom);
        bool suppression3 = gestionnaireFilms.supprimerFilm(film2.nom);
        bool suppression4 = gestionnaireFilms.supprimerFilm(film3.nom);
        tests.push_back(suppression1 && !suppression2 && suppression3 && !suppression4);
        afficherResultatTest(2, "GestionnaireFilms::supprimerFilm", tests.back());

        // Test 3
        std::size_t nombre1 = gestionnaireFilms.getNombreFilms();
        gestionnaireFilms.ajouterFilm(film1);
        gestionnaireFilms.ajouterFilm(film2);
        gestionnaireFilms.ajouterFilm(film3);
        std::size_t nombre2 = gestionnaireFilms.getNombreFilms();
        tests.push_back(nombre1 == 0 && nombre2 == 3);
        afficherResultatTest(3, "GestionnaireFilms::getNombreFilms", tests.back());

        // Test 4
        Film film4{"Nom4", Film::Genre::Horreur, Pays::Japon, "RéalisateurUnique", 1920};
        Film film5{"Nom5", Film::Genre::Documentaire, Pays::RoyaumeUni, "Réalisateur", 1970};
        gestionnaireFilms.ajouterFilm(film4);
        const Film* filmTrouve1 = gestionnaireFilms.getFilmParNom(film4.nom);
        bool filmTrouve1EstIdentique = filmTrouve1 != nullptr && filmTrouve1->nom == film4.nom &&
                                       filmTrouve1->genre == film4.genre && filmTrouve1->pays == film4.pays &&
                                       filmTrouve1->realisateur == film4.realisateur &&
                                       filmTrouve1->annee == film4.annee;
        const Film* filmTrouve2 = gestionnaireFilms.getFilmParNom(film5.nom);
        gestionnaireFilms.supprimerFilm(film4.nom);
        const Film* filmTrouve3 = gestionnaireFilms.getFilmParNom(film4.nom);
        tests.push_back(filmTrouve1EstIdentique && filmTrouve2 == nullptr && filmTrouve3 == nullptr);
        afficherResultatTest(4, "GestionnaireFilms::getFilmParNom", tests.back());

        // Test 5
        std::vector<const Film*> filmsParGenre1 = gestionnaireFilms.getFilmsParGenre(Film::Genre::ScienceFiction);
        Film film6{"Nom6", Film::Genre::Drame, Pays::RoyaumeUni, "Réalisateur", 1970};
        Film film7{"Nom7", Film::Genre::Drame, Pays::RoyaumeUni, "Réalisateur", 1970};
        Film film8{"Nom8", Film::Genre::Drame, Pays::RoyaumeUni, "Réalisateur", 1970};
        Film film9{"Nom9", Film::Genre::Drame, Pays::RoyaumeUni, "Réalisateur", 1970};
        gestionnaireFilms.ajouterFilm(film6);
        gestionnaireFilms.ajouterFilm(film7);
        gestionnaireFilms.ajouterFilm(film8);
        gestionnaireFilms.ajouterFilm(film9);
        const Film* pointeurFilm6 = gestionnaireFilms.getFilmParNom(film6.nom);
        const Film* pointeurFilm7 = gestionnaireFilms.getFilmParNom(film7.nom);
        const Film* pointeurFilm8 = gestionnaireFilms.getFilmParNom(film8.nom);
        const Film* pointeurFilm9 = gestionnaireFilms.getFilmParNom(film9.nom);
        std::vector<const Film*> filmsParGenre2 = gestionnaireFilms.getFilmsParGenre(Film::Genre::Drame);
        std::vector<const Film*> filmsParGenre2Attendus = {pointeurFilm6, pointeurFilm7, pointeurFilm8, pointeurFilm9};
        bool filmsParGenre2Valides = filmsParGenre2 == filmsParGenre2Attendus;
        gestionnaireFilms.supprimerFilm(film7.nom);
        std::vector<const Film*> filmsParGenre3 = gestionnaireFilms.getFilmsParGenre(Film::Genre::Drame);
        std::vector<const Film*> filmsParGenre3Attendus = {pointeurFilm6, pointeurFilm8, pointeurFilm9};
        bool filmsParGenre3Valides = filmsParGenre3 == filmsParGenre3Attendus;
        tests.push_back(filmsParGenre1.empty() && filmsParGenre2Valides && filmsParGenre3Valides);
        afficherResultatTest(5, "GestionnaireFilms::getFilmsParGenre", tests.back());

        // Test 6
        std::vector<const Film*> filmsParPays1 = gestionnaireFilms.getFilmsParPays(Pays::Bresil);
        Film film10{"Nom10", Film::Genre::Documentaire, Pays::Russie, "Réalisateur", 1970};
        Film film11{"Nom11", Film::Genre::Documentaire, Pays::Russie, "Réalisateur", 1970};
        Film film12{"Nom12", Film::Genre::Documentaire, Pays::Russie, "Réalisateur", 1970};
        Film film13{"Nom13", Film::Genre::Documentaire, Pays::Russie, "Réalisateur", 1970};
        gestionnaireFilms.ajouterFilm(film10);
        gestionnaireFilms.ajouterFilm(film11);
        gestionnaireFilms.ajouterFilm(film12);
        gestionnaireFilms.ajouterFilm(film13);
        const Film* pointeurFilm10 = gestionnaireFilms.getFilmParNom(film10.nom);
        const Film* pointeurFilm11 = gestionnaireFilms.getFilmParNom(film11.nom);
        const Film* pointeurFilm12 = gestionnaireFilms.getFilmParNom(film12.nom);
        const Film* pointeurFilm13 = gestionnaireFilms.getFilmParNom(film13.nom);
        std::vector<const Film*> filmsParPays2 = gestionnaireFilms.getFilmsParPays(Pays::Russie);
        std::vector<const Film*> filmsParPays2Attendus = {pointeurFilm10,
                                                          pointeurFilm11,
                                                          pointeurFilm12,
                                                          pointeurFilm13};
        bool filmsParPays2Valides = filmsParPays2 == filmsParPays2Attendus;
        gestionnaireFilms.supprimerFilm(film11.nom);
        std::vector<const Film*> filmsParPays3 = gestionnaireFilms.getFilmsParPays(Pays::Russie);
        std::vector<const Film*> filmsParPays3Attendus = {pointeurFilm10, pointeurFilm12, pointeurFilm13};
        bool filmsParPays3Valides = filmsParPays3 == filmsParPays3Attendus;
        tests.push_back(filmsParPays1.empty() && filmsParPays2Valides && filmsParPays3Valides);
        afficherResultatTest(6, "GestionnaireFilms::getFilmsParPays", tests.back());

        // Test 7
        std::vector<const Film*> filmsEntreAnnees1 = gestionnaireFilms.getFilmsEntreAnnees(1000, 1100);
        Film film14{"Nom14", Film::Genre::Documentaire, Pays::RoyaumeUni, "Réalisateur", 1970};
        Film film15{"Nom15", Film::Genre::Documentaire, Pays::RoyaumeUni, "Réalisateur", 1980};
        Film film16{"Nom16", Film::Genre::Documentaire, Pays::RoyaumeUni, "Réalisateur", 1985};
        Film film17{"Nom17", Film::Genre::Documentaire, Pays::RoyaumeUni, "Réalisateur", 1995};
        Film film18{"Nom18", Film::Genre::Documentaire, Pays::RoyaumeUni, "Réalisateur", 2000};
        Film film19{"Nom19", Film::Genre::Documentaire, Pays::RoyaumeUni, "Réalisateur", 2010};
        gestionnaireFilms.ajouterFilm(film14);
        gestionnaireFilms.ajouterFilm(film15);
        gestionnaireFilms.ajouterFilm(film16);
        gestionnaireFilms.ajouterFilm(film17);
        gestionnaireFilms.ajouterFilm(film18);
        gestionnaireFilms.ajouterFilm(film19);
        const Film* pointeurFilm15 = gestionnaireFilms.getFilmParNom(film15.nom);
        const Film* pointeurFilm16 = gestionnaireFilms.getFilmParNom(film16.nom);
        const Film* pointeurFilm17 = gestionnaireFilms.getFilmParNom(film17.nom);
        const Film* pointeurFilm18 = gestionnaireFilms.getFilmParNom(film18.nom);
        std::vector<const Film*> filmsEntreAnnees2 = gestionnaireFilms.getFilmsEntreAnnees(1980, 2000);
        std::vector<const Film*> filmsEntreAnnees2Attendus = {pointeurFilm15,
                                                              pointeurFilm16,
                                                              pointeurFilm17,
                                                              pointeurFilm18};
        gestionnaireFilms.supprimerFilm(film16.nom);
        bool filmsEntreAnnees2Valides = filmsEntreAnnees2 == filmsEntreAnnees2Attendus;
        std::vector<const Film*> filmsEntreAnnees3 = gestionnaireFilms.getFilmsEntreAnnees(1980, 2000);
        std::vector<const Film*> filmsEntreAnnees3Attendus = {pointeurFilm15, pointeurFilm17, pointeurFilm18};
        bool filmsEntreAnnees3Valides = filmsEntreAnnees3 == filmsEntreAnnees3Attendus;
        tests.push_back(filmsEntreAnnees1.empty() && filmsEntreAnnees2Valides && filmsEntreAnnees3Valides);
        afficherResultatTest(7, "GestionnaireFilms::getFilmsEntreAnnees", tests.back());

        // Test 8
        gestionnaireFilms = GestionnaireFilms(); // Réinitialiser le gestionnaire de films
        Film film20{"Nom20", Film::Genre::Documentaire, Pays::RoyaumeUni, "Réalisateur", 1970};
        Film film21{"Nom21", Film::Genre::Documentaire, Pays::RoyaumeUni, "Réalisateur", 1970};
        Film film22{"Nom22", Film::Genre::Documentaire, Pays::RoyaumeUni, "Réalisateur", 1970};
        Film film23{"Nom23", Film::Genre::Documentaire, Pays::RoyaumeUni, "Réalisateur", 1970};
        Film film24{"Nom24", Film::Genre::Documentaire, Pays::RoyaumeUni, "Réalisateur", 1970};
        gestionnaireFilms.ajouterFilm(film20);
        gestionnaireFilms.ajouterFilm(film21);
        gestionnaireFilms.ajouterFilm(film22);
        gestionnaireFilms.ajouterFilm(film23);
        gestionnaireFilms.ajouterFilm(film24);
        std::ostringstream stream;
        stream << gestionnaireFilms;
        static const std::string sortieAttendue =
            "Le gestionnaire de films contient 5 films.\n"
            "Affichage par catégories:\n"
            "Genre: Documentaire (5 films):\n"
            "\tNom: Nom20 | Genre: Documentaire | Pays: Royaume-Uni | Réalisateur: Réalisateur | Année: 1970\n"
            "\tNom: Nom21 | Genre: Documentaire | Pays: Royaume-Uni | Réalisateur: Réalisateur | Année: 1970\n"
            "\tNom: Nom22 | Genre: Documentaire | Pays: Royaume-Uni | Réalisateur: Réalisateur | Année: 1970\n"
            "\tNom: Nom23 | Genre: Documentaire | Pays: Royaume-Uni | Réalisateur: Réalisateur | Année: 1970\n"
            "\tNom: Nom24 | Genre: Documentaire | Pays: Royaume-Uni | Réalisateur: Réalisateur | Année: 1970\n";
        tests.push_back(stream.str() == sortieAttendue);
        afficherResultatTest(8, "GestionnaireFilms::operator<<", tests.back());

        // Test 9
        GestionnaireFilms gestionnaireFilms2(gestionnaireFilms);
        std::size_t nombre3 = gestionnaireFilms2.getNombreFilms();
        gestionnaireFilms2.chargerDepuisFichier("films.txt");
        std::size_t nombre4 = gestionnaireFilms2.getNombreFilms();
        tests.push_back(nombre3 == gestionnaireFilms.getNombreFilms() && nombre4 == 331);
        afficherResultatTest(9, "Chargement et copy ctor toujours fonctionnels", tests.back());

        int nombreTestsReussis = static_cast<int>(std::count(tests.begin(), tests.end(), true));
        double totalPointsSection =
            static_cast<double>(nombreTestsReussis) / static_cast<double>(tests.size()) * maxPointsSection;
#else
        std::cout << "[Tests désactivés]\n";
        double totalPointsSection = 0.0;
#endif
        afficherFooterTest(totalPointsSection, maxPointsSection);
        return totalPointsSection;
    }

    /// Teste la classe AnalyseurLogs.
    /// \return Le nombre de points obtenus aux tests.
    double testAnalyseurLogs()
    {
        afficherHeaderTest("AnalyseurLogs");
        static constexpr double maxPointsSection = 2.0;

#if TEST_ANALYSEUR_LOGS_ACTIF
        std::vector<bool> tests;

        GestionnaireUtilisateurs gestionnaireUtilisateurs;
        static constexpr std::size_t nombreUtilisateurs = 10;
        std::array<const Utilisateur*, nombreUtilisateurs> pointeursUtilisateurs;
        for (std::size_t i = 0; i < nombreUtilisateurs; i++)
        {
            std::string idUtilisateur = "prénom.nom." + std::to_string(i + 1) + "@email.com";
            gestionnaireUtilisateurs.ajouterUtilisateur(Utilisateur{idUtilisateur, "Prénom Nom", 20, Pays::Canada});
            pointeursUtilisateurs[i] = gestionnaireUtilisateurs.getUtilisateurParId(idUtilisateur);
        }

        GestionnaireFilms gestionnaireFilms;
        static constexpr std::size_t nombreFilms = 10;
        std::array<const Film*, nombreFilms> pointeursFilms;
        for (std::size_t i = 0; i < nombreFilms; i++)
        {
            std::string nomFilm = "Nom" + std::to_string(i + 1);
            gestionnaireFilms.ajouterFilm(
                Film{nomFilm, Film::Genre::Documentaire, Pays::RoyaumeUni, "Réalisateur", 1970});
            pointeursFilms[i] = gestionnaireFilms.getFilmParNom(nomFilm);
        }

        AnalyseurLogs analyseurLogs;

        // Test 1
        bool creation1 = analyseurLogs.creerLigneLog("2018-01-01T00:00:00Z",
                                                     "inconnu@email.com",
                                                     "Nom1",
                                                     gestionnaireUtilisateurs,
                                                     gestionnaireFilms);
        bool creation2 = analyseurLogs.creerLigneLog("2018-01-01T00:00:00Z",
                                                     "prénom.nom.1@email.com",
                                                     "Inconnu",
                                                     gestionnaireUtilisateurs,
                                                     gestionnaireFilms);
        bool creation3 = analyseurLogs.creerLigneLog("2018-01-01T00:00:00Z",
                                                     "prénom.nom.1@email.com",
                                                     "Nom1",
                                                     gestionnaireUtilisateurs,
                                                     gestionnaireFilms);
        tests.push_back(!creation1 && !creation2 && creation3);
        afficherResultatTest(1, "AnalyseurLogs::creerLigneLog", tests.back());

        // Test 2
        std::vector<LigneLog> logsAjoutes = {
            LigneLog{"2018-01-01T00:00:00Z", pointeursUtilisateurs[3], pointeursFilms[4]},
            LigneLog{"2018-01-01T12:00:00Z", pointeursUtilisateurs[3], pointeursFilms[4]},
            LigneLog{"2018-01-01T11:00:00Z", pointeursUtilisateurs[0], pointeursFilms[4]},
            LigneLog{"2018-01-01T10:00:00Z", pointeursUtilisateurs[1], pointeursFilms[4]},
            LigneLog{"2018-01-01T09:00:00Z", pointeursUtilisateurs[2], pointeursFilms[4]},
            LigneLog{"2018-01-01T08:00:00Z", pointeursUtilisateurs[3], pointeursFilms[8]},
            LigneLog{"2018-01-01T07:00:00Z", pointeursUtilisateurs[4], pointeursFilms[8]},
            LigneLog{"2018-01-01T06:00:00Z", pointeursUtilisateurs[5], pointeursFilms[8]},
            LigneLog{"2018-01-01T05:00:00Z", pointeursUtilisateurs[6], pointeursFilms[9]},
            LigneLog{"2018-01-01T04:00:00Z", pointeursUtilisateurs[7], pointeursFilms[9]},
            LigneLog{"2018-01-01T03:00:00Z", pointeursUtilisateurs[8], pointeursFilms[0]},
            LigneLog{"2018-01-01T02:00:00Z", pointeursUtilisateurs[9], pointeursFilms[1]},
            LigneLog{"2018-01-01T01:00:00Z", pointeursUtilisateurs[1], pointeursFilms[2]},
            LigneLog{"2018-01-01T01:00:00Z", pointeursUtilisateurs[1], pointeursFilms[3]},
            LigneLog{"2018-01-01T01:00:00Z", pointeursUtilisateurs[1], pointeursFilms[4]},
            LigneLog{"2019-01-01T01:00:00Z", pointeursUtilisateurs[3], pointeursFilms[5]},
            LigneLog{"2020-01-01T01:00:00Z", pointeursUtilisateurs[3], pointeursFilms[6]},
            LigneLog{"2019-05-01T01:00:00Z", pointeursUtilisateurs[3], pointeursFilms[7]},
            LigneLog{"2020-05-01T01:00:00Z", pointeursUtilisateurs[3], pointeursFilms[8]},
            LigneLog{"2019-03-01T01:00:00Z", pointeursUtilisateurs[1], pointeursFilms[9]},
        };
        analyseurLogs.logs_.clear();
        for (const auto& ligneLog : logsAjoutes)
        {
            analyseurLogs.ajouterLigneLog(ligneLog);
        }
        bool logsSontOrdonnes =
            std::is_sorted(analyseurLogs.logs_.begin(), analyseurLogs.logs_.end(), ComparateurLog());
        int nombreVuesFilm1 = analyseurLogs.vuesFilms_[pointeursFilms[4]];
        int nombreVuesFilm2 = analyseurLogs.vuesFilms_[pointeursFilms[5]];
        tests.push_back(logsSontOrdonnes && nombreVuesFilm1 == 6 && nombreVuesFilm2 == 1);
        afficherResultatTest(2, "AnalyseurLogs::ajouterLigneLog", tests.back());

        // Test 3
        Film filmInconnu{"Inconnu", Film::Genre::Documentaire, Pays::RoyaumeUni, "Réalisateur", 1970};
        int nombreVuesFilm3 = analyseurLogs.getNombreVuesFilm(&filmInconnu);
        int nombreVuesFilm4 = analyseurLogs.getNombreVuesFilm(nullptr);
        int nombreVuesFilm5 = analyseurLogs.getNombreVuesFilm(pointeursFilms[8]);
        tests.push_back(nombreVuesFilm3 == 0 && nombreVuesFilm4 == 0 && nombreVuesFilm5 == 4);
        afficherResultatTest(3, "AnalyseurLogs::getNombreVuesFilm", tests.back());

        // Test 4
        const Film* filmPlusPopulaire1 = analyseurLogs.getFilmPlusPopulaire();
        AnalyseurLogs temp = analyseurLogs; // Réinitialiser l'analyseur de logs
        analyseurLogs = AnalyseurLogs();
        const Film* filmPlusPopulaire2 = analyseurLogs.getFilmPlusPopulaire();
        analyseurLogs = temp;
        tests.push_back(filmPlusPopulaire1 == pointeursFilms[4] && filmPlusPopulaire2 == nullptr);
        afficherResultatTest(4, "AnalyseurLogs::getFilmPlusPopulaire", tests.back());

        // Test 5
        std::vector<std::pair<const Film*, int>> filmsPlusPopulaires1 = analyseurLogs.getNFilmsPlusPopulaires(3);
        std::vector<std::pair<const Film*, int>> filmsPlusPopulaires1Attendus = {
            std::pair<const Film*, int>(pointeursFilms[4], 6),
            std::pair<const Film*, int>(pointeursFilms[8], 4),
            std::pair<const Film*, int>(pointeursFilms[9], 3),
        };
        std::vector<std::pair<const Film*, int>> filmsPlusPopulaires2 = analyseurLogs.getNFilmsPlusPopulaires(300);
        AnalyseurLogs analyseurLogsVide;
        std::vector<std::pair<const Film*, int>> filmsPlusPopulaires3 = analyseurLogsVide.getNFilmsPlusPopulaires(3);
        tests.push_back(filmsPlusPopulaires1 == filmsPlusPopulaires1Attendus &&
                        filmsPlusPopulaires2.size() == pointeursFilms.size() && filmsPlusPopulaires3.empty());
        afficherResultatTest(5, "AnalyseurLogs::getNFilmsPlusPopulaires", tests.back());

        // Test 6
        Utilisateur utilisateurInconnu{"inconnu@email.com", "Prénom Nom", 20, Pays::Canada};
        std::size_t nombreVuesUtilisateur1 = analyseurLogs.getNombreVuesPourUtilisateur(&utilisateurInconnu);
        std::size_t nombreVuesUtilisateur2 = analyseurLogs.getNombreVuesPourUtilisateur(nullptr);
        std::size_t nombreVuesUtilisateur3 = analyseurLogs.getNombreVuesPourUtilisateur(pointeursUtilisateurs[1]);
        tests.push_back(nombreVuesUtilisateur1 == 0 && nombreVuesUtilisateur2 == 0 && nombreVuesUtilisateur3 == 5);
        afficherResultatTest(6, "AnalyseurLogs::getNombreVuesPourUtilisateur", tests.back());

        // Test 7
        std::vector<const Film*> filmsVus1 = analyseurLogs.getFilmsVusParUtilisateur(&utilisateurInconnu);
        std::vector<const Film*> filmsVus2 = analyseurLogs.getFilmsVusParUtilisateur(nullptr);
        std::vector<const Film*> filmsVus3 = analyseurLogs.getFilmsVusParUtilisateur(pointeursUtilisateurs[3]);
        std::vector<const Film*> filmsVus3Attendus = {
            pointeursFilms[4],
            pointeursFilms[8],
            pointeursFilms[5],
            pointeursFilms[6],
            pointeursFilms[7],
        };
        std::sort(filmsVus3.begin(), filmsVus3.end());
        std::sort(filmsVus3Attendus.begin(), filmsVus3Attendus.end());
        std::vector<const Film*> filmsVus4 = analyseurLogsVide.getFilmsVusParUtilisateur(pointeursUtilisateurs[3]);
        ;
        tests.push_back(filmsVus1.empty() && filmsVus2.empty() && filmsVus3 == filmsVus3Attendus && filmsVus4.empty());
        afficherResultatTest(7, "AnalyseurLogs::getFilmsVusParUtilisateur", tests.back());

        int nombreTestsReussis = static_cast<int>(std::count(tests.begin(), tests.end(), true));
        double totalPointsSection =
            static_cast<double>(nombreTestsReussis) / static_cast<double>(tests.size()) * maxPointsSection;
#else
        std::cout << "[Tests désactivés]\n";
        double totalPointsSection = 0.0;
#endif
        afficherFooterTest(totalPointsSection, maxPointsSection);
        return totalPointsSection;
    }
} // namespace Tests

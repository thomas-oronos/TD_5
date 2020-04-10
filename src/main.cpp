/// Fonction main.
/// \author Misha Krieger-Raynauld
/// \date 2020-01-12

#include <iostream>
#include "AnalyseurLogs.h"
#include "GestionnaireFilms.h"
#include "GestionnaireUtilisateurs.h"
#include "Tests.h"
#include "WindowsUnicodeConsole.h"

int main()
{
    // Change le code page de la console Windows en UTF-8 si l'OS est Windows
    initializeConsole();

    Tests::testAll();

    // Écrivez le code pour le bonus ici
}

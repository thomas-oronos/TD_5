#ifndef FONCTEURS_H
#define FONCTEURS_H

#include "LigneLog.h"

class EstDansIntervalleDatesFilm
{
    public :
    //Constructeur
    EstDansIntervalleDatesFilm(int borneInf, int borneSup) : borneInf_(borneInf), borneSup_(borneSup){};

    bool operator()(const std::unique_ptr<Film>& film)
    {
        return (film->annee >= borneInf_ && film->annee <=borneSup_);
    }

    private :

    int borneInf_;
    int borneSup_;
};

class ComparateurLog
{
    public :
    bool operator()(const LigneLog& ligne1, const LigneLog& ligne2)
    {
        return ligne1.timestamp < ligne2.timestamp;
    };

};

template<typename T1, typename T2>
class ComparateurSecondElementPaire
{
    public :
    bool operator()(const std::pair<T1, T2> paire1, const std::pair<T1, T2> paire2)
    {
        return (paire1.second < paire2.second);
    }

};

#endif 


#ifndef ARETE_H_INCLUDED
#define ARETE_H_INCLUDED
#include <vector>
#include "Sommet.h"

class Arete
{
    private :
        int m_indice;
        int m_id1, m_id2;
        double m_poids;

    public :
        Arete(int indice, int id1, int id2);
        void Successeurs(std::vector<Sommet>& m_sommets, bool oriente);
        void Poids(double poids, int indice);
        void DessinerPoids(Svgfile& index, int indice1, double x1, double y1, int indice2, double x2, double y2);
        void Afficher();
        double Recherche_Poids(double ind1, double ind2, bool oriente);
        void ParcoursConnex(std::vector<int> &Queue, int &compteur);
        bool RechercheIndice(int indice);
        void setArete(int &ind, int &ext1, int &ext2);
        void setPoids(int &ind, double &pds);
        int getIndice();
};

#endif // ARETE_H_INCLUDED

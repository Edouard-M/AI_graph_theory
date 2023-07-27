#include <iostream>
#include <vector>
#include "Arete.h"

Arete::Arete(int indice, int id1, int id2)
{
    m_indice = indice;
    m_id1 = id1;
    m_id2 = id2;
}

void Arete::Successeurs(std::vector<Sommet>& sommets, bool oriente)
{
    for(size_t i=0 ; i < sommets.size() ; i++)
    {
        sommets[i].Successeur(m_id1, m_id2, sommets, oriente);
    }
}

void Arete::Poids(double poids, int indice)
{
    if(indice == m_indice)
        m_poids = poids;
}

///Affiche le poids au milieu des arêtes
void Arete::DessinerPoids(Svgfile& index, int indice1, double x1, double y1, int indice2, double x2, double y2)
{
    x1=x1*100; y1=y1*100; x2=x2*100; y2=y2*100;
    if((m_id1 == indice1) && (m_id2==indice2))
    {
        //index.addDisk( x1+(x2-x1)/2, y1+(y2-y1)/2, 10,"white");
        //index.addText( x1+(x2-x1)/2 -4, y1+(y2-y1)/2 +4, m_poids, "grey");
        index.addText( x1+(x2-x1)/2 -4, y1+(y2-y1)/2 -8, m_poids, "black");
    }

}

void Arete::Afficher()
{
    std::cout << "indice : " << m_indice << " id1 : " << m_id1 << " id2 : " << m_id2 << " Poids : " << m_poids << std::endl;
}

double Arete::Recherche_Poids(double ind1, double ind2, bool oriente)
{
    double poids=0;
    if(oriente == true)
        if((m_id1 == ind1) && (m_id2 == ind2))
            poids = m_poids;
    if(oriente == false)
    {
        if(((m_id1 == ind1) && (m_id2 == ind2)) || ((m_id1 == ind2) && (m_id2 == ind1)))
        {
            poids = m_poids;
        }

    }

    return poids;
}

///
void Arete::ParcoursConnex(std::vector<int> &Queue, int &compteur)
{
    bool present1=0, present2=0;

    for(size_t j=0;j<Queue.size();++j)
    {
        if(m_id1==Queue[j])   ///Si le sommet est parcouru on ne l'ajoute pas
            present1=1;

        if(m_id2==Queue[j])   ///Si le sommet est parcouru on ne l'ajoute pas
            present2=1;
    }

    ///Si un des sommets est parcouru et son adjacence n'est pas marqué, on l'ajoute
    if(present1!=1 && present2==1)
    {
        Queue.push_back(m_id1);
        compteur++;
    }
    else if(present1==1 && present2!=1)
    {
        Queue.push_back(m_id2);
        compteur++;
    }
}

bool Arete::RechercheIndice(int indice)
{
    if(m_indice==indice)
        return true;
    else return false;
}

void Arete::setArete(int &ind, int &ext1, int &ext2)
{
    ind=m_indice;
    ext1=m_id1;
    ext2=m_id2;
}

void Arete::setPoids(int &ind, double &pds)
{
    ind=m_indice;
    pds=m_poids;
}

int Arete::getIndice()
{
    return m_indice;
}

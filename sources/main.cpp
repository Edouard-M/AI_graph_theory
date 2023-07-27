
/// MIGNIEN Edouard
/// BERNARD Pauline
#include <iostream>
#include <vector>
#include <fstream>
#include <math.h>
#include <time.h>
#include "svgfile.h"
#include "Arete.h"
#include "Sommet.h"
#include "Graph.h"

///Sources :
///Dégradé de couleur SVG : https://la-cascade.io/les-degrades-svg/
///FIchiers SVG issus des TP précédents
///Flèches orientées : http://xymaths.free.fr/Informatique-Programmation/javascript/canvas-dessin-fleche.php (3 lignes)
///Sous programme d'arrondi de valeur : https://www.developpez.net/forums/d29018/c-cpp/cpp/arrondir-chiffre-2-chiffres-apres-virgule/ (2 lignes)
///Dijkstra est identique à l'algorithme Dijkstra rendu au Tp 3, codé entièrement par nous même, sans source

std::vector<double> RechercheSommetVec(int indice, std::vector<std::vector<double>>vec)
{
    double ind = indice;
    int position=0;
    for(size_t i=0 ; i < vec.size() ; i++)
    {
        if(vec[i][0] == ind)
            position = i;
    }
    return vec[position];
}

bool OuvertureFichier(std::string fichiern)
{
    bool ouverture = false;
    std::ifstream fichier(fichiern);
    if(fichier)
        ouverture = true;
    return ouverture;
}

///Reinitialise un graphe
void Update_Univers(std::vector<Graph>&Univers)
{
    if(Univers.size() > 0)
        Univers.pop_back();
    Graph G(1, 5, 4);
    Univers.push_back(G);
}

int main()
{
    std::vector<Graph>Univers;
    std::vector<std::vector<double>> IndicesPrec,  NIndicesPrec;
    int indice=0;
    //int choix_m=0;
    int page = 1;
    bool cc1 = false, cc2 = false , cc3 = false , cc4 = false;
    bool CVP=false, CD=false, CP=false, CI=false, NCD=false, NCP=false, NCI=false, DIFF=false, mexico=false;
    std::string fichier;
    srand(time(NULL));
    do{
    std::cout << std::endl << "                                                 ( Please wait a few seconds )   " << std::endl;

    /*
    do{
    std::cout << std::endl;
    std::cout <<  "                              ------------------ MENU ------------------" << std::endl
              << "                              1-          Charger un Graphe           -1" << std::endl
              << "                              2-     Charger systeme de ponderation   -2" << std::endl
              << "                              3-     Calculer indices de centralite   -3" << std::endl
              << "                              4-        Tester la vulnerabilite       -4" << std::endl
              << "                              5-     Affichage Console des Indices    -5" << std::endl
              << "                              6-             K Connexite              -6" << std::endl
              << "                              0-               Arreter                -0" << std::endl << std::endl
              << "                                               -> ";
    */
    ///do{
    ///std::cin >> choix_m;
    ///}
    ///while (choix_m != 0 && choix_m != 1 && choix_m != 2 && choix_m != 3 && choix_m != 4 && choix_m!= 5 && choix_m != 6);

    Update_Univers(Univers);
    system("CLS");
    std::cout << std::endl << "                                                 ( Please wait a few seconds )   " << std::endl;
    std::cout <<              "                                                              10%                " << std::endl;
    Univers[0].Menu1(fichier, CVP, CD, CP, CI, NCD, NCP, NCI, IndicesPrec,  NIndicesPrec, DIFF, indice, mexico, cc1, cc2, cc3, cc4, page);
    system("CLS");
    std::cout << std::endl << "                                                 ( Please wait a few seconds )   " << std::endl;
    std::cout <<              "                                                              90%                " << std::endl;
    Univers[0].Sauvegarder(1);
    Univers[0].Sauvegarder_Ponderation(1);


    if(Univers.size()>0)
           Univers[0].Menu3(CVP, CD, CP, CI, NCD, NCP, NCI, IndicesPrec,  NIndicesPrec, DIFF, indice, mexico, cc1, cc2, cc3, cc4, page);

    }while(page==1 || page==2);
    /*
    switch(choix_m)
    {
    case 1 :
        {
        Update_Univers(Univers);
        Univers[0].Menu1(fichier, CVP, CD, CP, CI, NCD, NCP, NCI, IndicesPrec,  NIndicesPrec, DIFF, indice, mexico);
        Univers[0].Sauvegarder(1);
        Univers[0].Sauvegarder_Ponderation(1);
        }
        break;
    case 2 :
        {
        Update_Univers(Univers);
        if(Univers.size()>0)
            Univers[0].Menu2(fichier, CVP, CD, CP, CI, NCD, NCP, NCI, IndicesPrec,  NIndicesPrec, DIFF, indice, mexico);
        }
        break;
    case 3 :
        {
        if(Univers.size()>0)
           Univers[0].Menu3(CVP, CD, CP, CI, NCD, NCP, NCI, IndicesPrec,  NIndicesPrec, DIFF, indice, mexico);
        }
        break;
    case 4 :
        {
            if(Univers.size()>0)
            {
                std::cout << "                                     ---------Supprimer---------" << std::endl;
                std::cout << "                                    1-   Supprimer une arete   -1" << std::endl;
                std::cout << "                                    2- Afficher les connexites -2" << std::endl;
                std::cout << "                                                                 " << std::endl;
                std::cout << "                                                 > ";

                do
                std::cin >> choix_m;
                while (choix_m != 1 && choix_m != 2);

                if(choix_m==1)
                {
                    ///Sauvegarde du graphe + Centralites avant suppression d'une arete ("precedent")
                    Univers[0].Sauvegarder(0);
                    Univers[0].SaveComparaison();
                    Univers[0].Sauvegarder_Ponderation(0);
                    Univers[0].SupprimerArete();    ///Suppression de l'arete
                    Univers[0].Sauvegarder(1);  ///Sauvegarde du graphe actuel
                    Univers[0].Sauvegarder_Ponderation(1);
                    Update_Univers(Univers);    ///Reinitialisation du graphe
                    Univers[0].Chargement("GrapheActuel.txt");  ///On ouvre le graphe + centralites apres suppression ("actuel")
                    Univers[0].Chargement_Ponderation("PonderationActuelle.txt");
                    Univers[0].ChargerComparaison(IndicesPrec,  NIndicesPrec);
                    Univers[0].Calcul();    ///Calcul des successeurs + centralites + normalisation
                    Univers[0].Dessiner(CVP, CD, CP, CI, NCD, NCP, NCI, IndicesPrec,  NIndicesPrec, DIFF, indice, mexico);  ///Sortie en svg
                }
                else Univers[0].Recherche_Connexite();
            }
        }
        break;
    case 5 :
        if(Univers.size()>0)
           Univers[0].AfficherTout();
        break;
    case 6 :
        Univers[0].KConnexite();
        break;
    }
    */
   // }while(choix_m != 0);

    return 0;
}



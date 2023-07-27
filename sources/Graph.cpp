#include <iostream>
#include <vector>
#include "Graph.h"
#include "math.h"
#include <fstream>

Graph::Graph(bool oriente, int ordre, int taille)
{
    m_oriente = oriente;
    m_ordre = ordre;
    m_taille = taille;
}

void Graph::AddOriente_Ordre_Taille(bool oriente, int ordre, int taille)
{
    m_oriente = oriente;
    m_ordre = ordre;
    m_taille = taille;

    std::vector<double> vec;

    for(int i=0 ; i < m_ordre ; i++)
    {
        vec.push_back(-1);
    }

    for(int i=0 ; i < m_ordre ; i++)
    {
        m_distances.push_back(vec);
    }
}

///Ajoute un sommet dans la liste de sommets du graphe
void Graph::AddSommet(int indice, char nom, double x, double y)
{
    Sommet s(indice, nom, x, y);
    m_sommets.push_back(s);
}

void Graph::AddArete(int indice, int id1, int id2)
{
    Arete a(indice, id1, id2);
    m_aretes.push_back(a);
}

void Graph::Successeurs()
{
    for(size_t i=0 ; i < m_aretes.size() ; i++)
    {
        m_aretes[i].Successeurs(m_sommets, m_oriente);
    }
}

void Graph::Dessiner(bool& CVP, bool& CD, bool& CP, bool& CI, bool& N_CD, bool& N_CP, bool& N_CI, std::vector<std::vector<double>> IndicesPrec, std::vector<std::vector<double>> NIndicesPrec, bool DIFF, int& indice, bool mexico, bool& cc1, bool& cc2, bool& cc3, bool& cc4, int& page)
{
    Svgfile index;
    if(mexico == false && page != 2)
        index.addGrid(100, 1, "grey");
    for(size_t i=0 ; i < m_sommets.size() ; i++)
    {
        m_sommets[i].Dessiner(index, m_oriente, m_aretes, CVP, CD, CP, CI, m_pondere, N_CD, N_CP, N_CI, IndicesPrec, NIndicesPrec, DIFF, m_sommets, indice, mexico, cc1, cc2, cc3, cc4, page);
    }
}

void Graph::Afficher()
{
    std::cout << "Aretes Liste : " << std::endl;
    for(size_t i=0 ; i < m_aretes.size() ; i++)
    {
        m_aretes[i].Afficher();
    }

    std::cout << "Sommets Liste : " << std::endl;
    for(size_t i=0 ; i < m_sommets.size() ; i++)
    {
        m_sommets[i].Afficher();
    }
}

void Graph::Chargement(std::string nomF)
{
    int oriente, ordre, taille;
    int indice;
    double x, y;
    char nom;
    bool b_oriente;
    std::ifstream fichier(nomF);
    if(fichier)
    {
        fichier >> oriente;
        if(oriente == 0)
            b_oriente = false;
        if(oriente == 1)
            b_oriente = true;

        fichier >> ordre;
        for(int i=0 ; i < ordre ; i++)
        {
            fichier >> indice >> nom >> x >> y;
            //std::cout << "Indice : " << indice << " Nom : " << nom << " x : " << x << " y : " << y << std::endl;
            AddSommet(indice, nom, x, y);
        }
        fichier >> taille;

        int ind, ext1, ext2;
        for(int i=0 ; i < taille ; i++)
        {
            fichier >> ind >> ext1 >> ext2;
            AddArete(ind, ext1, ext2);
        }

        AddOriente_Ordre_Taille( b_oriente, ordre, taille);
    }
    else
        std::cout << "Probleme ouverture fichier" <<std::endl;
}

void Graph::Chargement_Ponderation(std::string nomF)
{
    std::ifstream fichier(nomF);
    if(fichier)
    {
        int taille, indice;
        double poids;
        fichier >> taille;
        for(int i=0 ; i < taille ; i++)
        {
            fichier >> indice >> poids;
            for(size_t j=0 ; j < m_aretes.size() ; j++)
            {
                m_aretes[j].Poids( poids, indice);
            }
        }
    }
    else
        std::cout << "Probleme ouverture fichier" <<std::endl;
}

///Calcul de la centralite par degre
void Graph::Centralite_Degre()
{
    for(size_t i=0 ; i < m_sommets.size() ; i++ )
    {
        m_sommets[i].Centralite_Degre(m_ordre);
    }
}

///Calcul de la centralite par vecteur propre
void Graph::Centralite_Vecteur_Propre()
{
    double lambda = 0, lambda_prec = 0;

    do
    {
        lambda_prec=lambda;
        std::vector<double> Vec_Csi;
        lambda = 0;
        for(size_t i=0 ; i < m_sommets.size() ; i++)
        {
            lambda += (m_sommets[i].Csi())*(m_sommets[i].Csi());
            Vec_Csi.push_back(m_sommets[i].Csi());
        }
        lambda = sqrt(lambda);

        for(size_t i=0 ; i < m_sommets.size() ; i++)
        {
            m_sommets[i].Recalcul_Indices(lambda, Vec_Csi[i]);
        }
    }
    while( (lambda < (lambda_prec-0.01)) || (lambda > (lambda_prec +0.01)) );

    //std::cout << "Lambda : " << lambda << std::endl;
}

///Cherche et affiche les sommets connexes
void Graph::Recherche_Connexite()
{
    int nbConnex=0, compteur=1, compteur2=0, prochain=0, present;
    std::vector<int> ListeConnex;

    std::cout << std::endl;

    do
    {
        compteur=1;

        for(size_t i=0; i<m_sommets.size(); ++i) ///Recherche si il existe un sommet non marque
        {
            present=0;

            for(size_t j=0; j<ListeConnex.size(); ++j)
            {
                if(ListeConnex[j]==m_sommets[i].get_indice())
                {
                    present=1;
                    break;
                }
            }
            if(present!=1)
            {
                prochain=m_sommets[i].get_indice();
                break;
            }
        }

        ListeConnex.push_back(prochain);

        for(size_t i=0; i<m_aretes.size()/2; ++i) ///Le refaire plusieurs fois
        {
            for(size_t j=0; j<m_aretes.size(); ++j) ///Recherche si l'arete a deja ete parcouru
            {
                m_aretes[j].ParcoursConnex(ListeConnex,compteur);
            }
        }

        nbConnex++;

        ///Affichage
        std::cout << "                                     ---------Supprimer---------" << std::endl;
        std::cout << "                                   --- " << nbConnex << " Composante(s) connexe ---" << std::endl;
        std::cout << "                                       > ";
        for(size_t i=compteur2; i<ListeConnex.size(); ++i)
            std::cout << m_sommets[ListeConnex[i]].getNom() << " ";
        std::cout << std::endl;

        compteur2+=compteur;
    }
    while(compteur2<m_ordre);
    std::cout << std::endl;
}

///Suppression d'une arete
void Graph::SupprimerArete()
{
    int choix, indice, found=0;
    int id1=0,id2=0;
    size_t i=0;

    std::cout << "                                     ---------Supprimer---------" << std::endl;
    std::cout << "                                    1-    Une certaine arete   -1" << std::endl;
    std::cout << "                                    2-   Une arete au hasard   -2" << std::endl;
    std::cout << "                                                                 " << std::endl;
    std::cout << "                                                 > ";

    do
        std::cin >> choix;
    while(choix<1 || choix>2);

    if(choix==1)
    {
        std::cout << "                                          Saisir son indice : ";
        std::cin >> indice;

        for(size_t i=0; i<m_aretes.size(); ++i)
        {
            if(m_aretes[i].RechercheIndice(indice)==true)
            {
                found=1;
                m_aretes[i].setArete(indice,id1,id2);
                indice=i;
                break;
            }
        }
    }
    else if(choix==2)
    {
        i=rand()%m_aretes[m_aretes.size()-1].getIndice();
        m_aretes[i].setArete(indice,id1,id2);
        indice=i;
        found=1;
    }

    if(found==0)
        std::cout << "\nAucune arete ne correspond a cette indice\n\n";
    else if(found==1)
    {
        ///Suppression des successeurs
        if(m_oriente==false)
        {
            m_sommets[id1].SuppSommet(id2);
            m_sommets[id2].SuppSommet(id1);
        }
        else if(m_oriente==true)
            m_sommets[id1].SuppSommet(id2);

        ///Suppression des aretes
        m_aretes.erase(m_aretes.begin()+indice);
        m_aretes.shrink_to_fit();
        std::cout << "                                         Arete no "<< indice <<" supprimee        " << std::endl;
        m_taille-=1;
    }
}



void Graph::Dijkstra(int s_depart)
{
    Sommet* s = RechercheSommet(s_depart);
    s->Marquage();
    double distance = 0;
    bool impossible = s->TestNombreSuccesseurs();;
    std::vector<double>distances;
    std::vector<Sommet*>sommetprec;
    std::vector<Sommet*>sommets;

    for(int i=0 ; i < m_ordre ; i++)
    {
        /// ///////
        //std::cout << "///////////" << std::endl;
        //std::cout << "  Etape : " << i+1 << std::endl;
        /// ///////
        s->Dijkstra(distances, sommetprec, sommets, distance, m_aretes, s, m_oriente);
        int minn=10000000;
        int s_position=0;
        for(size_t k=0 ; k < distances.size() ; k++)
        {
            if(minn > distances[k] && sommets[k]->Marque()==false)
            {
                minn=distances[k];
                s_position=k;
            }
        }
        /// ///////////
        /* for(size_t l=0 ; l < sommetprec.size() ; l++)
         {
             std::cout << "Ind sommet_prec : " << sommetprec[l]->get_indice() << std::endl;
         }
         for(size_t l=0 ; l < sommets.size() ; l++)
         {
             std::cout << "Ind sommets : " << sommets[l]->get_indice() << std::endl;
         }
         for(size_t l=0 ; l < distances.size() ; l++)
         {
             std::cout << "Ind distances : " << distances[l] << std::endl;
         }
         /// //////////*/


        if(impossible == true)
            s->Fini(0, NULL);
        if(impossible == false)
            s=sommets[s_position];
        /*if(s->Marque() == false)
            s->Marquage();*/
        //std::cout << "MARQUAGE : " << s->get_indice() << std::endl;
        //std::cout <<  std::endl;
        if(impossible == false && s->Marque() == false)
        {
            s->Fini(distances[s_position], sommetprec[s_position]);
            //  std::cout << "MARQUAGE : " << s->get_indice() << std::endl;
            //  std::cout <<  std::endl;
        }


        // //////////////
        for(size_t l=0 ; l < distances.size() ; l++)
        {
            for(size_t m=0 ; m < distances.size() ; m++)
            {
                if(m!=l)
                    if(distances[l] == distances[m])
                    {
                        if(sommets[m]->get_indice() == sommets[l]->get_indice())
                        {
                            /* std::cout << "DOUBLE PCC : distances : " << distances[l] << " | Indice : " << sommets[m]->get_indice() << std::endl;
                             std::cout << "(" << sommetprec[m]->get_indice() << "," << sommets[m]->get_indice() << ")" << std::endl;
                             std::cout << "(" << sommetprec[l]->get_indice() << "," << sommets[l]->get_indice() << ")" << std::endl;
                             *//*do{
     prec = sommetprec[m-1];
     prec2 = sommetprec[l-1];
 }while( prec->get_indice() != prec2->get_indice() );*/
                            sommets[m]->AddPccPrecedent(sommetprec[m]);
                            sommets[l]->AddPccPrecedent(sommetprec[l]);

                        }
                    }
            }
        }
        // //////////////


        if(s->Marque() == false)
            s->Marquage();
        if(impossible == false)
            distance = distances[s_position];
        if(distances.size()>0)
        {
            distances.erase(distances.begin()+s_position);
            sommetprec.erase(sommetprec.begin()+s_position);
            sommets.erase(sommets.begin()+s_position);
        }
    }
}

Sommet* Graph::RechercheSommet(int num)
{
    Sommet* sommet;
    for(size_t i=0 ; i < m_sommets.size() ; i++)
    {
        if(m_sommets[i].RechercheSommet(num) == true)
            sommet=&m_sommets[i];
    }
    return sommet;
}


void Graph::AfficherParcoursDijkstra(int s_arrivee, double& d)
{
    Sommet* s;
    std::vector<double>longueur;
    s=RechercheSommet(s_arrivee);
//    if(s->Get_precedent()!=NULL)
//        std::cout << s->get_indice();
    double poids=0;
    double poids2=0;
    while(s->Get_precedent()!=NULL)
    {
        //longueur.push_back(RechercheArete(s->get_indice(), s->Get_precedent()->get_indice())->Get_poids());
        ///longueur.push_back(RechercheArete(s->Get_precedent()->get_indice(), s->get_indice())->Get_poids());
        for(size_t i=0 ; i < m_aretes.size() ; i++)
        {
            poids2 = m_aretes[i].Recherche_Poids( s->Get_precedent()->get_indice(), s->get_indice(), m_oriente);
            if(poids2 != 0)
                poids = poids2;
        }
        longueur.push_back(poids);


        //std::cout << " <-- " << s->Get_precedent()->get_indice();
        s=s->Get_precedent();
    }
    if(longueur.size() == 0)
    {
        //std::cout << "Impossible" << std::endl;
    }
    else
    {
        //std::cout << " : longueur " << longueur[0] ;
        double somme=longueur[0];
        for(size_t i=1 ; i < longueur.size() ; i++)
        {
            //std::cout << " + " << longueur[i];
            somme+=longueur[i];
        }
        //std::cout << " = " << somme <<std::endl;
        d=somme;
    }
}

void Graph::Reset_Dijkstra()
{
    for(size_t i = 0 ; i < m_sommets.size() ; i++)
    {
        m_sommets[i].Reset_Dijkstra();
    }
}



void Graph::Auto_Dijkstra()
{
    double distance=0;
    double Cp=0;
    double sommeTotale=0;


    for(size_t i = 0 ; i < m_sommets.size() ; i++)
    {
        Reset_Dijkstra();
        Dijkstra(m_sommets[i].get_indice());
        double loading = ((i * 40)/m_sommets.size() ) +40;
        /*    system("CLS");
    std::cout << std::endl << "                                                 ( Please wait a few seconds )   " << std::endl;
    std::cout <<              "                                                              " << loading << "%                " << std::endl;
        */
        for(size_t j = 0 ; j < m_sommets.size() ; j++)
        {
            distance=0;
            AfficherParcoursDijkstra(m_sommets[j].get_indice(), distance);

            m_distances[i][j]=distance;
            sommeTotale+=distance;
            Cp+=distance;
        }
        Cp=1/Cp;
        m_sommets[i].AddCp(Cp, m_ordre);
        Cp=0;

        /// ////
        Test(&m_sommets[i]);

        //AfficherPcc(&m_sommets[i]);
        /// ////

    }
}


void Graph::Test(Sommet* s_depart)
{
    for(size_t i=0 ; i < m_sommets.size() ; i++)
    {
        m_sommets[i].CalculPccPrec(s_depart, m_aretes, m_oriente);
        /// ////
        for(size_t k=0 ; k < m_sommets.size() ; k++)
        {
            m_sommets[i].CalculPcci( &m_sommets[k], s_depart, m_oriente);
        }
        /// ////
    }
}



void Graph::AfficherPcc(Sommet* s_depart)
{
    for(size_t i=0 ; i < m_sommets.size() ; i++)
    {
        m_sommets[i].AfficherPcc(s_depart);
        std::cout << "--------------------------------------------------" << std::endl;
    }
}

void Graph::CalculPcci(Sommet* s_arrive)
{
    for(size_t i=0 ; i < m_sommets.size() ; i++)
    {
        //CalculPcci( m_sommets[i], s_arrive);
    }
}

void Graph::Normaliser()
{
    for(size_t i=0 ; i < m_sommets.size() ; i++)
    {
        m_sommets[i].Normaliser(m_ordre);
    }
}

///Graphe "Par défaut" (Pour eviter de taper leur nom systematiquement)
void Graph::Memoire(std::string& fichier, int& num)
{
    if(fichier == "1")
    {
        num = 1;
        fichier = "data/graphe-topo.txt";
    }
    if(fichier == "2")
    {
        num = 2;
        fichier = "data/graphe-topo2.txt";
    }
    if(fichier == "3")
    {
        num = 3;
        fichier = "data/graphe-topo3.txt";
    }
    if(fichier == "4")
    {
        num = 4;
        fichier = "data/graphe-topo4.txt";
    }
}

///Ponderations de graphe "Par défaut"
void Graph::Memoire_Ponderation(std::string& fichier, int num)
{
    if(num == 1)
        fichier = "data/graphe-topo-ponderation.txt";

    if(num == 2)
        fichier = "data/graphe-topo2-ponderation.txt";

    if(num == 3)
        fichier = "data/graphe-topo3-ponderation.txt";

    if(num == 4)
        fichier = "data/graphe-topo4-ponderation.txt";
}

bool OuvertureFichier(std::string fichiern);

void Graph::Menu1(std::string& fichierG, bool& CVP, bool& CD, bool& CP, bool& CI, bool& NCD, bool& NCP, bool& NCI, std::vector<std::vector<double>> IndicesPrec, std::vector<std::vector<double>> NIndicesPrec, bool& DIFF, int& indice, bool& mexico, bool& cc1, bool& cc2, bool& cc3, bool& cc4,int& page)
{
    int num=0;
    std::string fichier;
    do
    {
        ///std::cout << "                                    Saisir le nom du fichier : ";
        ///std::cin >> fichier;
        //fichier = "Mexico.txt";
            fichier = "data/Mexico.txt";
        if(page == 2)
            fichier = "data/Mexico_Train.txt";
    }
    while(OuvertureFichier(fichier) == false && fichier != "1" && fichier != "2" && fichier != "3" && fichier != "4");
    if(fichier == "data/Mexico.txt")
        mexico = true;
    else
        mexico = false;
    Memoire(fichier, num);
    fichierG=fichier;
    Chargement(fichier);


    ///std::cout << "                                        Charger ponderation ?" << std::endl
    ///          << "                                        NON : 0  | |  1 : OUI " << std::endl;
    int choix=0;
    do
    {
        ///std::cout << "                                               -> ";
        ///std::cin >> choix;
        choix = 1;
    }
    while(choix != 0 && choix != 1);

    if(num == 0)
        if(choix == 1)
        {
            do
            {
                ///std::cout << "                                   Saisir le nom du fichier : ";
                ///std::cin >> fichier;
                    fichier = "data/Pond-Mexico.txt";
                if(page == 2)
                    fichier = "data/Pond_Mexico_Train.txt";
            }
            while(OuvertureFichier(fichier) == false);
            Chargement_Ponderation(fichier);
            m_pondere = true;
        }

    if(num != 0)
        if( choix == 1)
        {
            Memoire_Ponderation(fichier, num);
            Chargement_Ponderation(fichier);
            m_pondere = true;
        }

    if(choix == 0)
    {
        m_pondere = false;
        for(size_t j=0 ; j < m_aretes.size() ; j++)
        {
            m_aretes[j].Poids( 1, j);
        }
    }
    Calcul();
    system("CLS");
    std::cout << std::endl << "                                                 ( Please wait a few seconds )   " << std::endl;
    std::cout <<              "                                                              80%                " << std::endl;
    Dessiner(CVP, CD, CP, CI, NCD, NCP, NCI, IndicesPrec, NIndicesPrec, DIFF, indice, mexico, cc1, cc2, cc3, cc4, page);
}

void Graph::Menu2(std::string fichierG, bool& CVP, bool& CD, bool& CP, bool& CI, bool& NCD, bool& NCP, bool& NCI, std::vector<std::vector<double>> IndicesPrec, std::vector<std::vector<double>> NIndicesPrec, bool& DIFF, int& indice, bool& mexico, bool& cc1, bool& cc2, bool& cc3, bool& cc4, int& page)
{
    Chargement("data/GrapheActuel.txt");

    std::string fichierP;
    do
    {
        std::cout << "                                   Saisir le nom du fichier : " ;
        std::cin >> fichierP;
    }
    while(OuvertureFichier(fichierP) == false);


    Chargement_Ponderation(fichierP);
    m_pondere=true;

    Calcul();
    Dessiner(CVP, CD, CP, CI, NCD, NCP, NCI, IndicesPrec, NIndicesPrec, DIFF, indice, mexico, cc1, cc2, cc3, cc4, page);
}

///Calcul des successeurs + centralites + normalisation
void Graph::Calcul()
{
    Successeurs();
    Centralite_Degre();
    Centralite_Vecteur_Propre();
        system("CLS");
    std::cout << std::endl << "                                                 ( Please wait a few seconds )   " << std::endl;
    std::cout <<              "                                                              40%                " << std::endl;
    Auto_Dijkstra();
        system("CLS");
    std::cout << std::endl << "                                                 ( Please wait a few seconds )   " << std::endl;
    std::cout <<              "                                                              70%                " << std::endl;
    Normaliser();
}

void Graph::Menu3(bool& CVP, bool& CD, bool& CP, bool& CI, bool& NCD, bool& NCP, bool& NCI, std::vector<std::vector<double>> IndicesPrec, std::vector<std::vector<double>> NIndicesPrec, bool& DIFF, int& indice, bool& mexico, bool& cc1, bool& cc2, bool& cc3, bool& cc4, int& page)
{
    Dessiner( CVP, CD, CP, CI, NCD, NCP, NCI, IndicesPrec, NIndicesPrec, DIFF, indice, mexico, cc1, cc2, cc3, cc4, page);
    std::string line = "vide";
    system("CLS");
do
    {
    std::cout << "                                                                                          " << std::endl;
    std::cout << "                                    ( Don't forget to open 'output.svg' to see the graph )" << std::endl;
    std::cout << "                                                                                          " << std::endl;
    std::cout << "                                                                                          " << std::endl;
    std::cout << "                                                                                          " << std::endl;
    if(page == 1)
    std::cout << "                                                   GRAPH OF THE SUBWAY OF MEXICO          " << std::endl;
    if(page == 2)
    std::cout << "                                                   GRAPH OF THE TRAIN OF MEXICO           " << std::endl;
    std::cout << "                                                                                          " << std::endl;
    std::cout << "                                                                                          " << std::endl;
    std::cout << "                                                      LIST Of THE COMMANDS                " << std::endl;
    std::cout << "                                     |---------------------------------------------------|" << std::endl;
    std::cout << "                                     |   MODE 1   |   MODE 2   |   MODE 3   |   MODE 4   |" << std::endl;
    std::cout << "                |--------------------|------------|------------|------------|------------|" << std::endl;
    std::cout << "                |                    |            |            |            |            |" << std::endl;
    std::cout << "                |  To Display Color  |     <1>    |     <2>    |     <3>    |     <4>    |" << std::endl;
    std::cout << "                |                    |            |            |            |            |" << std::endl;
    std::cout << "                |  To Display Value  |     <V1>   |     <V2>   |    <V3>    |    <V4>    |" << std::endl;
    std::cout << "                |                    |            |            |            |            |" << std::endl;
    std::cout << "                |--------------------|-------------------------|-------------------------|" << std::endl;
    std::cout << "                | To Change the page |           <P1>          or          <P2>          |" << std::endl;
    std::cout << "                |--------------------|-------------------------|-------------------------|" << std::endl;
    std::cout << "                |      To exit :     |                       <exit>                       " << std::endl;
    std::cout << "" << std::endl;
  //std::cout << "                                     |  Value CD  |  Value CVP |  Value CP  |  Value CI  |" << std::endl;
  //std::cout << "                                     |                      DIFF                     |" << std::endl;


        ///cc1=false;
        ///cc2=false;
        ///cc3=false;
        ///cc4=false;
        std::cout << "                                                           > ";
        getline(std::cin, line);
    system("CLS");
        if(line=="P1")
        {
            page = 1;
            line = "stop";
            system("CLS");
        }
        if(line=="P2")
        {
            page = 2;
            line = "stop";
            system("CLS");
        }

        if(line == "exit" || line == "return" || line=="quit")
            page=0;

        NCD = true;
        NCP = true;
        NCI = true;

        if(line == "K")
            KConnexite();


        if(line == "1"){
            line = "NCCD";
            cc1 = true;
            cc2 = false;
            cc3 = false;
            cc4 = false;
        }
        if(line == "2"){
            line = "CCVP";
            cc2 = true;
            cc1 = false;
            cc3 = false;
            cc4 = false;
        }
        if(line == "3"){
            line = "NCCP";
            cc3 = true;
            cc1 = false;
            cc2 = false;
            cc4 = false;
        }
        if(line == "4"){
            line = "NCCI";
            cc4 = true;
            cc1 = false;
            cc2 = false;
            cc3 = false;
        }

        if(line == "V1")
            line = "CD";
        if(line == "V2")
            line = "CVP";
        if(line == "V3")
            line = "CP";
        if(line == "V4")
            line = "CI";

        if(line == "exit")
            line = "return";

        if(line == "NO COLOR")
            indice=0;
        if(line == "CCD")
            indice=1;
        if(line == "CCVP")
            indice=2;
        if(line == "CCP")
            indice=3;
        if(line == "CCI")
            indice=4;
        if(line == "NCCD" || line == "1")
            indice=5;
        if(line == "NCCP" || line == "3")
            indice=6;
        if(line == "NCCI" || line == "4")
            indice=7;
        if(line == "NO COLOR" || line == "CCD" || line == "CCVP" || line == "CCP" || line == "CCI" || line == "NCCD" || line == "NCCP" || line == "NCCI")
            line = "dessin";


        if(line == "DIFF")
        {
            if(DIFF == false && line == "DIFF")
            {
                DIFF = true;
                line = "dessin";
            }
            if(DIFF == true && line == "DIFF")
            {
                DIFF = false;
                line = "dessin";
            }
        }

        if(line == "ALL")
        {
            CD = true;
            CVP = true;
            CP = true;
            CI = true;
            line = "dessin";
        }
        if(line == "NALL")
        {
            NCD = true;
            NCP = true;
            NCI = true;
            line = "dessin";
        }

        if(line == "CD")
        {
            if(CD == true && line =="CD")
            {
                CD=false;
                line = "dessin";
            }
            if(CD == false && line =="CD")
            {
                CD=true;
                line = "dessin";
            }
        }
        ///
        if(line == "CVP")
        {
            if(CVP == true && line =="CVP")
            {
                CVP=false;
                line = "dessin";
            }
            if(CVP == false && line =="CVP")
            {
                CVP=true;
                line = "dessin";
            }
        }
        ///
        if(line == "CP")
        {
            if(CP == true && line =="CP")
            {
                CP=false;
                line = "dessin";
            }
            if(CP == false && line =="CP")
            {
                CP=true;
                line = "dessin";
            }
        }
        ///
        if(line == "CI")
        {
            if(CI == true && line =="CI")
            {
                CI=false;
                line = "dessin";
            }
            if(CI == false && line =="CI")
            {
                CI=true;
                line = "dessin";
            }
        }
        ///
        ///
        if(line == "NCD")
        {
            if(NCD == true && line =="NCD")
            {
                NCD=false;
                line = "dessin";
            }
            if(NCD == false && line =="NCD")
            {
                NCD=true;
                line = "dessin";
            }
        }
        ///
        ///
        if(line == "NCP")
        {
            if(NCP == true && line =="NCP")
            {
                NCP=false;
                line = "dessin";
            }
            if(NCP == false && line =="NCP")
            {
                NCP=true;
                line = "dessin";
            }
        }
        ///
        if(line == "NCI")
        {
            if(NCI == true && line =="NCI")
            {
                NCI=false;
                line = "dessin";
            }
            if(NCI == false && line =="NCI")
            {
                NCI=true;
                line = "dessin";
            }
        }
        ///
        ///


        if(line == "dessin")
        {
            Dessiner( CVP, CD, CP, CI, NCD, NCP, NCI, IndicesPrec, NIndicesPrec, DIFF, indice, mexico, cc1, cc2, cc3, cc4, page);
            line = "vide";
        }
    }
    while(line != "return" && line != "stop");
}

void Graph::AfficherTout()
{
    std::cout << std::endl << std::endl;
    std::cout << "| Sommets ||   Degre    Deg Norm  ||   Vect Propre  ||  Proximite       Prox Norm   ||  Intermed        Inter Norm " << std::endl;
    for(size_t i=0 ; i < m_sommets.size() ; i++)
    {
        m_sommets[i].AfficherTout();
    }
    std::cout << "| Sommets ||   Degre    Deg Norm  ||   Vect Propre  ||  Proximite       Prox Norm   ||  Intermed        Inter Norm " << std::endl;
    std::cout << std::endl << std::endl;
}





void Graph::Sauvegarder(int mode)
{
    int indice, ind, ext1, ext2;
    double x, y;
    char nom;
    std::ofstream fichier;

    if(mode==0)
        fichier.open("data/GraphePrecedent.txt");
    else if(mode==1)
        fichier.open("data/GrapheActuel.txt");

    if(fichier)
    {
        fichier << m_oriente << std::endl;
        fichier << m_ordre << std::endl;
        for(int i=0 ; i < m_ordre ; i++)
        {
            m_sommets[i].setSommet(indice,nom,x,y);
            fichier << indice << " " << nom << " " << x << " " << y << std::endl;
            //std::cout << "Indice : " << indice << " Nom : " << nom << " x : " << x << " y : " << y << std::endl;
        }
        fichier << m_taille << std::endl;

        for(int i=0 ; i < m_taille ; i++)
        {
            m_aretes[i].setArete(ind,ext1,ext2);
            fichier << ind << " " << ext1 << " " << ext2 << std::endl;
        }
    }
    else
        std::cout << "Probleme ouverture de fichier pour sauvegarde" <<std::endl;
}

void Graph::Sauvegarder_Ponderation(int mode)
{
    int indice;
    double poids;
    std::ofstream fichier;

    if(mode==0)
        fichier.open("data/PonderationPrecedente.txt");
    else if(mode==1)
        fichier.open("data/PonderationActuelle.txt");

    if(fichier)
    {
        fichier << m_taille << std::endl;
        for(int i=0 ; i < m_taille ; i++)
        {
            m_aretes[i].setPoids(indice,poids);
            fichier << indice << " " << poids << std::endl;
        }
    }
    else
        std::cout << "Probleme ouverture fichier" << std::endl;
}

///Sauvegarde des centralites
void Graph::SaveComparaison()
{
    std::ofstream fichier;
    fichier.open("data/Comparaison.txt");

    if(fichier)
    {
        fichier << m_ordre << std::endl;
        for(size_t i=0 ; i < m_sommets.size() ; i++)
        {
            m_sommets[i].SaveComparaison(fichier);
        }
    }
    else
        std::cout << "Probleme ouverture fichier" << std::endl;

}

///Chargement des centralites sauvegardes
void Graph::ChargerComparaison(std::vector<std::vector<double>>& IndicesPrec, std::vector<std::vector<double>>& NIndicesPrec)
{
    int ordre;
    double indice=0, CdPrec=0,  CvpPrec=0,  CpPrec=0,  CiPrec=0;
    double          NCdPrec=0, NCvpPrec=0, NCpPrec=0, NCiPrec=0;
    IndicesPrec.clear();
    NIndicesPrec.clear();
    std::ifstream fichier("data/Comparaison.txt");
    if(fichier)
    {
        fichier >> ordre;
        for(int i=0 ; i < ordre ; i++)
        {
            std::vector<double> vec;
            std::vector<double> Nvec;
            fichier >> indice >> CdPrec >> CvpPrec >> CpPrec >> CiPrec >> NCdPrec >> NCvpPrec >> NCpPrec >> NCiPrec;
            vec.push_back(indice);
            vec.push_back(CdPrec);
            vec.push_back(CvpPrec);
            vec.push_back(CpPrec);
            vec.push_back(CiPrec);
            IndicesPrec.push_back(vec);
            Nvec.push_back(indice);
            Nvec.push_back(NCdPrec);
            Nvec.push_back(NCvpPrec);
            Nvec.push_back(NCpPrec);
            Nvec.push_back(NCiPrec);
            NIndicesPrec.push_back(Nvec);
        }
    }
    else
        std::cout << "Probleme ouverture fichier" << std::endl;


    /// ////
    //for(size_t i=0 ; i < IndicesPrec.size() ; i++)
    //{
    //        std::cout << "Indice : " << IndicesPrec[i][0] <<"  CdPrec : " << IndicesPrec[i][1] << "  CvpPrec : " << IndicesPrec[i][2] << "  CpPrec : " << IndicesPrec[i][3] << "  CiPrec : " << IndicesPrec[i][4] << std::endl;
    //}
    /// ////

}

///Pour k-Connexites : début
void Graph::SupprimerAreteAuto(int indice)
{
    int id1=0,id2=0;

    for(size_t i=0; i<m_aretes.size(); ++i)
    {
        if(m_aretes[i].RechercheIndice(indice)==true)
        {
            m_aretes[i].setArete(indice,id1,id2);
            indice=i;
            break;
        }
    }
    ///Suppression des successeurs
    if(m_oriente==false)
    {
        m_sommets[id1].SuppSommet(id2);
        m_sommets[id2].SuppSommet(id1);
    }
    else if(m_oriente==true)
        m_sommets[id1].SuppSommet(id2);

    ///Suppression des aretes
    m_aretes.erase(m_aretes.begin()+indice);
    m_aretes.shrink_to_fit();
    m_taille-=1;
}

void Graph::Recherche_Connexite_Auto(bool& stop)
{
    int nbConnex=0, compteur=1, compteur2=0, prochain=0, present, nbConnex2=0;
    std::vector<int> ListeConnex;
    std::vector<char> vec;
    stop = false;

    do
    {
        compteur=1;
        for(size_t i=0; i<m_sommets.size(); ++i) ///Recherche si il existe un sommet non marque
        {
            present=0;

            for(size_t j=0; j<ListeConnex.size(); ++j)
            {
                if(ListeConnex[j]==m_sommets[i].get_indice())
                {
                    present=1;
                    break;
                }
            }
            if(present!=1)
            {
                prochain=m_sommets[i].get_indice();
                break;
            }
        }
        ListeConnex.push_back(prochain);

        for(size_t i=0; i<m_aretes.size()/2; ++i) ///Le refaire plusieurs fois
        {
            for(size_t j=0; j<m_aretes.size(); ++j) ///Recherche si l'arete a deja ete parcouru
            {
                m_aretes[j].ParcoursConnex(ListeConnex,compteur);
            }
        }
        nbConnex++;

        for(size_t i=compteur2; i<ListeConnex.size(); ++i)
            vec.push_back(m_sommets[ListeConnex[i]].getNom());
        // for(size_t i=compteur2;i<ListeConnex.size();++i)
        //     std::cout << m_sommets[ListeConnex[i]].getNom() << " ";
        // std::cout << std::endl;

        if(vec.size() > 0)
        {
            nbConnex2+=1;
            vec.clear();
        }

        compteur2+=compteur;
    }
    while(compteur2<m_ordre);
    ///std::cout << std::endl;

    if(nbConnex2 > 1)
        stop = true;
}

void Graph::KConnexite()
{
    bool d2=false, d3=false, d4=false;
    bool stop = false, debut = true;
    size_t taille0=0, taille1=0, taille2=0, taille3=0;
    int kconnexe = 1;
    std::vector<Graph> listeG;
    Graph Initial(false, 1, 1), G(false, 1, 1), G2(false, 1, 1), G3(false, 1, 1), G4(false, 1, 1);

    Sauvegarder(1);
    Sauvegarder_Ponderation(1);
    Initial.Chargement("data/GrapheActuel.txt");
    Initial.Chargement_Ponderation("data/PonderationActuelle.txt");
    G2=Initial;
    G3=Initial;
    G4=Initial;
    taille1 = G2.m_aretes.size();


    //G=Initial;
    taille3 = G4.m_aretes.size();
    for(size_t l=0 ; l < taille3+1 ; l++)
    {
        if(l == 1 && stop == false)
            d4=true;

        if(debut == false)
        {
            G4=Initial;
            listeG.push_back(G);
            listeG[0]=G4;
            listeG[0].SupprimerAreteAuto(m_aretes[l-1].getIndice());
            listeG[0].Sauvegarder(1);
            listeG[0].Sauvegarder_Ponderation(1);
            listeG.clear();
            listeG.push_back(G);
            listeG[0].Chargement("data/GrapheActuel.txt");
            listeG[0].Chargement_Ponderation("data/PonderationActuelle.txt");
            G4 = listeG[0];
            listeG.clear();
        }



        taille2 = G4.m_aretes.size();
        for(size_t k=0 ; k < taille2+1 ; k++)
        {
            //std::cout << "K = " << k << std::endl;
            if(k == 1 && stop == false)
                d3=true;

            if(debut == false)
            {
                G3=G4;
                listeG.push_back(G);
                listeG[0]=G3;
                listeG[0].SupprimerAreteAuto(m_aretes[k-1].getIndice());
                listeG[0].Sauvegarder(1);
                listeG[0].Sauvegarder_Ponderation(1);
                listeG.clear();
                listeG.push_back(G);
                listeG[0].Chargement("data/GrapheActuel.txt");
                listeG[0].Chargement_Ponderation("data/PonderationActuelle.txt");
                G3 = listeG[0];
                listeG.clear();
            }



            taille1 = G3.m_aretes.size();
            for(size_t i=0 ; i < taille1+1 ; i++)
            {
                if(i == 1 && stop == false)
                    d2=true;

                if(debut == false)
                {
                    G2 = G3;
                    listeG.push_back(G);
                    listeG[0]=G2;
                    listeG[0].SupprimerAreteAuto(m_aretes[i-1].getIndice());
                    listeG[0].Sauvegarder(1);
                    listeG[0].Sauvegarder_Ponderation(1);
                    listeG.clear();
                    listeG.push_back(G);
                    listeG[0].Chargement("data/GrapheActuel.txt");
                    listeG[0].Chargement_Ponderation("data/PonderationActuelle.txt");
                    G2 = listeG[0];
                    listeG.clear();
                }
                debut = false;

                taille0 = G2.m_aretes.size();
                for(size_t j=0 ; j < taille0 ; j++)
                {
                    listeG.push_back(G);
                    listeG[0]=G2;

                    listeG[0].SupprimerAreteAuto(m_aretes[j].getIndice());
                    listeG[0].Sauvegarder(1);
                    listeG[0].Sauvegarder_Ponderation(1);
                    listeG.clear();
                    listeG.push_back(G);
                    listeG[0].Chargement("data/GrapheActuel.txt");
                    listeG[0].Chargement_Ponderation("data/PonderationActuelle.txt");

                    listeG[0].Recherche_Connexite_Auto(stop);

                    listeG.clear();

                    if(stop == true)
                    {
                        //kconnexe = i;
                        l=m_aretes.size();
                        k=m_aretes.size();
                        j=m_aretes.size();
                        i=m_aretes.size();
                    }
                }

            }
        }
    }

    if(d2 == true)
        kconnexe+=1;
    if(d3 == true)
        kconnexe+=1;
    if(d4 == true)
        kconnexe+=1;

    std::cout << "                                              K-Connexe " << std::endl;
    std::cout << "                                                > " << kconnexe << " <" << std::endl;
}
///Fin ssprgs pour k-connexites



#include <iostream>
#include <vector>
#include "math.h"
#include "Sommet.h"
#include "Arete.h"
#include <iomanip>
#include <iostream>
namespace std {
    const char* tab = "\t";
}
std::vector<double> RechercheSommetVec(int indice,std::vector<std::vector<double>> IndicesPrec);
void Mexico(Svgfile& index);
void MexicoTrain(Svgfile& index);

Sommet::Sommet(int indice, char nom, double x, double y)
{
    m_nom = nom;
    m_indice = indice;
    m_marque = false;
    m_x = x;
    m_y = y;
    m_Cd=0;
    m_N_Cd=0;
    m_Cvp=1;
    m_Cp=0;
    m_N_Cp=0;
    m_Ci=0;
    m_N_Ci=0;
    m_precedent=NULL;
}

void Sommet::AddSuccesseur(Sommet* s)
{
    m_successeurs.push_back(s);
}

void Sommet::Successeur(int id1, int id2, std::vector<Sommet>& sommets, bool oriente)
{
    if(id1 == m_indice)
    {
        for(size_t i=0 ; i < sommets.size() ; i++)
        {
            if(id2 == sommets[i].m_indice)
            {
                m_successeurs.push_back(&sommets[i]);

                if(oriente == false)
                {
                    sommets[i].m_successeurs.push_back(this);
                }
            }
        }
    }
}

///Source :
///Sous programme d'arrondi de valeur : https://www.developpez.net/forums/d29018/c-cpp/cpp/arrondir-chiffre-2-chiffres-apres-virgule/ (2 lignes)
double Arrondir(double valeur)
{
    int entier = (int)((0.0005 + valeur) * 1000.0);
    double result = (double)entier / 1000.0;

    return result;
}

double Sommet::Pourcentage(std::vector<Sommet>sommets, double val, std::string indice)
{
    double Vmax=0;
    if(indice == "cd")
    {
        for(size_t i=0 ; i < sommets.size() ; i++)
        {
            if(sommets[i].m_Cd > Vmax)
                Vmax = sommets[i].m_Cd;
        }
    }
    if(indice == "ncd")
    {
        for(size_t i=0 ; i < sommets.size() ; i++)
        {
            if(sommets[i].m_N_Cd > Vmax)
                Vmax = sommets[i].m_N_Cd;
        }
    }

    if(indice == "cvp")
    {
        for(size_t i=0 ; i < sommets.size() ; i++)
        {
            if(sommets[i].m_Cvp > Vmax)
                Vmax = sommets[i].m_Cvp;
        }
    }

    if(indice == "cp")
    {
        for(size_t i=0 ; i < sommets.size() ; i++)
        {
            if(sommets[i].m_Cp > Vmax)
                Vmax = sommets[i].m_Cp;
        }
    }
    if(indice == "ncp")
    {
        for(size_t i=0 ; i < sommets.size() ; i++)
        {
            if(sommets[i].m_N_Cp > Vmax)
                Vmax = sommets[i].m_N_Cp;
        }
    }

    if(indice == "ci")
    {
        for(size_t i=0 ; i < sommets.size() ; i++)
        {
            if(sommets[i].m_Ci > Vmax)
                Vmax = sommets[i].m_Ci;
        }
    }
    if(indice == "nci")
    {
        for(size_t i=0 ; i < sommets.size() ; i++)
        {
            if(sommets[i].m_N_Ci > Vmax)
                Vmax = sommets[i].m_N_Ci;
        }
    }

    val = val * (1 / Vmax);
    return val;
}

///Affichage pour svg
void Sommet::Dessiner(Svgfile& index, bool oriente, std::vector<Arete>& aretes, bool CVP, bool CD, bool CP, bool CI, bool pondere, bool NCD, bool NCP, bool NCI, std::vector<std::vector<double>> IndicesPrec, std::vector<std::vector<double>> NIndicesPrec, bool DIFF, std::vector<Sommet>sommets, int indice, bool mexico, bool& cc1, bool& cc2, bool& cc3, bool& cc4, int& page)
{
    double cd=0, cp=0, ci=0;
    double pourcent=0, val=0;
    int decalage=0;
    std::vector<double>vec;
    std::vector<double>Nvec;
    std::string couleur, couleurDiff, red = "red", green = "lightgreen";
    if(IndicesPrec.size() > 0)
        vec = RechercheSommetVec(m_indice, IndicesPrec);
    if(NIndicesPrec.size() > 0)
        Nvec = RechercheSommetVec(m_indice, NIndicesPrec);

    if(NCD == false)
        cd = m_Cd;
    if(NCD == true)
        cd = m_N_Cd;

    if(mexico == true)
        Mexico(index);

    if(page == 2)
        MexicoTrain(index);

    switch(indice)
    {
    case 1 :
        pourcent = Pourcentage(sommets, m_Cd, "cd");
        break;
    case 2 :
        pourcent = Pourcentage(sommets, m_Cvp, "cvp");
        break;
    case 3 :
        pourcent = Pourcentage(sommets, m_Cp, "cp");
        break;
    case 4 :
        pourcent = Pourcentage(sommets, m_Ci, "ci");
        break;
    case 5 :
        pourcent = Pourcentage(sommets, m_N_Cd, "ncd");
        break;
    case 6 :
        pourcent = Pourcentage(sommets, m_N_Cp, "ncp");
        break;
    case 7 :
        pourcent = Pourcentage(sommets, m_N_Ci, "nci");
        break;
    }
    val = pourcent;
    ///std::cout << "pourcentage = " << val <<std::endl;

    if( val < 0.25)
        couleur = "cyan";
    if( val >= 0.25 && val < 0.375)
        couleur = "green";
    if( val >= 0.375 && val < 0.5)
        couleur = "blue";
    if( val >= 0.5 && val < 0.625)
        couleur = "yellow";
    if( val >= 0.625 && val < 0.75)
        couleur = "orange";
    if( val >= 0.75)
        couleur = "red";

    if(indice == 0)
        couleur = "black";

    index.addRectangle(820, 35, 950, 280 +50, "white", 2, "black");
    index.addRectangle(820, 320 +25, 950, 542 +25, "white", 2, "black");
    //index.addText(862, 346+25, "Importance of the summits", "black");
    index.addText(859, 346+25, " Chosen ", "black");
    //index.addText(859, 346+25, "Analysis", "black");
    index.addText(866, 346 +19+25, "Mode", "black");
    //index.addText(839.5, 527+25, "N", "black");
    index.addText(840, 61,  "Importance of","black");
    index.addText(858, 80 ,  "Verteces","black");

    index.addDisk(850, 50 +50, 8, "cyan");
    index.addDisk(850, 80 +50, 8, "lightgreen");
    index.addDisk(850, 110 +50, 8, "green");
    index.addDisk(850, 140 +50, 8, "yellow");
    index.addDisk(850, 170 +50, 8, "orange");
    index.addDisk(850, 200 +50, 8, "red");

    index.addText(878, 50 +5 +50 ,  "0 - 25","black");
    index.addText(878, 80 +5 +50,  "25 - 37,5","black");
    index.addText(878, 110 +5 +50, "37,5 - 50","black");
    index.addText(878, 140 +5 +50, "50 - 62,5","black");
    index.addText(878, 170 +5 +50, "62,5 - 75","black");
    index.addText(878, 200 +5 +50, "75 - 100","black");

    //index.addRectangle(820, 35, 950, 280, "white", 2, "black");
    index.addRectangle(840,90,860,260,"redrect",2,"black");
    index.addRectangle(840,90,860,130,"bluerect");
    index.addRectangle(840,129,860,148,"greenrect");
    index.addRectangle(840,190,860,214,"yellow");
    index.addRectangle(840,147,860,191,"yellowrect");
    index.addRectangle(840,213,860,260,"redrect");

    index.addRectangle(840,90,860,260,"testrect");
    //index.addRectangle(840,90,860,260,"rvbrect");

    index.addRectangle(835, 275, 935, 280 +50 -15, "white", 2, "black");

    switch(indice)
    {
    case 1 :
        index.addText(870 -9, 370 +19 +7 +3 +2 -102, "Mode 1", "black");
        break;
    case 2 :
        index.addText(870 -9, 370 +19 +7 +3 +2 -102, "Mode 2", "black");
        //index.addText(873 -2 -9, 370 +19 +7 +3 -5 -102, "Vecteur", "black");
        //index.addText(876 -2 -9, 370 +19 +7 +3 +10 -102, "Propre", "black");
        break;
    case 3 :
        index.addText(870 -9, 370 +19 +7 +3 +2 -102, "Mode 3", "black");
        break;
    case 4 :
        index.addText(870 -9, 370 +19 +7 +3 +2 -102, "Mode 4", "black");
        break;
    case 5 :
        index.addText(870 -9, 370 +19 +7 +3 +2 -102, "Mode 1", "black");
        break;
    case 6 :
        index.addText(870 -9, 370 +19 +7 +3 +2 -102, "Mode 3", "black");
        break;
    case 7 :
        index.addText(870 -9, 370 +19 +7 +3 +2 -102, "Mode 4", "black");
        break;
    }


    ///INDICES

    //index.addText(875, 370 +19 +7 +3 +2+25, "Degre", "black");

    //index.addText(873 -2, 370 +19 +7 +3 +33 -5+25, "Vecteur", "black");
    //index.addText(876 -2, 370 +19 +7 +3 +33 +10+25, "Propre", "black");
    index.addText(881, 370 +19 +7 +3 +2+25,        "Mode 1  ", "black");
    index.addText(881 -2, 370  +19+7 +3 +33 +2 +25,"Mode 2  ", "black");
    //index.addText(867, 370 +19 +7 +3 +66 +2+25,    "Proximite", "black");
    index.addText(881, 370 +19 +7 +3 +66 +2+25,    "Mode 3  ", "black");
    index.addText(881, 370 +19 +7 +3 +99 +2+25,    "Mode 4  ", "black");
    /// ////

    index.addDisk(m_x*100, m_y*100, 5, couleur);
    std::string c(1, m_nom);
    index.addText(m_x*100 - 5, m_y*100 - 10, c,"black");

    if(CD == true)
    {
        if(DIFF == true)
        {
        if(vec.size() > 1)
        if(NCD == false)
            if(vec[1] != cd)
            {
                if((cd - vec[1]) > 0)
                    couleurDiff = red;
                if((cd - vec[1]) < 0)
                    couleurDiff = green;
                index.addText(m_x*100 - 22 + 45, m_y*100 + 17, "(" , couleurDiff);
                if(Arrondir(cd - vec[1]) < 0)
                    decalage=0;
                if(Arrondir(cd - vec[1]) > 0)
                {
                    decalage=8;
                    index.addText(m_x*100 - 22 + 45 + 4, m_y*100 + 17, "+" , couleurDiff);
                }
                index.addText(m_x*100 - 22 + 91 + decalage, m_y*100 + 17, ")" , couleurDiff);
                index.addText(m_x*100 - 17 + 45 + decalage, m_y*100 + 17, cd - vec[1], couleurDiff);
            }
        if(Nvec.size() > 1)
        if(NCD == true)
            if(Nvec[1] != cd)
            {
                if((cd - Nvec[1]) > 0)
                    couleurDiff = red;
                if((cd - Nvec[1]) < 0)
                    couleurDiff = green;
                index.addText(m_x*100 - 22 + 45, m_y*100 + 17, "(" , couleurDiff);
                if(Arrondir(cd - Nvec[1]) < 0)
                    decalage=0;
                if(Arrondir(cd - Nvec[1]) > 0)
                {
                    decalage=8;
                    index.addText(m_x*100 - 22 + 45 + 4, m_y*100 + 17, "+" , couleurDiff);
                }
                index.addText(m_x*100 - 22 + 91 + decalage, m_y*100 + 17, ")" , couleurDiff);
                index.addText(m_x*100 - 17 + 45 + decalage, m_y*100 + 17, cd - Nvec[1], couleurDiff);
                ///index.addText(m_x*100 - 17 + 45 + decalage, m_y*100 + 17, Arrondir(cd - Nvec[1]), couleurDiff);
                ///Arrondir(m_Cvp - vec[2])
                /// /////
            }
        }

    //index.addText(m_x*100 - 14, m_y*100 + 16, cd, "yellow");
    //index.addText(m_x*100 - 16, m_y*100 + 18, cd, "yellow");
    index.addText(m_x*100 - 15, m_y*100 + 17, Arrondir(cd), "orange");


    //index.addLine(30, 31, 50, 31, "yellow");
    //index.addLine(30, 29, 50, 29, "yellow");
    ///if(NCD==true)
        ///index.addDisk(845, 370 +19 +7+25, 4, "orange");
    index.addCircle(845, 370 +19 +7+25, 7, 2, "orange");
    //index.addLine(30, 30, 50, 30, "orange");

    //index.addText(66, 36, "Indice de centralite par Degre", "yellow");
    //index.addText(64, 34, "Indice de centralite par Degre", "yellow");
    //index.addText(875, 370 +19 +7 +3 +2, "Degre", "black");
    }


    if(CVP == true)
    {
        if(DIFF == true)
        {
        if(m_Cvp >= 0)
        if(vec.size() > 2)
            if(vec[2] != m_Cvp)
            {
                if(Arrondir(m_Cvp - vec[2]) > 0)
                    couleurDiff = red;
                if(Arrondir(m_Cvp - vec[2]) < 0)
                    couleurDiff = green;
                index.addText(m_x*100 - 22 + 45, m_y*100 + 33, "(" , couleurDiff);
                if(Arrondir(m_Cvp - vec[2]) < 0)
                    decalage=0;
                if(Arrondir(m_Cvp - vec[2]) > 0)
                {
                    decalage=8;
                    index.addText(m_x*100 - 22 + 45 + 4, m_y*100 + 33, "+" , couleurDiff);
                }
                index.addText(m_x*100 - 22 + 91 + decalage, m_y*100 + 33, ")" , couleurDiff);
                index.addText(m_x*100 - 17 + 45 + decalage, m_y*100 + 33, Arrondir(m_Cvp - vec[2]), couleurDiff);
            }
        }

        if(m_Cvp >= 0)
            index.addText(m_x*100-16, m_y*100+33, Arrondir(m_Cvp),"green");
        if(m_Cvp < 0)
            index.addText(m_x*100-16, m_y*100+33, "nan","green");

        index.addCircle(845, 370 +19 +33 +7+25, 7, 2, "green");
        ///index.addDisk(845, 370 +19 +33 +7+25, 4, "green");
        //index.addText(853, 370 +19 +7 +3 +33, "Vecteur Propre", "green");
        //index.addText(873 -2, 370 +19 +7 +3 +33 -5+25, "Vecteur", "black");
        //index.addText(876 -2, 370 +19 +7 +3 +33 +10+25, "Propre", "black");
    }

    if(NCP == true)
        cp = m_N_Cp;
    if(NCP == false)
        cp = m_Cp;

    if(CP == true)
    {

        if(DIFF == true)
        {
        if(cp >= 0 && cp<100000)
        if(vec.size() > 3)
        if(NCP == false)
            if(vec[3] != cp)
            {
                if(Arrondir(cp - vec[3]) > 0)
                    couleurDiff = red;
                if(Arrondir(cp - vec[3]) < 0)
                    couleurDiff = green;
                index.addText(m_x*100 - 22 + 45, m_y*100 + 51, "(" , couleurDiff);
                if(Arrondir(cp - vec[3]) < 0)
                    decalage=0;
                if(Arrondir(cp - vec[3]) > 0)
                {
                    decalage=8;
                    index.addText(m_x*100 - 22 + 45 + 4, m_y*100 + 51, "+" , couleurDiff);
                }
                index.addText(m_x*100 - 22 + 91 + decalage, m_y*100 + 51, ")" , couleurDiff);
                index.addText(m_x*100 - 17 + 45 + decalage, m_y*100 + 51, Arrondir(cp - vec[3]), couleurDiff);
            }
        if(cp >= 0 && cp<100000)
        if(Nvec.size() > 3)
        if(NCP == true)
            if(Nvec[3] != cp)
            {
                if(Arrondir(cp - Nvec[3]) > 0)
                    couleurDiff = red;
                if(Arrondir(cp - Nvec[3]) < 0)
                    couleurDiff = green;
                index.addText(m_x*100 - 22 + 45, m_y*100 + 51, "(" , couleurDiff);
                if(Arrondir(cp - Nvec[3]) < 0)
                    decalage=0;
                if(Arrondir(cp - Nvec[3]) > 0)
                {
                    decalage=8;
                    index.addText(m_x*100 - 22 + 45 + 4, m_y*100 + 51, "+" , couleurDiff);
                }
                index.addText(m_x*100 - 22 + 91 + decalage, m_y*100 + 51, ")" , couleurDiff);
                index.addText(m_x*100 - 17 + 45 + decalage, m_y*100 + 51, Arrondir(cp - Nvec[3]), couleurDiff);
            }
            if(cp < 0 || cp>100000)
            {
                index.addText(m_x*100 - 22 + 45, m_y*100 + 51, "(+inf)" , red);
            }
        }



        if(cp >= 0 && cp<100000)
            index.addText(m_x*100-16, m_y*100+51, Arrondir(cp),"blue");
        if(cp < 0 || cp>100000)
            index.addText(m_x*100-16, m_y*100+51, "inf","blue");

            ///if(NCP == true)
                ///index.addDisk(845, 370 +19 +66 +7+25, 4, "blue");
        index.addCircle(845, 370 +19 +66 +7+25, 7, 2, "blue");
        //index.addText(867, 370 +19 +7 +3 +66 +2+25, "Proximite", "black");
    }

    if(NCI == true)
        ci = m_N_Ci;
    if(NCI == false)
        ci = m_Ci;

    if(CI == true)
    {

        if(DIFF == true)
        {
        if(ci >= 0 && ci < 100000)
        if(vec.size() > 4)
        if(NCI == false)
            if(vec[4] != ci)
            {
                if(Arrondir(ci - vec[4]) > 0 )
                        couleurDiff = red;
                    if(Arrondir(ci - vec[4]) < 0 )
                        couleurDiff = green;
                    index.addText(m_x*100 - 22 + 45, m_y*100 + 69, "(" , couleurDiff);
                if(Arrondir(ci - vec[4]) < 0)
                    decalage=0;
                if(Arrondir(ci - vec[4]) > 0)
                {
                    decalage=8;
                    index.addText(m_x*100 - 22 + 45 + 4, m_y*100 + 69, "+" , couleurDiff);
                }
                    if(Arrondir(ci - vec[4]) < 100 && Arrondir(ci - vec[4]) > -10)
                        index.addText(m_x*100 - 22 + 91 + decalage, m_y*100 + 69, ")" , couleurDiff);
                    if(Arrondir(ci - vec[4]) >= 100 || Arrondir(ci - vec[4]) <= -10)
                        index.addText(m_x*100 - 22 + 91 + 8 + decalage, m_y*100 + 69, ")" , couleurDiff);
                        index.addText(m_x*100 - 17 + 45 + decalage, m_y*100 + 69, Arrondir(ci - vec[4]), couleurDiff);
            }
        if(ci >= 0 && ci < 100000)
        if(Nvec.size() > 4)
        if(NCI == true)
            if(Nvec[4] != ci)
            {
                if(Arrondir(ci - Nvec[4]) > 0)
                    couleurDiff = red;
                if(Arrondir(ci - Nvec[4]) < 0)
                    couleurDiff = green;
                index.addText(m_x*100 - 22 + 45, m_y*100 + 69, "(" , couleurDiff);
                if(Arrondir(ci - Nvec[4]) < 0)
                    decalage=0;
                if(Arrondir(ci - Nvec[4]) > 0)
                {
                    decalage=8;
                    index.addText(m_x*100 - 22 + 45 + 4, m_y*100 + 69, "+" , couleurDiff);
                }
                index.addText(m_x*100 - 22 + 91 + decalage, m_y*100 + 69, ")" , couleurDiff);
                index.addText(m_x*100 - 17 + 45 + decalage, m_y*100 + 69, Arrondir(ci - Nvec[4]), couleurDiff);
            }
            if(ci < 0 || ci > 100000)
                index.addText(m_x*100 - 22 + 45, m_y*100 + 69, "(+inf)" , red);
        }


        if(ci >= 0 && ci < 100000)
            index.addText(m_x*100-16, m_y*100+69, Arrondir(ci),"darkred");
        if(ci < 0 || ci > 100000)
            index.addText(m_x*100-16, m_y*100+69, "inf","darkred");

            ///if(NCI == true)
                ///index.addDisk(845, 370 +19 +99 +7+25, 4,"darkred");
        index.addCircle(845, 370 +19 +99 +7+25, 7, 2, "darkred");
        //index.addText(856, 370 +19 +7 +3 +99 +2+25, "Intermediarite", "black");
    }


    for(size_t i=0 ; i < m_successeurs.size() ; i++)
    {
        index.addLine(m_x*100,m_y*100, m_successeurs[i]->m_x*100, m_successeurs[i]->m_y*100, "black");

        if(pondere == true)
        for(size_t j=0 ; j < aretes.size() ; j++)
        {
            aretes[j].DessinerPoids(index, m_indice, m_x, m_y, m_successeurs[i]->m_indice, m_successeurs[i]->m_x, m_successeurs[i]->m_y);
        }

        if(oriente == true) ///Dessin des fleches
        {
            int ArrowLength = 12, ArrowWidth = 8;
            int x1=m_x*100, x2=m_successeurs[i]->m_x*100;
            int y1=m_y*100, y2=m_successeurs[i]->m_y*100;
            int AB=sqrt((x2-x1)*(x2-x1) + (y2-y1)*(y2-y1));
            int xC=x2+ArrowLength*(x1-x2)/AB,   yC=y2+ArrowLength*(y1-y2)/AB;
            int xD=xC+ArrowWidth*(-(y2-y1))/AB, yD=yC+ArrowWidth*((x2-x1))/AB;
            int xE=xC-ArrowWidth*(-(y2-y1))/AB, yE=yC-ArrowWidth*((x2-x1))/AB;

            index.addTriangle(xD, yD, xE, yE,x2, y2, "lightgrey",1, "black");
        }
    }
    if(cc1)
        index.addDisk(845, 370 +19     +7+25, 4, "orange");
    if(cc2)
        index.addDisk(845, 370 +19 +33 +7+25, 4, "green");
    if(cc3)
        index.addDisk(845, 370 +19 +66 +7+25, 4, "blue");
    if(cc4)
        index.addDisk(845, 370 +19 +99 +7+25, 4,"darkred");

    index.addText(200, 35, "( Please, don't forget to refresh the page after each input )", "red");

}

void Sommet::Centralite_Degre(int n)
{
    double ordre = m_successeurs.size();
    m_Cd = ordre;
}

void Sommet::Somme_Indices()
{
    m_Cvp=0;
    for(size_t i=0 ; i < m_successeurs.size() ; i++)
    {
        m_Cvp += m_successeurs[i]->m_Cvp ;
    }
}
void Sommet::Recalcul_Indices(double lambda, double Csi)
{
    m_Cvp = (Csi)/ lambda ;
}

double Sommet::Csi()
{
    double Csi=0;

    for(size_t i=0 ; i < m_successeurs.size() ; i++)
    {
        Csi += m_successeurs[i]->m_Cvp;
    }

    return Csi;
}



void Sommet::Afficher()
{
    std::cout << m_indice << std::endl;
    std::cout << "Cvp : " << m_Cvp << std::endl;
    std::cout << "Cp : " << m_Cp << std::endl;
    std::cout << "Successeurs : " << std::endl;
    for(size_t i=0 ; i < m_successeurs.size() ; i++)
    {
        std::cout << m_successeurs[i]->m_indice << std::endl;
    }
    std::cout << "---------" << std::endl;
}

void Sommet::Marquage()
{
    m_marque=true;
}

bool Sommet::RechercheSommet(int ind)
{
    bool resultat=false;
    if(ind == m_indice)
        resultat = true;

    return resultat;
}

bool Sommet::Marque()
{
    bool resultat = false;
    if(m_marque == true)
        resultat = true;
    return resultat;
}

void Sommet::Fini(int distance, Sommet* precedent)
{
//    if(distance == m_distance)
//        std::cout << "C'EST CA" << std::endl << "distance = " << distance << std::endl << std::endl;


    m_precedent = precedent;
    m_distance = distance;
}

int Sommet::get_indice()
{
    return m_indice;
}

Sommet* Sommet::Get_precedent()
{
    return m_precedent;
}

void Sommet::Dijkstra( std::vector<double>&distances, std::vector<Sommet*>&sommetprec, std::vector<Sommet*>&sommets, double distance, std::vector<Arete>aretes, Sommet* s, bool oriente)
{
    double poids=0, poids2=0;
    for(size_t i=0 ; i < m_successeurs.size() ; i++)
        {
            if(m_successeurs[i]->Marque() == false)
            {
                //std::cout << "INDICE : " << m_indice << "  Indice successeur : " << m_successeurs[i]->get_indice() << std::endl;
                for(size_t j=0 ; j < aretes.size() ; j++)
                {
                    poids2 = aretes[j].Recherche_Poids( m_indice, m_successeurs[i]->m_indice, oriente);
                    if(poids2 != 0)
                        poids=poids2;
                }





                // std::cout << "                      Chemin : " << poids+distance  << std::endl;
                distances.push_back( poids + distance);
                sommetprec.push_back(s);
                sommets.push_back(m_successeurs[i]);
            }
        }
}

void Sommet::Reset_Dijkstra()
{
    m_marque = false;
    m_distance = 0;
    m_precedent = NULL;
    m_PccPrecedents.clear();
    for(size_t i=0 ; i < m_Pcc.size() ; i++)
    {
        m_Pcc[i].clear();
    }
    m_Pcc.clear();
}

void Sommet::AddCp(double Cp, int n)
{
    m_Cp = Cp;
}

bool Sommet::TestNombreSuccesseurs()
{
    bool impossible = false;
    if(m_successeurs.size() == 0)
        impossible = true;

    return impossible;
}

void Sommet::AddPccPrecedent(Sommet* s)
{
    bool idem = false;
    for(size_t i=0 ; i < m_PccPrecedents.size() ; i++)
    {
        if(m_PccPrecedents[i] == s)
            idem = true;
    }
    if(idem == false)
        m_PccPrecedents.push_back(s);
}

void Sommet::ClearListe2(std::vector<int>& liste2, double poids, Sommet* s_depart, Sommet* s_arrive, bool oriente, std::vector<Arete> aretes, double poidsprec)
{
    if(poids == 0 )
    {
        poidsprec = 0;
        liste2.clear();
    }

    double manquant = 0;
    if(liste2.size() > 0 )
        manquant = RechercheAretePoids(aretes, s_depart->m_indice, liste2[liste2.size()-1], oriente);
    poidsprec += manquant;

    if(liste2.size() >= 1)
    {
        poidsprec -= RechercheAretePoids(aretes, s_depart->m_indice, liste2[liste2.size()-1], oriente);
    }
    while(poidsprec != poids)
    {
        if(liste2.size() == 1)
        {
            poidsprec -= RechercheAretePoids(aretes, liste2[liste2.size()-1], s_arrive->m_indice, oriente);
            liste2.pop_back();
        }
        if(liste2.size() >= 2)
        {
            poidsprec -= RechercheAretePoids(aretes, liste2[liste2.size()-1], liste2[liste2.size()-2], oriente);
            liste2.pop_back();
        }
    }
}



void Sommet::TestRec(Sommet* s_depart, int& n, std::vector<int>& liste, double distance, std::vector<Arete>aretes, bool oriente, double poids, Sommet* arrive, Sommet* prec, double& poidsprec, std::vector<int>& liste2, bool& test)
{
    if(poids <= arrive->m_distance)
    {

        if(poids <= poidsprec)
        {
            if(test == true)
            {
                ClearListe2(liste2, poids, s_depart, arrive, oriente, aretes, poidsprec);
            }
            else{
                ClearListe2(liste2, poids, s_depart, arrive, oriente, aretes, poidsprec);
            }
            test=false;
        }

        poidsprec = poids;

        ///poids += RechercheAretePoids(aretes, prec->m_indice, m_indice, oriente);
        poids += RechercheAretePoids(aretes, m_indice, prec->m_indice, oriente);
        if( m_indice != s_depart->m_indice )
        {
            liste.push_back(m_indice);

            if(m_indice != arrive->m_indice)
                liste2.push_back(m_indice);

        }

        for(size_t i=0 ; i < m_PccPrecedents.size() ; i++)
        {
            m_PccPrecedents[i]->TestRec( s_depart, n, liste, distance, aretes, oriente, poids, arrive, this, poidsprec, liste2, test);
        }

        if((m_PccPrecedents.size() == 0) && ((m_indice != s_depart->m_indice)))
        {
            m_precedent->TestRec( s_depart, n, liste, distance, aretes, oriente, poids, arrive, this, poidsprec, liste2, test);
        }

        if(m_indice == s_depart->m_indice)
            n+=1;

        /// ////
        if(poids == arrive->m_distance && m_indice == s_depart->m_indice)
        {
            ///AfficherListe(liste2);
            arrive->AjouterPcc(liste2, n);
        }
        /// ////

    }
    else{
        test = true;
    }
}


void Sommet::CalculPccPrec(Sommet* s_depart, std::vector<Arete>aretes, bool oriente)
{
    int n=0;
    std::vector<int> liste, liste2;
    double p=0;
    bool test=false;

    if(m_distance > 0)
        TestRec(s_depart, n, liste, m_distance, aretes, oriente, p, this, this, p, liste2, test);

    ///std::cout << "Il y a " << n << " plus courts chemins entre " << s_depart->m_nom << " et " << m_nom << std::endl;
}

double Sommet::RechercheAretePoids(std::vector<Arete> aretes, int ind1, int ind2, bool oriente)
{
    double poids=0, poids2 =0;
    for(size_t i=0 ; i < aretes.size() ; i++)
        {
            poids2 = aretes[i].Recherche_Poids( ind1, ind2, oriente);
            if(poids2 != 0)
            {
                poids = poids2;
            }
        }
    return poids;
}

bool Sommet::TestListe(std::vector<int> liste)
{
    bool stop=false;
    for(size_t j=0 ; j < liste.size() ; j++)
    {
        if((liste[j] == m_indice))
            stop = true;
    }
    return stop;
}

void Sommet::AjouterPcc(std::vector<int> liste, int n)
{
    //AfficherListe(liste);
    m_Pcc.push_back(liste);
}

///Affichage pour debug
void AfficherListe(std::vector<int> liste2)
{
    for(size_t i=0 ; i < liste2.size() ; i++)
    {
        std::cout << "Parcours : ";
        switch(liste2[i]){
            case 0 : std::cout << "A"; break;
            case 1 : std::cout << "B"; break;
            case 2 : std::cout << "C"; break;
            case 3 : std::cout << "D"; break;
            case 4 : std::cout << "E"; break;
            case 5 : std::cout << "F"; break;
            case 6 : std::cout << "G"; break;
            case 7 : std::cout << "H"; break;
            case 8 : std::cout << "I"; break;}
        std::cout << std::endl;
    }
    //if(liste2.size() > 0)
    //std::cout << "STOP" << std::endl;
}

///Affichage pour debug
void Sommet::AfficherPcc(Sommet* s_depart)
{
    std::cout << "Il y a " << m_Pcc.size() << " plus court(s) chemin(s) entre " << s_depart->m_nom << " et " << m_nom <<std::endl;
    if(m_Pcc.size() > 0)
        std::cout << "Passant(s) par : " << std::endl;
    for(size_t i=0 ; i < m_Pcc.size() ; i++)
    {
        std::cout << "Chemin " << i+1 << " : " <<std::endl;
        AfficherListe(m_Pcc[i]);
    }
    std::cout << "--------------------------------------------------" << std::endl;
}

void Sommet::CalculPcci(Sommet* si, Sommet* s_depart, bool oriente)
{
    double pcci = 0;
    for(size_t i=0 ; i < m_Pcc.size() ; i++)
    {
        for(size_t j=0 ; j < m_Pcc[i].size() ; j++)
        {
            if(s_depart->m_indice < m_indice || oriente == true)
                if(si->m_indice == m_Pcc[i][j])
                    pcci += 1;
        }
    }
    if(m_Pcc.size()>0)
        pcci = pcci / m_Pcc.size();
    si->m_Ci += pcci;
    //std::cout << "Le Sommet " << si->m_nom << " apparait " << pcci << " fois entre " << s_depart->m_nom << " et " << m_nom << std::endl;
}

///Normalisation
void Sommet::Normaliser(int n)
{
    m_N_Cd = m_Cd / (n-1) ;
    m_N_Cp = m_Cp*(n-1) ;
    m_N_Ci = 2 * m_Ci / ((n*n) - (3*n) + 2) ;
}


char Sommet::getNom()
{
    return m_nom;
}


void Sommet::setSommet(int &indice, char &nom, double &x, double &y)
{
    indice=m_indice;
    nom=m_nom;
    x=m_x;
    y=m_y;
}

///Suppression du successeur d'un sommet s'il n'est plus relié à lui
void Sommet::SuppSommet(int &id2)
{
    for(size_t i=0; i<m_successeurs.size(); ++i)
    {
        if(m_successeurs[i]->get_indice()==id2)
        {
            m_successeurs.erase(m_successeurs.begin()+i);
            m_successeurs.shrink_to_fit();
            break;
        }
    }
}

void Sommet::AfficherTout()
{
    //std::cout << "| Sommets ||   Degre   |  Deg Norm  || Vect Propre || Proximite | Prox Norm ||  Intermed  | Inter Norm |" << std::endl;
    //std::cout << "|    " << m_nom << "    ||     " << m_Cd << "     |    " << m_N_Cd << "   ||   " << m_Cvp << std::endl;
    std::cout << "|    " << m_nom << "    ||     " << m_Cd << std::tab << m_N_Cd << std::tab << std::tab << m_Cvp << std::tab << m_Cp << std::tab << m_N_Cp << std::tab << m_Ci << std::tab << std::tab << m_N_Ci << std::endl;
}

///Sauvegarde des centralites d'un sommet
void Sommet::SaveComparaison(std::ofstream& fichier)
{
    fichier << m_indice << " " << m_Cd << " " << m_Cvp << " " << m_Cp << " " << m_Ci<< " " << m_N_Cd << " " << m_Cvp << " " << m_N_Cp << " " << m_N_Ci  << std::endl;
}

///Légende des lignes de métro de Mexico (si son graphe est charge)
void Mexico(Svgfile& index)
{
    index.addRectangle(50, 690, 850, 790, "white", 2, "black");

    index.addRectangle(90 -15, 733 -28, 117 -15, 760 -28, "MediumVioletRed	");
    index.addText(101 -15 +30, 752-28, ":", "black");
    index.addText(101 -15 +45, 752-28, "F", "black");
    index.addText(101 -15, 752-28, "1", "white");
    index.addText(100-15, 752-28, "1", "white");
    index.addText(99-15, 752-28, "1", "white");
    index.addText(101-15, 751-28, "1", "white");
    index.addText(100-15, 751-28, "1", "white");
    index.addText(99-15, 751-28, "1", "white");
    index.addRectangle(90 -15, 733 +16, 117 -15, 760 +16, "DodgerBlue");
    index.addText(101 -15 +30, 752+16, ":", "black");
    index.addText(101 -15 +45, 752+16, "E", "black");
    index.addText(101 -15, 752+16, "2", "white");
    index.addText(100-15, 752+16, "2", "white");
    index.addText(99-15, 752+16, "2", "white");
    index.addText(101-15, 751+16, "2", "white");
    index.addText(100-15, 751+16, "2", "white");
    index.addText(99-15, 751+16, "2", "white");

    index.addRectangle(90 -15 +130, 733 -28, 117 -15 +130, 760 -28, "Olive");
    index.addText(101 -15 +30 +130, 752-28, ":", "black");
    index.addText(101 -15 +45 +130, 752-28, "D", "black");
    index.addText(101 -15 +130, 752-28, "3", "white");
    index.addText(100-15 +130, 752-28, "3", "white");
    index.addText(99-15 +130, 752-28, "3", "white");
    index.addText(101-15 +130, 751-28, "3", "white");
    index.addText(100-15 +130, 751-28, "3", "white");
    index.addText(99-15 +130, 751-28, "3", "white");
    index.addRectangle(90 -15 +130, 733 +16, 117 -15 +130, 760 +16, "PowderBlue");
    index.addText(101 -15 +30 +130, 752+16, ":", "black");
    index.addText(101 -15 +45 +130, 752+16, "S", "black");
    index.addText(101-15 +130 , 752+16, "4", "white");
    index.addText(100-15 +130, 752+16, "4", "white");
    index.addText(99-15 +130, 752+16, "4", "white");
    index.addText(101-15 +130, 751+16, "4", "white");
    index.addText(100-15 +130, 751+16, "4", "white");
    index.addText(99-15 +130, 751+16, "4", "white");

    index.addRectangle(90 -15 +130 +130, 733 -28, 117 -15 +130 +130, 760 -28, "Goldenrod");
    index.addText(101 -15 +30 +130 +130, 752-28, ":", "black");
    index.addText(101 -15 +45 +130 +130, 752-28, "C", "black");
    index.addText(101 -15 +130 +130, 752-28, "5", "white");
    index.addText(100-15 +130 +130, 752-28, "5", "white");
    index.addText(99-15 +130 +130, 752-28, "5", "white");
    index.addText(101-15 +130 +130, 751-28, "5", "white");
    index.addText(100-15 +130 +130, 751-28, "5", "white");
    index.addText(99-15 +130 +130, 751-28, "5", "white");
    index.addRectangle(90 -15 +130 +130, 733 +16, 117 -15 +130 +130, 760 +16, "Crimson");
    index.addText(101 -15 +30 +130 +130, 752+16, ":", "black");
    index.addText(101 -15 +45 +130 +130, 752+16, "A", "black");
    index.addText(101-15 +130 +130 , 752+16, "6", "white");
    index.addText(100-15 +130 +130, 752+16, "6", "white");
    index.addText(99-15 +130 +130, 752+16, "6", "white");
    index.addText(101-15 +130 +130, 751+16, "6", "white");
    index.addText(100-15 +130 +130, 751+16, "6", "white");
    index.addText(99-15 +130 +130, 751+16, "6", "white");

    index.addRectangle(90 -15 +130 +130 +130, 733 -28, 117 -15 +130 +130 +130, 760 -28, "DarkOrange");
    index.addText(101 -15 +30 +130 +130 +130, 752-28, ":", "black");
    index.addText(101 -15 +45 +130 +130 +130, 752-28, "B", "black");
    index.addText(101 -15 +130 +130 +130, 752-28, "7", "white");
    index.addText(100-15 +130 +130 +130, 752-28, "7", "white");
    index.addText(99-15 +130 +130 +130, 752-28, "7", "white");
    index.addText(101-15 +130 +130 +130, 751-28, "7", "white");
    index.addText(100-15 +130 +130 +130, 751-28, "7", "white");
    index.addText(99-15 +130 +130 +130, 751-28, "7", "white");
    index.addRectangle(90 -15 +130 +130 +130, 733 +16, 117 -15 +130 +130 +130, 760 +16, "SeaGreen");
    index.addText(101 -15 +30 +130 +130 +130, 752+16, ":", "black");
    index.addText(101 -15 +45 +130 +130 +130, 752+16, "H", "black");
    index.addText(101-15 +130 +130 +130 , 752+16, "8", "white");
    index.addText(100-15 +130 +130 +130, 752+16, "8", "white");
    index.addText(99-15 +130 +130 +130, 752+16, "8", "white");
    index.addText(101-15 +130 +130 +130, 751+16, "8", "white");
    index.addText(100-15 +130 +130 +130, 751+16, "8", "white");
    index.addText(99-15 +130 +130 +130, 751+16, "8", "white");

    index.addRectangle(90 -15 +130 +130 +130 +130, 733 -7, 117 -15 +130 +130 +130 +130, 760 -7, "SaddleBrown");
    index.addText(101 -15 +30 +130 +130 +130 +130, 752-7, ":", "black");
    index.addText(101 -15 +45 +130 +130 +130 +130, 752-7, "G", "black");
    index.addText(101 -15 +130 +130 +130 +130, 752-7, "9", "white");
    index.addText(100-15 +130 +130 +130 +130, 752-7, "9", "white");
    index.addText(99-15 +130 +130 +130 +130, 752-7, "9", "white");
    index.addText(101-15 +130 +130 +130 +130, 751-7, "9", "white");
    index.addText(100-15 +130 +130 +130 +130, 751-7, "9", "white");
    index.addText(99-15 +130 +130 +130 +130, 751-7, "9", "white");
    //index.addRectangle(90 -15 +130 +130 +130 +130, 733 +16, 117 -15 +130 +130 +130 +130, 760 +16, "red");

    index.addRectangle(90 -15 +130 +130 +130 +130 +130, 733 -28, 117 -15 +130 +130 +130 +130 +130, 760 -28, "DarkMagenta");
    index.addText(101 -15 +30 +130 +130 +130 +130 +130, 752-28, ":", "black");
    index.addText(101 -15 +45 +130 +130 +130 +130 +130, 752-28, "K", "black");
    index.addText(101 -15 +130 +130 +130 +130 +128, 752-28, "A", "white");
    index.addText(100-15 +130 +130 +130 +130 +128, 752-28, "A", "white");
    index.addText(99-15 +130 +130 +130 +130 +128, 752-28, "A", "white");
    index.addText(101-15 +130 +130 +130 +130 +128, 751-28, "A", "white");
    index.addText(100-15 +130 +130 +130 +130 +128, 751-28, "A", "white");
    index.addText(99-15 +130 +130 +130 +130 +128, 751-28, "A", "white");
    index.addRectangle(90 -15 +130 +130 +130 +130 +130, 733 +16, 117 -15 +130 +130 +130 +130 +130, 760 +16, "DarkGray");
    index.addText(101 -15 +30 +130 +130 +130 +130 +130, 752+16, ":", "black");
    index.addText(101 -15 +45 +130 +130 +130 +130 +130, 752+16, "J", "black");
    index.addText(101-15 +130 +130 +130 +130 +128 , 752+16, "B", "white");
    index.addText(100-15 +130 +130 +130 +130 +128, 752+16, "B", "white");
    index.addText(99-15 +130 +130 +130 +130 +128, 752+16, "B", "white");
    index.addText(101-15 +130 +130 +130 +130 +128, 751+16, "B", "white");
    index.addText(100-15 +130 +130 +130 +130 +128, 751+16, "B", "white");
    index.addText(99-15 +130 +130 +130 +130 +128, 751+16, "B", "white");

    index.addText(70, 680, "Subway Lines :", "black");
}




void MexicoTrain(Svgfile& index)
{
    index.addRectangle(50, 690, 850, 790, "white", 2, "black");


/// A
    index.addRectangle(90 -15, 733 -28, 117 -15, 760 -28, "DodgerBlue");
    index.addText(101 -15 +30, 752-28, ":", "black");
    index.addText(101 -15 +45, 752-28, "FERROMEX", "black");
    index.addText(-2 + 101 -15, 752-28, "A", "white");
    index.addText(-2 + 100-15, 752-28, "A", "white");
    index.addText(-2 + 99-15, 752-28, "A", "white");
    index.addText(-2 + 101-15, 751-28, "A", "white");
    index.addText(-2 + 100-15, 751-28, "A", "white");
    index.addText(-2 + 99-15, 751-28, "A", "white");

/// B
    index.addRectangle(90 -15, 733 +16, 117 -15, 760 +16, "Crimson");
    index.addText(101 -15 +30, 752+16, ":", "black");
    index.addText(101 -15 +45, 752+16, "KCSM", "black");
    index.addText(-2 + 101 -15, 752+16, "B", "white");
    index.addText(-2 + 100-15, 752+16, "B", "white");
    index.addText(-2 + 99-15, 752+16, "B", "white");
    index.addText(-2 + 101-15, 751+16, "B", "white");
    index.addText(-2 + 100-15, 751+16, "B", "white");
    index.addText(-2 + 99-15, 751+16, "B", "white");



/// C
    index.addRectangle(-20 -70 + 90 -15 +130 +130, 733 -28, -20 -70 +  117 -15 +130 +130, 760 -28, "SeaGreen");
    index.addText(-20 -70 + 101 -15 +30 +130 +130, 752-28, ":", "black");
    index.addText(-20 -70 + 101 -15 +45 +130 +130, 752-28, "FERROSUR", "black");
    index.addText(-2-20 -70 + 101 -15 +130 +130, 752-28, "C", "white");
    index.addText(-2-20 -70 + 100-15 +130 +130, 752-28, "C", "white");
    index.addText(-2-20 -70 + 99-15 +130 +130, 752-28, "C", "white");
    index.addText(-2-20 -70 + 101-15 +130 +130, 751-28, "C", "white");
    index.addText(-2-20 -70 + 100-15 +130 +130, 751-28, "C", "white");
    index.addText(-2-20 -70 + 99-15 +130 +130, 751-28, "C", "white");

/// D
    index.addRectangle(-20 -70 + 90 -15 +130 +130, 733 +16,-20 -70 +  117 -15 +130 +130, 760 +16, "Pink");
    index.addText(-20 -70 + 101 -15 +30 +130 +130, 752+16, ":", "black");
    index.addText(-20 -70 + 101 -15 +45 +130 +130, 752+16, "COAHUILA - DURANGO", "black");
    index.addText(-2-20 -70 + 101-15 +130 +130 , 752+16, "D", "white");
    index.addText(-2-20 -70 + 100-15 +130 +130, 752+16, "D", "white");
    index.addText(-2-20 -70 + 99-15 +130 +130, 752+16, "D", "white");
    index.addText(-2-20 -70 + 101-15 +130 +130, 751+16, "D", "white");
    index.addText(-2-20 -70 + 100-15 +130 +130, 751+16, "D", "white");
    index.addText(-2-20 -70 + 99-15 +130 +130, 751+16, "D", "white");



/// E
    index.addRectangle(-230 + 90 -15 +130 +130 +130 +130 +130,733 -28, -230 + 117 -15 +130 +130 +130 +130 +130, 760 -28, "Goldenrod");
    index.addText(-230 + 101 -15 +30 +130 +130 +130 +130 +130, 752-28, ":", "black");
    index.addText(-105 + 101 -15 +45 +130 +130 +130 +130, 752-28, "FERROCARRIL DE CHIAPAS Y MAYAB", "black");
    index.addText(-101 + 101 -15 +130 +130 +130 +130, 752-28, "E", "white");
    index.addText(-101 + 100-15 +130 +130 +130 +130, 752-28, "E", "white");
    index.addText(-101 + 99-15 +130 +130 +130 +130, 752-28, "E", "white");
    index.addText(-101 + 101-15 +130 +130 +130 +130, 751-28, "E", "white");
    index.addText(-101 + 100-15 +130 +130 +130 +130, 751-28, "E", "white");
    index.addText(-101 + 99-15 +130 +130 +130 +130, 751-28, "E", "white");

/// F
    index.addRectangle(-230 +90 -15 +130 +130 +130 +130 +130, 733 +16,-230 + 117 -15 +130 +130 +130 +130 +130, 760 +16, "SaddleBrown");
    index.addText(-230 +101 -15 +30 +130 +130 +130 +130 +130, 752+16, ":", "black");
    index.addText(-105+101 -15 +45 +130 +130 +130 +130, 752+16, "FERROCARRIL DEL ISTMO", "black");
    index.addText(-101 +  101 -15 +130 +130 +130 +130, 752+16, "F", "white");
    index.addText(-101 +  100-15 +130 +130 +130 +130, 752+16, "F", "white");
    index.addText(-101 + 99-15 +130 +130 +130 +130, 752+16, "F", "white");
    index.addText(-101 + 101-15 +130 +130 +130 +130, 751+16, "F", "white");
    index.addText(-101 + 100-15 +130 +130 +130 +130, 751+16, "F", "white");
    index.addText(-101 + 99-15 +130 +130 +130 +130, 751+16, "F", "white");

    index.addText(70, 680, "Train Lines :", "black");
}




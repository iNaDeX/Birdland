#ifndef PERSO_H_INCLUDED
#define PERSO_H_INCLUDED

typedef struct perso
{
    int imgcourante;
    float x, y;          // coordonnée (du coin sup. gauche)
    float precx, precy;  // coordonnées précédant le déplacement
    float dx, dy;        // vecteur deplacement
    int vivant;          // 0 mort, 1 vivant
    float angle;
    int victoire;
    float max_x;
    int color;
    BITMAP* hitbox[3];
    int tx[3];
    int ty[3];
    double angle_collision;
    int timer_col;
    int aile;
    int taille;
} t_perso;

typedef struct listePerso
{
    int nbperso;
    int tailleperso;
    t_perso** perso;
}t_listePerso;

typedef struct check
{
    int actif;
    int taille;
    int x;
    int y;
    int existant;
} t_check;

typedef struct bonus
{
    float x;
    float y;
    float posx;
    float posy;
    int type;
    int actif;
    int existant;
    float dx;
    float dy;
    float ax;
    float ay;

            ///TYPE
    /*
    0: vitesse+
    1: vitesse-
    2: taille+
    3: taille-
    4: nombre+
    5: mur lave
    */

} t_bonus;

int actualiserPerso(t_perso *perso,int test,int aile,float *v_scroll,t_bonus tab [NNIVEAU][MAXBONUS],float* mapx,int niv,int taille, int *bonusPris, int soundOn,t_listePerso *lp, int *compteurMurLave);
int actualiserListePerso(t_listePerso *lp,int* compt,float *v_scroll,t_bonus tab [NNIVEAU][MAXBONUS],float* mapx,int niv, int *bonusPris, int soundOn, int *LancerCompteurMurLave,int *compteurMurLave);
t_listePerso* creer_liste_perso();
t_perso * creerPerso(int x,int y,int couleurblindee);
t_perso * ajouterPerso(t_listePerso *lp,int x,int y);
int contactPerso(t_perso*perso,int taille, int niv,t_listePerso *lp, int *compteurMurLave, int soundOn);
void tuer_Perso(t_listePerso *lp, int soundOn);
int obtention_couleurblindee(t_listePerso *lp);
int testperdu(t_listePerso *lp);
int contactBonus(t_perso *perso,t_bonus tab [NNIVEAU][MAXBONUS],float* mapx,int niv,int taille);
void appliquerBonus(int typeContact, t_listePerso *lp,t_perso*perso, float *v_scroll, int *LancerCompteurMurLave, int niv);
void appliquerCollision (t_perso* perso,float *v_scroll,int test, int i, int taille, int niv,t_listePerso *lp,int *compteurMurLave, int soundOn);
void magnetiserPerso(t_bonus tab[NNIVEAU][MAXBONUS],t_perso *perso,int niv,float *mapx, int *bonusPris, int soundOn);
void murLave(int niv);
void murNormal(int niv, int *compteurMurLave);

#endif // PERSO_H_INCLUDED

#ifndef MENU_H_INCLUDED
#define MENU_H_INCLUDED

typedef struct graphmenu
{
    BITMAP *img_menu, *cursor, *img_boutons[NIMGMENU], *img_perso[NIMGPERSO], *img_son[NIMGSON];
    SAMPLE *music, *flap;
    int boutons_x[NIMGMENU/2];
    int boutons_y[NIMGMENU/2];
} t_graphmenu;

void affMenu(t_graphmenu gm,t_bonus tab [NNIVEAU][MAXBONUS],t_check tab_check[NNIVEAU][MAXCHECK]);
void chargerContenuMenu(t_graphmenu* pgm);
int menuChoixNiveau();
void affRegles(t_graphmenu gm);
void affCredits(t_graphmenu gm);
void gestionniveau(int choixniveau,t_bonus tab [NNIVEAU][MAXBONUS],int soundOn,t_check tab_check[NNIVEAU][MAXCHECK], t_graphmenu gm);

#endif // MENU_H_INCLUDED

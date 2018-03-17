#ifndef PROJET_H_INCLUDED
#define PROJET_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <allegro.h>
#include <time.h>
#include <math.h>

#define NIMAGE 6
#define MAXI 100
#define NIMGMENU 8
#define NIMGPERSO 10
#define NIMGSON 4
#define NNIVEAU 3
#define MAXBONUS 12
#define NBTYPE 6
#define min(a,b) (a<=b?a:b)
#define NIMGRETOUR 2
#define boutonRetourX 60
#define boutonRetourY 170
#define MAXCHECK 10
#define NIMGPAUSE 4
#define boutonsPauseY 400
#define bouton1PauseX 50


extern BITMAP* page;
extern BITMAP* oiseau[3][NIMAGE];
extern BITMAP* fond[NNIVEAU];
extern BITMAP* collision;
extern BITMAP* bonus[NBTYPE];
extern BITMAP* level[NNIVEAU];
extern BITMAP* credits;
extern BITMAP* regles;
extern BITMAP* bravo;
extern BITMAP* cursor;
extern BITMAP* bmp_pause;
extern BITMAP* boutonsPause[NIMGPAUSE];
extern BITMAP* scie;
extern BITMAP* hitboxscie;
extern BITMAP* selectionNiveau;
extern BITMAP* boutonRetour[NIMGRETOUR];
extern BITMAP* miniatures[2*NNIVEAU];

extern SAMPLE* music[NNIVEAU];
extern SAMPLE* son_aspirer_bonus;
extern SAMPLE* son_tuer_perso;
extern SAMPLE* flap;
extern SAMPLE* sonScie;
extern SAMPLE* sonBravo;

#include "complement.h"
#include "perso.h"
#include "menu.h"

#endif // PROJET_H_INCLUDED

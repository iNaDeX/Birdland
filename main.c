#include "projet.h"

/*
Fonctionnalités du jeu Birdland 1.0
- Menu chargement avec une barre qui se remplit en fonction de la vitesse des chargements
- Couleur aléatoire pour la barre de chargement
- Le perso bat des ailes et pivote
- Le perso bat des ailes plus vite quand il monte que quand il descend
- Survol des boutons dans les menus
- Bouton dans le menu pour désactiver tous les sons
- Musiques(1 par niveau + celle du menu) et bruitages (mort,scie, attraction bonus, fin du jeu, battement d'ailes)
- Les bonus: Taille +, Taille -, Vitesse +, Vitesse -, Clonage, Murs toxiques
- Les bonus sont attirés sur le personnage le plus proche par magnétisation
- La position et le type des bonus est récupérée dans un fichier
- Décor mobile: scies circulaires qui tuent le personnage
- Transition fluide entre chaque niveau
- Un thème par niveau
- Utilisation de tableaux de pointeurs sur structures pour gérer les persos
- Utilisation de tableaux de structures pour gérer les bonus
- Checkpoints (dont la position est récupérée dans un fichier)
- Menu pause en appuyant sur échap
- Découpage en .c .h
- Compilation en mode release permet de compiler sans la console

*/

int jouerNiv(int niv,t_bonus tab [NNIVEAU][MAXBONUS],t_check tab_check[NNIVEAU][MAXCHECK], int soundOn);
void actualiser_bitmap_collision(float*mapx,t_listePerso* lp,int niv);
void affichage(float*mapx,t_listePerso* lp,int niv,t_bonus tab [NNIVEAU][MAXBONUS],float* anglescie);
void scroll(float * mapx,float* v_scroll,int niv,t_check tab_check[NNIVEAU][MAXCHECK],int taille);
int victoire (float * mapx,float* v_scroll,t_listePerso *lp,int niv);
void chargementVariablesGlobales(BITMAP* fondchargement,BITMAP* barrechargement);
int score(t_listePerso *lp);
void init_bonus(t_bonus tab [NNIVEAU][MAXBONUS]);
void afficherBonus(t_bonus tab [NNIVEAU][MAXBONUS], float* mapx,int niv);
void avancerchargement(BITMAP* fondchargement,BITMAP* barrechargement,int barres_a_afficher,int* barres_deja_affichees);
void avancerchargement(BITMAP* fondchargement,BITMAP* barrechargement,int barres_a_afficher,int* barres_deja_affichees);
void reinitBonus(t_bonus tab [NNIVEAU][MAXBONUS],int niv);
void checkpoint(float* mapx,int niv,t_check tab_check[NNIVEAU][MAXCHECK],int taille);
void init_check(t_check tab_check[NNIVEAU][MAXCHECK]);
void felicitations (int soundOn);
void premierePos(float *persox,float *persoy,t_check tab_check[NNIVEAU][MAXCHECK],int niv,t_listePerso *lp);
int pause(int niv, int *compteurMurLave);
void afficherScie(int niv,float* anglescie,float*mapx);
void afficherHitboxScie(int niv,float* mapx);
BITMAP* creerHitbox(BITMAP*img);
BITMAP* selectionaleatoirecouleurbarrechargement();

BITMAP* page;
BITMAP* collision;
BITMAP* level[NNIVEAU];
BITMAP* oiseau[3][NIMAGE];
BITMAP* fond[NNIVEAU];
BITMAP* bonus[NBTYPE];
BITMAP* credits;
BITMAP* regles;
BITMAP* selectionNiveau;
BITMAP* boutonRetour[NIMGRETOUR];
BITMAP* miniatures[2*NNIVEAU];
BITMAP* bravo;
BITMAP* cursor;
BITMAP* bmp_pause;
BITMAP* boutonsPause[NIMGPAUSE];
BITMAP* scie;
BITMAP* hitboxscie;

SAMPLE* music[NNIVEAU];
SAMPLE* son_aspirer_bonus;
SAMPLE* son_tuer_perso;
SAMPLE* flap;
SAMPLE* sonScie;
SAMPLE* sonBravo;

int pause(int niv, int *compteurMurLave)
{
    while(1)
    {
        int mx = mouse_x;
        int my = mouse_y;
        int i;
        int ecart = 325; // écart entre bouton menu et bouton reprendre
        int coordx[NIMGPAUSE];

        for (i=0; i<NIMGPAUSE; i++)
        {
            coordx[i] = bouton1PauseX + ecart*i;
        }

        //AFFICHAGE
        draw_sprite(page, bmp_pause, 0,0);
        for (i=0; i<NIMGPAUSE/2; i++)
        {
            if ((mx>=coordx[i] && mx<=coordx[i]+boutonsPause[i]->w) && my>=boutonsPauseY && my<=boutonsPauseY+boutonsPause[i]->h)
                draw_sprite(page, boutonsPause[i+NIMGPAUSE/2], coordx[i], boutonsPauseY);
            else
                draw_sprite(page, boutonsPause[i], coordx[i], boutonsPauseY);
        }
        draw_sprite(page, cursor, mx, my);
        blit(page, screen, 0,0,0,0, SCREEN_W, SCREEN_H);
        //CLICK
        if (((mx>=coordx[0] && mx<=coordx[0]+boutonsPause[0]->w) && my>=boutonsPauseY && my<=boutonsPauseY+boutonsPause[0]->h) && mouse_b==1)
        {
            murNormal(niv, compteurMurLave);
            return 3;
        }

        if ((mx>=coordx[1] && mx<=coordx[1]+boutonsPause[1]->w) && my>=boutonsPauseY && my<=boutonsPauseY+boutonsPause[1]->h && mouse_b==1)
            return -1;
    }
}

void reinitBonus(t_bonus tab [NNIVEAU][MAXBONUS],int niv)
{
    int j;

    for (j=0; j<MAXBONUS; j++)
    {
        if (tab[niv][j].existant==1)
        {
            tab[niv][j].actif=1;
            tab[niv][j].posx=tab[niv][j].x;
            tab[niv][j].posy=tab[niv][j].y;
            tab[niv][j].dx=0;
            tab[niv][j].dy=0;
            tab[niv][j].ax=0;
            tab[niv][j].ay=0;
        }
    }
}

void avancerchargement(BITMAP* fondchargement,BITMAP* barrechargement,int barres_a_afficher,int* barres_deja_affichees)
{
    int i;
    for(i=0; i<barres_a_afficher; i++)
    {
        draw_sprite(page,barrechargement,209+2*i+2* (*barres_deja_affichees),272);
        rest(2);
        blit(page,screen,0,0,0,0,SCREEN_W,SCREEN_H);
    }
    (*barres_deja_affichees) = (*barres_deja_affichees) + barres_a_afficher;
}

void chargementVariablesGlobales(BITMAP* fondchargement,BITMAP* barrechargement)
{
    int barres_deja_affichees=0;
    int i;

    blit(fondchargement,page,0,0,0,0,SCREEN_W,SCREEN_H);
    blit(page,screen,0,0,0,0,SCREEN_W,SCREEN_H);
    avancerchargement(fondchargement,barrechargement,20,&barres_deja_affichees);
    collision=create_bitmap(800,600);
    avancerchargement(fondchargement,barrechargement,20,&barres_deja_affichees);
    credits=chargerImage("files/menu/credits.bmp");
    avancerchargement(fondchargement,barrechargement,20,&barres_deja_affichees);
    regles=chargerImage("files/menu/regles.bmp");
    avancerchargement(fondchargement,barrechargement,20,&barres_deja_affichees);
    charger_fonds(fond);
    avancerchargement(fondchargement,barrechargement,20,&barres_deja_affichees);
    charger_maps(level);
    avancerchargement(fondchargement,barrechargement,20,&barres_deja_affichees);

    for(i=0; i<3; i++)
    {
        charger_birds(oiseau[i],i);
    }

    avancerchargement(fondchargement,barrechargement,20,&barres_deja_affichees);
    charger_bonus(bonus);
    avancerchargement(fondchargement,barrechargement,20,&barres_deja_affichees);
    charger_music(music);
    avancerchargement(fondchargement,barrechargement,30,&barres_deja_affichees);
    rest(80);

    //
    son_aspirer_bonus=chargerSon("files/sounds/aspirerBonus.wav");
    son_tuer_perso=chargerSon("files/sounds/sontuerperso.wav");
    flap=chargerSon("files/sounds/flap.wav");
    sonScie=chargerSon("files/sounds/scie.wav");
    sonBravo=chargerSon("files/sounds/felicitations.wav");

    selectionNiveau=chargerImage("files/menu/selectionNiveau.bmp");
    chargerBoutonRetour(boutonRetour);
    chargerMiniatures(miniatures);
    bravo = chargerImage("files/images/felicitations.bmp");
    cursor = chargerImage("files/menu/cursor.bmp");
    bmp_pause = chargerImage("files/images/pause.bmp");
    scie=chargerImage("files/images/scie.bmp");
    chargerBoutonsPause(boutonsPause);

    hitboxscie=creerHitbox(scie);
}

void reinit_check(t_check tab_check[NNIVEAU][MAXCHECK],int niv) // on réinitialise les checkpoints
{
    int i;
    for (i=0; i<MAXCHECK; i++)
    {
        if (tab_check[niv][i].existant==1)
        {
            if (tab_check[niv][i].actif==1)
            {
                tab_check[niv][i].actif=0;
            }
        }
    }
}

void transition (int niv,int soundOn) // transition entre 2 niveaux
{
    BITMAP* noir;
    noir=create_bitmap(SCREEN_W,SCREEN_H);

    float i=0;
    int compt=0;
    int aile=0;
    int sens=0;
    int imgcourante=0;
    float oiseau_x=0;
    while (!key[KEY_ESC])
    {
        oiseau_x=3*SCREEN_H-i;
        if (oiseau_x<SCREEN_W/3)
        {
            oiseau_x=SCREEN_W/3;
        }
        compt++;
        if (compt>5)
        {
        aile=1;
        compt=0;
        }

        if (aile==1)
        {
            if (sens==0)
            {
                imgcourante++;
                if (imgcourante>=NIMAGE)
                {
                    sens=1;
                    if (soundOn)
                    {
                        play_sample(flap, 80, 127, 1000,0);
                    }

                    aile=0;
                }

            }
            if (sens==1)
            {
                imgcourante--;
                if (imgcourante<=0)
                   sens=0;
                   aile=0;
            }

        }
        i=i+2.6; //empirique

        clear_bitmap(page);

        blit(fond[niv],page,(level[niv]->w-SCREEN_W)/((level[niv]->w/fond[niv]->w)*3)+i,0,0,0,SCREEN_W,SCREEN_H);

        masked_blit(level[niv],page,(level[niv]->w-SCREEN_W)+i,0,0,0,SCREEN_W,SCREEN_H);

        blit(noir,page,-SCREEN_W+i,0,0,0,SCREEN_W,SCREEN_H);

        blit(fond[niv+1],page,(-SCREEN_W*3)+i,0,0,0,SCREEN_W,SCREEN_H);

        masked_blit(level[niv+1],page,(-SCREEN_W*3)+i,0,0,0,SCREEN_W,SCREEN_H);

        draw_sprite (page,oiseau[1][imgcourante],oiseau_x,SCREEN_H/2);

        blit(page,screen,0,0,0,0,SCREEN_W,SCREEN_H);

        if ((-SCREEN_W*3)+i>0)
        {
            break;
        }

        rest(2);

    }
    destroy_bitmap(noir);
}

void gestionniveau(int choixniveau,t_bonus tab [NNIVEAU][MAXBONUS],int soundOn,t_check tab_check[NNIVEAU][MAXCHECK], t_graphmenu gm)
{
    int fini=0,niv;
    niv=choixniveau;
    while(!key[KEY_ESC] && fini!=2 && niv!=NNIVEAU)
    {
        reinitBonus(tab, niv);
        if(soundOn) play_sample(music[niv], 255, 127, 1000, 1);
        fini=jouerNiv(niv,tab,tab_check, soundOn);
        if(soundOn) stop_sample(music[niv]);
        if(fini==1)
        {
            reinit_check(tab_check,niv);
            if (niv<2)
            {
                transition(niv,soundOn);
            }
            niv++;
        }
        if (fini==3)
        {
            break;
        }
        if (niv==NNIVEAU) felicitations(soundOn);
    }
}

// on met l'oiseau à une certaine position apres le checkpoint
void premierePos(float *persox,float *persoy,t_check tab_check[NNIVEAU][MAXCHECK],int niv,t_listePerso *lp)
{
    int i;
    for (i=0; i<MAXCHECK; i++)
    {
        if (tab_check[niv][i].existant==1)
        {
            if (tab_check[niv][i].actif==1)
            {
                *persox=tab_check[niv][i].x;
                *persoy=tab_check[niv][i].y;
                lp->tailleperso=tab_check[niv][i].taille;
            }
        }

    }
}

void felicitations (int soundOn) // écran de victoire à la fin des 3 niveaux
{
    if (soundOn==1)
    {
        play_sample(sonBravo, 110, 127, 1000, 1);
    }
    while(mouse_b==0)
    {
        draw_sprite(page, bravo, 0,0);
        draw_sprite(page, cursor, mouse_x, mouse_y);
        blit(page, screen, 0,0,0,0, SCREEN_W, SCREEN_H);
    }
    stop_sample(sonBravo);
}

// boucle de jeu
int jouerNiv(int niv,t_bonus tab [NNIVEAU][MAXBONUS],t_check tab_check[NNIVEAU][MAXCHECK],int soundOn)
{
    t_listePerso* liste;
    liste=creer_liste_perso();
    float mapx=0;
    float v_scroll=2;
    int compt=0;
    int LancerCompteurMurLave=0;
    int compteurMurLave = 0;
    int fini=-1;
    int bonusPris=0;
    float anglescie=0;

    float persox=0;
    float persoy=SCREEN_H/2;

    premierePos(&persox,&persoy,tab_check,niv,liste);

    mapx=persox;
    ajouterPerso(liste,SCREEN_W/3,persoy);

    while (1)
    {
        compt++;
        if (LancerCompteurMurLave==1)
        {
            compteurMurLave++;
        }
        if (compteurMurLave == 1000)
        {
            murNormal(niv, &compteurMurLave);
        }
        if (key[KEY_ESC])
        {
            fini = pause(niv, &compteurMurLave);
        }
        if (fini!=-1) // si on continue la boucle
        {
            // Si fini == 0 => le joueur a perdu
            // Si fini == 1 => le niveau a été gagné
            // Si fini == 2 => le jeu est fini
            // Si fini == 3 => retour au menu principal
            return fini;
        }

        fini=victoire(&mapx,&v_scroll,liste,niv);
        if (fini!=-1) // s'il n'a pas perdu
        {
            return fini;
        }
        score(liste);
        scroll(&mapx,&v_scroll,niv,tab_check,liste->tailleperso);
        fini=actualiserListePerso(liste,&compt,&v_scroll,tab,&mapx,niv,&bonusPris, soundOn, &LancerCompteurMurLave, &compteurMurLave);
        if (fini!=-1) // s'il n'a pas perdu
        {
            return fini;
        }
        tuer_Perso(liste, soundOn);
        affichage(&mapx,liste,niv,tab,&anglescie);
        actualiser_bitmap_collision(&mapx,liste,niv);
        rest(8);
    }
    return fini;
}

void actualiser_bitmap_collision(float*mapx,t_listePerso* lp,int niv)
{
    int i;
    clear_to_color(collision,makecol(255,0,255)); // on clear la bitmap collision en magenta
    masked_blit(level[niv],collision,*mapx,0,0,0,SCREEN_W,SCREEN_H); // on affiche le niveau sur collision

    afficherHitboxScie(niv,mapx); // on affiche les hitbox des scies

    for (i=0; i<MAXI; i++)
    {
        if (lp->perso[i]!=NULL)
        {
            // on affiche la hitbox de chaque perso
            rotate_sprite(collision,lp->perso[i]->hitbox[lp->tailleperso],lp->perso[i]->x,lp->perso[i]->y,ftofix(lp->perso[i]->angle));
        }
    }
}

BITMAP* creerHitbox(BITMAP*img) // sousprogramme utilisé pour créer hitbox des scies
{
    int a,b;
    int couleur;
    BITMAP* nouv;
    nouv=create_bitmap(img->w,img->h);
    for (a=0; a<img->w; a++)
    {
        for (b=0; b<img->h; b++)
        {
            couleur=getpixel(img, a, b);
            if (couleur==makecol(255,0,255))
                putpixel(nouv, a, b, makecol(255,0,255));
            else
                putpixel(nouv, a, b, makecol(0,0,255));
        }
    }
    return nouv;
}

void afficherHitboxScie(int niv,float* mapx)
{
    switch(niv)
    {
    case 1:
        draw_sprite(collision,hitboxscie,4127-*mapx,-170);
        draw_sprite(collision,hitboxscie,4027-*mapx,SCREEN_H-170);
        draw_sprite(collision,hitboxscie,4360-*mapx,-130);
        draw_sprite(collision,hitboxscie,4247-*mapx,SCREEN_H-145);
        draw_sprite(collision,hitboxscie,4595-*mapx,-140);
        draw_sprite(collision,hitboxscie,4497-*mapx,SCREEN_H-140);
        draw_sprite(collision,hitboxscie,4795-*mapx,-100);
        draw_sprite(collision,hitboxscie,4697-*mapx,SCREEN_H-215);
        draw_sprite(collision,hitboxscie,5222-*mapx,75);
        break;
    case 2:
        draw_sprite(collision,hitboxscie,4118-*mapx,360);
        break;
    }
}

void afficherScie(int niv,float* anglescie,float*mapx)
{
    (*anglescie) = (*anglescie) - 1.5;
    switch(niv)
    {
    case 1:
        rotate_sprite(page,scie,4127-*mapx,-170,ftofix(*anglescie));
        rotate_sprite(page,scie,4027-*mapx,SCREEN_H-170,ftofix(*anglescie));
        rotate_sprite(page,scie,4360-*mapx,-130,ftofix(*anglescie));
        rotate_sprite(page,scie,4247-*mapx,SCREEN_H-145,ftofix(*anglescie));
        rotate_sprite(page,scie,4595-*mapx,-140,ftofix(*anglescie));
        rotate_sprite(page,scie,4497-*mapx,SCREEN_H-140,ftofix(*anglescie));
        rotate_sprite(page,scie,4795-*mapx,-100,ftofix(*anglescie));
        rotate_sprite(page,scie,4697-*mapx,SCREEN_H-215,ftofix(*anglescie));
        rotate_sprite(page,scie,5222-*mapx,75,ftofix(*anglescie));
        break;
    case 2:
        rotate_sprite(page,scie,4118-*mapx,360,ftofix(*anglescie));
        break;
    }
}

void affichage(float*mapx,t_listePerso* lp,int niv,t_bonus tab [NNIVEAU][MAXBONUS],float* anglescie)
{
    int i;

    blit(fond[niv],page,(*mapx)/((level[niv]->w/fond[niv]->w)*3),0,0,0,SCREEN_W,SCREEN_H);

    masked_blit(level[niv],page,*mapx,0,0,0,SCREEN_W,SCREEN_H);

    afficherScie(niv,anglescie,mapx);

    afficherBonus(tab,mapx,niv);

    for (i=0; i<MAXI; i++)
    {
        if (lp->perso[i]!=NULL)
        {
            rotate_sprite(page,oiseau[lp->tailleperso][lp->perso[i]->imgcourante],lp->perso[i]->x,lp->perso[i]->y,ftofix(lp->perso[i]->angle));
        }
    }
    blit(page,screen,0,0,0,0,SCREEN_W,SCREEN_H);
}

void afficherBonus (t_bonus tab [NNIVEAU][MAXBONUS],float* mapx,int niv)
{
    int i=0;
    for (i=0; i<MAXBONUS; i++)
    {
        if (tab[niv][i].actif!=0)
        {
            draw_sprite(page,bonus[tab[niv][i].type],tab[niv][i].posx - *mapx,tab[niv][i].posy);
        }
    }
}

int contactBonus(t_perso *perso,t_bonus tab [NNIVEAU][MAXBONUS],float* mapx,int niv,int taille)
{
    int i;
    float xc,yc;
    float xcb,ycb;
    float xv,yv;
    float norme;
    int contact = -1;
    xc=perso->x + (perso->tx[taille] / 2);
    yc=perso->y + (perso->ty[taille] / 2);
    for (i=0; i<MAXBONUS; i++)
    {
        if (tab[niv][i].actif!=0) /// si on est sur un bonus actif
        {
            xcb= tab[niv][i].posx - *mapx + (bonus[tab[niv][i].type]->w / 2);
            ycb= tab[niv][i].posy + (bonus[tab[niv][i].type]->h / 2);
            xv= xcb-xc;
            yv= ycb-yc;
            norme= sqrt(xv*xv+yv*yv);
            if(norme < (bonus[tab[niv][i].type]->h / 2)+(perso->tx[taille] /2) )
            {
                printf("bonus pris\n");
                tab[niv][i].actif=0;
                contact=tab[niv][i].type;
            }
        }
    }
    return contact;
}

void scroll(float * mapx,float* v_scroll,int niv,t_check tab_check[NNIVEAU][MAXCHECK],int taille)
{
    (*mapx)= (*mapx)+(*v_scroll);
    checkpoint (mapx,niv,tab_check,taille);
}

void checkpoint(float* mapx,int niv,t_check tab_check[NNIVEAU][MAXCHECK],int taille)
{
    int i;
    for (i=0; i<MAXCHECK; i++)
    {
        if (tab_check[niv][i].existant==1)
        {
            if (*mapx>tab_check[niv][i].x)
            {
                tab_check[niv][i].actif=1;
                tab_check[niv][i].taille=taille;
            }
        }
    }
}

int victoire (float * mapx,float* v_scroll,t_listePerso *lp,int niv)
{
    int i=0;
    int j=0;
    int pos=SCREEN_W;
    if (*mapx >= level[niv]->w-SCREEN_W)
    {
        *v_scroll=0;
        for (i=0; i<MAXI; i++)
        {
            if (lp->perso[i]!=NULL)
            {
                lp->perso[i]->victoire=1;
            }
        }
        for (i=0; i<MAXI; i++)
        {
            if (lp->perso[i]!=NULL)
            {
                if (lp->perso[i]->victoire==1)
                {
                    j++;
                }
            }
        }
        if (j == lp->nbperso)
        {
            for (i=0; i<MAXI; i++)
            {
                if (lp->perso[i]!=NULL)
                {
                    if (lp->perso[i]->victoire==1)
                    {
                        pos = min (pos,lp->perso[i]->x);
                    }
                }
            }
            if (pos==SCREEN_W)
            {
                printf("victoire\n");
                return 1 ; // mettre a 1 quand il n'y aura plus de niveau
            }
        }
    }
    return -1;
}

int score(t_listePerso *lp)
{
    int i=0;
    int score=0;
    for (i=0; i<MAXI; i++)
    {
        if (lp->perso[i]!=NULL)
        {
            if (lp->perso[i]->victoire==1) score++;
        }
    }
    return score;
}

void init_bonus (t_bonus tab [NNIVEAU][MAXBONUS])
{
    int i=0;
    int j=0;
    int k=0;
    char nomfichier[256];
    FILE*fp;

    for (j=0; j<NNIVEAU; j++)
    {
        // sprintf permet de faire un printf dans une chaine
        sprintf(nomfichier,"files/bonus%d.txt",j);
        fp=fopen(nomfichier,"r");
        if(fp==NULL)
        {
            allegro_message("pas pu charger bonus.txt");
            allegro_exit();
            exit(EXIT_FAILURE);
        }
        while (!feof(fp))
        {
            fscanf(fp,"%f",&tab[j][i].posx);
            tab[j][i].x=tab[j][i].posx;
            fscanf(fp,"%f",&tab[j][i].posy);
            tab[j][i].y=tab[j][i].posy;
            fscanf(fp,"%d",&tab[j][i].type);
            tab[j][i].actif=1;
            tab[j][i].existant=1;
            tab[j][i].dx=0;
            tab[j][i].dy=0;
            i++;
        }
        for (k=i; k<MAXBONUS; k++)
        {
            tab[j][k].actif=0;
            tab[j][k].existant=0;
        }
        i=0;
        fclose(fp);
    }
}

void init_check(t_check tab_check[NNIVEAU][MAXCHECK]) // on initialise les checkpoints
{
    int i=0;
    int j=0;
    int k=0;
    char nomfichier[256];
    FILE*fp;

    for (j=0; j<NNIVEAU; j++)
    {
        // sprintf permet de faire un printf dans une chaine
        sprintf(nomfichier,"files/checkpoint%d.txt",j);
        fp=fopen(nomfichier,"r");
        if(fp==NULL)
        {
            allegro_message("pas pu charger checkpoint.txt");
            allegro_exit();
            exit(EXIT_FAILURE);
        }
        while (!feof(fp))
        {
            fscanf(fp,"%d",&tab_check[j][i].x);
            fscanf(fp,"%d",&tab_check[j][i].y);
            tab_check[j][i].actif=0;
            tab_check[j][i].existant=1;
            i++;
        }
        for (k=i; k<MAXCHECK; k++)
        {
            tab_check[j][k].actif=0;
            tab_check[j][k].existant=0;
        }
        i=0;
        fclose(fp);
    }
}

BITMAP* selectionaleatoirecouleurbarrechargement()
{
    int x;
    BITMAP* nouv;
    char ch[256];
    x= rand()%6;
    sprintf(ch,"files/images/barrechargement%d.bmp",x);
    nouv= chargerImage(ch);
    return nouv;
}

int main()
{
    lancerToutAllegro(800,600);
    srand(time(NULL));

    t_graphmenu gm;
    t_bonus tab[NNIVEAU][MAXBONUS];
    t_check tab_check[NNIVEAU][MAXCHECK];

    BITMAP* fondchargement=chargerImage("files/images/fondchargement.bmp");
    BITMAP* barrechargement= selectionaleatoirecouleurbarrechargement();

    chargementVariablesGlobales(fondchargement,barrechargement);

    init_bonus(tab);
    init_check(tab_check);

    chargerContenuMenu(&gm);
    affMenu(gm,tab,tab_check);

    return 0;
}
END_OF_MAIN();

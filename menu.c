#include "projet.h"

int menuChoixNiveau(t_graphmenu gm)
{
    int i;
    int coord[NNIVEAU]; //Tableau dans lequel on range les coordonnées des miniatures
    int miniature1_x = 40; //Coordonnée en x de la première miniature
    int miniature_y = 250; //Coordonnée en y des miniatures
    int ecart = 252; //Ecart entre les miniatures

    for (i=0; i<NNIVEAU; i++)
    {
        coord[i] = miniature1_x + i*ecart;
    }
    while(1)
    {
        int mx = mouse_x;
        int my = mouse_y;

        //AFFICHAGE
        draw_sprite(page, selectionNiveau, 0,0);
        if (mx>=boutonRetourX && mx<=boutonRetourX+boutonRetour[0]->w && my>=boutonRetourY && my<=boutonRetourY+boutonRetour[0]->h)
            draw_sprite(page, boutonRetour[1], boutonRetourX,boutonRetourY);
        else
            draw_sprite(page, boutonRetour[0], boutonRetourX,boutonRetourY);
        for (i=0; i<NNIVEAU; i++)
        {
            if (mx>=coord[i] && mx<=coord[i]+miniatures[0]->w && my>=miniature_y && my<=miniature_y+miniatures[0]->h)
                draw_sprite(page, miniatures[i+NNIVEAU], coord[i], miniature_y);
            else
                draw_sprite(page, miniatures[i], coord[i], miniature_y);
        }
        draw_sprite(page, gm.cursor, mx, my);
        blit(page, screen, 0,0,0,0, SCREEN_W, SCREEN_H);
        rest(10);

        //CLICK
        if (mx>=boutonRetourX && mx<=boutonRetourX+boutonRetour[0]->w && my>=boutonRetourY && my<=boutonRetourY+boutonRetour[0]->h && mouse_b==1)
            return -1;

        if (mx>=coord[0] && mx<=coord[0]+miniatures[0]->w && my>=miniature_y && my<=miniature_y+miniatures[0]->h && mouse_b == 1)
        {
            return 0;
        }
        if (mx>=coord[1] && mx<=coord[1]+miniatures[0]->w && my>=miniature_y && my<=miniature_y+miniatures[0]->h && mouse_b == 1)
        {
            return 1;
        }
        if (mx>=coord[2] && mx<=coord[2]+miniatures[0]->w && my>=miniature_y && my<=miniature_y+miniatures[0]->h && mouse_b == 1)
        {
            return 2;
        }
    }
}

void affCredits(t_graphmenu gm)
{
    while(1)
    {
        int mx = mouse_x;
        int my = mouse_y;
        //AFFICHAGE
        draw_sprite(page, credits, 0,0);
        if (mx>=boutonRetourX && mx<=boutonRetourX+boutonRetour[0]->w && my>=boutonRetourY && my<=boutonRetourY+boutonRetour[0]->h)
            draw_sprite(page, boutonRetour[1], boutonRetourX,boutonRetourY);
        else
            draw_sprite(page, boutonRetour[0], boutonRetourX,boutonRetourY);

        //CLICK
        if (mx>=boutonRetourX && mx<=boutonRetourX+boutonRetour[0]->w && my>=boutonRetourY && my<=boutonRetourY+boutonRetour[0]->h && mouse_b==1)
            break;

        draw_sprite(page, gm.cursor, mx, my);
        blit(page, screen, 0,0,0,0, SCREEN_W, SCREEN_H);
    }
}

void affRegles(t_graphmenu gm)
{
    while(1)
    {
        int mx = mouse_x;
        int my = mouse_y;
        //AFFICHAGE
        draw_sprite(page, regles, 0, 0);
        if (mx>=boutonRetourX && mx<=boutonRetourX+boutonRetour[0]->w && my>=boutonRetourY && my<=boutonRetourY+boutonRetour[0]->h)
            draw_sprite(page, boutonRetour[1], boutonRetourX,boutonRetourY);
        else
            draw_sprite(page, boutonRetour[0], boutonRetourX,boutonRetourY);

        //CLICK
        if (mx>=boutonRetourX && mx<=boutonRetourX+boutonRetour[0]->w && my>=boutonRetourY && my<=boutonRetourY+boutonRetour[0]->h && mouse_b==1)
            break;

        draw_sprite(page, gm.cursor, mx, my);
        blit(page, screen, 0,0,0,0, SCREEN_W, SCREEN_H);
    }
}

void affMenu(t_graphmenu gm,t_bonus tab [NNIVEAU][MAXBONUS],t_check tab_check[NNIVEAU][MAXCHECK])
{
    int mx, my, i;

    int niveauchoisi;
    int retourmenu=1;

    int decalPerso[10];
    decalPerso[0] = -6;
    decalPerso[1] = decalPerso[9] = -4;
    decalPerso[2] = decalPerso[8] = -2;
    decalPerso[3] = decalPerso[7] = 0;
    decalPerso[4] = decalPerso[6] = 2;
    decalPerso[5] = 4;

    int click=0, compteur=0, compteur2=0, soundOn=1;

    while (!key[KEY_ESC])
    {
        if (soundOn && retourmenu==1)
        {
            play_sample(gm.music, 255, 127, 1000, 1);
            retourmenu = 0;
        }
        mx=mouse_x;
        my=mouse_y;
        draw_sprite(page, gm.img_menu, 0,0);
        //AFFICHAGE
        for (i=0; i<NIMGMENU/2; i++)
        {
            if (mx>=gm.boutons_x[i] && mx<=(gm.boutons_x[i]+gm.img_boutons[i]->w) && my>=gm.boutons_y[i] && my<=(gm.boutons_y[i]+gm.img_boutons[i]->h))
                draw_sprite(page, gm.img_boutons[i+NIMGMENU/2], gm.boutons_x[i], gm.boutons_y[i]);
            else
                draw_sprite(page, gm.img_boutons[i], gm.boutons_x[i], gm.boutons_y[i]);
        }
        if (mx>=640 && mx<=640+gm.img_son[0]->w && my>=150 && my<=150+gm.img_son[0]->h)
        {
            if (soundOn)
                draw_sprite(page, gm.img_son[2], 640, 150);
            else
                draw_sprite(page, gm.img_son[3], 640, 150);
        }
        else
        {
            if (soundOn)
                draw_sprite(page, gm.img_son[0], 640, 150);
            else
                draw_sprite(page, gm.img_son[1], 640, 150);
        }
        draw_sprite(page, gm.img_perso[compteur], SCREEN_W/2-gm.img_perso[0]->w/2, 280+decalPerso[compteur]);
        draw_sprite(page, gm.cursor, mx, my);
        blit(page, screen, 0,0,0,0, SCREEN_W, SCREEN_H);

        //CLICK
        if (mouse_b==1 && click==0)
        {
            if (mx>=gm.boutons_x[0] && mx<=(gm.boutons_x[0]+gm.img_boutons[0]->w) && my>=gm.boutons_y[0] && my<=(gm.boutons_y[0]+gm.img_boutons[0]->h))
            {
                //JOUER
                printf("jouer\n");
                /// on lance le ssprog jouerpartie
                niveauchoisi = menuChoixNiveau(gm);
                if (niveauchoisi != -1)
                {
                    stop_sample(gm.music);
                    gestionniveau(niveauchoisi,tab,soundOn,tab_check, gm);
                }
                retourmenu=1;

            }
            if (mx>=gm.boutons_x[1] && mx<=(gm.boutons_x[1]+gm.img_boutons[1]->w) && my>=gm.boutons_y[1] && my<=(gm.boutons_y[1]+gm.img_boutons[1]->h))
            {
                //QUITTER
                break;
            }
            if (mx>=gm.boutons_x[2] && mx<=(gm.boutons_x[2]+gm.img_boutons[2]->w) && my>=gm.boutons_y[2] && my<=(gm.boutons_y[2]+gm.img_boutons[2]->h))
            {
                //REGLES
                affRegles(gm);
            }
            if (mx>=gm.boutons_x[3] && mx<=(gm.boutons_x[3]+gm.img_boutons[3]->w) && my>=gm.boutons_y[3] && my<=(gm.boutons_y[3]+gm.img_boutons[3]->h))
            {
                //CREDITS
                affCredits(gm);
            }
            if (mx>=640 && mx<=640+gm.img_son[0]->w && my>=150 && my<=150+gm.img_son[0]->h)
            {
                if (soundOn)
                {
                    soundOn=0;
                    stop_sample(gm.music);
                }
                else
                {
                    soundOn=1;
                    play_sample(gm.music, 255, 127, 1000, 1);
                }
            }
            click=1;
        }
        if (mouse_b==0 && click==1)
        {
            click = 0;
        }
        compteur2++;
        if (compteur2%6==0) // changement d'image du perso tous les 6 tours de boucle
        {
            compteur++; // on change l'image du perso qu'on affiche
            if (compteur==5 && soundOn)
                play_sample(gm.flap, 80, 127, 1000, 0);
        }

        if (compteur==10) // on remet le compteur à 0 car il n'y a que 10 images d'oiseau
        {
            compteur=0;
        }
        rest(10);
    }
}

void chargerContenuMenu(t_graphmenu* pgm)
{
    int i;
    char filename[100];
    FILE *boutons_xy;
    boutons_xy = fopen("files/menu/boutons_xy.txt", "r");
    pgm->music = load_sample("files/menu/music.wav");
    if (pgm->music==NULL)
    {
        allegro_message("pas pu trouver/charger music.wav");
        allegro_exit();
        exit(EXIT_FAILURE);
    }
    pgm->flap = load_sample("files/menu/flap.wav");
    if (pgm->flap==NULL)
    {
        allegro_message("pas pu trouver/charger flap.wav");
        allegro_exit();
        exit(EXIT_FAILURE);
    }

    if (boutons_xy==NULL)
    {
        allegro_message("Erreur chargement du fichier \"boutons_xy.txt\"");
        allegro_exit();
        exit(EXIT_FAILURE);
    }
    rewind(boutons_xy);
    for(i=0; i<NIMGMENU/2; i++)
    {
        fscanf(boutons_xy, "%d", &pgm->boutons_x[i]);
        fscanf(boutons_xy, "%d", &pgm->boutons_y[i]);
    }
    pgm->img_menu = chargerImage("files/menu/menu.bmp");
    pgm->cursor = chargerImage("files/menu/cursor.bmp");
    for (i=0; i<NIMGMENU; i++)
    {
        sprintf(filename, "files/menu/bouton%d.bmp", i);
        pgm->img_boutons[i] = chargerImage(filename);
    }
    for (i=0; i<NIMGSON; i++)
    {
        sprintf(filename, "files/menu/son%d.bmp", i);
        pgm->img_son[i] = chargerImage(filename);
    }
    for (i=0; i<6; i++)
    {
        sprintf(filename, "files/menu/bird%d.bmp", i);
        pgm->img_perso[i] = chargerImage(filename);
    }

    for (i=0; i<4; i++)
    {
        pgm->img_perso[i+6] = pgm->img_perso[4-i];
    }
}

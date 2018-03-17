#include "projet.h"

void chargerBoutonsPause(BITMAP* boutonsPause[NIMGPAUSE])
{
    char nomfichier[256];
    int i;
    for (i=0; i<NIMGPAUSE; i++)
    {
        // sprintf permet de faire un printf dans une chaine
        sprintf(nomfichier,"files/images/pauseBouton%d.bmp",i);

        boutonsPause[i] = load_bitmap(nomfichier,NULL);
        if (!boutonsPause[i])
        {
            allegro_message("pas pu trouver %s",nomfichier);
            exit(EXIT_FAILURE);
        }
    }
}

SAMPLE* chargerSon(char* nomfichier)
{
    SAMPLE* nouv;
    nouv=load_sample(nomfichier);
    if(nouv==NULL)
    {
        allegro_message("pas pu trouver/charger %s",nomfichier);
        allegro_exit();
        exit(EXIT_FAILURE);
    }
    return nouv;
}

void charger_birds (BITMAP* img[NIMAGE],int i)
{
    char nomfichier[256];
    char petitnom[256];
    int j;

    if (i==0)
    {
        sprintf(petitnom,"petit");
    }
    if (i==1)
    {
        sprintf(petitnom,""); // c'est normal que cette chaine soit vide
    }
    if (i==2)
    {
        sprintf(petitnom,"gros");
    }

    for (j=0; j<NIMAGE; j++)
    {
        // sprintf permet de faire un printf dans une chaine
        sprintf(nomfichier,"files/images/%sbird%d.bmp",petitnom,j);

        img[j] = load_bitmap(nomfichier,NULL);
        if (!img[j])
        {
            allegro_message("pas pu trouver %s",nomfichier);
            exit(EXIT_FAILURE);
        }
    }
}

void charger_fonds (BITMAP* img[NNIVEAU])
{
    char nomfichier[256];
    int i;
    for (i=0; i<NNIVEAU; i++)
    {
        // sprintf permet de faire un printf dans une chaine
        sprintf(nomfichier,"files/images/fond%d.bmp",i);

        img[i] = load_bitmap(nomfichier,NULL);
        if (!img[i])
        {
            allegro_message("pas pu trouver %s",nomfichier);
            exit(EXIT_FAILURE);
        }
    }
}

void charger_maps (BITMAP* img[NNIVEAU])
{
    char nomfichier[256];
    int i;
    for (i=0; i<NNIVEAU; i++)
    {
        // sprintf permet de faire un printf dans une chaine
        sprintf(nomfichier,"files/images/map%d.bmp",i);

        img[i] = load_bitmap(nomfichier,NULL);
        if (!img[i])
        {
            allegro_message("pas pu trouver %s",nomfichier);
            exit(EXIT_FAILURE);
        }
    }
}

void chargerMiniatures(BITMAP *miniatures[2*NNIVEAU])
{
    char nomfichier[256];
    int i;
    for (i=0; i<2*NNIVEAU; i++)
    {
        sprintf(nomfichier, "files/menu/miniature%d.bmp", i);

        miniatures[i] = load_bitmap(nomfichier,NULL);
        if (!miniatures[i])
        {
            allegro_message("pas pu trouver %s",nomfichier);
            exit(EXIT_FAILURE);
        }
    }
}

void chargerBoutonRetour(BITMAP *boutonRetour[NIMGRETOUR])
{
    char nomfichier[256];
    int i;
    for (i=0; i<NIMGRETOUR; i++)
    {
        sprintf(nomfichier, "files/menu/boutonRetour%d.bmp", i);

        boutonRetour[i] = load_bitmap(nomfichier,NULL);
        if (!boutonRetour[i])
        {
            allegro_message("pas pu trouver %s",nomfichier);
            exit(EXIT_FAILURE);
        }
    }
}

void charger_bonus(BITMAP* img[NBTYPE])
{
    char nomfichier[256];
    int i;
    for (i=0; i<NBTYPE; i++)
    {
        // sprintf permet de faire un printf dans une chaine
        sprintf(nomfichier,"files/images/bonus%d.bmp",i);

        img[i] = load_bitmap(nomfichier,NULL);
        if (!img[i])
        {
            allegro_message("pas pu trouver %s",nomfichier);
            exit(EXIT_FAILURE);
        }
    }
}

void charger_music(SAMPLE* music[NNIVEAU])
{
    char nomfichier[256];
    int i;
    for (i=0; i<NNIVEAU; i++)
    {
        // sprintf permet de faire un printf dans une chaine
        sprintf(nomfichier,"files/sounds/music%d.wav",i);
        music[i] = load_sample(nomfichier);
        if (music[i]==NULL)
        {
            allegro_message("pas pu trouver/charger %s",nomfichier);
            allegro_exit();
            exit(EXIT_FAILURE);
        }
    }
}

BITMAP* chargerImage(char *nomFichierImage)
{
    BITMAP *bmp;
    bmp=load_bitmap(nomFichierImage,NULL);
    if (bmp==NULL)
    {
        allegro_message("pas pu trouver/charger %s",nomFichierImage);
        allegro_exit();
        exit(EXIT_FAILURE);
    }
    printf("Image charg\202e : %s\n", nomFichierImage);
    return bmp;
}

void lancerToutAllegro(int largeur, int hauteur)
{
    allegro_init();
    install_keyboard();
    install_mouse();
    install_sound(DIGI_AUTODETECT,MIDI_AUTODETECT,NULL);

    /// OUVERTURE MODE GRAPHIQUE (ouverture fenêtre allegro)
    set_color_depth(desktop_color_depth());
    if (set_gfx_mode(GFX_AUTODETECT_WINDOWED,largeur,hauteur,0,0)!=0)
    {
        allegro_message("probleme mode graphique : %s", allegro_error);
        allegro_exit();
        exit(EXIT_FAILURE);
    }
    //show_mouse(screen); // Affiche pointeur de souris en mode allegro

    // CREATION DU BUFFER D'AFFICHAGE à la taille de l'écran
    page=create_bitmap(SCREEN_W,SCREEN_H);
    clear_bitmap(page);
}

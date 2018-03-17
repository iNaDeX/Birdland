#ifndef COMPLEMENT_H_INCLUDED
#define COMPLEMENT_H_INCLUDED

BITMAP* chargerImage(char *nomFichierImage);
void charger_birds (BITMAP* img[NIMAGE],int i);
void lancerToutAllegro(int largeur, int hauteur);
void charger_maps (BITMAP* img[NNIVEAU]);
void charger_bonus(BITMAP* img[NBTYPE]);
void charger_fonds (BITMAP* img[NNIVEAU]);
void charger_music(SAMPLE* music[NNIVEAU]);
void chargerMiniatures(BITMAP *miniatures[2*NNIVEAU]);
void chargerBoutonRetour(BITMAP *boutonRetour[NIMGRETOUR]);
SAMPLE* chargerSon(char* nomfichier);
void chargerBoutonsPause(BITMAP *boutonsPause[NIMGPAUSE]);

#endif // COMPLEMENT_H_INCLUDED

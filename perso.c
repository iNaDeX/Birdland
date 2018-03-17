#include "projet.h"

void tuer_Perso(t_listePerso *lp, int soundOn)
{
    int i;
    for (i=0; i<MAXI; i++)
    {
        if (lp->perso[i]!=NULL) // on étudie tous les persos vivants
        {
            if (lp->perso[i]->x < -lp->perso[i]->hitbox[lp->tailleperso]->w) // si un perso mérite de mourir
            {
                destroy_bitmap(lp->perso[i]->hitbox[lp->tailleperso]);
                free(lp->perso[i]);
                lp->perso[i]=NULL;
                lp->nbperso--;
                printf("le perso est mort\n");
                if (soundOn) play_sample(son_tuer_perso, 200, 127, 1000, 0);
            }
        }
    }
}

int testperdu(t_listePerso *lp)
{
    int perdu=0;
    if (lp->nbperso == 0) /// SI IL N Y A PLUS DE PERSOS
    {
        perdu=1;
    }
    return perdu;
}

int obtention_couleurblindee(t_listePerso *lp)// ni noire ni magenta ni d'une autre couleur d'une hitbox déjà existante
{
    int couleur,probleme;
    int i;
    do
    {
        probleme=0;
        couleur=makecol(1+rand()%255,1+rand()%255,1+rand()%255);
        for (i=0; i<MAXI; i++)
        {
            if (lp->perso[i]!=NULL)
            {
                if (couleur == lp->perso[i]->color) // si la couleur qu'on crée existe déjà
                {
                    probleme=1;
                }
            }
        }
    }
    while(probleme==1);

    return couleur;
}

int contactPerso(t_perso*perso,int taille, int niv,t_listePerso *lp, int *compteurMurLave, int soundOn)
{
    int contact=0;
    int i,j;
    double anglegrad;
    int magenta = makecol(255,0,255);
    int xc,yc;
    xc=26; // coordonnées centre hitbox
    yc=43; // coordonnées centre hitbox

    perso->angle_collision=0;

    if (perso->x>=0) // on ne détecte les collisions pour le perso que s'il est dans l'écran
    {
        for(i=0; i<perso->hitbox[taille]->w; i++) // on parcourt tous les pixels de la hitbox
        {
            for(j=0; j<perso->hitbox[taille]->h; j++)
            {
                if (getpixel(perso->hitbox[taille],i,j)== perso->color)
                {
                    if (getpixel(collision,perso->x+perso->dx+i,perso->y+perso->dy+j) == makecol(200,0,0) )
                    {
                        perso->x=-100; //Le personnage a touché un mur rouge, il est tué (emporté en coordonnée négative).
                        if (lp->nbperso==1)
                        {
                            murNormal(niv, compteurMurLave);
                        }
                    }
                    if (getpixel(collision,perso->x+perso->dx+i,perso->y+perso->dy+j) == makecol(0,0,255) )// si contact avec scie
                    {
                        perso->x=-100;
                        if (soundOn) play_sample(sonScie, 200, 127, 1000, 0);
                    }
                }
            }
        }

        for(i=0; i<perso->hitbox[taille]->w; i++) // on parcourt tous les pixels de la hitbox
        {
            if (contact==1) break;
            for(j=0; j<perso->hitbox[taille]->h; j++)
            {
                if (contact==1) break;
                if (getpixel(perso->hitbox[taille],i,j)== perso->color) // si on tombe sur un pixel de la vraie hitbox (pas du magenta)
                {
                    if ((getpixel(collision,perso->x+perso->dx+i,perso->y+perso->dy+j) != magenta) && (getpixel(collision,perso->x+perso->dx+i,perso->y+perso->dy+j) != perso->color)) // si à l'endroit où on voudrait afficher le perso il y a déjà quelquechose
                    {
                        if ((i+perso->x+perso->dx >0) && (i+perso->x+perso->dx <800))
                        {
                            contact = 1;
                            anglegrad= acos( (i-xc) / sqrt(   (i-xc)*(i-xc)+(j-yc)*(j-yc)   )  );
                            perso->angle_collision=anglegrad * 180.0 / 3.14;

                            if (perso->dy>0) perso->angle_collision=-perso->angle_collision;
                        }
                    }
                }
            }
        }
    }

    else
    {
        perso->dx=-2;
    }

    return contact;
}

// attire les bonus vers le perso le plus proche
void magnetiserPerso(t_bonus tab[NNIVEAU][MAXBONUS],t_perso *perso,int niv,float *mapx, int *bonusPris, int soundOn)
{
    float vx,vy,norme;
    float facteurForce = 2000.0; // Réglage empirique
    int i;
    float visco=0.02;

    for (i=0; i<MAXBONUS; i++)
    {
        if (tab[niv][i].actif!=0)
        {
            vx = perso->x+*mapx - tab[niv][i].posx;
            vy = perso->y - tab[niv][i].posy;
            norme = sqrt( vx*vx + vy*vy );


            if ((norme < 102) && (norme >97))
            {
                if (*bonusPris == 0)
                {
                    if (soundOn) play_sample(son_aspirer_bonus, 255, 127, 1000, 0);
                    *bonusPris=1;
                }
            }
            if ( norme <250 )
            {
                // Normalisation (ATTENTION DIVISION PAR 0)
                vx = vx/(norme*norme*norme);
                vy = vy/(norme*norme*norme);
            }
            else
            {
                vx = 0.0;
                vy = 0.0;
            }

            tab[niv][i].ax = facteurForce*vx;
            tab[niv][i].ay = facteurForce*vy;

            tab[niv][i].dx *= (1.0-visco);
            tab[niv][i].dy *= (1.0-visco);

            tab[niv][i].dx = tab[niv][i].dx + tab[niv][i].ax;
            tab[niv][i].dy = tab[niv][i].dy + tab[niv][i].ay;

            tab[niv][i].posx = tab[niv][i].posx + tab[niv][i].dx;
            tab[niv][i].posy = tab[niv][i].posy + tab[niv][i].dy;
        }
    }
}

int actualiserPerso(t_perso *perso,int test,int aile,float *v_scroll,t_bonus tab [NNIVEAU][MAXBONUS],float* mapx,int niv,int taille, int *bonusPris, int soundOn,t_listePerso *lp, int *compteurMurLave)
{
    float r;

    perso->angle_collision=0; //reintialisation de l'angle de collision pour chaque oiseau

    int contact;

    magnetiserPerso(tab,perso,niv,mapx, bonusPris, soundOn);  //attirer bonus

    int typeContact = contactBonus(perso,tab,mapx,niv,taille);    //verifie contact avec les bonus les plus proches


    perso->precx = perso->x;
    perso->precy = perso->y;            //sauvegarde de x et de y

    if (test==1) //touche espace enfoncée
    {
        //caractère aléatoire du saut (compris entre -1 et 1) ((RAND_MAX/2)=2)
        r = (float)rand() / (float)(RAND_MAX/2)-1;

        //saut + caractere aleatoire
        perso->dy= -2.2+r;
        perso->angle= perso->angle - 4;         // changer angle de l'oiseau
    }

    perso->x += perso->dx;
    perso->y += perso->dy;  //actualisation de la position en x et y

    contact=contactPerso(perso,taille, niv, lp, compteurMurLave, soundOn);      //verifie les collisions entre acteurs, et les collisions avec la map

    if (aile==1)                    // l'oiseau change d'image
    {
        if (perso->aile==0)         //sens 1
        {
            perso->imgcourante++;
            if (perso->imgcourante>=NIMAGE)
            {
                perso->aile=1;
                if (key[KEY_SPACE] && soundOn) play_sample(flap, 80, 127, 1000,0);      //son de battement d'ailes
            }

        }
        if (perso->aile==1)         //sens 2
        {
            perso->imgcourante--;
            if (perso->imgcourante<=0)
                perso->aile=0;
        }

    }

    if (contact==0)     //actions si il n'y a pas contact
    {
        if (perso->victoire==1)     //si le scroll est fini
        {
            perso->dx=2;
        }
        else  if (perso->x < perso->max_x)
        {
            perso->dx=perso->dx+0.05;
            if (perso->dx>0.5)
                perso->dx=0.5;
        }
        else if (perso->x == perso->max_x)
        {
            perso->dx = 0;
        }
        else if (perso->x > perso->max_x)
        {
            perso->dx = -0.2;
        }
    }

    if (contact==1)    // si il y a contact
    {
        appliquerCollision(perso,v_scroll,test,0,taille, niv,lp,compteurMurLave, soundOn);
    }

    perso->angle=perso->angle+0.4;      //gestion des angles
    if (perso->angle>=10)
        perso->angle=10;

    if (perso->angle<=-15)
        perso->angle=-15;


    perso->dy=perso->dy + 0.07;         // gravité

    return typeContact;
}

void murLave(int niv) // transforme les murs en lave
{
    floodfill(level[niv], 1000, 1, makecol(200,0,0)); //Transformation du mur du haut
    floodfill(level[niv], 1000, 599, makecol(200,0,0)); //Transformation du mur du bas
}

void murNormal(int niv, int *compteurMurLave) // remet les murs en normal
{
    *compteurMurLave=0;
    floodfill(level[niv], 1000, 1, makecol(0,0,0)); //Transformation du mur du haut
    floodfill(level[niv], 1000, 599, makecol(0,0,0)); //Transformation du mur du bas
}

void appliquerCollision(t_perso* perso,float *v_scroll,int test, int i,int taille, int niv,t_listePerso *lp, int *compteurMurLave, int soundOn)
{
    perso->dx=0;
    perso->x = perso->precx;
    perso->y = perso->precy;

    perso->x += perso->dx;     //on applique le deplacement
    if ( contactPerso(perso,taille,niv,lp, compteurMurLave, soundOn) ) //on verifie si il y a toujours contact
        perso->x = perso->precx; //sinon on revient en arrière

    perso->y += perso->dy;       //on applique le deplacement
    if ( contactPerso(perso,taille,niv,lp, compteurMurLave, soundOn) ) //on verifie si il y a toujours contact
        perso->y = perso->precy; //sinon on revient en arrière

    if (perso->dy>=0)
    {
        perso->dy=perso->dy-1/2*(*v_scroll);
        if (perso->y==perso->precy)
            perso->dy=perso->dy-(*v_scroll);

    }

    if (perso->dy<-1)
    {
        perso->dy=perso->dy+1;
        if (perso->y==perso->precy)
            perso->dy=perso->dy+0.2;
    }

    ///Collision à l'aide des angles de collision

    if (((perso->angle_collision)<=40) && ((perso->angle_collision)>= -40))
    {
        perso->x=perso->x -(*v_scroll)-1;
        if (test!=1)
        {
            perso->y=perso->y+2.3;
        }
        else
        {
            perso->y=perso->y-2.7;
        }
    }

    if (((perso->angle_collision)<90) && ((perso->angle_collision)>= 20))
    {
        perso->x=perso->x -(*v_scroll)-4;
        if (test!=1)
        {
            perso->dy=perso->dy+1.6;
        }
        else
        {
            perso->dy=perso->dy-2;
        }
    }

    if (((perso->angle_collision)<-20) && ((perso->angle_collision)>= -90))
    {
        perso->x=perso->x -(*v_scroll)-4;
        perso->y=perso->y-0.1;
    }

    if (((perso->angle_collision)<-110) && ((perso->angle_collision)>= -180))
    {
        perso->x=perso->x -2;
    }

    if (((perso->angle_collision)<180) && ((perso->angle_collision)>= 110))
    {
        perso->x=perso->x -2;
    }

    ///Si il n'y a plus de scroll

    if (perso->victoire==1)
    {
        perso->dx=5;
        perso->x=perso->x+perso->dx;
    }
}

int actualiserListePerso(t_listePerso *lp,int* compt,float *v_scroll,t_bonus tab [NNIVEAU][MAXBONUS],float* mapx,int niv, int*bonusPris, int soundOn, int *lancerCompteurMurLave, int *compteurMurLave)
{
    int i;
    int test=0;
    int aile=0;
    int perdu=0;
    int typeContact;

    perdu=testperdu(lp);
    if (perdu==1) //  si le joueur a perdu
    {
        printf("perdu\n");
        return 0;
    }

    if (key[KEY_SPACE])
    {
        test=1;
        *compt=*compt+5;
    }

    if (*compt>10)
    {
        aile=1; // au prochain appel de actualiserPerso, l'oiseau changera d'image
        *compt=0;
    }

    for (i=0; i<MAXI; i++)
    {
        if (lp->perso[i]!=NULL)
        {
            typeContact = actualiserPerso(lp->perso[i],test,aile,v_scroll,tab,mapx,niv,lp->tailleperso, bonusPris, soundOn,lp, compteurMurLave);// on fait le déplacement
            appliquerBonus(typeContact, lp, lp->perso[i],v_scroll, lancerCompteurMurLave, niv);
            *bonusPris = 0;
        }
    }
    return -1;
}

void appliquerBonus(int typeContact, t_listePerso *lp, t_perso*perso,float *v_scroll, int *lancerCompteurMurLave, int niv)
{
    switch(typeContact)
    {
    case 0 :
        //Vitesse +
        (*v_scroll)=(*v_scroll)+1;
        break;
    case 1 :
        //Vitesse -
        if (*v_scroll > 1) // on blinde la vitesse minimale du scroll
            (*v_scroll)=(*v_scroll)-1;
        break;
    case 2 :
        //Taille +
        if(lp->tailleperso <2) lp->tailleperso++;
        break;
    case 3 :
        //Taille -
        if(lp->tailleperso >0) lp->tailleperso--;
        break;
    case 4 :
        //Nombre +
        ajouterPerso(lp,perso->x,perso->y+100); // on affiche un clone en dessous de celui qui a pris le bonus
        break;
    case 5 :
        //Mur lave
        murLave(niv);
        *lancerCompteurMurLave=1;
        break;
    }
}

t_listePerso* creer_liste_perso() // on créée une liste de perso rassemblant tous les persos
{
    int i;
    t_listePerso* nouv;

    nouv=(t_listePerso *)malloc(1*sizeof(t_listePerso));

    nouv=(t_listePerso *)malloc(1*sizeof(t_listePerso));

    nouv->nbperso=0;
    nouv->tailleperso=1;
    nouv->perso=(t_perso **)malloc(MAXI*sizeof(t_perso*));

    for (i=0; i<MAXI; i++)
        nouv->perso[i]=NULL;

    return nouv;
}

t_perso * creerPerso(int x,int y,int couleurblindee)
{
    t_perso *nouv;
    int couleur,a,b;
    int i;

    nouv=(t_perso *)malloc(1*sizeof(t_perso));

    float r = (float)rand() / (float)(RAND_MAX/200);

    nouv->x=x;
    nouv->y=y;
    nouv->dx=0;
    nouv->dy=0;
    nouv->imgcourante=0;
    nouv->angle=-10;
    nouv->vivant=1;
    nouv->victoire=0;
    nouv->max_x=SCREEN_W/3+r;   //Position max d'un oiseau
    nouv->color=couleurblindee;

    for(i=0; i<3; i++)
    {
        nouv->hitbox[i]= create_bitmap(oiseau[i][1]->w,oiseau[i][1]->h);
        nouv->tx[i]=oiseau[i][1]->w;
        nouv->ty[i]=oiseau[i][1]->h;
    }

    nouv->angle=0;
    nouv->timer_col=0;
    nouv->aile=0;
    nouv->taille=1;

    // création d'une bitmap hitbox à partir de l'image de l'ennemi
    for(i=0; i<3; i++)
    {
        for (a=0; a<oiseau[i][1]->w; a++)
        {
            for (b=0; b<oiseau[i][1]->h; b++)
            {
                couleur=getpixel(oiseau[i][1], a, b);
                if (couleur==makecol(255,0,255))
                    putpixel(nouv->hitbox[i], a, b, makecol(255,0,255));
                else
                    putpixel(nouv->hitbox[i], a, b, nouv->color);
            }
        }
    }

    return nouv;
}

t_perso * ajouterPerso(t_listePerso *lp,int x,int y)
{
    int i;
    t_perso *perso;
    int couleur;

    // Liste pleine, on alloue rien et on retourne NULL...
    if (lp->nbperso >= MAXI)
        return NULL;

    // Allouer un acteur initialisé
    couleur=obtention_couleurblindee(lp);

    perso=creerPerso(x,y,couleur);

    // Chercher un emplacement libre
    i=0;
    while (lp->perso[i]!=NULL && i<MAXI)
        i++;

    // Si on a trouvé ...
    // (normalement oui car on a vérifié n<max)
    if (i<MAXI)
    {
        // Accrocher le acteur à l'emplacement trouvé
        lp->perso[i]=perso;
        lp->nbperso++;
    }
    // Sinon c'est qu'il y a un problème de cohérence
    else
        allegro_message("Anomalie gestion ajouterActeur : liste corrompue");

    return perso;
}

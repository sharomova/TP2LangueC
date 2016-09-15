/*
*
* Programmeur : Olga Sharomova
* Date : 20. 11. 2014
* Fichier :      SqueletTp2.cpp
*
* Gestionnaire de répertoires - Squelette du TP 2
* Ce programme doit offrir à ses utilisateurs un jeu de commandes restreint qui
* permet de simuler des commandes de base standard.
*
*/
#include <iostream>
#include <fstream>
#include <locale>
#include <stdio.h>
#include <string.h>
using namespace std;
#include "tp2.h"

// constantes
const char FICHIER_DONNES[] = "Donnees.txt";    // Le nom du fichier
const char LET_F[] = "/f";                     // Le lettre pour ecrir dans le fichier
const int LONGUEUR = 100;                     // Le nombre de lettres maximale
const int MAX_F = 3;                         //Le nombre de lettre pour lire /f 
const int INEXIST = 90;                     //Si commande n'existe pas

struct Donnes
{
	int valeur;             //la commande
	char nom[LONGUEUR];     //pour lire le nom du repertoire ou fichier
	char texte[LONGUEUR];   //pour lire le texte 
	char reste[MAX_F];   //pour lire /f a la fin de ligne si il existe
	bool option;
};

// prototypes des fonctions
void ouvrirFichier(ifstream &lecture);
void lireFichier(ifstream &lecture, Donnes &lesDonnes, repertoire *&repertoireCourant, fichier *&listeFichier);
void creerRepertoire(repertoire &racine, repertoire *&repertoireCourant, char repertoire[LONGUEUR]);
void detruireRepertoire(repertoire *&repertoireCourant, char repertoire[LONGUEUR], bool ls);
void allerAuRepertoire(repertoire *&repertoireCourant, char repertoire[LONGUEUR], bool ls);
void listerRepertoire(repertoire *repertoireCourant, bool ls);
void listerRepertoireFichier(repertoire *repertoireCourant, fichier *listeFichier);
void creerFichier(repertoire *&repertoireCourant, fichier *&listeFichier, char fichier[LONGUEUR]);
void creerFichierAvecTexte(repertoire *&repertoireCourant, fichier *&listeFichier, Donnes &lesDonnes);
void detruireFichier(repertoire *&repertoireCourant, fichier *&listeFichier, char fichier[LONGUEUR]);
void listerFichier(repertoire *&repertoireCourant, fichier *listeFichier, char fichier[LONGUEUR]);
void detruireArbre(repertoire *&repertoireCourant, fichier *&listeFichier);

void main() {
	setlocale(LC_ALL, "");     // pour afficher les caractères accentués
	repertoire racine = { "racine", NULL, NULL, NULL, NULL }; //répetoire racine
	repertoire *repertoireCourant = &racine;  //le répertoire courant
	fichier *listeFichier = NULL; //les fichiers
	Donnes lesDonnes;    //pour lire les donnes sur le fichier
	ifstream lecture;   //le fichier
	bool ls;
	bool areter = false;

	ouvrirFichier(lecture);
	lireFichier(lecture, lesDonnes, repertoireCourant, listeFichier);

	//while (!lecture.eof())
	while (lesDonnes.valeur != 100)
	{
		switch (lesDonnes.valeur) {
		case LSDIR:
			ls = false;
			listerRepertoire(repertoireCourant, ls);
			break;
		case MKDIR:
			creerRepertoire(racine, repertoireCourant, lesDonnes.nom);
			break;
		case RMDIR:
			ls = false;
			//detruireRepertoire(repertoireCourant, lesDonnes.nom, ls);
			break;
		case CD:
			ls = false;
			allerAuRepertoire(repertoireCourant, lesDonnes.nom, ls);
			break;
		case MKFILE:
			if (lesDonnes.option == true){
				creerFichierAvecTexte(repertoireCourant, listeFichier, lesDonnes);
				lesDonnes.option = false;
			}
			else
			{
				creerFichier(repertoireCourant, listeFichier, lesDonnes.nom);
			}
			break;
		case RMFILE:
			detruireFichier(repertoireCourant, listeFichier, lesDonnes.nom);
			break;
		case LSFILE:
			listerFichier(repertoireCourant, listeFichier, lesDonnes.nom);
			break;
		case DELTREE:
			detruireArbre(repertoireCourant, listeFichier);
			break;
			;
		}
		
		lireFichier(lecture, lesDonnes, repertoireCourant, listeFichier);
	}
	lecture.close(); //fermer le fichier

	system("pause");

}
/*
*
* Cette fonction ouvre le fichier
*
* Param entrée : le fichier
* Param sortie : aucun
*
*/
void ouvrirFichier(ifstream &lecture){
	lecture.open(FICHIER_DONNES, ios::in);
	if (lecture.fail()) {
		cout << "Erreur d'ouverture du fichier input" << endl;
		exit(1);
	}
}
/*
*
* Cette fonction lit les donnes sur le fichier txt
*
* Param entrée : le fichier, structe pour lire les donnes
* Param sortie : les donnes (commande, nom du fichier ou repertoire)
*
*/
void lireFichier(ifstream &lecture, Donnes &lesDonnes, repertoire *&repertoireCourant, fichier *&listeFichier){
	char commande[LONGUEUR];	//la valeur pour lire la debut de la ligne 
	bool verifier = false;     //pour verifier si la commande existe
	char * resteLinge;         //la valeur pour savoir la reste de la ligne
	char *ff;

	if (lecture.eof()){
		lesDonnes.valeur = 100;
	}
	else
	{
		lecture.getline(commande, LONGUEUR);

		char *pDiese = strchr(commande, ' ');
		if (pDiese != NULL)
		{
			resteLinge = pDiese + 1;
			*pDiese = '\0';
			strcpy_s(lesDonnes.nom, LONGUEUR, resteLinge);
		}

		if (strcmp(commande, COMMANDES[4]) == 0)
		{
			char *f = strchr(resteLinge, ' ');
			if (f != NULL){
				ff = f + 1;
				*f = '\0';
				strcpy_s(lesDonnes.nom, LONGUEUR, resteLinge);
				lesDonnes.option = true;
				while (*lesDonnes.texte != FIN_DE_TEXTE){
					lecture.getline(lesDonnes.texte, LONGUEUR);
				}
			}
			lesDonnes.valeur = MKFILE;

		}
		if (strcmp(commande, COMMANDES[0]) == 0)
		{
			if (strcmp(lesDonnes.nom, LET_F) == 0){
				lesDonnes.valeur = LSFILE;
			}
			else
			{
				lesDonnes.valeur = LSDIR;
			}
		}
		else
		{
			for (int i = 0; i < NB_COMMANDES; i++){
				if (strcmp(commande, COMMANDES[i]) == 0)
				{
					lesDonnes.valeur = i;
					verifier = true;
				}
			}
			if (verifier == false){
				cout << "racine>" << commande << endl;
				cout << "Cette commande n'est pas recommue" << endl;
				lesDonnes.valeur = INEXIST;
			}
		}
	}
}

/*
*
* Cette fonction crée un repertoire dans le répertoire courant
*
* Param entrée : le répertoire courant, le nom du répertoire
* Param sortie : le répertoire courant
*
*/
void creerRepertoire(repertoire &racine, repertoire *&repertoireCourant, char dosier[LONGUEUR]){
	if (strcmp(repertoireCourant->nom, "racine") == 0){
		cout << "racine>mkdir " << dosier << endl;
	}
	else
	{
		cout << "racine\\" << repertoireCourant->nom << ">mkdir " << dosier << endl;
	}

	repertoire *p = new repertoire;
	p->nom = new char[LONGUEUR];
	strcpy_s(p->nom, LONGUEUR, dosier);
	p->parent = repertoireCourant;
	p->fichiers = NULL;
	p->suivant = NULL;

	if (repertoireCourant->repertoires == NULL){
		
		
		repertoireCourant->repertoires = p;
		repertoireCourant->suivant = NULL;
	}
	else
	{
		/*if (strcmp(repertoireCourant->repertoires->nom, dosier) >= 0){
			repertoire *tmp = NULL;
			repertoire *change = repertoireCourant->repertoires;
			tmp = change;	
			repertoireCourant->repertoires = p;
			p = change;
		}*/
		
		repertoire *p_tmp = NULL;
		repertoire *p_tete = repertoireCourant->repertoires->suivant;
		
			while (p_tete != NULL && strcmp(p_tete->nom, p->nom) <= 0)
			{
				p_tmp = p_tete;
				p_tete = p_tete->suivant;

			}
			p->suivant = p_tete;

			if (p_tmp != NULL)
			{
				p_tmp->suivant = p;

			}
			else
			{
				repertoireCourant->repertoires->suivant = p;
				repertoireCourant->repertoires->repertoires = NULL;
				repertoireCourant->repertoires->suivant->repertoires = NULL;
			}
		}

	cout << "racine>mkdir " << dosier << endl;
}
/*
*
* Cette fonction détruit un repertoire dans le répertoire courant
*
* Param entrée : le répertoire courant, le nom du répertoire
* Param sortie : le répertoire courant
*
*/
void detruireRepertoire(repertoire *&repertoireCourant, char dosier[LONGUEUR], bool ls){
	if (ls != true){
		repertoire *p = repertoireCourant;
		cout << p->nom << ">rmdir " << dosier << endl;
		ls = true;
		detruireRepertoire(repertoireCourant->repertoires, dosier, ls);
	}
	if (repertoireCourant != NULL && strcmp(repertoireCourant->nom, "racine") != 0){
		repertoire *d = repertoireCourant;
		if (d != NULL)
		{
			if (strcmp(d->nom, dosier) == 0){
				delete d;			
			}
			else
			{
				detruireRepertoire(repertoireCourant->suivant, dosier, ls);
			}
		}
	}
}
/*
*
* Cette fonction permet aller au répertoire spécifié
*
* Param entrée : le répertoire courant, le nom du répertoire
* Param sortie : aucun
*
*/
void allerAuRepertoire(repertoire *&repertoireCourant, char dosier[LONGUEUR], bool ls){
	//if (strcmp(dosier, "..") == 0){
		if (ls != true){
			repertoire *p = repertoireCourant;
			cout << p->nom << ">cd " << dosier << endl;
			ls = true;
			allerAuRepertoire(repertoireCourant->repertoires, dosier, ls);
		}
		if (repertoireCourant != NULL && strcmp(repertoireCourant->nom, "racine") != 0){
			repertoire *d = repertoireCourant;
			if (d != NULL)
			{
				if (strcmp(d->nom, dosier) == 0){
					//repertoireCourant = d;
				}
				allerAuRepertoire(repertoireCourant->suivant, dosier, ls);
			}
		}
	
}
/*
*
* Cette fonction liste tous les sous-repertoire à partir du niveau courant
* sans les fichiers
*
* Param entrée : le répertoire courant
* Param sortie : aucun
*
*/
void listerRepertoire(repertoire *repertoireCourant, bool ls){

	if (ls != true){
		repertoire *p = repertoireCourant;
		cout <<p->nom<< ">lsdir " << endl;
		ls = true;
		listerRepertoire(repertoireCourant->repertoires, ls);
	}
	if (repertoireCourant != NULL && strcmp(repertoireCourant->nom, "racine") != 0){
		repertoire *d = repertoireCourant;
		if (d != NULL)
		{
			cout << d->nom << endl;
			listerRepertoire(repertoireCourant->suivant, ls);
		}
	}
	/*repertoire *d = repertoireCourant;
	if (ls != true){
		
		cout <<d->nom<< ">lsdir " << endl;
		
		ls = true;
		
	}
	if (d != NULL)
	{
		listerRepertoire(repertoireCourant->repertoires, ls);

		cout << d->nom << endl;
			listerRepertoire(repertoireCourant->suivant, ls);		
	}*/
	
}
/*
*
* Cette fonction liste tous les sous-repertoire à partir du niveau courant
* avec les fichiers
*
* Param entrée : le répertoire courant, les fichier
* Param sortie : aucun
*
*/
void listerRepertoireFichier(repertoire *repertoireCourant, fichier *listeFichier){

}
/*
*
* Cette fonction crée un fichier dans le répertoire courant
*
* Param entrée : le répertoire courant, les fichiers, le nom du fichier
* Param sortie : le répertoire courant, les fichiers
*
*/
void creerFichier(repertoire *&repertoireCourant, fichier *&listeFichier, char fichiers[LONGUEUR]){
	fichier *p = new fichier;
	p->nom = new char[LONGUEUR];
	strcpy_s(p->nom, LONGUEUR, fichiers);
	p->parent = repertoireCourant;
	p->contenu = NULL;
	p->suivant = NULL;

	if (listeFichier == NULL){


		listeFichier = p;
		listeFichier->suivant = NULL;
	}
	else
	{
		/*if (strcmp(repertoireCourant->repertoires->nom, dosier) >= 0){
		repertoire *tmp = NULL;
		repertoire *change = repertoireCourant->repertoires;
		tmp = change;
		repertoireCourant->repertoires = p;
		p = change;
		}*/

		fichier *p_tmp = NULL;
		fichier *p_tete = listeFichier->suivant;

		while (p_tete != NULL && strcmp(p_tete->nom, p->nom) <= 0)
		{
			p_tmp = p_tete;
			p_tete = p_tete->suivant;

		}
		p->suivant = p_tete;

		if (p_tmp != NULL)
		{
			p_tmp->suivant = p;

		}
		else
		{
			listeFichier->suivant = p;
			listeFichier->suivant->suivant = NULL;
			
		}
	}

	cout << "racine>mkfile " << fichiers << endl;
}
/*
*
* Cette fonction crée un fichier dans le répertoire courant avec le texte
*
* Param entrée : le répertoire courant, les fichiers, le nom du fichier, le texte
* Param sortie : le répertoire courant, les fichiers
*
*/
void creerFichierAvecTexte(repertoire *&repertoireCourant, fichier *&listeFichier, Donnes &lesDonnes){
	for (int i = 0; i < MAX_F; ++i)
	{
		//lesDonnes.reste[i] = NULL;
	}
}
/*
*
* Cette fonction detruit un fichier dans le répertoire courant
*
* Param entrée : le répertoire courant, les fichiers, le nom du fichier
* Param sortie : les fichiers
*
*/
void detruireFichier(repertoire *&repertoireCourant, fichier *&listeFichier, char fichier[LONGUEUR]){

}
/*
*
* Cette fonction affiche le contenue d'un fichier
*
* Param entrée : le répertoire courant, les fichiers, le nom du fichier
* Param sortie : aucun
*
*/
void listerFichier(repertoire *&repertoireCourant, fichier *listeFichier, char fichier[LONGUEUR]){

}
/*
*
* Cette fonction détruit l'arborescence à partir du répertoire courant
*
* Param entrée : le répertoire courant, les fichiers
* Param sortie : le répertoire courant, les fichiers
*
*/
void detruireArbre(repertoire *&repertoireCourant, fichier *&listeFichier){

}
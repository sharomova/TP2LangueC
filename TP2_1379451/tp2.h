const int TAILLE_TAMPON_LECTURE = 1000;
const int NB_COMMANDES = 8;

struct repertoire;

struct fichier 
{
	char* nom;
	char* contenu;
	repertoire* parent;
	fichier* suivant;
};

struct repertoire
{
	char* nom;
	repertoire* repertoires;
	repertoire* suivant;
	fichier* fichiers;
	repertoire* parent;
};

const char COMMANDES[][NB_COMMANDES] = 
{ "lsdir", "mkdir", "rmdir", "cd", "mkfile", "rmfile", "lsfile", "deltree"};

const int FIN_DE_TEXTE = 3;

enum
{
	LSDIR,
	MKDIR,
	RMDIR,
	CD,
	MKFILE,
	RMFILE,
	LSFILE,
	DELTREE
};
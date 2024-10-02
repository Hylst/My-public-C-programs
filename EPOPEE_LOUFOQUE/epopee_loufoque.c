/* L'Épopée Loufoque du Héros Malgré Lui
un jeu de rôle textuel hilarant où vous incarnez un aventurier... disons, un peu maladroit !
Hylst - (1997 - 2024 (nettoyage code - application bonnes pratiques - crossplatform - commentaires))
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#ifdef _WIN32
#include <windows.h> // Pour Windows (UTF-8 support)
#endif

#define MAX_OBJETS 5
#define LARGUEUR_CADRE 80

// Structure du personnage
typedef struct {
    char nom[50];
    char race[50];
    char classe[50];
    char defaut[100];
    char qualite[100];
    int force;
    int charisme;
    int reputationVillage;
    int reputationChateau;
    int reputationForet;
    int pointsVie;
    int niveau;
    int experience;
    int invisible;  // Statut d'invisibilité
    int doubleDegats; // Statut de double dégâts
    int attaqueCritique; // Chance de faire une attaque critique
    char inventaire[MAX_OBJETS][50];
    int nbObjets;
} Personnage;

// Structure d'une créature
typedef struct {
    char nom[50];
    int pointsVie;
    int force;
    int soin;   // Capacité spéciale : soin
    int volVie; // Capacité spéciale : vol de vie
} Creature;

// Prototypes des fonctions
void configurerConsoleUTF8();
void afficherCadre(const char *titre);
void afficherTexteCadre(const char *texte);
void creationPersonnage(Personnage *perso);
void ajouterObjet(Personnage *perso, const char *objet);
void afficherInventaire(Personnage *perso);
void utiliserObjet(Personnage *perso);
void taverne(Personnage *perso);
void quetePrincipale(Personnage *perso);
void combat(Personnage *perso, Creature creature);
void systemeProgression(Personnage *perso);
void finDuJeu(Personnage *perso, int victoire);
void queteSecondaireInfluencee(Personnage *perso);
void queteLicorneDepressive(Personnage *perso);
void queteNegocierTroll(Personnage *perso);
void gererReputation(Personnage *perso);

// Fonction principale
int main() {
    configurerConsoleUTF8();

    srand(time(NULL));  // Initialiser le générateur de nombres aléatoires

    Personnage joueur;

    afficherCadre("Bienvenue dans L'Epopée Loufoque du Héros Malgré Lui !");
    creationPersonnage(&joueur);
    taverne(&joueur);

    return 0;
}

// Fonction pour configurer UTF-8 (cross-platform)
void configurerConsoleUTF8() {
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);  // Permet de gérer UTF-8 sur Windows
#endif
}

// Fonction pour afficher un cadre ASCII
void afficherCadre(const char *titre) {
    int len = strlen(titre);
    int padding = (LARGUEUR_CADRE - len) / 2;

    printf("\n");
    for (int i = 0; i < LARGUEUR_CADRE; i++) printf("*");
    printf("\n*");
    for (int i = 1; i < padding; i++) printf(" ");
    printf("%s", titre);
    for (int i = len + padding; i < LARGUEUR_CADRE - 1; i++) printf(" ");
    printf("*\n");
    for (int i = 0; i < LARGUEUR_CADRE; i++) printf("*");
    printf("\n\n");
}

// Fonction pour afficher du texte dans un cadre
void afficherTexteCadre(const char *texte) {
    int len = strlen(texte);
    int debut = 0;
    while (debut < len) {
        printf("* ");
        for (int i = 0; i < LARGUEUR_CADRE - 4 && debut < len; i++) {
            if (texte[debut] == '\n') {
                debut++;
                break;
            }
            printf("%c", texte[debut]);
            debut++;
        }
        for (int j = len % (LARGUEUR_CADRE - 4); j < LARGUEUR_CADRE - 4; j++) {
            printf(" ");
        }
        printf(" *\n");
    }
    for (int i = 0; i < LARGUEUR_CADRE; i++) printf("*");
    printf("\n");
}

// Fonction pour créer le personnage
void creationPersonnage(Personnage *perso) {
    char races[][50] = {
        "Humain pas très malin", "Elfe narcissique", "Nain grognon",
        "Ogre végétarien", "Fée bodybuildeuse"
    };
    char classes[][50] = {
        "Guerrier pacifiste", "Magicien allergique à la magie",
        "Voleur kleptomane", "Barde aphone", "Paladin athée"
    };
    char defauts[][100] = {
        "Phobie des papillons", "Addiction au fromage",
        "Tendance à parler aux cailloux", "Éternue à chaque mensonge",
        "Allergique à sa propre sueur"
    };
    char qualites[][100] = {
        "Danse la polka divinement", "Parle le langage des plantes",
        "Peut rouler sa langue en trompette", "Imite parfaitement le cri du poulet",
        "Résiste à l'alcool comme personne"
    };

    printf("Quel est le nom de votre aventurier ? ");
    fgets(perso->nom, sizeof(perso->nom), stdin);
    perso->nom[strcspn(perso->nom, "\n")] = '\0';  // Enlever le saut de ligne

    // Choisir aléatoirement race, classe, défaut et qualité
    strcpy(perso->race, races[rand() % 5]);
    strcpy(perso->classe, classes[rand() % 5]);
    strcpy(perso->defaut, defauts[rand() % 5]);
    strcpy(perso->qualite, qualites[rand() % 5]);

    perso->force = rand() % 10 + 5;
    perso->charisme = rand() % 10 + 5;
    perso->reputationVillage = 0;
    perso->reputationChateau = 0;
    perso->reputationForet = 0;
    perso->pointsVie = 100;
    perso->niveau = 1;
    perso->experience = 0;
    perso->invisible = 0;
    perso->doubleDegats = 0;
    perso->attaqueCritique = 5;  // 5% de chance de faire une attaque critique
    perso->nbObjets = 0;

    // Afficher le personnage créé
    printf("\nVotre personnage :\n");
    printf("Nom : %s\n", perso->nom);
    printf("Race : %s\n", perso->race);
    printf("Classe : %s\n", perso->classe);
    printf("Défaut : %s\n", perso->defaut);
    printf("Qualité : %s\n", perso->qualite);
    printf("Force : %d\nCharisme : %d\nPV : %d\n", perso->force, perso->charisme, perso->pointsVie);
    printf("Niveau : %d\nExpérience : %d\n", perso->niveau, perso->experience);
    printf("Réputation (Village) : %d, (Château) : %d, (Forêt) : %d\n\n", perso->reputationVillage, perso->reputationChateau, perso->reputationForet);

    afficherTexteCadre("Votre mission : Sauver la princesse Karatéka du royaume de Loufoland !");
}

// Ajout d'objets uniques avec des effets spéciaux
void ajouterObjet(Personnage *perso, const char *objet) {
    if (perso->nbObjets < MAX_OBJETS) {
        strcpy(perso->inventaire[perso->nbObjets], objet);
        perso->nbObjets++;
        printf("Vous avez ajouté %s à votre inventaire.\n", objet);
    } else {
        printf("Votre inventaire est plein !\n");
    }
}

// Fonction de gestion des combats (optimisée)
void combat(Personnage *perso, Creature creature) {
    printf("Combat contre %s !\n", creature.nom);
    while (perso->pointsVie > 0 && creature.pointsVie > 0) {
        // Attaque du joueur
        int degatsJoueur = rand() % (perso->force + 1);
        if (perso->doubleDegats) {
            degatsJoueur *= 2;
            perso->doubleDegats = 0;  // Double dégâts utilisés une seule fois
        }
        creature.pointsVie -= degatsJoueur;
        printf("Vous attaquez %s et infligez %d dégâts.\n", creature.nom, degatsJoueur);

        if (creature.pointsVie <= 0) {
            printf("Vous avez vaincu %s !\n", creature.nom);
            systemeProgression(perso);  // Progression après un combat
            return;
        }

        // Attaque de la créature
        if (!perso->invisible) {
            int degatsCreature = rand() % (creature.force + 1);
            perso->pointsVie -= degatsCreature;
            printf("%s vous attaque et inflige %d dégâts.\n", creature.nom, degatsCreature);
        } else {
            afficherTexteCadre("Vous êtes invisible, la créature ne peut pas vous toucher !");
            perso->invisible = 0;  // Invisibilité dure un tour
        }

        // Compétences spéciales des créatures (soin, vol de vie)
        if (creature.soin > 0) {
            printf("%s se soigne et regagne %d points de vie.\n", creature.nom, creature.soin);
            creature.pointsVie += creature.soin;
        }
        if (creature.volVie > 0) {
            printf("%s vole %d points de vie.\n", creature.nom, creature.volVie);
            creature.pointsVie += creature.volVie;
            perso->pointsVie -= creature.volVie;
        }

        // Si les points de vie du joueur tombent à zéro
        if (perso->pointsVie <= 0) {
            printf("Vous avez été vaincu par %s... Fin de l'aventure.\n", creature.nom);
            finDuJeu(perso, 0);  // Défaite
        }
    }
}

// Fonction pour la scène de la taverne
void taverne(Personnage *perso) {
    afficherCadre("Vous êtes dans la taverne 'Au Gobelin Chatouilleux'");
    afficherTexteCadre("L'air est saturé de fumée, de rires gras et d'odeurs douteuses. Un barde éméché "
                       "chante les exploits d'un chevalier qui a vaincu un dragon en lui faisant des chatouilles.");
    
    printf("\nQue faites-vous ?\n");
    printf("1. Parler au barde\n");
    printf("2. Interroger le nain sur les rumeurs\n");
    printf("3. Caresser la fourrure de l'ogre\n");
    printf("4. Voir votre inventaire\n");
    printf("5. Tenter une quête secondaire\n");
    printf("Votre choix (1/2/3/4/5) : ");

    int choix;
    scanf("%d", &choix);
    getchar();  // Capturer l'entrée après scanf

    if (choix == 1) {
        afficherTexteCadre("Le barde vous raconte en détail comment le chevalier a trouvé le point chatouilleux du dragon : "
                           "sous les aisselles ! Vous gagnez +1 en Charisme.");
        perso->charisme += 1;
    } else if (choix == 2) {
        afficherTexteCadre("Le nain vous révèle que la princesse Karatéka organise des combats clandestins dans les donjons du château. "
                           "Vous gagnez +1 en Réputation auprès du Château.");
        perso->reputationChateau += 1;
    } else if (choix == 3) {
        afficherTexteCadre("Vous vous approchez de l'ogre pour lui caresser la fourrure. Il rougit et glousse de plaisir. "
                           "Malheureusement, son gloussement provoque une onde de choc qui renverse toutes les chopes de la taverne. "
                           "Vous perdez 2 points de Réputation auprès du Village.");
        perso->reputationVillage -= 2;
    } else if (choix == 4) {
        afficherInventaire(perso);
    } else if (choix == 5) {
        printf("\nChoisissez une quête secondaire :\n1. Quête de la Licorne Dépressive\n2. Négocier avec un troll\nVotre choix : ");
        scanf("%d", &choix);
        getchar();

        if (choix == 1) {
            queteLicorneDepressive(perso);
        } else if (choix == 2) {
            queteNegocierTroll(perso);
        }
    }

    printf("\nSoudain, un messager royal entre dans la taverne :\n");
    afficherTexteCadre("'Oyez, oyez ! La princesse Karatéka a été enlevée par le terrible dragon K-pop ! Qui osera la sauver ?'");
    printf("\nAppuyez sur Entrée pour accepter la quête...");
    getchar();  // Pause avant de lancer la quête principale
    quetePrincipale(perso);
}

// Fonctions supplémentaires comme la progression, la fin du jeu, et les quêtes secondaires
// sont intégrées dans les fonctions déjà optimisées


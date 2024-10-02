/* L'Épopée Loufoque du Héros Malgré Lui
un jeu de rôle textuel hilarant où vous incarnez un aventurier... disons, un peu maladroit !
Hylst v0.8 - (1997 - 2024 (nettoyage code - application bonnes pratiques - crossplatform - commentaires))
A big part of my sources lost :( Would have to make double work.
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
    int boostForce; // Capacité spéciale : boost de force
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
void quetesSecondaires(Personnage *perso);
void queteSecondaireInfluencee(Personnage *perso);
void queteLicorneDepressive(Personnage *perso);
void queteNegocierTroll(Personnage *perso);
void gererReputation(Personnage *perso);
void queteCristalEnchante(Personnage *perso);
void queteVillageDetresse(Personnage *perso);
Creature creerCreatureAleatoire(int niveau);
void systemeProgression(Personnage *perso);
void combatBoss(Personnage *perso, Creature *boss);
void piegeBoss(Creature *boss);
Creature creerBoss();

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

// Fonction pour créer un boss avec des compétences spéciales
Creature creerBoss() {
    Creature dragonKpop;
    strcpy(dragonKpop.nom, "Dragon K-pop");
    dragonKpop.pointsVie = 100;
    dragonKpop.force = 20;
    dragonKpop.soin = 10;         // Le dragon peut se soigner
    dragonKpop.volVie = 5;        // Le dragon vole des PV pendant ses attaques
    dragonKpop.boostForce = 5;  
        return dragonKpop;
}
 
// Fonction de combat contre le boss Dragon K-pop avec plusieurs phases
void combatBoss(Personnage *perso, Creature *boss) {
    int phase = 1;  // On commence à la phase 1 du combat

    printf("Le combat contre %s commence !\n", boss->nom);
    while (perso->pointsVie > 0 && boss->pointsVie > 0) {
        printf("\n[Phase %d] Vous affrontez le %s\n", phase, boss->nom);

        // Attaque du joueur
        int degatsJoueur = rand() % (perso->force + 1);
        if (perso->doubleDegats) {
            degatsJoueur *= 2;
            perso->doubleDegats = 0;  // Double dégâts utilisés une seule fois
        }
        boss->pointsVie -= degatsJoueur;
        printf("Vous attaquez %s et infligez %d dégâts.\n", boss->nom, degatsJoueur);

        if (boss->pointsVie <= 0) {
            printf("Vous avez vaincu le %s !\n", boss->nom);
            systemeProgression(perso);  // Progression du joueur après avoir vaincu le boss
            return;
        }

        // Attaque du boss
        if (!perso->invisible) {
            int degatsBoss = rand() % (boss->force + 1);
            perso->pointsVie -= degatsBoss;
            printf("%s vous attaque et inflige %d dégâts.\n", boss->nom, degatsBoss);
        } else {
            afficherTexteCadre("Vous êtes invisible, le boss ne peut pas vous toucher !");
            perso->invisible = 0;  // L'invisibilité dure un tour
        }

        // Compétences spéciales du boss
        if (boss->pointsVie <= 50 && phase == 1) {
            afficherTexteCadre("Le Dragon K-pop entre dans une frénésie musicale ! Il booste sa force.");
            boss->force += boss->boostForce;  // Le boss booste sa force à mi-combat
            phase++;  // Passer à la phase suivante du combat
        }

        if (boss->soin > 0 && boss->pointsVie <= 30) {
            printf("%s se soigne et regagne %d points de vie.\n", boss->nom, boss->soin);
            boss->pointsVie += boss->soin;
            boss->soin = 0;  // Le boss ne peut se soigner qu'une seule fois
        }

        if (boss->volVie > 0) {
            int vol = rand() % (boss->volVie + 1);
            perso->pointsVie -= vol;
            boss->pointsVie += vol;
            printf("%s vole %d points de vie !\n", boss->nom, vol);
        }

        // Vérification de la défaite du joueur
        if (perso->pointsVie <= 0) {
            printf("Vous avez été vaincu par %s... Fin de l'aventure.\n", boss->nom);
            finDuJeu(perso, 0);  // Défaite du joueur
        }
    }
}

// Fonction de gestion des combats (incluant le boost de force des créatures)
void combat(Personnage *perso, Creature creature) {
    printf("Combat contre %s !\n", creature.nom);
    int tour = 1;
    
    while (perso->pointsVie > 0 && creature.pointsVie > 0) {
        printf("\n[Tour %d] Vous affrontez %s\n", tour, creature.nom);

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
            systemeProgression(perso);  // Progression après un combat réussi
            return;
        }

        // Attaque de la créature
        if (!perso->invisible) {
            int degatsCreature = rand() % (creature.force + 1);
            perso->pointsVie -= degatsCreature;
            printf("%s vous attaque et inflige %d dégâts.\n", creature.nom, degatsCreature);
        } else {
            afficherTexteCadre("Vous êtes invisible, la créature ne peut pas vous toucher !");
            perso->invisible = 0;  // L'invisibilité dure un tour
        }

        // Compétence spéciale de boost de force
        if (creature.boostForce > 0 && creature.pointsVie <= (creature.pointsVie / 2)) {
            printf("%s se met en colère et augmente sa force de %d points !\n", creature.nom, creature.boostForce);
            creature.force += creature.boostForce;  // La créature booste sa force
            creature.boostForce = 0;  // Le boost de force ne peut être utilisé qu'une seule fois
        }

        // Compétences spéciales des créatures (soin, vol de vie)
        if (creature.soin > 0 && creature.pointsVie <= (creature.pointsVie / 2)) {
            printf("%s se soigne et regagne %d points de vie.\n", creature.nom, creature.soin);
            creature.pointsVie += creature.soin;
            creature.soin = 0;  // La créature ne peut se soigner qu'une seule fois
        }
        if (creature.volVie > 0) {
            int vol = rand() % (creature.volVie + 1);
            perso->pointsVie -= vol;
            creature.pointsVie += vol;
            printf("%s vole %d points de vie !\n", creature.nom, vol);
        }

        // Vérification de la défaite du joueur
        if (perso->pointsVie <= 0) {
            printf("Vous avez été vaincu par %s... Fin de l'aventure.\n", creature.nom);
            finDuJeu(perso, 0);  // Défaite du joueur
        }

        tour++;
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

// Gestion des quêtes influencées par la réputation
void queteSecondaireInfluencee(Personnage *perso) {
    if (perso->reputationVillage > 5) {
        afficherTexteCadre("Les villageois vous font confiance et vous demandent de l'aide pour renforcer leur village.");
        // Ajout d'une quête exclusive pour les joueurs avec une bonne réputation au village
        printf("Vous commencez une quête secondaire pour aider les villageois à construire une nouvelle forteresse.\n");
    } else if (perso->reputationVillage < 0) {
        afficherTexteCadre("Les villageois ne vous font plus confiance et refusent de vous parler.");
    } else {
        afficherTexteCadre("Les villageois vous ignorent pour le moment. Vous devrez gagner leur confiance.");
    }
}

// Correction et gestion finale de l'utilisation des objets
void utiliserObjet(Personnage *perso) {
    afficherInventaire(perso);
    if (perso->nbObjets == 0) {
        printf("Vous n'avez aucun objet à utiliser.\n");
        return;
    }

    printf("Choisissez un objet à utiliser (1-%d) : ", perso->nbObjets);
    int choix;
    scanf("%d", &choix);
    getchar();  // Capturer l'entrée après scanf

    if (choix > 0 && choix <= perso->nbObjets) {
        if (strcmp(perso->inventaire[choix - 1], "Potion de soin") == 0) {
            printf("Vous buvez la Potion de soin et regagnez 20 points de vie.\n");
            perso->pointsVie += 20;
            if (perso->pointsVie > 100) perso->pointsVie = 100;  // Limiter les PV à 100
        } else if (strcmp(perso->inventaire[choix - 1], "Potion de force") == 0) {
            printf("Vous buvez la Potion de force et gagnez 5 points de force.\n");
            perso->force += 5;
        } else if (strcmp(perso->inventaire[choix - 1], "Potion de charisme") == 0) {
            printf("Vous buvez la Potion de charisme et gagnez 5 points de charisme.\n");
            perso->charisme += 5;
        } else if (strcmp(perso->inventaire[choix - 1], "Potion d'invisibilité") == 0) {
            printf("Vous devenez invisible pour un tour !\n");
            perso->invisible = 1;
        } else if (strcmp(perso->inventaire[choix - 1], "Artefact de double dégâts") == 0) {
            printf("Vous activez l'artefact de double dégâts ! Vos prochaines attaques infligeront le double de dégâts.\n");
            perso->doubleDegats = 1;
        }

        // Retirer l'objet de l'inventaire après utilisation
        for (int i = choix - 1; i < perso->nbObjets - 1; i++) {
            strcpy(perso->inventaire[i], perso->inventaire[i + 1]);
        }
        perso->nbObjets--;
    } else {
        printf("Choix invalide.\n");
    }
}

// Fonction d'affichage de l'inventaire
void afficherInventaire(Personnage *perso) {
    printf("\nVotre inventaire :\n");
    if (perso->nbObjets == 0) {
        printf("Rien !\n");
    } else {
        for (int i = 0; i < perso->nbObjets; i++) {
            printf("- %s\n", perso->inventaire[i]);
        }
    }
    printf("\n");
}

// Fonction de progression du joueur après un combat réussi
void systemeProgression(Personnage *perso) {
    perso->experience += 50;  // Augmentation de l'expérience après chaque combat réussi
    if (perso->experience >= 100) {
        perso->niveau++;
        perso->force += rand() % 3 + 1;
        perso->charisme += rand() % 3 + 1;
        perso->pointsVie += rand() % 10 + 5;
        perso->attaqueCritique += rand() % 2 + 1;  // Augmenter la chance de faire une attaque critique
        perso->experience = 0;  // Réinitialiser l'expérience
        printf("Félicitations ! Vous êtes passé au niveau %d.\n", perso->niveau);
        printf("Nouvelle force : %d, Nouveau charisme : %d, Nouveaux PV : %d, Chance de critique : %d%%\n", 
               perso->force, perso->charisme, perso->pointsVie, perso->attaqueCritique);
    }
}

// Quête secondaire : La licorne dépressive
void queteLicorneDepressive(Personnage *perso) {
    int choix;
    afficherCadre("Quête de la Licorne Dépressive");
    afficherTexteCadre("Vous traversez une clairière et trouvez une licorne assise tristement sur un rocher.\n"
                       "'Pourquoi êtes-vous si triste, noble créature ?' demandez-vous.\n"
                       "'Oh, je suis fatiguée de galoper dans des arcs-en-ciel et d'être admirée,' répond-elle en soupirant.\n");

    printf("Que faites-vous ?\n");
    printf("1. La réconforter en lui racontant des blagues\n");
    printf("2. Lui proposer un combat pour lui remonter le moral\n");
    printf("Votre choix (1/2) : ");
    scanf("%d", &choix);
    getchar();  // Capturer l'entrée

    if (choix == 1) {
        afficherTexteCadre("Vous racontez à la licorne votre meilleure blague :\n"
                           "'Pourquoi les licornes détestent-elles les arcs-en-ciel ? Parce qu'elles n'aiment pas être mises en lumière !'\n"
                           "La licorne éclate de rire et vous remercie : 'Je me sens beaucoup mieux maintenant, merci !'\n"
                           "Vous gagnez +5 en Charisme pour avoir redonné le sourire à une créature magique.");
        perso->charisme += 5;
    } else if (choix == 2) {
        afficherTexteCadre("Vous proposez un petit combat amical à la licorne pour qu'elle puisse libérer son énergie.\n"
                           "'D'accord,' dit-elle en galopant vers vous. Le combat commence !");
        Creature licorne = {"Licorne Dépressive", 35, 12, 0, 0, 0};
        combat(perso, licorne);

        if (perso->pointsVie > 0) {
            afficherTexteCadre("Vous avez vaincu la licorne, et elle semble beaucoup plus joyeuse après ce duel.\n"
                               "'Merci pour ce combat ! Je me sens revigorée !'");
        }
    }
}

// Quête secondaire : Négocier avec un troll
void queteNegocierTroll(Personnage *perso) {
    int choix;
    afficherCadre("Quête : Négocier avec le Troll");
    afficherTexteCadre("En traversant un pont branlant, vous êtes arrêté par un grand troll poilu.\n"
                       "'Tu veux passer ? Ça va te coûter cher !' gronde-t-il en vous fixant de ses petits yeux malicieux.\n");

    printf("Que faites-vous ?\n");
    printf("1. Négocier avec le troll en utilisant votre charisme\n");
    printf("2. Combattre le troll pour passer en force\n");
    printf("Votre choix (1/2) : ");
    scanf("%d", &choix);
    getchar();  // Capturer l'entrée

    if (choix == 1) {
        if (perso->charisme >= 10) {
            afficherTexteCadre("Avec un sourire charmeur, vous parvenez à convaincre le troll de vous laisser passer gratuitement.\n"
                               "'T'es pas si mal, toi. Passe !' dit-il en s'écartant.");
        } else {
            afficherTexteCadre("Vous essayez de convaincre le troll, mais il éclate de rire.\n"
                               "'Trop faible ! Si tu veux passer, va falloir payer !' Vous êtes forcé de trouver une autre solution.");
        }
    } else if (choix == 2) {
        afficherTexteCadre("Vous sortez votre arme et défiez le troll en duel.\n"
                           "'Alors, c'est la guerre !' grogne-t-il en s'approchant.");
        Creature troll = {"Troll du Pont", 45, 15, 0, 0, 0};
        combat(perso, troll);

        if (perso->pointsVie > 0) {
            afficherTexteCadre("Vous avez vaincu le troll, et vous traversez le pont victorieux.");
        }
    }
}

// Mini-jeu de devinettes (Sphinx)
void miniJeuDevinettes(Personnage *perso) {
    int choix;
    afficherCadre("Le Sphinx des Montagnes");
    afficherTexteCadre("Un Sphinx apparaît devant vous, bloquant votre chemin.\n"
                       "'Réponds à mes devinettes, aventurier, ou bien péris !' dit-il en vous fixant avec ses yeux perçants.\n");
    
    printf("Voici ma devinette :\n");
    printf("'Je peux voler sans ailes, je peux pleurer sans yeux. Que suis-je ?'\n");
    printf("1. Le vent\n");
    printf("2. Un nuage\n");
    printf("3. Une ombre\n");
    printf("Votre choix (1/2/3) : ");
    scanf("%d", &choix);
    getchar();  // Capturer l'entrée

    if (choix == 1) {
        afficherTexteCadre("Le Sphinx sourit. 'Bien joué, aventurier. Tu peux continuer ton chemin.'\n"
                           "Vous gagnez +5 en Charisme pour avoir brillamment résolu la devinette.");
        perso->charisme += 5;
    } else {
        afficherTexteCadre("Le Sphinx éclate de rire. 'Mauvaise réponse ! Prépare-toi à affronter mon courroux.'\n"
                           "Vous êtes forcé de combattre le Sphinx.");
        Creature sphinx = {"Sphinx des Montagnes", 60, 15, 0, 0, 0};
        combat(perso, sphinx);
    }
}

// Mini-jeu de hasard (dés avec le nain)
void miniJeuHasard(Personnage *perso) {
    int choix, desJoueur, desNain;
    afficherCadre("Partie de dés avec le Nain");
    afficherTexteCadre("Dans la taverne, un nain farceur vous propose une partie de dés.\n"
                       "'Si tu gagnes, je te donne un objet rare. Si tu perds, je garde ton argent !' dit-il en ricanant.\n");

    printf("Voulez-vous jouer aux dés ? (1. Oui / 2. Non) : ");
    scanf("%d", &choix);
    getchar();  // Capturer l'entrée

    if (choix == 1) {
        // Lancer des dés pour le joueur et le nain
        desJoueur = rand() % 6 + 1;
        desNain = rand() % 6 + 1;
        printf("Vous lancez un dé et obtenez : %d\n", desJoueur);
        printf("Le nain lance un dé et obtient : %d\n", desNain);

        if (desJoueur > desNain) {
            afficherTexteCadre("Félicitations ! Vous avez gagné la partie. Le nain, bien que grognon, vous tend un artefact mystérieux.");
            ajouterObjet(perso, "Artefact mystique");
        } else if (desJoueur < desNain) {
            afficherTexteCadre("Hélas, vous avez perdu ! Le nain éclate de rire et vous vole 5 pièces d'or.");
            // Hypothèse : Si le joueur a un système d'or, on lui en retire
        } else {
            afficherTexteCadre("Égalité ! Vous proposez de relancer les dés, mais le nain refuse : 'Pas aujourd'hui !'");
        }
    } else {
        afficherTexteCadre("Vous refusez poliment l'invitation du nain et retournez à vos occupations.");
    }
}

// Quête secondaire : Récupérer un artefact perdu en négociant avec un marchand louche
void queteArtefactPerdu(Personnage *perso) {
    int choix;
    afficherCadre("Quête de l'artefact perdu");
    afficherTexteCadre("Vous entendez parler d'un artefact ancien, perdu dans un désert lointain.\n"
                       "Un marchand louche vous propose de vous donner des informations sur son emplacement, à condition de payer un prix élevé.\n");

    printf("Que faites-vous ?\n");
    printf("1. Payer le marchand pour les informations\n");
    printf("2. Refuser et partir à la recherche de l'artefact par vous-même\n");
    printf("Votre choix (1/2) : ");
    scanf("%d", &choix);
    getchar();  // Capturer l'entrée

    if (choix == 1) {
        afficherTexteCadre("Vous décidez de payer le marchand. Il vous tend une carte détaillant l'emplacement exact de l'artefact.\n"
                           "Après avoir suivi la carte, vous trouvez l'artefact perdu et ressentez sa puissance.");
        ajouterObjet(perso, "Artefact ancien");
        printf("Vous avez gagné un Artefact ancien !\n");
    } else {
        afficherTexteCadre("Vous refusez de payer le marchand et partez à la recherche de l'artefact par vos propres moyens.\n"
                           "Malheureusement, après plusieurs jours de recherche, vous ne trouvez rien.");
    }
}

// Fonction de gestion de la réputation
void gererReputation(Personnage *perso) {
    printf("\nVotre réputation actuelle :\n");
    printf("Village : %d\n", perso->reputationVillage);
    printf("Château : %d\n", perso->reputationChateau);
    printf("Forêt : %d\n", perso->reputationForet);

    if (perso->reputationVillage < -5) {
        afficherTexteCadre("Les villageois refusent de vous aider et vous évitent à tout prix.\n"
                           "Vous devrez peut-être regagner leur confiance.");
    }
    if (perso->reputationChateau > 10) {
        afficherTexteCadre("Les nobles du château vous considèrent comme un héros et vous offrent une épée en récompense.");
        ajouterObjet(perso, "Épée noble");
    }
    if (perso->reputationForet < 0) {
        afficherTexteCadre("Les créatures de la forêt deviennent hostiles à votre égard. Attendez-vous à des embuscades.");
    }
}

// Créatures avec des compétences uniques (soin, vol de vie, boost de force)
Creature creerCreatureAleatoire(int niveau) {
    char noms[][50] = {"Dragon peureux", "Licorne enragée", "Gobelin farceur", "Sorcier fou", "Troll des cavernes"};
    int force[] = {8, 6, 4, 7, 10};
    int soin[] = {0, 10, 0, 0, 0};  // Certaines créatures peuvent se soigner
    int volVie[] = {0, 0, 5, 0, 0};  // Le gobelin peut voler des points de vie
    int boostForce[] = {0, 0, 0, 5, 2};  // Le sorcier et le troll peuvent augmenter leur force
    int pv[] = {30, 20, 15, 25, 40};

    int index = rand() % 5;
    Creature creature;
    strcpy(creature.nom, noms[index]);
    creature.force = force[index];
    creature.pointsVie = pv[index];
    creature.soin = soin[index];
    creature.volVie = volVie[index];
    creature.boostForce = boostForce[index];

    return creature;
}

// Ajout des fins multiples
void finDuJeu(Personnage *perso, int victoire) {
    if (victoire) {
        afficherCadre("VICTOIRE !");
        if (perso->reputationChateau > 10) {
            afficherTexteCadre("Vous êtes devenu un héros légendaire du royaume, honoré par les nobles et les rois.\n"
                               "Le château organise une grande fête en votre honneur, et vous vivez vos jours entouré de richesse et de gloire.");
        } else if (perso->reputationVillage > 5 && perso->reputationChateau <= 10) {
            afficherTexteCadre("Les villageois célèbrent votre bravoure et vous élisent comme chef de leur village.\n"
                               "Vous passez vos journées à défendre les terres contre les créatures sauvages et à organiser des banquets.");
        } else if (perso->reputationForet > 5) {
            afficherTexteCadre("Les créatures de la forêt vous considèrent comme un allié et un gardien.\n"
                               "Vous vous retirez dans les bois, où vous vivez en harmonie avec la nature.");
        } else {
            afficherTexteCadre("Vous avez vaincu toutes les épreuves, mais vous choisissez de vivre dans l'ombre, loin de la gloire.\n"
                               "Peut-être qu'un jour, votre nom sera raconté dans les légendes, mais pour l'instant, vous préférez la tranquillité.");
        }
    } else {
        afficherCadre("DÉFAITE...");
        if (perso->reputationVillage < -5) {
            afficherTexteCadre("Les villageois, fatigués de vos échecs, vous chassent de leur village.\n"
                               "Vous errez seul dans le monde, sans foyer ni amis.");
        } else if (perso->reputationChateau < -5) {
            afficherTexteCadre("Les nobles du château vous considèrent comme un paria et refusent de vous laisser entrer dans le royaume.\n"
                               "Vous êtes condamné à vivre en exil, loin des terres que vous avez tenté de sauver.");
        } else {
            afficherTexteCadre("Vous avez échoué dans votre quête, mais peut-être aurez-vous une autre chance dans une autre vie...\n");
        }
    }

    printf("Merci d'avoir joué à 'L'Épopée Loufoque du Héros Malgré Lui' !\n");
    exit(0);
}

// Fonction de la quête principale : Sauver la princesse Karatéka du Dragon K-pop
void quetePrincipale(Personnage *perso) {
    int choix;

    afficherCadre("Début de la quête pour sauver la princesse Karatéka");
    afficherTexteCadre("Vous vous apprêtez à escalader la Montagne du Délire pour atteindre le repaire du Dragon K-pop.\n"
                       "Avant de commencer l'ascension, vous entendez des bruits étranges provenant d'une forêt voisine...");

    printf("\nQue faites-vous ?\n");
    printf("1. Explorer la forêt avant de grimper\n");
    printf("2. Commencer immédiatement l'ascension de la montagne\n");
    printf("Votre choix (1/2) : ");
    scanf("%d", &choix);
    getchar();

    if (choix == 1) {
        afficherTexteCadre("Vous décidez d'explorer la forêt. À l'intérieur, vous tombez sur un mystérieux sphinx.");
        miniJeuDevinettes(perso);  // Le joueur doit résoudre une énigme avant de continuer

        // Après avoir résolu l'énigme, le joueur peut continuer la quête
        afficherTexteCadre("Ayant résolu l'énigme du Sphinx, vous retournez à la Montagne du Délire.");
    }

    afficherCadre("Continuation de la quête principale");
    afficherTexteCadre("Vous commencez votre ascension de la Montagne du Délire. Des sons étranges résonnent dans l'air...");

    printf("\nQue faites-vous ?\n");
    printf("1. Pénétrer dans une grotte sombre à proximité\n");
    printf("2. Ignorer la grotte et continuer votre ascension\n");
    printf("Votre choix (1/2) : ");
    scanf("%d", &choix);
    getchar();

    if (choix == 1) {
        afficherTexteCadre("Vous entrez dans la grotte. L'air devient plus lourd, et soudain, vous êtes attaqué par une créature.");
        
        // Nouvelle créature rencontrée dans la grotte
        Creature creatureGrotte = {"Serpent des Ombres", 40, 10, 5, 0, 0};  // Le serpent peut se soigner
        combat(perso, creatureGrotte);

        if (perso->pointsVie > 0) {
            afficherTexteCadre("Vous avez vaincu le Serpent des Ombres et trouvez une potion dans la grotte.");
            ajouterObjet(perso, "Potion de soin");
        }
    }

    afficherTexteCadre("Vous continuez votre ascension, mais un autre obstacle vous attend...");

    printf("\nVous tombez sur une vieille cabane avec un vieil homme devant. Que faites-vous ?\n");
    printf("1. Parler à l'homme\n");
    printf("2. Ignorer l'homme et continuer\n");
    printf("Votre choix (1/2) : ");
    scanf("%d", &choix);
    getchar();

    if (choix == 1) {
        afficherTexteCadre("L'homme vous propose un marché : répondre à une énigme ou combattre une créature.");
        miniJeuDevinettes(perso);  // Si le joueur choisit de répondre à l'énigme
    } else {
        afficherTexteCadre("Vous continuez sans interagir avec l'homme, mais vous sentez que vous avez manqué une opportunité...");
    }

    // Continuation de la quête principale
    afficherCadre("Vous entamez l'ascension de la Montagne du Délire");
    afficherTexteCadre("Le chemin est parsemé de rochers et de pancartes absurdes : 'Zone de high kicks' et 'Attention aux chutes de paillettes'.\n"
                       "En progressant, vous tombez sur une créature gardant l'entrée du repaire du Dragon K-pop.");

    // Rencontre avec une créature gardienne avant le Dragon
    Creature gardienMontagne = {"Ogre Dansant", 50, 12, 0, 0, 3};  // Une créature avec un boost de force
    combat(perso, gardienMontagne);

    if (perso->pointsVie > 0) {
        afficherTexteCadre("Après avoir vaincu l'Ogre Dansant, vous vous tenez enfin devant l'entrée du repaire du Dragon K-pop.\n"
                           "Le moment de l'affrontement final approche.");

        Creature boss = creerBoss();
        combatBoss(perso, &boss);  // Lancer le combat final contre le Dragon K-pop
    }
    afficherCadre("Vous êtes au pied de la Montagne du Délire");
    afficherTexteCadre("Le chemin jusqu'au repaire du Dragon K-pop est jonché de pancartes absurdes :\n"
                       "'Attention : Zone de high kicks'\n"
                       "'Danger : Chutes de paillettes'\n"
                       "Vous entendez au loin des échos de musique entraînante. Le moment est venu de sauver la princesse Karatéka !");
    
    printf("\nQue faites-vous ?\n");
    printf("1. Grimper discrètement\n");
    printf("2. Chanter une chanson K-pop pour attirer le dragon\n");
    printf("3. Crier un défi à la princesse\n");
    printf("Votre choix (1/2/3) : ");
    scanf("%d", &choix);
    getchar();  // Capturer l'entrée pour éviter les sauts de lignes

    if (choix == 1) {
        afficherTexteCadre("Vous tentez de grimper discrètement le long de la montagne, mais soudain vous marchez sur une branche étrange. "
                           "Elle se met à jouer le refrain de 'Gangnam Style' à plein volume !\n"
                           "Le Dragon K-pop sort de sa caverne en dansant furieusement, suivi de la princesse Karatéka qui exécute un flip arrière parfait.");
    } else if (choix == 2) {
        afficherTexteCadre("Vous entonnez 'Baby Dragon' de Justin Dragonber avec toute votre âme. "
                           "Ému par votre performance, le Dragon K-pop sort en pleurant des paillettes. "
                           "La princesse Karatéka applaudit en dansant.");
    } else if (choix == 3) {
        afficherTexteCadre("Vous criez : 'Princesse Karatéka, je vous défie en combat singulier !'\n"
                           "La princesse sort en trombe, exécutant une série de coups de pied volants impressionnants. "
                           "Le Dragon K-pop la suit, agitant des pompons et criant : 'Fighting!' à tue-tête.");
    }

    printf("\nLa princesse s'exclame : 'Je n'ai pas besoin d'être sauvée, mais je m'ennuyais ! Que diriez-vous d'un petit combat ?'\n");
    printf("1. Accepter le défi de la princesse\n");
    printf("2. Proposer une battle de danse avec le dragon\n");
    printf("3. Suggérer un pique-nique et des jeux de société\n");
    printf("Votre choix (1/2/3) : ");
    scanf("%d", &choix);
    getchar();

    if (choix == 1) {
        afficherTexteCadre("Vous acceptez le défi. Après un combat épique rempli de pirouettes et de jeux de mots, vous finissez par une égalité. "
                           "La princesse, impressionnée, décide de vous nommer son partenaire d'entraînement officiel.\n"
                           "Félicitations ! Vous avez sauvé la princesse... de l'ennui !");
    } else if (choix == 2) {
        afficherTexteCadre("Vous proposez une battle de danse contre le Dragon K-pop. Le dragon accepte avec enthousiasme et lance la première chorégraphie sur 'Dracarys Style'. "
                           "Vous répondez avec une danse épique sur 'Fire Dance', gagnant le respect du dragon.\n"
                           "Félicitations ! Le Dragon K-pop décide d'abandonner ses activités de terreur pour devenir une star de K-pop.");
    } else if (choix == 3) {
        afficherTexteCadre("Vous proposez un pique-nique et des jeux de société. La princesse et le dragon acceptent volontiers. Vous passez l'après-midi à jouer au Monopoly version 'Royaume Enchanté'. "
                           "À la fin de la journée, vous avez gagné deux nouveaux amis.\n"
                           "Félicitations ! Vous avez sauvé l'ambiance du royaume !");
    }

    finDuJeu(perso, 1);  // Victoire du joueur
}

// Fonction de progression après un choix crucial ou un combat
void progressionAvecRecompense(Personnage *perso, const char* recompense, int gainForce, int gainCharisme) {
    printf("Vous avez reçu une récompense : %s\n", recompense);
    ajouterObjet(perso, recompense);
    
    // Augmentation des caractéristiques
    if (gainForce > 0) {
        printf("Vous gagnez %d points de force.\n", gainForce);
        perso->force += gainForce;
    }
    if (gainCharisme > 0) {
        printf("Vous gagnez %d points de charisme.\n", gainCharisme);
        perso->charisme += gainCharisme;
    }
}

// Quête du village en détresse
void queteVillageDetresse(Personnage *perso) {
    int choix;
    afficherCadre("Quête du village en détresse");
    afficherTexteCadre("En vous approchant du village, vous entendez des cris et des hurlements.\n"
                       "Un villageois apeuré vous interpelle : 'Aidez-nous, s'il vous plaît ! Des créatures sauvages attaquent notre village !'\n"
                       "'Nous ne pouvons pas nous défendre seuls. Si vous ne nous aidez pas, tout sera détruit !'");
    
    printf("\nQue faites-vous ?\n");
    printf("1. Aider les villageois à défendre le village\n");
    printf("2. Refuser et laisser les villageois se débrouiller\n");
    printf("Votre choix (1/2) : ");
    scanf("%d", &choix);
    getchar(); // Pour capturer la nouvelle ligne après scanf

    if (choix == 1) {
        afficherTexteCadre("Vous dégainez votre arme et vous préparez à affronter les créatures.\n"
                           "Les villageois sont soulagés de vous voir prendre position pour défendre leur village.");
        Creature creaturesVillage[] = {
            {"Loup féroce", 30, 8, 0, 0, 0},
            {"Ogre sauvage", 40, 10, 0, 0, 0}
        };

        afficherTexteCadre("Le premier adversaire est un loup féroce qui surgit des bois.");
        combat(perso, creaturesVillage[0]);
        if (perso->pointsVie > 0) {
            afficherTexteCadre("Après avoir vaincu le loup, un ogre sauvage attaque !");
            combat(perso, creaturesVillage[1]);
        }

        if (perso->pointsVie > 0) {
            afficherTexteCadre("Vous avez sauvé le village. Les villageois vous remercient chaleureusement.\n"
                               "En signe de gratitude, ils vous offrent une Potion de force.");
            ajouterObjet(perso, "Potion de force");
            perso->reputationVillage += 5;
            printf("Votre réputation dans le village augmente de 5.\n");
        } else {
            afficherTexteCadre("Vous avez échoué à défendre le village et êtes contraint de fuir.\n"
                               "Le village subit de lourds dégâts.");
        }
    } else {
        afficherTexteCadre("Vous refusez d'intervenir, laissant les villageois désespérés. "
                           "Votre réputation dans le village diminue.");
        perso->reputationVillage -= 5;
    }
}

// Quête du cristal enchanté
void queteCristalEnchante(Personnage *perso) {
    int choix;
    afficherCadre("Quête du cristal enchanté");
    afficherTexteCadre("Un vieux magicien mystérieux vous accoste alors que vous traversez la forêt.\n"
                       "'Hé, toi là-bas ! J'ai besoin de ton aide !' dit-il en s'approchant. "
                       "'Mon cristal enchanté a été volé par une créature maléfique et caché dans une caverne. "
                       "Seuls les plus braves peuvent le récupérer.'\n"
                       "Le magicien vous regarde avec des yeux pleins d'espoir.");
    
    printf("\nQue faites-vous ?\n");
    printf("1. Accepter d'aider le magicien\n");
    printf("2. Refuser et continuer votre chemin\n");
    printf("Votre choix (1/2) : ");
    scanf("%d", &choix);
    getchar(); // Pour capturer la nouvelle ligne après scanf

    if (choix == 1) {
        afficherTexteCadre("Vous acceptez la quête du magicien. 'Excellent !' dit-il. "
                           "'Je savais que je pouvais compter sur toi ! La caverne est au nord, mais fais attention, "
                           "la créature qui garde le cristal est dangereuse.'\n"
                           "Vous partez en direction de la caverne, l'air rempli de mystère.");
        Creature gardien = {"Gardien du cristal", 50, 12, 0, 0, 0};
        afficherTexteCadre("Vous arrivez à l'entrée de la caverne, une brume mystérieuse flotte autour de vous.\n"
                           "Soudain, un grand monstre surgit des ténèbres ! C'est le Gardien du Cristal !");
        combat(perso, gardien);

        if (perso->pointsVie > 0) {
            afficherTexteCadre("Vous avez vaincu le Gardien du Cristal et récupéré l'artefact magique. "
                               "Le cristal brille d'une lumière mystique, et vous ressentez une énergie puissante.");
            ajouterObjet(perso, "Cristal enchanté");
            perso->force += 5;  // Le cristal augmente la force du joueur
            printf("Votre force augmente de 5 grâce au pouvoir du cristal !\n");
        } else {
            afficherTexteCadre("Vous avez échoué à vaincre le gardien et vous fuyez de la caverne, blessé.");
        }
    } else {
        afficherTexteCadre("Vous refusez poliment la requête du magicien et continuez votre chemin. "
                           "'Peut-être trouverai-je quelqu'un d'autre...' murmure-t-il déçu en disparaissant dans la forêt.");
    }
}
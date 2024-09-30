// Silver Creek
// v 0.7 - Hylst (1996 - 2024 déterrage d'anciens sources, restructuration, documentation, mise à jour, ... ) 
// Etat : en  cours de développement

// Simmulation de prise de contôle d'une ville du FAR WEST
// avec gestion de combats, réputations, négociations, évènements, ...

/*
Objectifs principaux :

    Cohérence du gameplay : Un flux continu entre les actions du joueur, la gestion des ressources, les compétences et les missions.
    Interface en mode texte : Afficher un tableau de bord visuel (statut des personnages, ressources) et permettre une navigation fluide via des choix interactifs.
    Système de progression : Assurer que les compétences des personnages évoluent de manière fluide avec des choix stratégiques à chaque niveau.
    Système de missions : Permettre au joueur de prendre des décisions basées sur la réputation avec les factions et les ressources disponibles.
    Événements dynamiques : Intégrer des événements qui modifient l’état du jeu, influençant les décisions du joueur.
    Interface utilisateur simple mais immersive : L’utilisateur doit pouvoir lire rapidement les informations importantes.

Plan général des fonctionnalités :

    Menu principal : Le joueur peut choisir ses actions : attaquer, gérer la ville, négocier ou consulter des rapports.
    Événements aléatoires : Chaque tour a une chance de déclencher un événement affectant les ressources ou les factions.
    Progression des personnages : Les personnages gagnent de l'XP et montent de niveau, débloquant de nouvelles compétences.
    Missions influencées par la réputation : Les missions sont résolues en fonction de la réputation du joueur avec les factions.
    Gestion des ressources : Le joueur doit surveiller et gérer ses ressources (or, munitions, provisions) pour continuer à défendre Silver Creek.

Fonctionnalités avançées et structure du programme :

    Gestion dynamique des ressources et de la ville :
        La ville a des ressources (or, munitions, provisions, moral) qui évoluent selon les événements et les actions du joueur.
        Un tableau de bord du statut de la ville permet de suivre les ressources à tout moment.

    Progression des personnages :
        Le shérif gagne de l'XP et monte de niveau, débloquant des compétences spécifiques aux branches choisies (combat, diplomatie, stratégie).
        Les compétences spéciales sont débloquées après trois améliorations dans une branche.

    Événements aléatoires :
        À chaque tour, un événement aléatoire peut se produire, affectant la réputation, les ressources ou le moral de la ville.
        Ces événements forcent le joueur à adapter sa stratégie (ex. baisse de moral, gain d'or, tensions avec les factions).

    Système de missions influencées par la réputation :
        Les missions sont résolues en fonction de la réputation du joueur avec la faction associée.
        Une bonne réputation facilite les missions, tandis qu'une mauvaise réputation complique les choses.

A implémenter :

Menu principal : Accès aux différentes phases du jeu.
Interface générale : La carte de Silver Creek avec les différentes actions disponibles.
Écran de combat : Affichage du combat entre le Shérif et les bandits.
Écran de négociation : Pour négocier avec les mineurs ou d'autres factions.
Gestion de la ville : Construction, amélioration et gestion des ressources.
Système de progression : Gagner de l’expérience, améliorer les compétences et débloquer des capacités spéciales.
Événements aléatoires : Modifiant la réputation, les ressources, et le moral de la ville.
Affichage des ressources et du statut de la ville.
Missions influencées par la réputation avec les factions.

Notes : 

afficher_interface() : Affiche l'interface générale avec une carte ASCII de Silver Creek et des options d'actions comme déplacer le shérif, attaquer les bandits, etc.
afficher_combat() : Écran détaillant un combat entre le Shérif Cooper et Billy "Le Crochet", avec des informations sur les points de vie (PV) et munitions.
afficher_negociation() : Écran pour une négociation avec des options de dialogue, un tableau visuel montrant le statut de la négociation et les demandes des mineurs.
afficher_gestion_ville() : Écran pour gérer la ville de Silver Creek, incluant des actions comme construire un nouveau bâtiment, ajuster les taxes, ou recruter des députés.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Définition des structures pour les personnages, factions, et missions
typedef struct {
    char nom[50];
    int force;
    int precision;
    int agilite;
    int endurance;
    int pv;
    int xp;
    int niveau;
    int branche_combat;
    int branche_diplomatie;
    int branche_strategie;
    int competence_speciale_debloquee;
} Personnage;

typedef struct {
    char nom[50];
    int reputation;
} Faction;

typedef struct {
    char description[100];
    Faction *faction_associee;
    int difficulte;
} Mission;

typedef struct {
    int or;
    int munitions;
    int provisions;
    int moral;
    int reputation;
} Ville;

// -----------------------------------------------------------------------------
// Initialisation des données de jeu
// -----------------------------------------------------------------------------
void init_personnage(Personnage *p, char *nom, int force, int precision, int agilite, int endurance) {
    strcpy(p->nom, nom);
    p->force = force;
    p->precision = precision;
    p->agilite = agilite;
    p->endurance = endurance;
    p->pv = 100;
    p->xp = 0;
    p->niveau = 1;
    p->branche_combat = 0;
    p->branche_diplomatie = 0;
    p->branche_strategie = 0;
    p->competence_speciale_debloquee = 0;
}

void init_ville(Ville *v) {
    v->or = 1000;
    v->munitions = 50;
    v->provisions = 100;
    v->moral = 80;
    v->reputation = 60;
}

// -----------------------------------------------------------------------------
// Gestion des compétences et expérience
// -----------------------------------------------------------------------------
void gagner_xp(Personnage *p, int xp) {
    p->xp += xp;
    printf("%s a gagné %d XP !\n", p->nom, xp);
    
    if (p->xp >= 100) {
        p->xp -= 100;
        p->niveau++;
        printf("%s monte au niveau %d !\n", p->nom, p->niveau);

        // Choisir une branche à améliorer
        int choix;
        printf("Choisissez une amélioration pour %s :\n", p->nom);
        printf("1. Combat (+1 Force, +1 Précision)\n");
        printf("2. Diplomatie (+2 Réputation lors des négociations)\n");
        printf("3. Stratégie (+2 Moral pour l'équipe)\n");
        printf("Entrez votre choix: ");
        scanf("%d", &choix);

        switch(choix) {
            case 1:
                p->force += 1;
                p->precision += 1;
                p->branche_combat++;
                printf("Branche Combat améliorée ! Force et Précision augmentées.\n");
                if (p->branche_combat >= 3 && !p->competence_speciale_debloquee) {
                    p->competence_speciale_debloquee = 1;
                    printf("Compétence spéciale débloquée : Tir rapide (attaque deux fois par tour) !\n");
                }
                break;
            case 2:
                p->branche_diplomatie++;
                printf("Branche Diplomatie améliorée ! Bonus de Réputation gagné.\n");
                if (p->branche_diplomatie >= 3 && !p->competence_speciale_debloquee) {
                    p->competence_speciale_debloquee = 1;
                    printf("Compétence spéciale débloquée : Maître Négociateur (améliore les négociations) !\n");
                }
                break;
            case 3:
                p->branche_strategie++;
                printf("Branche Stratégie renforcée ! Bonus de Moral pour l'équipe.\n");
                if (p->branche_strategie >= 3 && !p->competence_speciale_debloquee) {
                    p->competence_speciale_debloquee = 1;
                    printf("Compétence spéciale débloquée : Moral d'acier (boost de Moral pour toute l'équipe) !\n");
                }
                break;
            default:
                printf("Choix invalide. Aucune amélioration effectuée.\n");
                break;
        }
    }
}

// -----------------------------------------------------------------------------
// Gestion des ressources de la ville
// -----------------------------------------------------------------------------
void afficher_statut_ville(Ville *ville) {
    printf("\n══════════════════════ Statut de Silver Creek ══════════════════════\n");
    printf("╔════════════════╗\n");
    printf("║  Or: %d         ║\n", ville->or);
    printf("║  Munitions: %d   ║\n", ville->munitions);
    printf("║  Provisions: %d  ║\n", ville->provisions);
    printf("║  Moral: %d       ║\n", ville->moral);
    printf("║  Réputation: %d  ║\n", ville->reputation);
    printf("╚════════════════╝\n");
}

// -----------------------------------------------------------------------------
// Gestion des événements aléatoires
// -----------------------------------------------------------------------------
void generer_evenement(Ville *ville) {
    int evenement_type = rand() % 3;  // Générer un événement aléatoire (0-2)

    switch(evenement_type) {
        case 0:
            printf("Événement : Les tensions avec les tribus indiennes augmentent. Baisse de réputation.\n");
            ville->reputation -= 10;
            break;
        case 1:
            printf("Événement : Un filon d'or est découvert ! Vous gagnez 200$.\n");
            ville->or += 200;
            break;
        case 2:
            printf("Événement : Une tempête de sable frappe la ville, le moral baisse.\n");
            ville->moral -= 10;
            break;
        default:
            printf("Rien de spécial aujourd'hui. Profitez du calme... Pour l'instant !\n");
            break;
    }
}

// -----------------------------------------------------------------------------
// Résolution des missions influencées par la réputation
// -----------------------------------------------------------------------------
void resoudre_mission(Mission *mission, Ville *ville, Faction *f) {
    printf("Mission : %s\n", mission->description);
    printf("Réputation avec %s : %d\n", f->nom, f->reputation);

    if (f->reputation > 50) {
        printf("Votre bonne réputation facilite la mission.\n");
        mission->difficulte -= 1;
    } else if (f->reputation < 20) {
        printf("Votre mauvaise réputation complique la mission.\n");
        mission->difficulte += 1;
    }

    if (mission->difficulte <= 3) {
        printf("Mission réussie ! Réputation et ressources augmentées.\n");
        f->reputation += 10;
        ville->or += 100;
    } else {
        printf("Mission échouée... Réputation et moral en baisse.\n");
        f->reputation -= 10;
        ville->moral -= 5;
    }
}

// -----------------------------------------------------------------------------
// Interfaces du jeu
// -----------------------------------------------------------------------------
void afficher_interface() {
    printf("   /                                                                                    \\\n");
    printf("  |    _____ _ _                     ___               _    _____ _ _                    |\n");
    printf("  |   |   __| |_|_____ ___ ___    ___|  _|___ ___ ___ | |_ |     | | |_ _ ___ ___        |\n");
    printf("  |   |__   | | |     | -_|  _|  | . |  _|  _| -_| -_|  _| |   --| |  _| | -_| -_|       |\n");
    printf("  |   |_____|_|_|_|_|_|___|_|    |___|_| |_| |___|___|\\__| |_____|_|_| |_|___|___|       |\n");
    printf("  |                                                                                      |\n");
    printf("  |                                 Défense de Silver Creek                              |\n");
    printf("  |                                                                                      |\n");
    printf("  |  Date: 15 Mai 1885          Heure: 12:00          Tour: 3          Difficulté: Dur   |\n");
    printf("  |                                                                                      |\n");
    printf("  |  +-----------------------------------------------------------------------------+    |\n");
    printf("  |  |                               Carte de Silver Creek                          |    |\n");
    printf("  |  |                                                                              |    |\n");
    printf("  |  |     ^            *B*                  _____                                  |    |\n");
    printf("  |  |    /|\\           [ ]      ____       | BAR |        *M*      ____            |    |\n");
    printf("  |  |     |    *S*     [ ]  ___/    \\___   |_____|       [~$~]    /    \\           |    |\n");
    printf("  |  |          [+]     [ ]_/  MAIN ST.  \\_    ||         [~$~]___/GOLD  \\___       |    |\n");
    printf("  |  |                  [ ]    [ ] [ ]    |    ||         [~$~]    MINE     |       |    |\n");
    printf("  |  |         ___      [ ]    [ ] [ ]    |    ||         [~$~]    [ ] [ ]  |       |    |\n");
    printf("  |  |        /   \\     [ ]____[ ] [ ]____|    ||         [~$~]____[ ] [ ]__|       |    |\n");
    printf("  |  |       | BANK|         ||     ||         ||              ||     ||            |    |\n");
    printf("  |  |        \\___/                                                                 |    |\n");
    printf("  |  |                                                                              |    |\n");
    printf("  |  |  Légende:  *S* Sheriff  *B* Bandits  *M* Mineurs  [+] Hôpital  [$] Or        |    |\n");
    printf("  +-----------------------------------------------------------------------------+\n");
    printf("  |  Actions disponibles:                     Statut:                                    |\n");
    printf("  |  1. Déplacer le shérif                    Or: 1000$  Réputation: Respecté            |\n");
    printf("  |  2. Attaquer les bandits                  Munitions: 50  Provisions: 75%             |\n");
    printf("  |  3. Renforcer la banque                   Moral de la ville: Bon                     |\n");
    printf("  |  4. Négocier avec les mineurs                                                        |\n");
    printf("  |  5. Consulter les rapports                Alertes:                                   |\n");
    printf("  |                                           ! Bandits repérés près de la mine          |\n");
    printf("  |  Entrez votre choix: _                                                               |\n");
    printf("   \\____________________________________________________________________________________/\n");
}

void afficher_combat() {
    printf("    __________________________________________________________________________________\n");
    printf("   /                                                                                    \\\n");
    printf("  |                         Combat à Silver Creek - Tour 3                               |\n");
    printf("  |                                                                                      |\n");
    printf("  |  +-----------------------------------------------------------------------------+    |\n");
    printf("  |  |                     >>>>>>>> CONFRONTATION! <<<<<<<                          |    |\n");
    printf("  |  |                                                                              |    |\n");
    printf("  |  |    Shérif Cooper          15m          Billy \"Le Crochet\"                    |    |\n");
    printf("  |  |         _O_/¯             ~~~~               _O_                             |    |\n");
    printf("  |  |          |                                    |                              |    |\n");
    printf("  |  |         / \\                                  / \\                             |    |\n");
    printf("  |  |     PV: ████████░░ 80%                   PV: ██████░░░░ 60%                  |    |\n");
    printf("  |  |   Mun: ███████░░░ 70%                   Mun: ████████░░ 80%                  |    |\n");
    printf("  +-----------------------------------------------------------------------------+    |\n");
    printf("  |                                                                                      |\n");
    printf("  |  Actions de Cooper:                      Statut du combat:                           |\n");
    printf("  |  1. Tirer (70% de réussite)              Tour: 3/10                                  |\n");
    printf("  |  2. Se mettre à couvert                  Avantage: Shérif                            |\n");
    printf("  |  3. Recharger                            Terrain: Rue principale                     |\n");
    printf("  |  4. Utiliser un objet                                                                |\n");
    printf("  |  5. Tenter une arrestation               Dernière action:                            |\n");
    printf("  |                                          Billy a tiré et manqué!                     |\n");
    printf("  |  Entrez votre choix: _                                                               |\n");
    printf("   \\____________________________________________________________________________________/\n");
}

// -----------------------------------------------------------------------------
// Menu et boucle de jeu
// -----------------------------------------------------------------------------
void afficher_menu() {
    printf("═══════════════════════ Défense de Silver Creek ═══════════════════════\n");
    printf("                Bienvenue, cow-boy ou cow-girl !\n");
    printf("     Le Far West vous attend pour sauver Silver Creek d'une fin terrible...\n");
    printf("                  Préparez vos six-coups et votre courage.\n");
    printf("──────────────────────────────────────────────────────────────────────\n");
    printf("                    1. Commencer l'aventure\n");
    printf("                    2. Quitter (comme un lâche)\n");
    printf("──────────────────────────────────────────────────────────────────────\n");
}

int main() {
    srand(time(0));  // Initialisation de la génération aléatoire

    int choix_menu, choix_action;
    afficher_menu();
    printf("Entrez votre choix: ");
    scanf("%d", &choix_menu);

    if (choix_menu == 2) {
        printf("Au revoir, cow-boy... Peut-être la prochaine fois ?\n");
        return 0;
    }

    // Initialisation du shérif et des factions
    Personnage sherif;
    Faction tribus_indiennes = {"Tribus indiennes", 40};  // Réputation moyenne
    Faction mineurs = {"Mineurs de Silver Creek", 70};    // Bonne réputation
    Ville silver_creek;

    init_personnage(&sherif, "Shérif Abigail \"Steel\" Cooper", 7, 9, 8, 8);
    init_ville(&silver_creek);

    // Boucle de jeu sur plusieurs tours
    for (int tour = 1; tour <= 5; tour++) {
        printf("\n----- Tour %d -----\n", tour);
        afficher_interface();
        
        printf("Entrez votre choix d'action : ");
        scanf("%d", &choix_action);

        switch(choix_action) {
            case 1:
                printf("Vous avez déplacé le shérif dans une nouvelle zone...\n");
                break;
            case 2:
                afficher_combat();
                gagner_xp(&sherif, 20);  // Gagner de l'XP après un combat
                break;
            case 3:
                printf("Vous avez renforcé la banque, augmentant la défense de Silver Creek !\n");
                silver_creek.or -= 100;  // Utilisation de l'or pour renforcer la banque
                break;
            case 4:
                printf("Négociation en cours avec les mineurs...\n");
                Mission mission_negociation = {"Négocier avec les mineurs de Silver Creek", &mineurs, 3};
                resoudre_mission(&mission_negociation, &silver_creek, &mineurs);
                break;
            case 5:
                printf("Consultation des rapports...\n");
                afficher_statut_ville(&silver_creek);
                break;
            default:
                printf("Action invalide, essayez à nouveau.\n");
                break;
        }

        // Générer un événement aléatoire à chaque tour
        generer_evenement(&silver_creek);
        
        // Afficher le statut de la ville après chaque tour
        afficher_statut_ville(&silver_creek);
    }

    printf("\nFin de la partie ! Merci d'avoir joué à Défense de Silver Creek.\n");

    return 0;
}

// Silver Creek   !!! WIP !!!
// v 0.9 - Hylst (1996 - 2024 déterrage d'anciens sources, restructuration, documentation, mise à jour, ... ) 
// Etat : Gros bordel... en  cours de développement - 
// 2 versions du jeux de plus de 20 années de décalage en cours de '''merge'''
// Est ce que ça en vaut toujours vraiment le temps ?

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
...
TO DO :
Much to merge / to do / to implement / to correct...
2 versions of the unfinished game to join
French -> English
Testing...
...
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// SDL2 is still a problem...

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
    char description[100];
    Faction *faction_associee;
    int difficulte;
} Mission;

// Enumeration for AI behaviors
typedef enum { AGGRESSIVE, DEFENSIVE, PATROLLING } AI_Behavior;

// Structure for town resources
typedef struct {
    int gold, ammo, supplies, food, military_fund, morale, reputation;
} Town;

//Struct for older version
typedef struct {
    int or;
    int munitions;
    int provisions;
    int moral;
    int reputation;
} Ville;

// Structure for factions and their relationships with the player
typedef struct {
    char name[50];
    int reputation;
    int status;  // 0: Hostile, 1: Neutral, 2: Allied
} Faction;

// Structure for enemies with AI behavior and patrol level
typedef struct {
    char name[50];
    int strength, precision, agility, endurance, hp, patrol_level;
    AI_Behavior behavior;
} Enemy;

// Structure for storyline and branching progression
typedef struct {
    int patrol_level;
    int missions_failed;
    int missions_successful;
} StorylineProgress;

// Structure to store player inventory items
typedef struct {
    int medical_kits;
    int ammo_boxes;
    int explosives;
} Inventory;

// Structure to store character skill levels
typedef struct {
    int precision;
    int strength;
    int defense;
} CharacterSkills;

typedef struct {
    int patrol_level;
    int missions_failed;
    int missions_successful;
} BranchingState;

// ----- FUNCTION DECLARATIONS -----
void endgame_scenarios_inventory_main();
void diplomacy_strategic_combat_test_main();
void new_missions_main();
void strategic_combat(Town *town, Enemy *enemy);
void resource_raid_mission(Town *town, Enemy *enemy);
void evaluate_storyline(StorylineProgress *state, Town *town, Faction *factions, int num_factions);
void check_victory_or_defeat(Town *town, Faction *factions, int num_factions);
void use_inventory_item(Inventory *inventory);
void key_mission_event(Town *town, Faction *factions, int success);
void handle_mission_outcome(Town *town, Faction *factions, int num_factions, int success, int impact_on_enemy_patrol);
void combat_mission(Town *town, Enemy *enemy);
void game_loop();
void afficher_menu();
void simulate_gunshot();
void simulate_explosion();
void save_game(Town *town, Faction *factions, int num_factions);
void load_game(Town *town, Faction *factions, int num_factions);
void retrieval_mission(Town *town, Enemy *enemy);
void sabotage_mission(Town *town, Enemy *enemy);
void escort_mission(Town *town, Enemy *enemy);
void stealth_mission(Town *town, Enemy *enemy);
void siege_event(Town *town, Enemy *enemy);
void random_event(Town *town);
void resoudre_mission(Mission *mission, Ville *ville, Faction *f);
void afficher_interface();
void afficher_combat();
void afficher_negociation();
void afficher_gestion_ville();
void on_mission_success(CharacterSkills *skills);
void level_up(CharacterSkills *skills);
void gagner_xp(Personnage *p, int xp);
void init_personnage(Personnage *p, char *nom, int force, int precision, int agilite, int endurance);
void init_ville(Ville *v);
void afficher_statut_ville(Ville *ville);
void generer_evenement(Ville *ville);
void evaluate_branching_paths(BranchingState *state, Town *town, Faction *factions, int num_factions);
void diplomacy_interaction(Town *town, Faction *factions, int num_factions);
void rescue_mission(Town *town, Enemy *enemy);

// ----- FUNCTION DEFINITIONS -----

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

void level_up(CharacterSkills *skills) {
    int choice;
    printf("You have leveled up! Choose a skill to improve:\n");
    printf("1. Precision (+5)\n");
    printf("2. Strength (+5)\n");
    printf("3. Defense (+5)\n");
    scanf("%d", &choice);

    switch (choice) {
        case 1:
            skills->precision += 5;
            printf("Precision increased by 5!\n");
            break;
        case 2:
            skills->strength += 5;
            printf("Strength increased by 5!\n");
            break;
        case 3:
            skills->defense += 5;
            printf("Defense increased by 5!\n");
            break;
        default:
            printf("Invalid choice. No skills were improved.\n");
    }
}

// Evaluate branching paths based on player actions
void evaluate_branching_paths(BranchingState *state, Town *town, Faction *factions, int num_factions) {
    printf("\n--- Story Progression Based on Missions ---\n");

    // If the player has failed multiple missions, the story takes a darker turn
    if (state->missions_failed >= 3) {
        printf("The enemy is gaining strength due to repeated mission failures. The town is struggling to hold out.\n");
        town->morale -= 20;
        printf("Town morale decreased by 20.\n");
    }
    // If the player has been successful in missions, the story progresses positively
    else if (state->missions_successful >= 3) {
        printf("The town is growing stronger with every successful mission. Factions are more willing to ally with you.\n");
        town->reputation += 20;
        printf("Town reputation increased by 20.\n");
    }

    // If patrol levels are high, the enemy is on high alert for future missions
    if (state->patrol_level > 50) {
        printf("The enemy is on high alert. Future missions will be more difficult.\n");
    } else {
        printf("The enemy's defenses are weakened, and future missions will be easier.\n");
    }

    // Display town status and faction relations
    printf("\n--- Town Status ---\n");
    printf("Gold: %d\n", town->gold);
    printf("Supplies: %d\n", town->supplies);
    printf("Military Fund: %d\n", town->military_fund);
    printf("Morale: %d\n", town->morale);
    printf("Reputation: %d\n", town->reputation);

    // Display faction relations
    for (int i = 0; i < num_factions; i++) {
        printf("%s: %s (Reputation: %d)\n", factions[i].name, factions[i].status == 2 ? "Allied" : factions[i].status == 1 ? "Neutral" : "Hostile", factions[i].reputation);
    }
}

// Example usage during combat or mission success
void on_mission_success(CharacterSkills *skills) {
    printf("Mission successful! You gained experience.\n");
    level_up(skills);
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

void simulate_gunshot() {
    printf("Drawing your weapon...\n");
    sleep(1);
    printf("BANG! 💥\n");
}

void simulate_explosion() {
    printf("Lighting the fuse...\n");
    sleep(2);
    printf("BOOM! 💣💥\n");
}

void save_game(Town *town, Faction *factions, int num_factions) {
    FILE *file = fopen("savegame.txt", "w");
    if (file == NULL) {
        printf("Error saving game!\n");
        return;
    }

    // Save town data
    fprintf(file, "%d %d %d %d %d %d %d\n", town->gold, town->ammo, town->supplies, town->food, town->military_fund, town->morale, town->reputation);

    // Save faction data
    for (int i = 0; i < num_factions; i++) {
        fprintf(file, "%s %d %d\n", factions[i].name, factions[i].reputation, factions[i].status);
    }

    fclose(file);
    printf("Game saved successfully!\n");
}

void load_game(Town *town, Faction *factions, int num_factions) {
    FILE *file = fopen("savegame.txt", "r");
    if (file == NULL) {
        printf("No save game found.\n");
        return;
    }

    // Load town data
    fscanf(file, "%d %d %d %d %d %d %d", &town->gold, &town->ammo, &town->supplies, &town->food, &town->military_fund, &town->morale, &town->reputation);

    // Load faction data
    for (int i = 0; i < num_factions; i++) {
        fscanf(file, "%s %d %d", factions[i].name, &factions[i].reputation, &factions[i].status);
    }

    fclose(file);
    printf("Game loaded successfully!\n");
}

// Retrieval mission: Retrieve a valuable resource
void retrieval_mission(Town *town, Enemy *enemy) {
    int approach;
    int success = rand() % 100;

    printf("\n--- Retrieval Mission ---\n");
    printf("You must retrieve valuable resources from enemy territory.\n");
    printf("Choose your approach: 1. Stealth 2. Combat\n");
    scanf("%d", &approach);

    if (approach == 1) {  // Stealth approach
        if (success > 60) {
            printf("Success! You retrieved the resource undetected.\n");
            town->supplies += 30;
            printf("Supplies increased by 30.\n");
        } else {
            printf("You were detected! Mission failed.\n");
            town->morale -= 10;
            printf("Morale decreased by 10.\n");
        }
    } else if (approach == 2) {  // Combat approach
        if (success > 50) {
            printf("Victory! You secured the resource after a fierce battle.\n");
            town->supplies += 20;
            printf("Supplies increased by 20.\n");
        } else {
            printf("Defeat! You were overwhelmed.\n");
            town->morale -= 15;
            printf("Morale decreased by 15.\n");
        }
    }
}

// Sabotage mission: Infiltrate and sabotage enemy defenses
void sabotage_mission(Town *town, Enemy *enemy) {
    int success = rand() % 100;

    printf("\n--- Sabotage Mission ---\n");
    printf("You are infiltrating %s's stronghold to sabotage their defenses.\n", enemy->name);

    if (success > 70) {
        printf("Success! You sabotaged the enemy's defenses.\n");
        enemy->patrol_level -= 30;
        printf("Enemy patrol level decreased by 30.\n");
    } else {
        printf("Failure! You were detected and had to retreat.\n");
        town->military_fund -= 20;
        printf("Military fund decreased by 20.\n");
    }
}

void escort_mission(Town *town, Enemy *enemy) {
    int success = rand() % 100;
    printf("\n--- Escort Mission ---\n");
    printf("You are tasked with escorting a supply convoy through hostile territory.\n");

    // More immersive details
    printf("The wagon rolls slowly through the dusty plains. You hear distant howls of wolves.\n");
    printf("Suddenly, you spot a group of bandits approaching from the hills...\n");

    // Chance-based outcome
    if (success > 70) {
        printf("Success! The convoy arrives safely in Silver Creek without any trouble.\n");
        town->supplies += 30;
        printf("Supplies increased by 30.\n");
    } else if (success > 40) {
        printf("Partial success! The convoy made it, but some supplies were lost during a skirmish.\n");
        town->supplies += 15;
        printf("Supplies increased by 15.\n");
    } else {
        printf("Failure! The convoy was ambushed and most of the supplies were lost.\n");
        town->supplies -= 20;
        town->morale -= 10;
        printf("Supplies decreased by 20. Morale decreased by 10.\n");
    }

    // Dynamic text based on the outcome
    if (success < 40) {
        printf("The remaining survivors look shaken, and the people of Silver Creek are worried about future convoys.\n");
    }
}

// Generate a random event that impacts town resources
void random_event(Town *town) {
    int event_type = rand() % 4;

    switch (event_type) {
        case 0:
            printf("\nRandom Event: A bandit raid on your supply convoy!\n");
            town->supplies -= 30;
            printf("Supplies decreased by 30.\n");
            break;
        case 1:
            printf("\nRandom Event: A trade caravan brings unexpected wealth!\n");
            town->gold += 100;
            printf("Gold increased by 100.\n");
            break;
        case 2:
            printf("\nRandom Event: Famine in Silver Creek!\n");
            town->food -= 40;
            town->morale -= 10;
            printf("Food decreased by 40. Morale decreased by 10.\n");
            break;
        case 3:
            printf("\nRandom Event: A famous bounty hunter offers their services!\n");
            town->military_fund += 50;
            printf("Military fund increased by 50.\n");
            break;
    }
}

// Siege event: An enemy attacks the town
void siege_event(Town *town, Enemy *enemy) {
    printf("\n--- Siege Event ---\n");
    printf("%s is attacking the town!\n", enemy->name);

    if (town->military_fund > 50) {
        printf("Your defenses hold strong, repelling the attackers!\n");
        town->military_fund -= 30;
        town->morale += 5;
        printf("Military fund decreased by 30. Morale increased by 5.\n");
    } else {
        printf("The town's defenses crumble under the assault!\n");
        town->supplies -= 50;
        town->morale -= 20;
        printf("Supplies decreased by 50. Morale decreased by 20.\n");
    }
}

// Stealth mission: The enemy responds to player actions
void stealth_mission(Town *town, Enemy *enemy) {
    int success = rand() % 100;
    int stealth_detected = rand() % 100;

    printf("\n--- Stealth Mission ---\n");
    printf("You are trying to infiltrate enemy territory without being detected by %s.\n", enemy->name);

    // If detected during stealth mission
    if (stealth_detected > 60) {
        printf("You have been detected! %s increases patrols.\n", enemy->name);
        enemy->patrol_level += 20;  // Patrol level increases when detected
        enemy->behavior = AGGRESSIVE;  // Enemy becomes aggressive
        printf("%s is now in AGGRESSIVE mode.\n", enemy->name);
    }

    // If successful
    if (success > 70 && stealth_detected <= 60) {
        printf("Success! You completed the mission without being detected.\n");
        town->supplies += 30;
        printf("Supplies increased by 30.\n");
    } else if (success > 40) {
        printf("The mission was partially successful, but you were detected near the end.\n");
        town->supplies += 15;
        printf("Supplies increased by 15.\n");
    } else {
        printf("Mission failed! You were caught, and the enemy is on high alert.\n");
        enemy->patrol_level += 30;
        printf("%s's patrol level increased by 30.\n", enemy->name);
    }
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

    printf("═══════════════════ Tests Défense de Silver Creek ════════════════════\n");
    printf("    1- Essai de base sherif et factions\n");
    printf("    2- Gestion des Scénarios et inventaires\n");
    printf("    3- Gestion de la réputation et des combats stratégiques\n");
    printf("    4- Gestion des nouvelles missions\n");
    printf("──────────────────────────────────────────────────────────────────────\n");
 
    printf("Entrez votre choix: ");
    scanf("%d", &choix_menu);

    if (choix_menu == 2) {
        endgame_scenarios_inventory_main();
    } else  if (choix_menu == 3) {
            diplomacy_strategic_combat_test_main();
    } else  if (choix_menu == 4) {
        new_missions_main();
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

    game_loop();

    printf("\nFin de la partie ! Merci d'avoir joué à Défense de Silver Creek.\n");

    return 0;
}

// Main game loop for full integration
void game_loop() {
    Town town = {1000, 100, 100, 100, 60, 85, 70};
    Faction factions[3];
    Enemy bandit_leader;
    StorylineProgress progress = {40, 2, 3};  // Example data

    // Initialize factions
    strcpy(factions[0].name, "Bandits");
    factions[0].reputation = 20;
    factions[0].status = 0;

    strcpy(factions[1].name, "Miners");
    factions[1].reputation = 60;
    factions[1].status = 2;

    strcpy(factions[2].name, "Farmers");
    factions[2].reputation = 50;
    factions[2].status = 1;

    // Initialize enemy leader
    strcpy(bandit_leader.name, "Bandit Leader");
    bandit_leader.strength = 7;
    bandit_leader.precision = 6;
    bandit_leader.agility = 5;
    bandit_leader.endurance = 8;
    bandit_leader.hp = 100;
    bandit_leader.patrol_level = 40;
    bandit_leader.behavior = DEFENSIVE;

    // Simulate game events and missions
    random_event(&town);
    siege_event(&town, &bandit_leader);
    escort_mission(&town, &bandit_leader);
    retrieval_mission(&town, &bandit_leader);
    sabotage_mission(&town, &bandit_leader);
    combat_mission(&town, &bandit_leader);
    stealth_mission(&town, &bandit_leader);

    // Display final town status
    printf("\n--- Final Town Status ---\n");
    printf("Gold: %d\n", town->gold);
    printf("Supplies: %d\n", town->supplies);
    printf("Military Fund: %d\n", town->military_fund);
    printf("Morale: %d\n", town->morale);
    printf("Reputation: %d\n", town->reputation);

    // Evaluate the story based on outcomes
    evaluate_storyline(&progress, &town, factions, 3);

    // Check for victory or defeat
    check_victory_or_defeat(&town, 3, 2);
}


// Diplomacy action with resource management consequences
void diplomacy_interaction(Town *town, Faction *factions, int num_factions) {
    int faction_choice, offer;
    printf("\n--- Diplomacy Menu ---\n");
    printf("Choose a faction to interact with:\n");

    for (int i = 0; i < num_factions; i++) {
        printf("%d. %s (Reputation: %d)\n", i + 1, factions[i].name, factions[i].reputation);
    }

    scanf("%d", &faction_choice);
    Faction *chosen_faction = &factions[faction_choice - 1];

    // Diplomatic actions
    printf("1. Offer supplies\n2. Offer gold\n3. Promise protection\nChoose your action: ");
    scanf("%d", &offer);

    if (offer == 1) {
        if (town->supplies > 50) {
            printf("You offer supplies to %s.\n", chosen_faction->name);
            town->supplies -= 50;
            chosen_faction->reputation += 15;
            printf("Supplies decreased by 50. Reputation with %s increased by 15.\n", chosen_faction->name);
        } else {
            printf("Not enough supplies to offer.\n");
        }
    } else if (offer == 2) {
        if (town->gold > 100) {
            printf("You offer gold to %s.\n", chosen_faction->name);
            town->gold -= 100;
            chosen_faction->reputation += 20;
            printf("Gold decreased by 100. Reputation with %s increased by 20.\n", chosen_faction->name);
        } else {
            printf("Not enough gold to offer.\n");
        }
    } else if (offer == 3) {
        printf("You promise protection to %s.\n", chosen_faction->name);
        if (chosen_faction->reputation > 50) {
            printf("Your promise was accepted. %s is now allied with you.\n", chosen_faction->name);
            chosen_faction->status = 2; // Allied
        } else {
            printf("Your promise was rejected. They do not trust you enough yet.\n");
        }
    }
}

// Enhanced rescue mission with multiple paths
void rescue_mission(Town *town, Enemy *enemy) {
    int approach;
    int success = rand() % 100;

    printf("\n--- Rescue Mission ---\n");
    printf("A townsperson has been kidnapped by the enemy. You must rescue them.\n");
    printf("Choose your approach:\n1. Stealth\n2. Negotiation\n3. Combat\n");
    scanf("%d", &approach);

    if (approach == 1) { // Stealth approach
        printf("You attempt to sneak into the enemy camp undetected.\n");
        if (success > 60) {
            printf("Success! You rescued the townsperson without being detected.\n");
            town->morale += 20;
        } else {
            printf("You were spotted! The enemy is now on high alert.\n");
            town->morale -= 10;
        }
    } else if (approach == 2) { // Negotiation
        printf("You attempt to negotiate for the townsperson's release.\n");
        if (success > 50) {
            printf("Success! You convinced the enemy to release the townsperson peacefully.\n");
            town->supplies -= 20;
            town->morale += 15;
        } else {
            printf("Negotiations failed! The enemy refused to cooperate.\n");
            town->morale -= 10;
        }
    } else if (approach == 3) { // Combat approach
        printf("You charge in, weapons drawn, ready for a fight.\n");
        if (success > 70) {
            printf("Victory! You defeated the enemy and rescued the townsperson.\n");
            town->military_fund += 30;
        } else {
            printf("Defeat! The enemy outnumbered you, and the mission failed.\n");
            town->supplies -= 20;
        }
    } else {
        printf("Invalid choice.\n");
    }
}

int endgame_Scenarios_Inventory_main() {
    srand(time(NULL));  // Initialize random seed

    // Initialize town, factions, enemies, and inventory
    Town town = {1000, 100, 100, 100, 60, 85, 70};
    Faction factions[3];
    Enemy bandit_leader;
    StorylineProgress progress = {40, 2, 3};  // Example data
    Inventory player_inventory = {3, 5, 2};   // Initial inventory

    // Initialize factions
    strcpy(factions[0].name, "Bandits");
    factions[0].reputation = 20;
    factions[0].status = 0;

    strcpy(factions[1].name, "Miners");
    factions[1].reputation = 60;
    factions[1].status = 2;

    strcpy(factions[2].name, "Farmers");
    factions[2].reputation = 50;
    factions[2].status = 1;

    // Initialize enemy leader
    strcpy(bandit_leader.name, "Bandit Leader");
    bandit_leader.strength = 7;
    bandit_leader.precision = 6;
    bandit_leader.agility = 5;
    bandit_leader.endurance = 8;
    bandit_leader.hp = 100;
    bandit_leader.patrol_level = 40;
    bandit_leader.behavior = DEFENSIVE;

    // Game loop
    int choice;
    while (1) {
        printf("\n--- Silver Creek Defense ---\n");
        printf("1. Start a mission\n");
        printf("2. Trigger a random event\n");
        printf("3. Defend the town (siege event)\n");
        printf("4. Check town status\n");
        printf("5. Use inventory\n");
        printf("6. Diplomacy\n");
        printf("7. End game\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                printf("Which mission?\n1. Escort\n2. Retrieval\n3. Sabotage\n4. Combat\n5. Rescue\n6. Resource Raid\n");
                scanf("%d", &choice);
                if (choice == 1) escort_mission(&town, &bandit_leader);
                else if (choice == 2) retrieval_mission(&town, &bandit_leader);
                else if (choice == 3) sabotage_mission(&town, &bandit_leader);
                else if (choice == 4) strategic_combat(&town, &bandit_leader);
                else if (choice == 5) rescue_mission(&town, &bandit_leader);
                else resource_raid_mission(&town, &bandit_leader);
                break;
            case 2:
                random_event(&town);
                break;
            case 3:
                siege_event(&town, &bandit_leader);
                break;
            case 4:
                printf("\n--- Town Status ---\n");
                printf("Gold: %d\nSupplies: %d\nMilitary Fund: %d\nMorale: %d\nReputation: %d\n",
                       town.gold, town.supplies, town.military_fund, town.morale, town.reputation);
                break;
            case 5:
                use_inventory_item(&player_inventory);
                break;
            case 6:
                diplomacy_interaction(&town, factions, 3);
                break;
            case 7:
                check_victory_or_defeat(&town, factions, 3);
                printf("Exiting game...\n");
                exit(0);
            default:
                printf("Invalid choice.\n");
        }
    }

    return 0;
}

int diplomacy_strategic_combat_test_main() {
    srand(time(NULL));  // Initialize random seed

    // Initialize town, factions, and enemies
    Town town = {1000, 100, 100, 100, 60, 85, 70};
    Faction factions[3];
    Enemy bandit_leader;
    StorylineProgress progress = {40, 2, 3};  // Example data

    // Initialize factions
    strcpy(factions[0].name, "Bandits");
    factions[0].reputation = 20;
    factions[0].status = 0;

    strcpy(factions[1].name, "Miners");
    factions[1].reputation = 60;
    factions[1].status = 2;

    strcpy(factions[2].name, "Farmers");
    factions[2].reputation = 50;
    factions[2].status = 1;

    // Initialize enemy leader
    strcpy(bandit_leader.name, "Bandit Leader");
    bandit_leader.strength = 7;
    bandit_leader.precision = 6;
    bandit_leader.agility = 5;
    bandit_leader.endurance = 8;
    bandit_leader.hp = 100;
    bandit_leader.patrol_level = 40;
    bandit_leader.behavior = DEFENSIVE;

    // Game loop
    int choice;
    while (1) {
        printf("\n--- Silver Creek Defense ---\n");
        printf("1. Start a mission\n");
        printf("2. Trigger a random event\n");
        printf("3. Defend the town (siege event)\n");
        printf("4. Check town status\n");
        printf("5. Diplomacy\n");
        printf("6. Exit game\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                printf("Which mission?\n1. Escort\n2. Retrieval\n3. Sabotage\n4. Combat\n5. Rescue\n6. Resource Raid\n");
                scanf("%d", &choice);
                if (choice == 1) escort_mission(&town, &bandit_leader);
                else if (choice == 2) retrieval_mission(&town, &bandit_leader);
                else if (choice == 3) sabotage_mission(&town, &bandit_leader);
                else if (choice == 4) strategic_combat(&town, &bandit_leader);
                else if (choice == 5) rescue_mission(&town, &bandit_leader);
                else resource_raid_mission(&town, &bandit_leader);
                break;
            case 2:
                random_event(&town);
                break;
            case 3:
                siege_event(&town, &bandit_leader);
                break;
            case 4:
                printf("\n--- Town Status ---\n");
                printf("Gold: %d\nSupplies: %d\nMilitary Fund: %d\nMorale: %d\nReputation: %d\n",
                       town.gold, town.supplies, town.military_fund, town.morale, town.reputation);
                break;
            case 5:
                diplomacy_interaction(&town, factions, 3);
                break;
            case 6:
                printf("Exiting game...\n");
                exit(0);
            default:
                printf("Invalid choice.\n");
        }
    }

    return 0;
}


// Function to use an item from the inventory
void use_inventory_item(Inventory *inventory) {
    int choice;
    printf("\n--- Inventory ---\n");
    printf("1. Use Medical Kit (x%d)\n", inventory->medical_kits);
    printf("2. Use Ammo Box (x%d)\n", inventory->ammo_boxes);
    printf("3. Use Explosives (x%d)\n", inventory->explosives);
    printf("Choose an item to use: ");
    scanf("%d", &choice);

    switch (choice) {
        case 1:
            if (inventory->medical_kits > 0) {
                printf("You used a medical kit. Restored health to your forces.\n");
                inventory->medical_kits--;
            } else {
                printf("No medical kits left!\n");
            }
            break;
        case 2:
            if (inventory->ammo_boxes > 0) {
                printf("You resupplied your troops with ammo. Increased attack power.\n");
                inventory->ammo_boxes--;
            } else {
                printf("No ammo boxes left!\n");
            }
            break;
        case 3:
            if (inventory->explosives > 0) {
                printf("You set up explosives! The enemy's defenses are weakened.\n");
                inventory->explosives--;
            } else {
                printf("No explosives left!\n");
            }
            break;
        default:
            printf("Invalid choice.\n");
    }
}

// Example of a branching event triggered after a key mission success or failure
void key_mission_event(Town *town, Faction *factions, int success) {
    if (success) {
        printf("\n--- Key Mission Event: Success ---\n");
        printf("Your recent victory has drawn the attention of the local militia. They offer support.\n");
        town->military_fund += 50;
        printf("Military fund increased by 50. You now have more resources for future defenses.\n");

        // Improved reputation with allied factions
        for (int i = 0; i < 3; i++) {
            if (factions[i].status == 2) { // If allied
                factions[i].reputation += 10;
                printf("Your allies are impressed by your success. Reputation with %s increased.\n", factions[i].name);
            }
        }
    } else {
        printf("\n--- Key Mission Event: Failure ---\n");
        printf("Your failure has emboldened the enemy. The bandits are now more aggressive.\n");
        town->morale -= 20;
        town->supplies -= 30;
        printf("Morale decreased by 20. Supplies decreased by 30.\n");

        // Allied factions lose trust
        for (int i = 0; i < 3; i++) {
            if (factions[i].status == 2) {
                factions[i].reputation -= 10;
                printf("Your allies are disappointed. Reputation with %s decreased.\n", factions[i].name);
            }
        }
    }
}

// Function to evaluate endgame conditions
void check_victory_or_defeat(Town *town, Faction *factions, int num_factions) {
    printf("\n--- Endgame Evaluation ---\n");

    // Check if morale is too low for survival
    if (town->morale <= 20) {
        printf("Defeat: The town of Silver Creek has fallen. The people have lost hope and fled.\n");
        printf("The bandits have overrun the town. Game Over.\n");
    }
    // Check if alliances are strong enough for victory
    else {
        int alliances_formed = 0;
        for (int i = 0; i < num_factions; i++) {
            if (factions[i].status == 2) { // Allied factions
                alliances_formed++;
            }
        }

        if (alliances_formed >= 2 && town->morale > 50) {
            printf("Victory: You have secured alliances with key factions and kept morale high.\n");
            printf("Silver Creek thrives under your leadership, and the bandit threat is diminished.\n");
        } else if (alliances_formed < 2 && town->morale > 20) {
            printf("Neutral Survival: The town survives, but without strong alliances.\n");
            printf("Silver Creek will live to see another day, but its future remains uncertain.\n");
        } else {
            printf("Defeat: Despite your efforts, the town was unable to stand against the rising threats.\n");
        }
    }

    // Display final town status and resource levels
    printf("Final Town Status:\n");
    printf("Gold: %d, Supplies: %d, Military Fund: %d, Morale: %d, Reputation: %d\n", 
           town->gold, town->supplies, town->military_fund, town->morale, town->reputation);
}

// Combat with tactical choices
void strategic_combat(Town *town, Enemy *enemy) {
    int tactic_choice, success;
    printf("\n--- Strategic Combat ---\n");
    printf("You face %s in a showdown.\n", enemy->name);
    printf("Choose your tactic:\n");
    printf("1. Take cover (+defense)\n2. Charge aggressively (+attack)\n3. Flank the enemy (+speed)\n");
    scanf("%d", &tactic_choice);

    // Determine success based on tactic
    success = rand() % 100;
    if (tactic_choice == 1) {
        printf("You take cover behind some crates. The enemy shoots wildly but misses.\n");
        if (success > 40) {
            printf("Victory! You outlasted the enemy by using cover.\n");
            town->morale += 10;
        } else {
            printf("Defeat! The enemy eventually overran your position.\n");
            town->morale -= 10;
        }
    } else if (tactic_choice == 2) {
        printf("You charge aggressively, hoping to catch the enemy off-guard.\n");
        if (success > 50) {
            printf("Victory! Your aggressive tactics worked, and you overwhelmed the enemy.\n");
            town->military_fund += 10;
        } else {
            printf("Defeat! The enemy repelled your charge.\n");
            town->military_fund -= 10;
        }
    } else if (tactic_choice == 3) {
        printf("You flank the enemy, trying to outmaneuver them.\n");
        if (success > 60) {
            printf("Victory! You caught the enemy by surprise and won the battle.\n");
            town->supplies += 20;
        } else {
            printf("Defeat! The enemy saw through your strategy and countered.\n");
            town->supplies -= 20;
        }
    }
}

// Dynamic consequences based on mission outcomes
void handle_mission_outcome(Town *town, Faction *factions, int num_factions, int success, int impact_on_enemy_patrol) {
    if (success) {
        printf("\n--- Mission Success ---\n");
        town->morale += 10;
        printf("Morale increased by 10. The town is rallying behind you.\n");

        // Impact factions positively
        for (int i = 0; i < num_factions; i++) {
            if (factions[i].status == 2) { // If allied, reputation improves
                factions[i].reputation += 5;
                printf("Faction %s is pleased with your success. Reputation increased.\n", factions[i].name);
            }
        }

        // Reduce enemy patrol level, making future missions easier
        printf("The enemy's patrol level is reduced due to your success.\n");
        impact_on_enemy_patrol -= 20;

    } else {
        printf("\n--- Mission Failure ---\n");
        town->morale -= 10;
        printf("Morale decreased by 10. The town is anxious about future missions.\n");

        // Impact factions negatively
        for (int i = 0; i < num_factions; i++) {
            if (factions[i].status == 2) { // If allied, reputation decreases
                factions[i].reputation -= 5;
                printf("Faction %s is disappointed in your failure. Reputation decreased.\n", factions[i].name);
            }
        }

        // Increase enemy patrol level, making future missions harder
        printf("The enemy's patrol level is increased due to your failure.\n");
        impact_on_enemy_patrol += 20;

        // Additional consequences based on failure (e.g., retaliation)
        printf("In retaliation, the enemy launches a raid on the town!\n");
        town->supplies -= 30;
        printf("Supplies decreased by 30.\n");
    }
}

// Enhanced Combat mission: Enemy AI adapts based on player's strategies
void combat_mission(Town *town, Enemy *enemy) {
    int success = rand() % 100;

    printf("\n--- Combat Mission ---\n");
    printf("You face off against %s in a hostile encounter.\n", enemy->name);

    // AI behavior based on patrol level
    if (enemy->patrol_level > 50) {
        printf("The enemy is heavily fortified. They seem prepared for a tough fight!\n");
        enemy->behavior = DEFENSIVE;
    } else if (enemy->patrol_level < 30) {
        printf("The enemy appears weakened, with minimal fortifications.\n");
        enemy->behavior = PATROLLING;
    } else {
        printf("The enemy seems cautious, observing your every move.\n");
        enemy->behavior = AGGRESSIVE;
    }

    // Player’s actions vs. enemy's behavior
    if (enemy->behavior == DEFENSIVE) {
        printf("The enemy takes cover and defends their position!\n");
        if (success > 70) {
            printf("Victory! You breached their defenses and won the battle.\n");
            town->morale += 10;
            printf("Town morale increased by 10.\n");
        } else {
            printf("Defeat! The enemy held their ground, and your forces were repelled.\n");
            town->morale -= 15;
            printf("Town morale decreased by 15.\n");
        }
    } else if (enemy->behavior == AGGRESSIVE) {
        printf("The enemy charges at your forces!\n");
        if (success > 60) {
            printf("Victory! You outmaneuvered the aggressive attackers.\n");
            town->military_fund += 10;
            printf("Military fund increased by 10.\n");
        } else {
            printf("Defeat! The enemy's aggressive tactics overwhelmed your forces.\n");
            town->morale -= 10;
            printf("Town morale decreased by 10.\n");
        }
    } else {
        printf("The enemy seems unsure and scattered, giving you a tactical advantage.\n");
        if (success > 50) {
            printf("Victory! You took advantage of their disorganization and won.\n");
            town->supplies += 15;
            printf("Supplies increased by 15.\n");
        } else {
            printf("Defeat! The enemy managed to regroup and repel your attack.\n");
            town->supplies -= 10;
            printf("Supplies decreased by 10.\n");
        }
    }
}

int new_missions_main() {
    srand(time(NULL));  // Initialize random seed

    // Initialize town, factions, and enemies
    Town town = {1000, 100, 100, 100, 60, 85, 70};
    Faction factions[3];
    Enemy bandit_leader;
    StorylineProgress progress = {40, 2, 3};  // Example data

    // Initialize factions
    strcpy(factions[0].name, "Bandits");
    factions[0].reputation = 20;
    factions[0].status = 0;

    strcpy(factions[1].name, "Miners");
    factions[1].reputation = 60;
    factions[1].status = 2;

    strcpy(factions[2].name, "Farmers");
    factions[2].reputation = 50;
    factions[2].status = 1;

    // Initialize enemy leader
    strcpy(bandit_leader.name, "Bandit Leader");
    bandit_leader.strength = 7;
    bandit_leader.precision = 6;
    bandit_leader.agility = 5;
    bandit_leader.endurance = 8;
    bandit_leader.hp = 100;
    bandit_leader.patrol_level = 40;
    bandit_leader.behavior = DEFENSIVE;

    // Game loop
    int choice;
    while (1) {
        printf("\n--- Silver Creek Defense ---\n");
        printf("1. Start a mission\n");
        printf("2. Trigger a random event\n");
        printf("3. Defend the town (siege event)\n");
        printf("4. Check town status\n");
        printf("5. Exit game\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                printf("Which mission?\n1. Escort\n2. Retrieval\n3. Sabotage\n4. Combat\n5. Rescue\n6. Resource Raid\n");
                scanf("%d", &choice);
                if (choice == 1) escort_mission(&town, &bandit_leader);
                else if (choice == 2) retrieval_mission(&town, &bandit_leader);
                else if (choice == 3) sabotage_mission(&town, &bandit_leader);
                else if (choice == 4) combat_mission(&town, &bandit_leader);
                else if (choice == 5) rescue_mission(&town, &bandit_leader);
                else resource_raid_mission(&town, &bandit_leader);
                break;
            case 2:
                random_event(&town);
                break;
            case 3:
                siege_event(&town, &bandit_leader);
                break;
            case 4:
                printf("\n--- Town Status ---\n");
                printf("Gold: %d\nSupplies: %d\nMilitary Fund: %d\nMorale: %d\nReputation: %d\n",
                       town.gold, town.supplies, town.military_fund, town.morale, town.reputation);
                break;
            case 5:
                printf("Exiting game...\n");
                exit(0);
            default:
                printf("Invalid choice.\n");
        }
    }

    return 0;
}

// Resource raid mission: Steal supplies from enemy forces
void resource_raid_mission(Town *town, Enemy *enemy) {
    int success = rand() % 100;
    printf("\n--- Resource Raid Mission ---\n");
    printf("You are raiding an enemy camp to steal vital supplies.\n");

    if (success > 70) {
        printf("Success! You stole valuable supplies from the enemy.\n");
        town->supplies += 50;
        printf("Supplies increased by 50.\n");
    } else if (success > 40) {
        printf("Partial success. You managed to steal some supplies, but were detected.\n");
        town->supplies += 20;
        town->military_fund -= 10; // Losses from skirmish
        printf("Supplies increased by 20, but military funds decreased by 10.\n");
    } else {
        printf("Failure. The enemy caught you in the act and retaliated with a raid on the town.\n");
        town->supplies -= 30;
        town->military_fund -= 20;
        town->morale -= 10;
        printf("Supplies decreased by 30, military funds decreased by 20, and morale decreased by 10.\n");
    }
}

// Evaluate the branching storyline based on mission outcomes
void evaluate_storyline(StorylineProgress *state, Town *town, Faction *factions, int num_factions) {
    printf("\n--- Storyline Progress ---\n");

    // If too many missions are failed, the enemy gains strength and the town suffers
    if (state->missions_failed >= 3) {
        printf("The enemy is growing stronger due to your mission failures. The town is in danger.\n");
        town->morale -= 20;
        town->military_fund -= 30; // Reduced military resources due to failures
        for (int i = 0; i < num_factions; i++) {
            if (factions[i].status == 2) { // Allies may lose trust if you're failing
                factions[i].reputation -= 10;
                printf("Your alliance with %s weakens. Reputation with them decreased by 10.\n", factions[i].name);
            }
        }
        printf("Town morale decreased by 20, military funds decreased by 30.\n");
    } 
    // If many missions succeed, the town grows stronger, and alliances improve
    else if (state->missions_successful >= 3) {
        printf("The town is growing stronger due to your successes. The factions are rallying behind you.\n");
        town->morale += 20;
        town->military_fund += 50; // Increased military resources due to victories
        for (int i = 0; i < num_factions; i++) {
            if (factions[i].status == 2) { // Allied factions are happier
                factions[i].reputation += 10;
                printf("Your alliance with %s strengthens. Reputation with them increased by 10.\n", factions[i].name);
            }
        }
        printf("Town morale increased by 20, military funds increased by 50.\n");
    }

    // Adjust future missions based on enemy patrol levels
    if (state->patrol_level > 50) {
        printf("The enemy is on high alert, making future missions more difficult.\n");
        printf("You must act carefully, as enemy reinforcements will be more frequent.\n");
    } else if (state->patrol_level < 30) {
        printf("The enemy's defenses are weakened, making future missions easier.\n");
        printf("This is an opportunity to strike decisively!\n");
    } else {
        printf("The enemy is neither weak nor overly strong. Prepare wisely for upcoming missions.\n");
    }

    // Display current faction relationships
    for (int i = 0; i < num_factions; i++) {
        printf("%s: %s (Reputation: %d)\n", factions[i].name, 
               factions[i].status == 2 ? "Allied" : factions[i].status == 1 ? "Neutral" : "Hostile",
               factions[i].reputation);
    }
}

// Silver Creek  !!! WIP !!!
// Hylst (1996 - 2024 déterrage d'anciens sources, restructuration, documentation, mise à jour, ... ) 
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

Let's integrate : 
Structures for characters, enemies, town, and factions.
Skill Trees, Leveling, and Abilities.
AI Behaviors for enemies.
Combat System.
Mission System and Diplomacy.
Game Loop with turns, player choices, and AI interactions.
...
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Enum for mission types
typedef enum {
    COMBAT_MISSION,
    DIPLOMATIC_MISSION,
    SUPPLY_MISSION
} MissionType;

// Structure to define a mission
typedef struct {
    char description[100];
    Faction *faction;
    MissionType type;
    int difficulty;        // 1: Easy, 2: Medium, 3: Hard
    int reward_gold;
    int reward_reputation;
    int penalty_morale;
} Mission;


// Enum for different abilities
typedef enum {
    NONE,
    INCREASE_DAMAGE,
    INCREASE_HEALTH,
    INCREASE_PRECISION,
    NEGOTIATION_BOOST,
    REPUTATION_BOOST,
    DODGE_BOOST,
    CRITICAL_HIT
} Ability;

// Enum for different AI behaviors
typedef enum {
    AGGRESSIVE,
    DEFENSIVE,
    BALANCED
} AI_Behavior;

// Enum for faction status
typedef enum {
    ALLIED,
    NEUTRAL,
    HOSTILE
} FactionStatus;

// Structure for a character
typedef struct {
    char name[50];
    int strength;
    int precision;
    int agility;
    int endurance;
    int hp;
    int xp;
    int level;
    int ammo;
    int special_ability_unlocked;
} Character;

// Structure for an enemy
typedef struct {
    char name[50];
    int strength;
    int precision;
    int agility;
    int endurance;
    int hp;
    int ammo;
    AI_Behavior behavior;
    int can_call_reinforcements;
} Enemy;

// Structure for a faction
typedef struct {
    char name[50];
    int reputation;
    FactionStatus status;
} Faction;

// Structure for the town's resources
typedef struct {
    int gold;
    int ammo;
    int supplies;
    int morale;
    int reputation;
} Town;

// Structure for a skill tree node
typedef struct {
    char name[50];
    Ability ability;
    int unlocked;  // 0: Locked, 1: Unlocked
    int required_level;  // Minimum level to unlock this skill
} SkillNode;

// Structure for a character's skill tree
typedef struct {
    SkillNode combat_tree[3];  // Combat branch
    SkillNode diplomacy_tree[3];  // Diplomacy branch
    SkillNode stealth_tree[3];  // Stealth/Agility branch
} SkillTree;

// Initialize SDL2 for sound
void init_sdl_audio() {
    if (SDL_Init(SDL_INIT_AUDIO) < 0) {
        printf("Failed to initialize SDL Audio: %s\n", SDL_GetError());
        exit(1);
    }

    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        printf("SDL Mixer could not initialize! SDL Mixer Error: %s\n", Mix_GetError());
        exit(1);
    }
}

// Function to initialize a character
void init_character(Character *c, char *name, int strength, int precision, int agility, int endurance, int ammo) {
    strcpy(c->name, name);
    c->strength = strength;
    c->precision = precision;
    c->agility = agility;
    c->endurance = endurance;
    c->hp = 100;
    c->xp = 0;
    c->level = 1;
    c->ammo = ammo;
    c->special_ability_unlocked = 0;
}

// Function to initialize an enemy
void init_enemy(Enemy *enemy, char *name, int strength, int precision, int agility, int endurance, int hp, int ammo, AI_Behavior behavior) {
    strcpy(enemy->name, name);
    enemy->strength = strength;
    enemy->precision = precision;
    enemy->agility = agility;
    enemy->endurance = endurance;
    enemy->hp = hp;
    enemy->ammo = ammo;
    enemy->behavior = behavior;
    enemy->can_call_reinforcements = 1;
}

// Function to initialize a town's resources
void init_town(Town *t) {
    t->gold = 1000;
    t->ammo = 100;
    t->supplies = 100;
    t->morale = 80;
    t->reputation = 60;
}

// Function to initialize a faction
void init_faction(Faction *f, char *name, int reputation) {
    strcpy(f->name, name);
    f->reputation = reputation;
    f->status = NEUTRAL;
}

// Function to initialize the skill tree for a character
void init_skill_tree(SkillTree *tree) {
    // Combat Tree
    strcpy(tree->combat_tree[0].name, "Increase Damage");
    tree->combat_tree[0].ability = INCREASE_DAMAGE;
    tree->combat_tree[0].unlocked = 0;
    tree->combat_tree[0].required_level = 2;

    strcpy(tree->combat_tree[1].name, "Increase Health");
    tree->combat_tree[1].ability = INCREASE_HEALTH;
    tree->combat_tree[1].unlocked = 0;
    tree->combat_tree[1].required_level = 4;

    strcpy(tree->combat_tree[2].name, "Increase Precision");
    tree->combat_tree[2].ability = INCREASE_PRECISION;
    tree->combat_tree[2].unlocked = 0;
    tree->combat_tree[2].required_level = 6;

    // Diplomacy Tree
    strcpy(tree->diplomacy_tree[0].name, "Negotiation Boost");
    tree->diplomacy_tree[0].ability = NEGOTIATION_BOOST;
    tree->diplomacy_tree[0].unlocked = 0;
    tree->diplomacy_tree[0].required_level = 2;

    strcpy(tree->diplomacy_tree[1].name, "Reputation Boost");
    tree->diplomacy_tree[1].ability = REPUTATION_BOOST;
    tree->diplomacy_tree[1].unlocked = 0;
    tree->diplomacy_tree[1].required_level = 4;

    // Stealth/Agility Tree
    strcpy(tree->stealth_tree[0].name, "Dodge Boost");
    tree->stealth_tree[0].ability = DODGE_BOOST;
    tree->stealth_tree[0].unlocked = 0;
    tree->stealth_tree[0].required_level = 2;

    strcpy(tree->stealth_tree[1].name, "Critical Hit Chance");
    tree->stealth_tree[1].ability = CRITICAL_HIT;
    tree->stealth_tree[1].unlocked = 0;
    tree->stealth_tree[1].required_level = 5;
}

// Function to display character stats
void display_character_stats(Character *c) {
    printf("\nCharacter: %s\n", c->name);
    printf("HP: %d\n", c->hp);
    printf("Strength: %d\n", c->strength);
    printf("Precision: %d\n", c->precision);
    printf("Agility: %d\n", c->agility);
    printf("Endurance: %d\n", c->endurance);
    printf("Ammo: %d\n", c->ammo);
    printf("XP: %d | Level: %d\n", c->xp, c->level);
}

// Function to display town resources
void display_town_stats(Town *t) {
    printf("\nTown Resources:\n");
    printf("Gold: %d\n", t->gold);
    printf("Ammo: %d\n", t->ammo);
    printf("Supplies: %d\n", t->supplies);
    printf("Morale: %d\n", t->morale);
    printf("Reputation: %d\n", t->reputation);
}

// Function to display faction status
void display_faction_status(Faction *f) {
    printf("\nFaction: %s\n", f->name);
    printf("Reputation: %d\n", f->reputation);
    printf("Status: ");
    switch (f->status) {
        case ALLIED:
            printf("Allied\n");
            break;
        case NEUTRAL:
            printf("Neutral\n");
            break;
        case HOSTILE:
            printf("Hostile\n");
            break;
    }
}

/*
Core Structures:
The game has character, enemy, town, and faction structures, all initialized at the start.
The skill tree structure holds combat, diplomacy, and stealth branches, each with abilities that unlock as the player levels up.
Display Functions:
These functions will help display stats for the character, town, and factions throughout the game.
*/

/*
Let's integrate the skill tree system with leveling up and add the ability to unlock skills as the player gains experience.
*/

// Function to level up the player and offer skill choices
void level_up(Character *player, SkillTree *tree) {
    player->level++;
    printf("\nCongratulations! You leveled up to Level %d.\n", player->level);

    // Offer the player a skill choice based on their new level
    unlock_skill(tree, player);
}

// Function to unlock a skill when leveling up
void unlock_skill(SkillTree *tree, Character *player) {
    int choice, branch, skill;

    printf("\nYou have leveled up to Level %d! Choose a branch to unlock a skill:\n", player->level);
    printf("1. Combat Branch\n");
    printf("2. Diplomacy Branch\n");
    printf("3. Stealth/Agility Branch\n");
    scanf("%d", &branch);

    // Display available skills in the chosen branch
    switch (branch) {
        case 1:
            printf("Combat Skills:\n");
            for (int i = 0; i < 3; i++) {
                if (tree->combat_tree[i].unlocked == 0 && player->level >= tree->combat_tree[i].required_level) {
                    printf("%d. %s (Requires Level %d)\n", i + 1, tree->combat_tree[i].name, tree->combat_tree[i].required_level);
                }
            }
            break;
        case 2:
            printf("Diplomacy Skills:\n");
            for (int i = 0; i < 3; i++) {
                if (tree->diplomacy_tree[i].unlocked == 0 && player->level >= tree->diplomacy_tree[i].required_level) {
                    printf("%d. %s (Requires Level %d)\n", i + 1, tree->diplomacy_tree[i].name, tree->diplomacy_tree[i].required_level);
                }
            }
            break;
        case 3:
            printf("Stealth/Agility Skills:\n");
            for (int i = 0; i < 3; i++) {
                if (tree->stealth_tree[i].unlocked == 0 && player->level >= tree->stealth_tree[i].required_level) {
                    printf("%d. %s (Requires Level %d)\n", i + 1, tree->stealth_tree[i].name, tree->stealth_tree[i].required_level);
                }
            }
            break;
        default:
            printf("Invalid choice.\n");
            return;
    }

    printf("Choose a skill to unlock: ");
    scanf("%d", &skill);
    skill--;  // Adjust for 0-based index

    // Unlock the chosen skill
    if (branch == 1) {
        tree->combat_tree[skill].unlocked = 1;
        printf("You have unlocked: %s\n", tree->combat_tree[skill].name);
    } else if (branch == 2) {
        tree->diplomacy_tree[skill].unlocked = 1;
        printf("You have unlocked: %s\n", tree->diplomacy_tree[skill].name);
    } else if (branch == 3) {
        tree->stealth_tree[skill].unlocked = 1;
        printf("You have unlocked: %s\n", tree->stealth_tree[skill].name);
    }
}

// Function to add XP and check if the player can level up
void add_xp(Character *player, int xp, SkillTree *tree) {
    player->xp += xp;
    printf("%s gained %d XP! Current XP: %d\n", player->name, xp, player->xp);

    // Check if the player should level up
    if (player->xp >= 100) {
        player->xp -= 100;
        level_up(player, tree);
    }
}

// Function to apply skills during combat
void apply_combat_skills(SkillTree *tree, Character *player) {
    if (tree->combat_tree[0].unlocked) {
        player->strength += 2;  // Increase damage
        printf("Skill Applied: Increase Damage. Strength boosted by 2.\n");
    }
    if (tree->combat_tree[1].unlocked) {
        player->hp += 20;  // Increase health
        printf("Skill Applied: Increase Health. HP boosted by 20.\n");
    }
    if (tree->combat_tree[2].unlocked) {
        player->precision += 1;  // Increase precision
        printf("Skill Applied: Increase Precision. Precision boosted by 1.\n");
    }
}

/*
Leveling Up: When the player gains enough XP, they level up and can unlock new skills from one of the three branches: Combat, Diplomacy, or Stealth.
Skill Application: Once skills are unlocked, they provide stat bonuses during combat, diplomacy, or stealth missions.
*/

/*
AI Behaviors and Combat System
Complex AI behaviors and combat system, making it interactive and challenging for the player. 
Enemies will act based on their AI behavior (Aggressive, Defensive, or Balanced), and the player will have several choices each turn, 
such as attacking, taking cover, or reloading.
Combat System with AI Behaviors and Sound Effects:
*/

// Function to make an AI decision based on its behavior
void enemy_decision(Enemy *enemy, Character *player) {
    if (enemy->behavior == AGGRESSIVE) {
        printf("\nEnemy AI (%s): Aggressive Mode\n", enemy->name);

        // Aggressive AI: attacks frequently
        if (enemy->ammo > 0) {
            printf("%s attacks you!\n", enemy->name);
            if (rand() % 100 < enemy->precision) {
                play_sound_effect("attack.wav");
                player->hp -= 20;
                printf("%s hits you for 20 damage!\n", enemy->name);
            } else {
                printf("%s missed their attack!\n", enemy->name);
            }
            enemy->ammo -= 10;
        } else {
            printf("%s is out of ammo! Reloading...\n", enemy->name);
            enemy->ammo += 30;
        }

    } else if (enemy->behavior == DEFENSIVE) {
        printf("\nEnemy AI (%s): Defensive Mode\n", enemy->name);

        // Defensive behavior: takes cover or retreats when health is low
        if (enemy->hp < 30 && enemy->can_call_reinforcements) {
            printf("%s calls for reinforcements!\n", enemy->name);
            play_sound_effect("reinforcements.wav");
            printf("Reinforcements are arriving!\n");
            enemy->can_call_reinforcements = 0;
        } else if (enemy->hp < 40) {
            printf("%s is taking cover.\n", enemy->name);
            enemy->agility += 20;  // Temporarily boost agility
        } else {
            printf("%s cautiously attacks.\n");
            if (enemy->ammo > 0) {
                if (rand() % 100 < enemy->precision) {
                    play_sound_effect("attack.wav");
                    player->hp -= 10;
                    printf("%s hits you for 10 damage!\n", enemy->name);
                } else {
                    printf("%s missed their cautious attack!\n", enemy->name);
                }
                enemy->ammo -= 5;
            } else {
                printf("%s is reloading cautiously...\n", enemy->name);
                enemy->ammo += 15;
            }
        }

    } else if (enemy->behavior == BALANCED) {
        printf("\nEnemy AI (%s): Balanced Mode\n", enemy->name);

        // Balanced behavior: switches between attack and defense
        if (enemy->hp > 50) {
            printf("%s attacks you!\n", enemy->name);
            if (enemy->ammo > 0) {
                if (rand() % 100 < enemy->precision) {
                    play_sound_effect("attack.wav");
                    player->hp -= 15;
                    printf("%s hits you for 15 damage!\n", enemy->name);
                } else {
                    printf("%s missed their attack!\n", enemy->name);
                }
                enemy->ammo -= 10;
            } else {
                printf("%s is reloading...\n", enemy->name);
                enemy->ammo += 20;
            }
        } else {
            printf("%s takes a defensive stance.\n");
            enemy->agility += 10;  // Boost agility when defending
        }
    }
}

// Function to simulate combat between player and AI
void simulate_combat(Character *player, Enemy *enemy) {
    int player_action;
    int turn = 1;

    // Play combat ambient sound
    play_ambient_sound("combat_ambient.wav");

    while (player->hp > 0 && enemy->hp > 0 && turn <= 10) {
        printf("\n--- Turn %d ---\n", turn);
        printf("Your HP: %d | Enemy HP (%s): %d\n", player->hp, enemy->name, enemy->hp);
        printf("Your Ammo: %d | Enemy Ammo: %d\n", player->ammo, enemy->ammo);

        // Player's action
        printf("Choose your action:\n");
        printf("1. Attack\n");
        printf("2. Take Cover (Increase Agility)\n");
        printf("3. Reload\n");
        scanf("%d", &player_action);

        // Handle player actions
        if (player_action == 1 && player->ammo > 0) {
            printf("You attack %s!\n", enemy->name);
            if (rand() % 100 < player->precision) {
                play_sound_effect("player_attack.wav");
                enemy->hp -= 20;
                printf("You hit %s for 20 damage!\n", enemy->name);
            } else {
                printf("You missed!\n");
            }
            player->ammo -= 10;
        } else if (player_action == 2) {
            printf("You take cover, increasing your agility for this turn.\n");
            player->agility += 20;
        } else if (player_action == 3) {
            printf("You reload your weapon.\n");
            player->ammo += 30;
        } else {
            printf("Invalid action.\n");
        }

        // Enemy makes a decision based on its AI behavior
        if (enemy->hp > 0) {
            enemy_decision(enemy, player);
        }

        // Reset agility boosts after cover
        player->agility -= 20;
        enemy->agility -= 20;

        turn++;
    }

    // Determine outcome
    if (player->hp <= 0) {
        printf("You have been defeated by %s.\n", enemy->name);
        play_sound_effect("defeat.wav");
    } else if (enemy->hp <= 0) {
        printf("You have defeated %s!\n", enemy->name);
        play_sound_effect("victory.wav");
    }
}

/*
AI Decision-Making:
Aggressive AI will attack frequently, even if it runs low on ammo.
Defensive AI will retreat or take cover if low on health and can also call for reinforcements.
Balanced AI adjusts its behavior based on health, switching between attack and defense.

Ambient Sounds and Sound Effects:
SDL2 is used to play ambient combat sounds and sound effects for attacks, reloading, reinforcements, victory, or defeat.
play_ambient_sound plays looping background music, while play_sound_effect plays short sounds like attacks or victory cues.

Player Combat Choices:
The player can attack, take cover (boosting agility), or reload their weapon.
Combat progresses until either the player or the enemy is defeated.
*/

/*
Mission System, Diplomacy, and Game Loop
Mission system that provides different types of missions (combat, diplomatic, supply).
Diplomacy system where players can interact with factions.
Game loop that progresses over time, allowing the player to choose missions, manage resources, and handle combat.
Turn-based progression with dynamic events that influence the town’s status and player decisions.

Mission System Implementation
3 mission types:
- Combat Mission: Fight enemies (bandits, outlaws).
- Diplomatic Mission: Improve relations with factions.
- Supply Mission: Deliver resources to other towns.

Mission Structure:
Each mission has its description, associated faction, difficulty, and rewards.
*/



// Function to create a mission
void create_mission(Mission *m, Faction *f, char *desc, MissionType type, int difficulty, int reward_gold, int reward_reputation, int penalty_morale) {
    strcpy(m->description, desc);
    m->faction = f;
    m->type = type;
    m->difficulty = difficulty;
    m->reward_gold = reward_gold;
    m->reward_reputation = reward_reputation;
    m->penalty_morale = penalty_morale;
}

// Function to offer a mission based on player choices
void offer_mission(Faction *f, Character *player, Town *town) {
    Mission m;
    int mission_choice;

    printf("\nMission Options from %s:\n", f->name);
    printf("1. Combat Mission (Fight bandits)\n");
    printf("2. Diplomatic Mission (Improve faction relations)\n");
    printf("3. Supply Mission (Deliver resources)\n");
    printf("Enter your mission choice: ");
    scanf("%d", &mission_choice);

    switch (mission_choice) {
        case 1:
            create_mission(&m, f, "Fight bandits attacking the mine", COMBAT_MISSION, 2, 200, 10, 5);
            attempt_mission(&m, player, town);
            break;
        case 2:
            create_mission(&m, f, "Negotiate a truce with the faction", DIPLOMATIC_MISSION, 1, 150, 20, 0);
            attempt_mission(&m, player, town);
            break;
        case 3:
            create_mission(&m, f, "Deliver supplies to the neighboring town", SUPPLY_MISSION, 1, 100, 5, 3);
            attempt_mission(&m, player, town);
            break;
        default:
            printf("Invalid choice.\n");
            return;
    }
}
/*
Attempting Missions
Based on the player's stats, the mission can succeed or fail, affecting the town's resources or faction relations.
*/

// Function to attempt a mission based on player stats
void attempt_mission(Mission *m, Character *player, Town *town) {
    printf("\nMission: %s\n", m->description);
    printf("Offered by: %s | Difficulty: %d\n", m->faction->name, m->difficulty);

    // Success chance based on player's stats and mission difficulty
    int success_chance = (player->agility * 10) - (m->difficulty * 20);
    int roll = rand() % 100;

    // If success
    if (roll < success_chance) {
        printf("Mission success! You earned %d gold and %d reputation with %s.\n", m->reward_gold, m->reward_reputation, m->faction->name);
        town->gold += m->reward_gold;
        m->faction->reputation += m->reward_reputation;
    } else {
        printf("Mission failed. Morale decreases by %d.\n", m->penalty_morale);
        town->morale -= m->penalty_morale;
    }
}
/*
Diplomacy System Integration
Players can interact with factions to negotiate alliances, bribe them for better relations, or threaten them.
*/

// Function for player interaction with a faction
void interact_with_faction(Faction *faction, Town *town) {
    int choice;
    printf("\nDiplomacy with %s:\n", faction->name);
    printf("1. Negotiate (+10 reputation)\n");
    printf("2. Offer bribe (Costs 100 Gold, +20 reputation)\n");
    printf("3. Threaten (-15 reputation)\n");
    printf("Enter your action: ");
    scanf("%d", &choice);

    switch (choice) {
        case 1:
            printf("You negotiate with %s. Relations improve.\n", faction->name);
            faction->reputation += 10;
            break;
        case 2:
            if (town->gold >= 100) {
                printf("You offer a bribe to %s. Relations improve significantly.\n", faction->name);
                town->gold -= 100;
                faction->reputation += 20;
            } else {
                printf("Not enough gold to bribe.\n");
            }
            break;
        case 3:
            printf("You threaten %s. Relations worsen.\n", faction->name);
            faction->reputation -= 15;
            break;
        default:
            printf("Invalid choice.\n");
    }

    // Update faction status based on reputation
    update_faction_status(faction);
}

// Function to update faction status based on reputation
void update_faction_status(Faction *faction) {
    if (faction->reputation >= 70) {
        faction->status = ALLIED;
    } else if (faction->reputation >= 30) {
        faction->status = NEUTRAL;
    } else {
        faction->status = HOSTILE;
    }
}
/*
Turn-based game loop that allows the player to choose actions like missions, 
managing the town, or diplomacy each turn. The game will progress dynamically,
with random events that affect the town and player decisions.
*/

// Function to run the main game loop
void game_loop(Character *player, Town *town, Faction *factions, int num_factions) {
    int turn = 1;
    int action;
    srand(time(NULL));  // Initialize random seed

    while (turn <= 20) {  // Example: 20 turns in the game
        printf("\n--- Turn %d ---\n", turn);
        display_character_stats(player);
        display_town_stats(town);

        // Player's action choice
        printf("Choose your action:\n");
        printf("1. Perform a mission\n");
        printf("2. Manage town resources\n");
        printf("3. Diplomacy with factions\n");
        printf("4. End turn\n");
        scanf("%d", &action);

        switch (action) {
            case 1: {
                // Offer a mission from a random faction
                int faction_index = rand() % num_factions;
                offer_mission(&factions[faction_index], player, town);
                break;
            }
            case 2:
                printf("Managing town resources (in future versions).\n");
                break;
            case 3: {
                // Interact with a random faction
                int faction_index = rand() % num_factions;
                interact_with_faction(&factions[faction_index], town);
                break;
            }
            case 4:
                printf("Ending turn...\n");
                break;
            default:
                printf("Invalid action. Try again.\n");
                continue;
        }

        // Dynamic events occur randomly each turn
        if (turn % 3 == 0) {  // Example: Every 3 turns, trigger a random event
            int event_chance = rand() % 2;
            if (event_chance == 0) {
                printf("\nRandom Event: Bandits attack the town!\n");
                Enemy bandit;
                init_enemy(&bandit, "Bandit Leader", 8, 7, 6, 9, 50, 30, AGGRESSIVE);
                simulate_combat(player, &bandit);
            } else {
                printf("\nRandom Event: Traders arrive, boosting town supplies.\n");
                town->supplies += 50;
            }
        }

        // Check win/lose conditions (e.g., morale or gold levels)
        if (town->morale <= 0) {
            printf("Game Over: The town's morale has fallen too low. You lose.\n");
            break;
        }

        turn++;
    }

    printf("The game has ended after %d turns.\n", turn);
}

// Function to create a mission
void create_mission(Mission *m, Faction *f, char *desc, MissionType type, int difficulty, int reward_gold, int reward_reputation, int penalty_morale) {
    strcpy(m->description, desc);
    m->faction = f;
    m->type = type;
    m->difficulty = difficulty;
    m->reward_gold = reward_gold;
    m->reward_reputation = reward_reputation;
    m->penalty_morale = penalty_morale;
}

// Function to offer a mission based on player choices
void offer_mission(Faction *f, Character *player, Town *town) {
    Mission m;
    int mission_choice;

    printf("\nMission Options from %s:\n", f->name);
    printf("1. Combat Mission (Fight bandits)\n");
    printf("2. Diplomatic Mission (Improve faction relations)\n");
    printf("3. Supply Mission (Deliver resources)\n");
    printf("Enter your mission choice: ");
    scanf("%d", &mission_choice);

    switch (mission_choice) {
        case 1:
            create_mission(&m, f, "Fight bandits attacking the mine", COMBAT_MISSION, 2, 200, 10, 5);
            attempt_mission(&m, player, town);
            break;
        case 2:
            create_mission(&m, f, "Negotiate a truce with the faction", DIPLOMATIC_MISSION, 1, 150, 20, 0);
            attempt_mission(&m, player, town);
            break;
        case 3:
            create_mission(&m, f, "Deliver supplies to the neighboring town", SUPPLY_MISSION, 1, 100, 5, 3);
            attempt_mission(&m, player, town);
            break;
        default:
            printf("Invalid choice.\n");
            return;
    }
}

/*
Attempting Missions
Based on the player's stats, the mission can succeed or fail, affecting the town's resources or faction relations.
*/

// Function to attempt a mission based on player stats
void attempt_mission(Mission *m, Character *player, Town *town) {
    printf("\nMission: %s\n", m->description);
    printf("Offered by: %s | Difficulty: %d\n", m->faction->name, m->difficulty);

    // Success chance based on player's stats and mission difficulty
    int success_chance = (player->agility * 10) - (m->difficulty * 20);
    int roll = rand() % 100;

    // If success
    if (roll < success_chance) {
        printf("Mission success! You earned %d gold and %d reputation with %s.\n", m->reward_gold, m->reward_reputation, m->faction->name);
        town->gold += m->reward_gold;
        m->faction->reputation += m->reward_reputation;
    } else {
        printf("Mission failed. Morale decreases by %d.\n", m->penalty_morale);
        town->morale -= m->penalty_morale;
    }
}

/*
Diplomacy System Integration
Players can interact with factions to negotiate alliances, bribe them for better relations, or threaten them.
*/

// Function for player interaction with a faction
void interact_with_faction(Faction *faction, Town *town) {
    int choice;
    printf("\nDiplomacy with %s:\n", faction->name);
    printf("1. Negotiate (+10 reputation)\n");
    printf("2. Offer bribe (Costs 100 Gold, +20 reputation)\n");
    printf("3. Threaten (-15 reputation)\n");
    printf("Enter your action: ");
    scanf("%d", &choice);

    switch (choice) {
        case 1:
            printf("You negotiate with %s. Relations improve.\n", faction->name);
            faction->reputation += 10;
            break;
        case 2:
            if (town->gold >= 100) {
                printf("You offer a bribe to %s. Relations improve significantly.\n", faction->name);
                town->gold -= 100;
                faction->reputation += 20;
            } else {
                printf("Not enough gold to bribe.\n");
            }
            break;
        case 3:
            printf("You threaten %s. Relations worsen.\n", faction->name);
            faction->reputation -= 15;
            break;
        default:
            printf("Invalid choice.\n");
    }

    // Update faction status based on reputation
    update_faction_status(faction);
}

// Function to update faction status based on reputation
void update_faction_status(Faction *faction) {
    if (faction->reputation >= 70) {
        faction->status = ALLIED;
    } else if (faction->reputation >= 30) {
        faction->status = NEUTRAL;
    } else {
        faction->status = HOSTILE;
    }
}
/*
Turn-based game loop with Dynamic Events that allows the player 
to choose actions like missions, managing the town, or diplomacy each turn. 
The game will progress dynamically, with random events that affect the town 
and player decisions.
*/

// Function to run the main game loop
void game_loop(Character *player, Town *town, Faction *factions, int num_factions) {
    int turn = 1;
    int action;
    srand(time(NULL));  // Initialize random seed

    while (turn <= 20) {  // Example: 20 turns in the game
        printf("\n--- Turn %d ---\n", turn);
        display_character_stats(player);
        display_town_stats(town);

        // Player's action choice
        printf("Choose your action:\n");
        printf("1. Perform a mission\n");
        printf("2. Manage town resources\n");
        printf("3. Diplomacy with factions\n");
        printf("4. End turn\n");
        scanf("%d", &action);

        switch (action) {
            case 1: {
                // Offer a mission from a random faction
                int faction_index = rand() % num_factions;
                offer_mission(&factions[faction_index], player, town);
                break;
            }
            case 2:
                printf("Managing town resources (in future versions).\n");
                break;
            case 3: {
                // Interact with a random faction
                int faction_index = rand() % num_factions;
                interact_with_faction(&factions[faction_index], town);
                break;
            }
            case 4:
                printf("Ending turn...\n");
                break;
            default:
                printf("Invalid action. Try again.\n");
                continue;
        }

        // Dynamic events occur randomly each turn
        if (turn % 3 == 0) {  // Example: Every 3 turns, trigger a random event
            int event_chance = rand() % 2;
            if (event_chance == 0) {
                printf("\nRandom Event: Bandits attack the town!\n");
                Enemy bandit;
                init_enemy(&bandit, "Bandit Leader", 8, 7, 6, 9, 50, 30, AGGRESSIVE);
                simulate_combat(player, &bandit);
            } else {
                printf("\nRandom Event: Traders arrive, boosting town supplies.\n");
                town->supplies += 50;
            }
        }

        // Check win/lose conditions (e.g., morale or gold levels)
        if (town->morale <= 0) {
            printf("Game Over: The town's morale has fallen too low. You lose.\n");
            break;
        }

        turn++;
    }

    printf("The game has ended after %d turns.\n", turn);
}

Explanation:

    Mission System:
        Players can choose between Combat, Diplomatic, or Supply Missions. Success depends on player stats and mission difficulty.

    Diplomacy:
        Players can interact with factions to improve or damage relations. Reputation affects faction status (Allied, Neutral, Hostile).

    Turn-Based Game Loop:
        The game progresses over 20 turns. Each turn, the player can take actions (missions, diplomacy, town management).
        Random events (bandit attacks, traders arriving) occur dynamically, impacting the game.
        The game ends if town morale falls to 0 or after 20 turns.
*/
/*
Expanding the Mission System and Diplomacy
Adding more mission types (rescue missions, escort missions and Recon Mission).
Expanding the diplomatic system to include treaties and alliances.
Increasing the complexity of random events to make the game more unpredictable.
These missions will have different objectives and outcomes, allowing the player to choose how they want to approach each turn.

New Mission Types:
Rescue :
Rescue a captured townsperson or ally.
Success increases town morale and gains faction reputation.
Escort :
Escort a VIP or caravan safely to another location.
Success boosts town supplies and reputation.
Recon Mission:
Gather intelligence about enemy movements.
Success increases faction reputation, and defense preparedness of the town.
*/

// Function to create a rescue mission
void create_rescue_mission(Mission *m, Faction *f, int difficulty) {
    create_mission(m, f, "Rescue a captured townsperson", COMBAT_MISSION, difficulty, 300, 15, 10);
}

// Function to create an escort mission
void create_escort_mission(Mission *m, Faction *f, int difficulty) {
    create_mission(m, f, "Escort a VIP caravan safely", SUPPLY_MISSION, difficulty, 250, 10, 5);
}

// Function to create a recon mission
void create_recon_mission(Mission *m, Faction *f, int difficulty) {
    create_mission(m, f, "Gather intelligence about enemy movements", RECON_MISSION, difficulty, 200, 20, 0);
}

// Function to offer new types of missions
void offer_advanced_mission(Faction *f, Character *player, Town *town) {
    Mission m;
    int mission_choice;

    printf("\nAdvanced Mission Options from %s:\n", f->name);
    printf("1. Combat Mission (Fight bandits)\n");
    printf("2. Diplomatic Mission (Improve faction relations)\n");
    printf("3. Supply Mission (Deliver resources)\n");
    printf("4. Rescue Mission (Save a captured townsperson)\n");
    printf("5. Escort Mission (Protect a caravan)\n");
    printf("6. Recon Mission (Gather intelligence)\n");
    printf("Enter your mission choice: ");
    scanf("%d", &mission_choice);

    switch (mission_choice) {
        case 1:
            create_mission(&m, f, "Fight bandits attacking the mine", COMBAT_MISSION, 2, 200, 10, 5);
            attempt_mission(&m, player, town);
            break;
        case 2:
            create_mission(&m, f, "Negotiate a truce with the faction", DIPLOMATIC_MISSION, 1, 150, 20, 0);
            attempt_mission(&m, player, town);
            break;
        case 3:
            create_mission(&m, f, "Deliver supplies to the neighboring town", SUPPLY_MISSION, 1, 100, 5, 3);
            attempt_mission(&m, player, town);
            break;
        case 4:
            create_rescue_mission(&m, f, 2);
            attempt_mission(&m, player, town);
            break;
        case 5:
            create_escort_mission(&m, f, 1);
            attempt_mission(&m, player, town);
            break;
        case 6:
            create_recon_mission(&m, f, 1);
            attempt_mission(&m, player, town);
            break;
        default:
            printf("Invalid choice.\n");
            return;
    }
}
/*
Mission Rewards and Consequences:
Each mission type should offer different rewards and consequences. 
Example:
Rescue Missions: Increase morale and faction reputation.
Escort Missions: Boost supplies and reputation.
Recon Missions: Improve the defensive readiness of the town.

This allows the player to choose missions based on what the town needs at that moment.
*/

/*
Diplomacy Enhancements:
Diplomacy system allows players to form alliances with factions. 
Players can also improve relations by successfully completing missions or bribing factions. 
A negative relationship can result in war or hostile actions from factions.

Diplomatic Actions:
Alliance: If a faction’s reputation is high enough, players can form an alliance, which will offer military or resource support.
Bribe: Players can bribe factions to improve relations.
Threaten: Risk lowering the faction's reputation but possibly force concessions.
*/

// Diplomatic function to form alliances or go to war
void form_alliance_or_declare_war(Faction *faction, Town *town) {
    if (faction->reputation >= 70) {
        printf("%s has formed an alliance with you!\n", faction->name);
        town->supplies += 100;  // Alliance provides support
        town->morale += 10;     // Boost morale
        printf("Supplies increased by 100, morale increased by 10.\n");
    } else if (faction->reputation <= 20) {
        printf("%s has declared war on your town!\n", faction->name);
        town->morale -= 20;     // Morale drops
        printf("Morale decreased by 20.\n");
        // Trigger a combat event with this faction later
    } else {
        printf("%s remains neutral.\n", faction->name);
    }
}

// Function to manage advanced diplomacy
void advanced_diplomacy(Faction *faction, Town *town) {
    int action;
    printf("\nDiplomacy with %s:\n", faction->name);
    printf("1. Form an Alliance (Requires 70+ reputation)\n");
    printf("2. Bribe (Costs 100 gold, +20 reputation)\n");
    printf("3. Threaten (-15 reputation)\n");
    printf("4. Declare War (Lowers morale)\n");
    scanf("%d", &action);

    switch (action) {
        case 1:
            if (faction->reputation >= 70) {
                form_alliance_or_declare_war(faction, town);
            } else {
                printf("You need at least 70 reputation to form an alliance.\n");
            }
            break;
        case 2:
            if (town->gold >= 100) {
                printf("You bribed %s. Reputation increased.\n", faction->name);
                town->gold -= 100;
                faction->reputation += 20;
            } else {
                printf("Not enough gold to bribe.\n");
            }
            break;
        case 3:
            printf("You threaten %s. Reputation decreased.\n", faction->name);
            faction->reputation -= 15;
            break;
        case 4:
            form_alliance_or_declare_war(faction, town);
            break;
        default:
            printf("Invalid choice.\n");
    }

    // Update faction status
    update_faction_status(faction);
}
/*
Random Event System Enhancements:
Random events to include both positive and negative events that can happen randomly each turn.
Examples :
Bandit Raids: Reduces supplies and morale unless defended.
Gold Discovery: Increases gold.
Disease Outbreak: Lowers town morale and supplies.
*/

// Function to generate a random event
void generate_random_event(Town *town) {
    int event = rand() % 4;  // Choose one of 4 random events

    switch (event) {
        case 0:
            printf("\nRandom Event: Bandits raid the town!\n");
            town->supplies -= 50;
            town->morale -= 10;
            printf("Supplies decreased by 50, morale decreased by 10.\n");
            break;
        case 1:
            printf("\nRandom Event: A gold discovery boosts the town's wealth!\n");
            town->gold += 200;
            printf("Gold increased by 200.\n");
            break;
        case 2:
            printf("\nRandom Event: Disease outbreak lowers town morale.\n");
            town->morale -= 20;
            town->supplies -= 30;
            printf("Morale decreased by 20, supplies decreased by 30.\n");
            break;
        case 3:
            printf("\nRandom Event: Merchants arrive, boosting supplies.\n");
            town->supplies += 100;
            printf("Supplies increased by 100.\n");
            break;
        default:
            printf("Nothing happened this turn.\n");
    }
}

// Function to create a rescue mission
void create_rescue_mission(Mission *m, Faction *f, int difficulty) {
    create_mission(m, f, "Rescue a captured townsperson", COMBAT_MISSION, difficulty, 300, 15, 10);
}

// Function to create an escort mission
void create_escort_mission(Mission *m, Faction *f, int difficulty) {
    create_mission(m, f, "Escort a VIP caravan safely", SUPPLY_MISSION, difficulty, 250, 10, 5);
}

// Function to create a recon mission
void create_recon_mission(Mission *m, Faction *f, int difficulty) {
    create_mission(m, f, "Gather intelligence about enemy movements", RECON_MISSION, difficulty, 200, 20, 0);
}

// Function to offer new types of missions
void offer_advanced_mission(Faction *f, Character *player, Town *town) {
    Mission m;
    int mission_choice;

    printf("\nAdvanced Mission Options from %s:\n", f->name);
    printf("1. Combat Mission (Fight bandits)\n");
    printf("2. Diplomatic Mission (Improve faction relations)\n");
    printf("3. Supply Mission (Deliver resources)\n");
    printf("4. Rescue Mission (Save a captured townsperson)\n");
    printf("5. Escort Mission (Protect a caravan)\n");
    printf("6. Recon Mission (Gather intelligence)\n");
    printf("Enter your mission choice: ");
    scanf("%d", &mission_choice);

    switch (mission_choice) {
        case 1:
            create_mission(&m, f, "Fight bandits attacking the mine", COMBAT_MISSION, 2, 200, 10, 5);
            attempt_mission(&m, player, town);
            break;
        case 2:
            create_mission(&m, f, "Negotiate a truce with the faction", DIPLOMATIC_MISSION, 1, 150, 20, 0);
            attempt_mission(&m, player, town);
            break;
        case 3:
            create_mission(&m, f, "Deliver supplies to the neighboring town", SUPPLY_MISSION, 1, 100, 5, 3);
            attempt_mission(&m, player, town);
            break;
        case 4:
            create_rescue_mission(&m, f, 2);
            attempt_mission(&m, player, town);
            break;
        case 5:
            create_escort_mission(&m, f, 1);
            attempt_mission(&m, player, town);
            break;
        case 6:
            create_recon_mission(&m, f, 1);
            attempt_mission(&m, player, town);
            break;
        default:
            printf("Invalid choice.\n");
            return;
    }
}

// Diplomatic function to form alliances or go to war
void form_alliance_or_declare_war(Faction *faction, Town *town) {
    if (faction->reputation >= 70) {
        printf("%s has formed an alliance with you!\n", faction->name);
        town->supplies += 100;  // Alliance provides support
        town->morale += 10;     // Boost morale
        printf("Supplies increased by 100, morale increased by 10.\n");
    } else if (faction->reputation <= 20) {
        printf("%s has declared war on your town!\n", faction->name);
        town->morale -= 20;     // Morale drops
        printf("Morale decreased by 20.\n");
        // Trigger a combat event with this faction later
    } else {
        printf("%s remains neutral.\n", faction->name);
    }
}

// Function to manage advanced diplomacy
void advanced_diplomacy(Faction *faction, Town *town) {
    int action;
    printf("\nDiplomacy with %s:\n", faction->name);
    printf("1. Form an Alliance (Requires 70+ reputation)\n");
    printf("2. Bribe (Costs 100 gold, +20 reputation)\n");
    printf("3. Threaten (-15 reputation)\n");
    printf("4. Declare War (Lowers morale)\n");
    scanf("%d", &action);

    switch (action) {
        case 1:
            if (faction->reputation >= 70) {
                form_alliance_or_declare_war(faction, town);
            } else {
                printf("You need at least 70 reputation to form an alliance.\n");
            }
            break;
        case 2:
            if (town->gold >= 100) {
                printf("You bribed %s. Reputation increased.\n", faction->name);
                town->gold -= 100;
                faction->reputation += 20;
            } else {
                printf("Not enough gold to bribe.\n");
            }
            break;
        case 3:
            printf("You threaten %s. Reputation decreased.\n", faction->name);
            faction->reputation -= 15;
            break;
        case 4:
            form_alliance_or_declare_war(faction, town);
            break;
        default:
            printf("Invalid choice.\n");
    }

    // Update faction status
    update_faction_status(faction);
}

/*
Random Event System Enhancements:
Include both positive and negative events that can happen randomly each turn.
Examples :
Bandit Raids: Reduces supplies and morale unless defended.
Gold Discovery: Increases gold.
Disease Outbreak: Lowers town morale and supplies.
*/

// Function to generate a random event
void generate_random_event(Town *town) {
    int event = rand() % 4;  // Choose one of 4 random events

    switch (event) {
        case 0:
            printf("\nRandom Event: Bandits raid the town!\n");
            town->supplies -= 50;
            town->morale -= 10;
            printf("Supplies decreased by 50, morale decreased by 10.\n");
            break;
        case 1:
            printf("\nRandom Event: A gold discovery boosts the town's wealth!\n");
            town->gold += 200;
            printf("Gold increased by 200.\n");
            break;
        case 2:
            printf("\nRandom Event: Disease outbreak lowers town morale.\n");
            town->morale -= 20;
            town->supplies -= 30;
            printf("Morale decreased by 20, supplies decreased by 30.\n");
            break;
        case 3:
            printf("\nRandom Event: Merchants arrive, boosting supplies.\n");
            town->supplies += 100;
            printf("Supplies increased by 100.\n");
            break;
        default:
            printf("Nothing happened this turn.\n");
    }
}
/*
Game Loop with Expanded Missions and Diplomacy:
Finally, let's integrate everything into the main game loop and allow the player to choose missions, engage in diplomacy, and experience random events over multiple turns.
*/
// Main game loop
void game_loop(Character *player, Town *town, Faction *factions, int num_factions) {
    int turn = 1;
    int action;
    srand(time(NULL));

    while (turn <= 20) {
        printf("\n--- Turn %d ---\n", turn);
        display_character_stats(player);
        display_town_stats(town);

        // Player's action choice
        printf("Choose your action:\n");
        printf("1. Perform a mission\n");
        printf("2. Manage town resources\n");
        printf("3. Diplomacy with factions\n");
        printf("4. End turn\n");
        scanf("%d", &action);

        switch (action) {
            case 1:
                // Offer a mission from a random faction
                offer_advanced_mission(&factions[rand() % num_factions], player, town);
                break;
            case 2:
                printf("Managing town resources (in future versions).\n");
                break;
            case 3:
                advanced_diplomacy(&factions[rand() % num_factions], town);
                break;
            case 4:
                printf("Ending turn...\n");
                break;
            default:
                printf("Invalid action. Try again.\n");
                continue;
        }

        // Random events every 2 turns
        if (turn % 2 == 0) {
            generate_random_event(town);
        }

        // Check win/lose conditions (e.g., morale or gold levels)
        if (town->morale <= 0) {
            printf("Game Over: Town morale dropped too low.\n");
            break;
        }

        turn++;
    }

    printf("Game ended after %d turns.\n", turn);
}
/*
Expanded Mission System:
    New mission types such as Rescue Missions, Escort Missions, and Recon Missions provide variety and depth to gameplay.

Diplomacy Enhancements:
    Players can now form alliances, bribe, or declare war on factions. Faction relations have more significant consequences for the town.

Random Events:
    The game now has a random event system that affects the town's supplies, morale, and gold. Events occur every few turns and can be positive or negative.

Dynamic Game Loop:
    The player’s choices drive the game, and every turn offers a new challenge through missions, diplomacy, or random events.
*/
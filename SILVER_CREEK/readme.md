// Silver Creek   !!! WIP !!! UNFINISHED
// v 0.9 - Hylst (1996 - 2024 déterrage d'anciens sources, restructuration, documentation, mise à jour, ... ) 
// Etat : Gros bordel... en  cours de développement - 
// 2 versions du jeux de plus de 20 années de décalage en cours de '''merge'''
// Est ce que ça en vaut toujours vraiment le temps ?

// Simmulation de prise de contôle d'une ville du FAR WEST
// avec gestion de combats, réputations, négociations, évènements, ...

TO DO :
Much to merge / to do / to implement / to correct...
2 versions of the unfinished game to join
French -> English
Testing...
...

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
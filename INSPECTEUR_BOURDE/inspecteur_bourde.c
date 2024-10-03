// 'Inspecteur Bourde et la Mystérieuse Disparition du Chien Saucisse'
// Un jeu d'enquête policière loufoque textuel.
// Hylst  1997 ->  2024 (maj, cross platform, corrections, bonnes pratiques programmation)
// v1.2

#include <stdio.h>
#include <string.h>
#include <locale.h>
#include <stdlib.h>
#include <time.h>

#ifdef _WIN32
    #include <windows.h>
    #define PLAY_SOUND(command) system(command)
    #define SLEEP(seconds) Sleep((seconds) * 1000)
#else
    #include <unistd.h>
    #define PLAY_SOUND(command) system(command)
    #define SLEEP(seconds) sleep(seconds)
#endif

// Liste des suspects et coupable
char* suspects[5] = {"Jean-Patoche", "Mme Boulanger", "Le Facteur", "Le Plombier", "L'Enfant du Quartier"};
char* coupable;

// Liste des objets trouvés pendant l'enquête
char* objets[3] = {"une corde", "un couteau", "un os de chien"};

// Fonction d'introduction avec son
void introduction() {
    printf("Bienvenue dans 'Inspecteur Bourde et la Mystérieuse Disparition du Chien Saucisse'.\n");
    printf("Vous incarnez l'inspecteur Bourde, un enquêteur célèbre, mais un peu... disons... maladroit.\n");
    printf("Votre mission : résoudre l'énigme de la disparition d'un chien appelé 'Saucisse'.\n");
    printf("Analysez les indices, interrogez les suspects et trouvez le coupable !\n\n");

    // Jouer un son d'introduction (remplacez 'intro.wav' par un fichier audio réel)
    PLAY_SOUND("start intro.wav");
    SLEEP(1);
}

// Sélection aléatoire du coupable parmi les suspects
void choisir_coupable() {
    srand(time(NULL));  // Initialisation du générateur aléatoire
    coupable = suspects[rand() % 5];  // Sélection aléatoire d'un coupable
}

// Fonction pour afficher les indices, avec objets trouvés
void afficher_indices() {
    srand(time(NULL));
    int objet_trouve = rand() % 3;  // Choix aléatoire d'un objet trouvé

    printf("\n--- Indices disponibles ---\n");
    printf("1. Une empreinte de patte suspecte a été trouvée près du lieu du crime.\n");
    printf("2. Un morceau de saucisson traîne mystérieusement dans le jardin.\n");
    printf("3. Le voisin, un certain 'Jean-Patoche', a été vu en train de parler tout seul...\n");
    printf("4. Un étrange hurlement a été entendu la nuit précédente, mais personne ne sait d'où il vient.\n");
    printf("5. Une clé à molette a été retrouvée près de la scène...\n");
    printf("6. Un enfant a été aperçu en train de fuir en courant juste après le crime.\n");
    printf("7. Un objet a été trouvé près de la scène : %s.\n", objets[objet_trouve]);
    printf("---------------------------\n\n");

    // Jouer un son d'affichage d'indices (remplacez 'clue.wav' par un fichier audio réel)
    PLAY_SOUND("start clue.wav");
    SLEEP(1);
}

// Fonction d'interrogation aléatoire avec plusieurs réponses pour chaque suspect
void interroger_suspect(const char* nom_suspect) {
    int question;
    printf("\n--- Interrogatoire de %s ---\n", nom_suspect);
    printf("1. Où étiez-vous la nuit du crime ?\n");
    printf("2. Pourquoi parliez-vous tout seul ?\n");
    printf("3. Connaissiez-vous 'Saucisse' ?\n");
    printf("4. Que savez-vous de l'objet trouvé ?\n");  // Nouvelle question sur l'objet trouvé
    printf("Choisissez une question (1-4) : ");
    scanf("%d", &question);

    srand(time(NULL));
    int reponse_aleatoire = rand() % 4;  // Sélectionne une des quatre réponses possibles pour plus de diversité et d'humour

    switch (question) {
        case 1:
            switch (reponse_aleatoire) {
                case 0:
                    printf("%s : 'J'étais chez moi, en train de cuisiner... de la soupe à la saucisse.'\n", nom_suspect);
                    break;
                case 1:
                    printf("%s : 'Je me baladais... avec un saucisson dans chaque main, pour rester équilibré.'\n", nom_suspect);
                    break;
                case 2:
                    printf("%s : 'J'ai vu une licorne passer, donc j'ai décidé de la suivre. Rien à voir avec le crime.'\n", nom_suspect);
                    break;
                case 3:
                    printf("%s : 'Oh, je faisais du jogging... en pyjama. C'est plus confortable.'\n", nom_suspect);
                    break;
            }
            break;
        case 2:
            switch (reponse_aleatoire) {
                case 0:
                    printf("%s : 'Moi ? Je ne parle pas tout seul, je raconte juste des blagues à mon reflet.'\n", nom_suspect);
                    break;
                case 1:
                    printf("%s : 'J'étais en pleine discussion sérieuse avec mon chien imaginaire. Vous voulez le rencontrer ?'\n", nom_suspect);
                    break;
                case 2:
                    printf("%s : 'Je faisais des répétitions pour mon one-man-show ! Mon miroir m'adore.'\n", nom_suspect);
                    break;
                case 3:
                    printf("%s : 'Ce n'était pas moi qui parlais, c'était... le saucisson. Vous ne le croirez jamais.'\n", nom_suspect);
                    break;
            }
            break;
        case 3:
            switch (reponse_aleatoire) {
                case 0:
                    printf("%s : 'Saucisse ? C'était mon meilleur ami... jusqu'à ce qu'il me vole une tranche de saucisson.'\n", nom_suspect);
                    break;
                case 1:
                    printf("%s : 'Je l'ai rencontré une fois. Il m'a regardé comme si j'étais un morceau de viande.'\n", nom_suspect);
                    break;
                case 2:
                    printf("%s : 'Bien sûr, on jouait souvent à cache-cache, mais il était très mauvais.'\n", nom_suspect);
                    break;
                case 3:
                    printf("%s : 'Saucisse ? Ce nom me donne faim... Vous avez un sandwich ?'\n", nom_suspect);
                    break;
            }
            break;
        case 4:
            switch (reponse_aleatoire) {
                case 0:
                    printf("%s : 'Ah, %s ? Ça m'appartient, mais je l'ai prêté à un ami... un peu étrange.'\n", nom_suspect, objets[rand() % 3]);
                    break;
                case 1:
                    printf("%s : 'Cet objet ? Il était là quand je suis arrivé... Peut-être un souvenir de Saucisse ?'\n", nom_suspect);
                    break;
                case 2:
                    printf("%s : 'Oh, %s... c'est un cadeau de ma grand-mère. Très pratique pour attraper des chiens.'\n", nom_suspect, objets[rand() % 3]);
                    break;
                case 3:
                    printf("%s : 'Je ne me souviens pas avoir vu cet objet... mais avec toutes les saucisses autour, qui sait ?'\n", nom_suspect);
                    break;
            }
            break;
        default:
            printf("%s : 'Je ne comprends pas cette question... Vous êtes sûr d'être un vrai inspecteur ?'\n", nom_suspect);
            break;
    }

    // Jouer un son d'interrogatoire (remplacez 'question.wav' par un fichier audio réel)
    PLAY_SOUND("start question.wav");
    SLEEP(1);
}

// Fonction pour interroger les suspects
void interroger_suspects() {
    int choix;
    printf("\n--- Choisissez un suspect à interroger ---\n");
    for (int i = 0; i < 5; i++) {
        printf("%d. %s\n", i + 1, suspects[i]);
    }
    printf("Votre choix (1-5) : ");
    scanf("%d", &choix);

    if (choix >= 1 && choix <= 5) {
        interroger_suspect(suspects[choix - 1]);
    } else {
        printf("Ce suspect n'existe pas.\n");
    }
}

// Fonction pour vérifier la solution proposée par le joueur
int verifier_solution(char* reponse) {
    if (strcmp(reponse, coupable) == 0) {
        return 1; // Bonne réponse
    }
    return 0; // Mauvaise réponse
}

// Fonction pour demander au joueur s'il souhaite rejouer
int rejouer() {
    char choix[10];
    printf("\nVoulez-vous rejouer ? (oui/non) : ");
    scanf("%s", choix);
    return strcmp(choix, "oui") == 0;
}

int main() {
    setlocale(LC_ALL, "en_US.UTF-8"); // Définit la locale à UTF-8
    #ifdef _WIN32
    SetConsoleOutputCP(65001); // Définit le code de page de sortie de la console à UTF-8
    #endif
    char choix[10];
    char reponse[50];
    int jeu_termine = 0;
    int score = 100; // Score initial

    do {
        choisir_coupable();  // Sélection aléatoire du coupable
        introduction();
        score = 100; // Réinitialiser le score pour chaque partie
        jeu_termine = 0;

        while (!jeu_termine) {
            printf("Que souhaitez-vous faire ?\n");
            printf("1. Analyser les indices\n");
            printf("2. Interroger les suspects\n");
            printf("3. Résoudre l'enquête\n");
            printf("Votre choix : ");
            scanf("%s", choix);

            if (strcmp(choix, "1") == 0) {
                afficher_indices();
            } else if (strcmp(choix, "2") == 0) {
                interroger_suspects();
            } else if (strcmp(choix, "3") == 0) {
                printf("Qui est le coupable ? Entrez le nom : ");
                scanf("%s", reponse);
                
                if (verifier_solution(reponse)) {
                    printf("\nFélicitations, Inspecteur Bourde ! Vous avez résolu l'enquête : %s est le coupable !\n", coupable);
                    jeu_termine = 1; // Fin du jeu
                    PLAY_SOUND("start victory.wav");
                } else {
                    printf("\nMauvaise réponse, Inspecteur ! Ce n'est pas la bonne personne.\n");
                    score -= 10; // Réduction du score
                    printf("Votre score actuel est de %d.\n", score);
                    PLAY_SOUND("start wrong.wav");
                }
            } else {
                printf("Choix invalide. Veuillez réessayer.\n");
            }
        }

        printf("Merci d'avoir joué à 'Inspecteur Bourde et la Mystérieuse Disparition du Chien Saucisse'.\n");
        printf("Votre score final est de %d.\n", score);

    } while (rejouer());  // Demander au joueur s'il veut rejouer avec un nouveau coupable

    printf("Au revoir, Inspecteur !\n");
    return 0;
}

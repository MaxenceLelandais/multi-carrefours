#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <semaphore.h>

// Structure représentant un échangeur
typedef struct {
    int carrefour_id;
    sem_t semaphore; // Semaphore pour contrôler l'accès au carrefour
} Echangeur;

// Structure représentant la position
typedef struct {
    int carrefour_id;
    char emplacement[10]; // "haut", "bas", "gauche", "droite", etc.
} Position;

// Structure représentant un véhicule
typedef struct {
    char type[20];
    int vitesse;
    int priorite; // Nouveau champ pour la priorité du véhicule
    Position position;
    Echangeur* itineraire; // Modification du type à Echangeur*
} Vehicule;

// Structure représentant le serveur-contrôleur
typedef struct {
    int etat_trafic[4]; // Un tableau pour stocker l'état du trafic pour chaque carrefour
    sem_t semaphore; // Semaphore pour contrôler l'accès à la mémoire partagée
} ServeurControleur;

// Fonction pour transmettre une requête au serveur-contrôleur
void transmettre_requete(Echangeur* echangeur, Vehicule* vehicule, ServeurControleur* serveur_controleur) {
    // Acquérir le sémaphore du carrefour
    sem_wait(&echangeur->semaphore);

    // Acquérir le sémaphore de la mémoire partagée
    sem_wait(&serveur_controleur->semaphore);

    // Traitement de la requête et envoi au serveur-contrôleur
    // ...

    // Exemple : Mise à jour de l'état du trafic sur le serveur-contrôleur
    serveur_controleur->etat_trafic[echangeur->carrefour_id - 1] += 1; // Supposons que chaque véhicule incrémente le trafic de 1

    // Mise à jour de la position du véhicule
    vehicule->position.carrefour_id = echangeur->carrefour_id;
    strcpy(vehicule->position.emplacement, "haut"); // Supposons que tous les véhicules arrivent en haut par défaut

    // Libérer le sémaphore de la mémoire partagée
    sem_post(&serveur_controleur->semaphore);

    // Libérer le sémaphore du carrefour
    sem_post(&echangeur->semaphore);
}

void* vehicule_thread(void* arg) {
    Vehicule* vehicule = (Vehicule*)arg;

    // Simulation du mouvement du véhicule
    for (int i = 0; i < 5; ++i) {
        transmettre_requete(vehicule->itineraire, vehicule, NULL);
        sleep(1); // Supposons qu'une unité de temps est équivalente à 1 seconde
    }

    pthread_exit(NULL);
}

int main() {
    // Initialisation des composants
    ServeurControleur serveur_controleur;
    sem_init(&serveur_controleur.semaphore, 0, 1); // Initialisation du sémaphore de la mémoire partagée

    Echangeur echangeurs[4];
    for (int i = 0; i < 4; ++i) {
        echangeurs[i].carrefour_id = i + 1;
        sem_init(&echangeurs[i].semaphore, 0, 1); // Initialisation des sémaphores des carrefours avec 1 comme valeur initiale
    }

    // Initialisation de l'état du trafic à zéro
    memset(serveur_controleur.etat_trafic, 0, sizeof(serveur_controleur.etat_trafic));

    Vehicule vehicules[3];
    strcpy(vehicules[0].type, "Voiture");
    vehicules[0].vitesse = 60;
    vehicules[0].itineraire = &echangeurs[0];
    vehicules[0].position.carrefour_id = 0;
    strcpy(vehicules[0].position.emplacement, "inconnu");
    vehicules[0].priorite = 2;

    strcpy(vehicules[1].type, "Ambulance");
    vehicules[1].vitesse = 80;
    vehicules[1].itineraire = &echangeurs[0];
    vehicules[1].position.carrefour_id = 0;
    strcpy(vehicules[1].position.emplacement, "inconnu");
    vehicules[1].priorite = 1;

    strcpy(vehicules[2].type, "Camion");
    vehicules[2].vitesse = 40;
    vehicules[2].itineraire = &echangeurs[0];
    vehicules[2].position.carrefour_id = 0;
    strcpy(vehicules[2].position.emplacement, "inconnu");
    vehicules[2].priorite = 3;

    // Création des threads pour les véhicules
    pthread_t threads[3];
    for (int i = 0; i < 3; ++i) {
        pthread_create(&threads[i], NULL, vehicule_thread, (void*)&vehicules[i]);
    }

    // Attente de la fin des threads
    for (int i = 0; i < 3; ++i) {
        pthread_join(threads[i], NULL);
    }

    // Destruction des sémaphores
    sem_destroy(&serveur_controleur.semaphore);
    for (int i = 0; i < 4; ++i) {
        sem_destroy(&echangeurs[i].semaphore);
    }

    return 0;
}

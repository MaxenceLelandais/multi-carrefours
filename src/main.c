#include <ncurses.h>
#include <sys/ioctl.h>
#include <stdio.h>
#include <unistd.h>

void draw(int width, int height, int x, int y, int modev, int modeh);

void carrefour(int width, int height){
    int tailleCaseX = 1;
    int tailleCaseY = 1;

    draw(6,3, 3, 1, 0, 0);
    draw(12,3, 19, 1, 0, 0);
    draw(6,3, 42, 1, 0, 0);

    draw(6,5, 3, 13, 0, 0);
    draw(12,5, 19, 13, 0, 0);
    draw(6,5, 42, 13, 0, 0);

    draw(6,3, 3, 29, 0, 0);
    draw(12,3, 19, 29, 0, 0);
    draw(6,3, 42, 29, 0, 0);
}

void menu(int x, int y){
    mvprintw(y - 1, x - 5, "1. Générer véhicule.");
    mvprintw(y, x - 5, "2. Tout effacer.");
    mvprintw(y + 1, x - 5, "3. Automatiser.");
    refresh();
}

void generer(int x, int y){
    mvprintw(y - 1, x - 5, "  Type véhicule.");
    mvprintw(y, x - 5, "1. Simple.");
    mvprintw(y + 1, x - 5, "2. Prioritaire.");
    refresh();
}

void select(){
    int choice;
    // Get user input
    while (1) {
        choice = getch();
        // Process the choice
        switch (choice) {
            case '1':
                // Handle Option 1
                generer(70, 17);
                break;
            case '2':
                // Handle Option 2
                mvprintw(2, 0, "You selected Option 2");
                break;
            case '3':
                // Handle Option 3
                mvprintw(4, 0, "You selected Option 3");
                break;
            case 'q':
                // Quit the program if 'q' is pressed
                endwin();
                break;
        }
        refresh(); // Refresh the screen after processing user input
    }
}

bool load(WINDOW *win){
    FILE *file;
    char filename[] = "map.txt";
    char line[255];  // A buffer to store each line

    // Ouvre le fichier en mode lecture
    file = fopen(filename, "r");

    // Vérifie si le fichier est ouvert avec succès
    if (file == NULL) {
        fprintf(stderr, "Impossible d'ouvrir le fichier %s\n", filename);
        return false;
    }

    // Lit et affiche chaque ligne du fichier
    while (fgets(line, sizeof(line), file) != NULL) {
        wprintw(win, "%s", line);
    }
    refresh();

    // Ferme le fichier
    fclose(file);
}

void draw(int width, int height, int x, int y, int modev, int modeh){

    WINDOW *win = newwin(height, width, y,x); // height, width, posty, postx
    box(win, modev, modeh); //remplace les bordures (pas les angles) par des chars, >0 char noir, <0 char blanc, =0 rien
    wrefresh(win);
}

int main() {
    

    initscr(); // Initialise ncurses
    noecho();  // N'affiche pas les caractères saisis
    curs_set(0); // Masque le curseur

    int maxY, maxX;
    getmaxyx(stdscr, maxY, maxX); // Obtient les dimensions du terminal

    // Crée une nouvelle fenêtre pour afficher le texte
    WINDOW *win = newwin(maxY, maxX, 0, 0);
    
    load(win);
    // Rafraîchit la fenêtre pour afficher les modifications
    wrefresh(win);
    carrefour(1,1);
    // Attend que l'utilisateur appuie sur une touche avant de quitter
    menu(70, 17);
    select();

    // Nettoie les ressources ncurses et ferme le fichier
    endwin();

    return 0;
}

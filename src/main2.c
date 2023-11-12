#include <ncurses.h>
#include <sys/ioctl.h>
#include <stdio.h>
#include <unistd.h>

void setTerminalSize(int rows, int cols) {
    struct winsize size;
    size.ws_row = rows;
    size.ws_col = cols;
    ioctl(STDOUT_FILENO, TIOCSWINSZ, &size);
}

int main() {
    int desiredRows = 25;
    int desiredCols = 80;

    // Check and set the terminal size if needed
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    if (w.ws_row < desiredRows || w.ws_col < desiredCols) {
        setTerminalSize(desiredRows, desiredCols);
    }

    initscr(); // Initialize ncurses
    noecho();  // Don't echo input characters
    curs_set(0); // Hide the cursor
    int maxY, maxX;
    getmaxyx(stdscr, maxY, maxX); // Get terminal dimensions

    // Print the menu options
    mvprintw(maxY / 2 - 1, maxX / 2 - 5, "1. Option 1");
    mvprintw(maxY / 2, maxX / 2 - 5, "2. Option 2");
    mvprintw(maxY / 2 + 1, maxX / 2 - 5, "3. Option 3");

    refresh(); // Refresh the screen

    int choice;
    // Get user input
    while (1) {
        choice = getch();
        // Process the choice
        switch (choice) {
            case '1':
                // Handle Option 1
                mvprintw(maxY - 1, 0, "You selected Option 1");
                break;
            case '2':
                // Handle Option 2
                mvprintw(maxY - 1, 0, "You selected Option 2");
                break;
            case '3':
                // Handle Option 3
                mvprintw(maxY - 1, 0, "You selected Option 3");
                break;
            case 'q':
                // Quit the program if 'q' is pressed
                endwin();
                return 0;
        }
        refresh(); // Refresh the screen after processing user input
    }
    // Rest of your ncurses code here...

    endwin();  // End ncurses

    return 0;
}

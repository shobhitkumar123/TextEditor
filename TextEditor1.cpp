#include <iostream>
#include <fstream>
#include <ncurses.h>
#include <string>

using namespace std;

void openFile(string &filename, string &content) {
    ifstream file(filename);
    if (!file) {
        cout << "Error opening file. A new file will be created." << endl;
        return;
    }

    string line;
    content = "";
    while (getline(file, line)) {
        content += line + "\n";
    }
    file.close();
}

void saveFile(string &filename, string &content) {
    ofstream file(filename);
    if (!file) {
        cerr << "Error saving file." << endl;
        return;
    }
    file << content;
    file.close();
    cout << "File saved successfully." << endl;
}

int main() {
    initscr();
    noecho();
    cbreak();
    keypad(stdscr, TRUE);

    string filename;
    string content;

    printw("Enter the filename to open or create: ");
    char nameBuffer[256];
    getstr(nameBuffer);
    filename = string(nameBuffer);

    openFile(filename, content);

    clear();
    printw("Editing file: %s\n", filename.c_str());
    printw("Press CTRL+S to save and CTRL+Q to quit.\n");

    int row = 2, col = 0;
    move(row, col);

    for (char c : content) {
        if (c == '\n') {
            row++;
            col = 0;
        } else {
            mvaddch(row, col++, c);
        }
    }
    move(row, col);

    string editedContent = content;
    while (true) {
        int ch = getch();

        if (ch == 17) { 
            break;
        } else if (ch == 19) { 
            char buffer[1000];
            for (int r = 2; r <= row; ++r) {
                mvinnstr(r, 0, buffer, 1000);
                editedContent += string(buffer) + "\n";
            }
            saveFile(filename, editedContent);
            editedContent.clear();
            mvprintw(row + 1, 0, "File saved. Press CTRL+Q to exit.");
        } else if (ch == KEY_BACKSPACE || ch == 127){
            int y, x;
            getyx(stdscr, y, x);
            if (x > 0) {
                mvdelch(y, x - 1);
                move(y, x - 1);
            }
        } else {
            addch(ch);
        }
    }

    endwin();
    return 0;
}

#include "AnsiTerminal.h"
#include <iostream>
#include <unistd.h>
#include <algorithm>
#include <cctype>
#include <sys/ioctl.h>

namespace utils{
termios AnsiTerminal::original_tio;

std::string getColumnLabel(int col) {
    std::string label;
    col--;
    while (col >= 0) {

        label = char('A' + col % 26) + label;
        col = col / 26 - 1;
    }
    return label;
}

AnsiTerminal::AnsiTerminal() {
    tcgetattr(STDIN_FILENO, &original_tio);
    struct termios new_tio = original_tio;
    new_tio.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &new_tio);
}

AnsiTerminal::~AnsiTerminal() {
    tcsetattr(STDIN_FILENO, TCSANOW, &original_tio);
}

// Prints text at the specified terminal position (row, col).
void AnsiTerminal::printAt(int row, int col, const std::string &text) {
    std::cout << "\033[" << row << ";" << col << "H" << text << std::flush;
}

// Prints text at a position (row, col) with inverted colors.
void AnsiTerminal::printInvertedAt(int row, int col, const std::string &text) {
    std::cout << "\033[" << row << ";" << col << "H\033[7m" << text << "\033[0m" << std::flush;
}

// Clears the entire terminal screen.
void AnsiTerminal::clearScreen() {
    std::cout << "\033[2J\033[H" << std::flush;
}

// Retrieves the size of the terminal window (rows and columns).
void AnsiTerminal::getTerminalSize(int &rows, int &cols) {
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    rows = w.ws_row;
    cols = w.ws_col;
}

// Function implementation that handles char operations.
char AnsiTerminal::getKeystroke() {
// Function implementation that handles char operations.
    char ch;
    read(STDIN_FILENO, &ch, 1);
    return ch;
}

// Function implementation that handles char operations.
char AnsiTerminal::getSpecialKey() {
// Function implementation that handles char operations.
    char ch = getKeystroke();
    if (ch == '\033') {
// Function implementation that handles char operations.
        char next_ch;
        if (read(STDIN_FILENO, &next_ch, 1) == 0) return '\033';
        if (next_ch == '[') {
// Function implementation that handles char operations.
            char arrow_key;
            if (read(STDIN_FILENO, &arrow_key, 1) == 1) {
                switch (arrow_key) {
                    case 'A': return 'U';
                    case 'B': return 'D';
                    case 'C': return 'R';
                    case 'D': return 'L';
                }
            }
        } else {
            return next_ch | 0x80;
        }
    }
    return ch;
}

// Displays a grid in the terminal with a cursor at the specified position.
void AnsiTerminal::displayGrid(const std::vector<std::vector<std::string>>& grid, int cursorRow, int cursorCol, int startRow, int startCol) {
    int termRows = 10;  // maximum row count to show
    int termCols = 10;  // maximum column count to show

   // Show row and column labels by calculation
    for (int c = 0; c < termCols && startCol + c < grid[0].size(); ++c) {
        printInvertedAt(3,10 + c * 10,"          ");
        std::string colLabel = getColumnLabel(startCol + c + 1);  // labels
        printInvertedAt(3, 15 + c * 10, colLabel);
    }

    for (int r = 0; r < termRows && startRow + r < grid.size(); ++r) {
        
    printInvertedAt(5 + r * 2, 4, "      ");
    printInvertedAt(6 + r * 2, 4, "      ");

    //Print row label in contrasting color
    printInvertedAt(5 + r * 2, 6, std::to_string(startRow + r + 1));
        for (int c = 0; c < termCols && startCol + c < grid[0].size(); ++c) {
            int cellRow = 4 + r * 2;
            int cellCol = 10 + c * 10;
            printAt(cellRow, cellCol, "+---------+");
            printAt(cellRow + 1, cellCol, "|         |");
            std::string content = trim_copy(grid[startRow + r][startCol + c]);
            printAt(cellRow + 1, cellCol + 1, content);
            printAt(cellRow + 2, cellCol, "+---------+");
        }
    }

// Highlight the cursor
    int cursorDisplayRow = 4 + (cursorRow - startRow) * 2 + 1;
    int cursorDisplayCol = 10 + (cursorCol - startCol) * 10 + 1;
    printInvertedAt(cursorDisplayRow, cursorDisplayCol, grid[cursorRow][cursorCol]);
}


// Displays a formula bar in the terminal at a specific position.
void AnsiTerminal::displayFormulaBar(int row, int col, const std::string &content) {
    //Clear previous formula bar contents
    printAt(1, 1, std::string(50, ' '));  //This clears the formula bar

    //Calculate column label correctly
    std::string columnLabel = getColumnLabel(col + 1);  // +1 for Excel style index

//Create the text to be displayed with the cell address and content
    std::string displayText = "Cell [" + columnLabel + "," + std::to_string(row + 1) + "]: " + content;

    //Show updated text in formula bar
    printAt(1, 1, displayText);
}

// Displays an information section in the terminal.
void AnsiTerminal::displayInformationSection(){
    
    printAt(3,120,"Welcome Excell Simulation!");
    printAt(5,110,"1.)Move with Arrow Keys.Select cell for press Enter");
    printAt(7,110,"2.) You can calculate two cells. =A1+B1 ");
    printAt(9,110,"3.) You can select multiple cells. =AVER(A1..C4)");
    printAt(11,110,"4.) Save to file press 's'");
    printAt(13,110,"5.) Load from file press 'l'");
    printAt(15,110,"6.) Exit the program for  press 'q'");

}

// Function implementation for: AnsiTerminal::trim
void AnsiTerminal::trim(std::string &s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) {
        return !std::isspace(ch);
    }));
    s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch) {
        return !std::isspace(ch);
    }).base(), s.end());
}

std::string AnsiTerminal::trim_copy(const std::string &s) {
    std::string copy = s;
    trim(copy);
    return copy;
}
}


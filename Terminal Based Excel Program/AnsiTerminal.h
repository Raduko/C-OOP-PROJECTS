#ifndef ANSI_TERMINAL_H
#define ANSI_TERMINAL_H

#include <string>
#include <vector>
#include <termios.h>
#include <sys/ioctl.h>  // For terminal size detection
namespace utils{
class AnsiTerminal {
public:
    AnsiTerminal();
    ~AnsiTerminal();

    void printAt(int row, int col, const std::string &text);
    void printInvertedAt(int row, int col, const std::string &text);
    void clearScreen();

    char getKeystroke();
    char getSpecialKey();

    void displayInformationSection();
    void displayGrid(const std::vector<std::vector<std::string>>& grid, int cursorRow, int cursorCol, int startRow, int startCol);
    void displayFormulaBar(int row, int col, const std::string &content);

    static void trim(std::string &s);
    static std::string trim_copy(const std::string &s);

    // Function to get terminal size
    void getTerminalSize(int &rows, int &cols);

private:
    static struct termios original_tio;
};
}
#endif // ANSI_TERMINAL_H

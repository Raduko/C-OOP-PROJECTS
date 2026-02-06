
#ifndef SPREADSHEET_H
#define SPREADSHEET_H

#include <vector>
#include "Cell.h"
#include "FormulaParser.h"
#include "FileManager.h"
namespace spreadsheetspace{
class Spreadsheet {
public:
    Spreadsheet();  // Constructor

    // Updates a specific cell with a value or formula
    void updateValue(int row,int column, const std::string& value);
    void updateDoubleValue(int rows, int columns, double value);
    // Saves the spreadsheet to a file
    void save(const std::string& filename);

    // Loads spreadsheet data from a file
    void load(const std::string& filename);

    const std::vector<std::vector<cellspace::Cell>>& getCells()const; 

    // Displays the spreadsheet
    void display();

    std::string getValueFromCell(int row, int column)const;
    double getDoubleFromCell(int row, int column)const;
    std::string getFormulaFromCell(int row, int column)const;

private:
    std::vector<std::vector<cellspace::Cell>> cells;  // 2D vector to hold cells
    filemanagerspace::FileManager fileManager;               // FileManager object
    formulaparserspace::FormulaParser formulaParser;           // FormulaParser object
};
}
#endif // SPREADSHEET_H

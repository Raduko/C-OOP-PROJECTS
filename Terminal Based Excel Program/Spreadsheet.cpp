
#include "Spreadsheet.h"
#include <iostream>
#include "Cell.h"
namespace spreadsheetspace{
// Constructor
Spreadsheet::Spreadsheet() {
    cells.resize(10, std::vector<cellspace::Cell>(10));  // Example: 10x10 grid
}

// Updates a specific cell with a value or formula

const std::vector<std::vector<cellspace::Cell>>& Spreadsheet::getCells() const {
    return cells;  // Return a reference to the 2D vector of cells
}

// Updates a specific cell with a given string value.
void Spreadsheet::updateValue(int row, int column, const std::string& value){

    
    cells[row][column].setValue(value);

}

std::string Spreadsheet::getValueFromCell(int row, int column)const{
    return cells[row][column].getValue();
}
double Spreadsheet::getDoubleFromCell(int row, int column)const{
    return cells[row][column].getDoubleValue();
}
std::string Spreadsheet::getFormulaFromCell(int row, int column)const{
    return cells[row][column].getFormula();
}

// Updates a specific cell with a given double value.
void Spreadsheet::updateDoubleValue(int rows, int columns, double value){
    
    cells[rows][columns].setDoubleValue(value);
}







// Saves the spreadsheet to a file
// Saves the spreadsheet data to a file.
void Spreadsheet::save(const std::string& filename) {
    fileManager.saveToFile(filename, *this);
}

// Loads spreadsheet data from a file
// Loads spreadsheet data from a file.
void Spreadsheet::load(const std::string& filename) {
    *this = fileManager.loadFromFile(filename);
}

}

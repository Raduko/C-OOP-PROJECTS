#include "AnsiTerminal.h"
#include "Spreadsheet.h"
#include <iostream>
#include <vector>
#include <string>


// Main entry point of the program. Handles initialization and overall workflow.
int main() {
    utils::AnsiTerminal terminal;
    spreadsheetspace::Spreadsheet spreadsheet;  // Use the updated spreadsheet class
    formulaparserspace::FormulaParser formulaparser; 
    filemanagerspace::FileManager filemanager;
    const std::vector<std::vector<cellspace::Cell>>& cells = spreadsheet.getCells();

    terminal.clearScreen();

    // Initialize grid with some default size
    std::vector<std::vector<std::string>> grid(10, std::vector<std::string>(5, "     "));
    int startRow = 0, startCol = 0;
    int row = 0, col = 0;
    bool editing = false;
    std::string currentInput;

    


    terminal.displayInformationSection();
    terminal.displayFormulaBar(row, col, utils::AnsiTerminal::trim_copy(grid[row][col]));
    terminal.displayGrid(grid, row, col, startRow, startCol);

    char key;
    
    while (true) {
        key = terminal.getSpecialKey();
        
        if (!editing) {
            switch (key) {
        case 'l': {
            terminal.clearScreen();
            // Kullanıcıdan dosya adını al
            terminal.printAt(0, 0, "load from file:");
            terminal.printAt(1, 0, "Enter file name with '.csv' (e.g., example.csv): ");

            std::string fileName;
            std::cin >> fileName;
            spreadsheet = filemanager.loadFromFile(fileName);
            const std::vector<std::vector<cellspace::Cell>>& cells = spreadsheet.getCells();

            // Grid boyutunu spreadsheet ile senkronize et
            grid.resize(cells.size());
            for (size_t i = 0; i < cells.size(); i++) {
                grid[i].resize(cells[i].size());
            }

            // Hücreleri dolaş ve grid'i güncelle
            for (size_t i = 0; i < cells.size(); i++) { 
                for (size_t j = 0; j < cells[i].size(); j++) { 
                    std::string current_grid_data = cells[i][j].getValue();

                    // Grid'i her hücre için güncelle
                    if (current_grid_data.empty() || current_grid_data[0] != '=') {
                        grid[i][j] = cells[i][j].getValue();  // Boş veya formül değilse düz değer ata
                        continue;
                    }

                    // Formül içeriyorsa, hesapla ve grid'i güncelle
                    std::string formula = spreadsheet.getFormulaFromCell(i, j);
                    if (formula == "/" || formula == "-" || formula == "+" || formula == "*") {
                        double result = formulaparser.evaluateFormulaTwoCell(
                            formula, spreadsheet.getValueFromCell(i, j), cells);
                        grid[i][j] = std::to_string(result);
                        spreadsheet.updateValue(i, j, grid[i][j]);  // Formül sonucu olarak string güncelle
                        spreadsheet.updateDoubleValue(i, j, result);  // Numerik sonucu güncelle
                    } else {
                        double result = formulaparser.evaluateFormulaInRange(
                            formula, spreadsheet.getValueFromCell(i, j), cells);
                        grid[i][j] = std::to_string(result);
                        spreadsheet.updateValue(i, j, grid[i][j]);  // Formül sonucu olarak string güncelle
                        spreadsheet.updateDoubleValue(i, j, result);  // Numerik sonucu güncelle
                    }
                }
            }

            terminal.displayGrid(grid, row, col, startRow, startCol);  // Güncellenmiş grid'i göster
            break;
        }


                case 's': {
                    terminal.clearScreen();

                    // Kullanıcıdan dosya adını al
                    terminal.printAt(0, 0, "Save to file:");
                    terminal.printAt(1, 0, "Enter file name with '.csv' (e.g., example.csv): ");

                    std::string fileName;
                    std::cin >> fileName;

                    // Dosya adı boş değilse kaydet
                    if (!fileName.empty()) {
                        filemanager.saveToFile(fileName, spreadsheet);
                        terminal.printAt(3, 0, "File saved successfully to " + fileName);
                    } else {
                        terminal.printAt(3, 0, "Error: File name cannot be empty.");
                    }

                    // Bekleme ve geri dönüş
                    terminal.printAt(4, 0, "Press any key to continue...");
                    terminal.getKeystroke();

                    terminal.clearScreen();
                    break;
                }
                case 'U': 
                    if (row > 0) row--;
                    if (row < startRow) startRow--;
                    break;
                case 'D': 
                    row++;
                    // Add a new row if cursor goes beyond current grid rows
                    if (row >= grid.size()) {
                        grid.emplace_back(grid[0].size(), "     ");
                    }
                    if (row >= startRow + 10) startRow++;
                    break;
                case 'R': 
                    col++;
                    // Add a new column if cursor goes beyond current grid columns
                    if (col >= grid[0].size()) {
                        for (auto& row : grid) {
                            row.emplace_back("     ");
                        }
                    }
                    if (col >= startCol + 10) startCol++;  // Pencereyi sağa kaydır
                    break;
                case 'L': 
                    if (col > 0) col--;
                    if (col < startCol) startCol--;  // Pencereyi sola kaydır
                    break;
                case 'q': 
                    return 0; // Exit on 'q'
                case '\n': case '\r':
                    editing = true;
                    currentInput = utils::AnsiTerminal::trim_copy(grid[row][col]);
                    terminal.displayFormulaBar(row, col, currentInput);
                    break;
            }
        } 
        else {
            // In editing mode, display each character as typed in the formula bar
            if (key == '\n' || key == '\r') {
                // Check if input is a formula (starts with '=')
                if (currentInput[0] == '=') {
                    // Update the cell with the evaluated result from the formula
                    
                    
                    spreadsheet.updateValue(row,col,currentInput);
                    std::string formula = spreadsheet.getFormulaFromCell(row,col);
                    if(formula == "/" || formula == "-" || formula == "+" || formula == "*"){
                        double result = formulaparser.evaluateFormulaTwoCell(formula,spreadsheet.getValueFromCell(row,col),spreadsheet.getCells());
                        grid[row][col] = std::to_string(result);
                        spreadsheet.updateDoubleValue(row,col,result);
                    }
                    else{

                        double result = formulaparser.evaluateFormulaInRange(formula,spreadsheet.getValueFromCell(row,col),spreadsheet.getCells());
                        grid[row][col] = std::to_string(result);
                        spreadsheet.updateDoubleValue(row,col,result);
                    }


                    

                    
                    
                } 
                else {
                    // Check if currentInput is 
                    grid[row][col] = currentInput;
                    if (formulaparser.isNumeric(currentInput)) {
                        
                        spreadsheet.updateDoubleValue(row, col, std::stod(currentInput));  // Store as a double value
                        spreadsheet.updateValue(row, col, currentInput);
                    } else {
                        spreadsheet.updateValue(row, col, currentInput);  // Store as a regular string value
                    }
                        
                }
                editing = false;
            } else if (key == 127 || key == '\b') {
                if (!currentInput.empty()) {
                    currentInput.pop_back();
                }
            } else if (key >= ' ' && key <= '~') {
                currentInput += key;
            }

            terminal.displayFormulaBar(row, col, currentInput);  // Show real-time input in formula bar
        }

        //bu bloğu eğer bir sayının değeri değişirse her defasında işlemleri tek tek cell leri kontrol edip tekrar hesaplasın diye ekledim
// Tüm hücreleri kontrol et ve formülleri yeniden hesapla
        for (int i = 0; i < cells.size(); i++) { // Satırları dolaş
            for (int j = 0; j < cells[i].size(); j++) { // Sütunları dolaş
                std::string current_grid_data = cells[i][j].getValue();

                if (!current_grid_data.empty() && current_grid_data[0] == '=') {
                    std::string formula = spreadsheet.getFormulaFromCell(i, j);
                    if (formula == "/" || formula == "-" || formula == "+" || formula == "*") {
                        double result = formulaparser.evaluateFormulaTwoCell(
                            formula, spreadsheet.getValueFromCell(i, j), cells);
                        grid[i][j] = std::to_string(result);
                        spreadsheet.updateDoubleValue(i,j,result);
                    } else {
                        double result = formulaparser.evaluateFormulaInRange(
                            formula, spreadsheet.getValueFromCell(i, j), cells);
                            spreadsheet.updateDoubleValue(i,j,result);
                            grid[i][j] = std::to_string(result);
                    }
                }
            }
        }
        
       


    // to avoid, at empty grids no cursor seen
    if (grid[row][col].empty()) {
    grid[row][col] = "     ";  
    }

        
        
    
        
        // Update grid to show cursor position with the new dimensions if expanded
        terminal.displayGrid(grid, row, col, startRow, startCol);
        terminal.displayInformationSection();

        

       
        

      

    }

    return 0;
}

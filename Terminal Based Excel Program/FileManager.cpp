#include "FileManager.h"
#include "Spreadsheet.h"
#include <fstream>      // For std::ifstream
#include <sstream>      // For std::stringstream
#include <algorithm>    // For std::find_if
#include <cctype>       // For std::isspace
#include <iostream>     // For std::cerr and std::cout
namespace filemanagerspace{
filemanagerspace::FileManager::FileManager() {
    // Initialization code here, if needed
    currentFilePath = "";
    
}


// Saves the spreadsheet data to a specified file.
void FileManager::saveToFile(const std::string& filename, const spreadsheetspace::Spreadsheet& spreadsheet) {
    std::ofstream file(filename); // Open the file for writing
    if (file.is_open()) {
        const auto& cells = spreadsheet.getCells(); // Get all cells from the spreadsheet
        
        // Iterate through rows
        for (int row = 0; row < cells.size(); ++row) {
            bool firstColumn = true;

            // Iterate through columns in the row
            for (int col = 0; col < cells[row].size(); ++col) {
                if (!firstColumn) {
                    file << ","; // Add a comma between columns
                }

                // Get the value of the current cell
                std::string cellValue = spreadsheet.getValueFromCell(row, col);

                // Escape special characters (commas, quotes, and newlines)
                if (cellValue.find(',') != std::string::npos || cellValue.find('\n') != std::string::npos || cellValue.find('"') != std::string::npos) {
                    // Wrap value in quotes and escape internal quotes
                    cellValue = "\"" + cellValue + "\"";
                }

                file << cellValue; // Write the value to the file
                firstColumn = false;
            }

            file << "\n"; // End the current row
        }

        file.close(); // Close the file
        
    } else {
        std::cerr << "Unable to open file for writing." << std::endl;
    }
}


spreadsheetspace::Spreadsheet FileManager::loadFromFile(const std::string& filename) {
    std::ifstream file(filename);
    spreadsheetspace::Spreadsheet spreadsheet;

    if (file.is_open()) {
        std::string line;
        int row = 0;

        // Dosyadaki verileri oku ve Spreadsheet'e yükle
        while (std::getline(file, line)) {
            std::stringstream ss(line);
            std::string cellValue;
            int col = 0;

            while (std::getline(ss, cellValue, ',')) {
                // Hücre boşsa, geç
                if (cellValue.empty()) {
                    col++;
                    continue;
                }

                // Hücreye değeri ata
                spreadsheet.updateValue(row, col, cellValue);

                // Eğer değer numerik ise, numerik olarak sakla
                if (!cellValue.empty() && std::isdigit(cellValue[0])) {
                    try {
                        double numericValue = std::stod(cellValue);
                        spreadsheet.updateDoubleValue(row, col, numericValue);
                        spreadsheet.updateValue(row,col,cellValue);
                    } catch (const std::exception&) {
                        // Numerik değil, string olarak kalacak
                    }
                }

                col++;
            }

            row++;
        }

        file.close();
    } else {
        std::cerr << "Error: Could not open file " << filename << std::endl;
    }

    return spreadsheet;
}
}


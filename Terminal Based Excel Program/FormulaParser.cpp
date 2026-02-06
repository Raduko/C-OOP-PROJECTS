
#include "FormulaParser.h"
#include <cctype>
#include <sstream>
#include <stdexcept>
#include <stack>
#include <cmath>

// Constructor
namespace formulaparserspace{
FormulaParser::FormulaParser() {}
std::string FormulaParser::getFirstCell(std::string value_str)const{
    if(std::string::npos !=value_str.find_first_of("+-/*") ){
        std::string first_cell= value_str.substr(1,value_str.find_first_of("+-/*")-1);
        return first_cell;
    }
    else if (std::string::npos != value_str.find_first_of("(")) {
        // Find the position of "("
        size_t start_pos = value_str.find("(") + 1;  // Start just after "("
        
        // Find the position of ".." to stop at that point
        size_t end_pos = value_str.find("..", start_pos);  // Find ".." after "("

        if (end_pos != std::string::npos) {
            // Return the part between "(" and ".."
            return value_str.substr(start_pos, end_pos - start_pos);
        }
    }
    return "";
}
std::string FormulaParser::getSecondCell(std::string value_str)const{
    if(std::string::npos !=value_str.find_first_of("+-/*") ){
        std::string second_cell= value_str.substr(value_str.find_first_of("+-/*")+1);
        return second_cell;
    }
    else if (std::string::npos != value_str.find("..")) {
    // If the string contains "..", get the part between ".." and ")"
    size_t start_pos = value_str.find("..") + 2;  // Start just after ".."
    size_t end_pos = value_str.find(")", start_pos);  // Find the closing parenthesis

    // Extract the portion between ".." and ")"
    std::string range = value_str.substr(start_pos, end_pos - start_pos);
    return range;
    }   
    return "";
}
bool FormulaParser::isNumeric(const std::string& str) {
    if (str.empty()) return false;
    bool decimalPointFound = false;
    for (char c : str) {
        if (std::isdigit(c) == 0) {
            // Allow only one decimal point
            if (c == '.' && !decimalPointFound) {
                decimalPointFound = true;
            } else {
                return false;  // Not a number if it contains non-digit or more than one decimal
            }
        }
    }
    return true;
}


// Parses a single cell reference (e.g., "A1") into row and column indices
int FormulaParser::getRow(std::string cell_value){
    //find till any integer returns integer part.
    if(std::string::npos !=cell_value.find_first_of("0123456789") ){

        std::string row_value = cell_value.substr(cell_value.find_first_of("0123456789"));
    
        return stoi(row_value) - 1;
    }
    return 0;
}
std::string FormulaParser::getColumn(std::string cell_value){
    // its 0 to find any integer.
    if(std::string::npos !=cell_value.find_first_of("0123456789") ){
        std::string column_value = cell_value.substr(0,cell_value.find_first_of("0123456789"));

        
        return column_value;
    }
    return "";
}
int FormulaParser::ColumnLetterToInt(std::string letter){
    int result = 0;
    for (int i = 0; i < letter.size(); ++i) {
        result = result * 26 + (letter[i] - 'A' + 1); // 'A' = 1, 'B' = 2, ..., 'Z' = 26
    }
    return result - 1; // 0 tabanlı indeksi almak için 1 çıkarıyoruz
}


// Evaluates a formula and returns the result
double FormulaParser::evaluateFormulaTwoCell(const std::string&formula, std::string value_str ,const std::vector<std::vector<cellspace::Cell>> &cells){

        
        std::string first_cell_raw = getFirstCell(value_str);
        std::string second_cell_raw = getSecondCell(value_str);
        cellspace::Cell first_cell = cells[getRow(first_cell_raw)][ColumnLetterToInt(getColumn(first_cell_raw))];
        cellspace::Cell second_cell = cells[getRow(second_cell_raw)][ColumnLetterToInt(getColumn(second_cell_raw))];
        if(formula == "+"){
            
            return first_cell.getDoubleValue() + second_cell.getDoubleValue();
        }
        else if(formula == "-"){
            return first_cell.getDoubleValue() - second_cell.getDoubleValue();
        }
        else if(formula == "/"){
            if(second_cell.getDoubleValue() == 0.0){
                return -999.0;
            }
            return first_cell.getDoubleValue() / second_cell.getDoubleValue();
        }
        else if(formula == "*"){
            return first_cell.getDoubleValue() * second_cell.getDoubleValue();
        }
        return -999.0;
}
//range yukardan aşağı A1 A2 A3 A4 olarak çalışıyor  en sonda row değil column alıyor aslında
double FormulaParser::evaluateFormulaInRange(const std::string&formula, std::string value_str,const std::vector<std::vector<cellspace::Cell>> &cells){
    double result = 0.0;
        std::string first_cell_raw = getFirstCell(value_str);
        std::string second_cell_raw = getSecondCell(value_str);
        cellspace::Cell first_cell = cells[getRow(first_cell_raw)][ColumnLetterToInt(getColumn(first_cell_raw))];
        cellspace::Cell second_cell = cells[getRow(second_cell_raw)][ColumnLetterToInt(getColumn(second_cell_raw))];
        int column_1 =ColumnLetterToInt(getColumn(first_cell_raw));
        int column_2 = ColumnLetterToInt(getColumn(second_cell_raw));
        int row_1 = getRow(first_cell_raw);
        int row_2 = getRow(second_cell_raw);

        
        int row_count = row_2 - row_1 + 1;
        int column_count = column_2 - column_1 + 1;
        int total_cells = row_count * column_count;
            if(formula == "SUM"){
                for(int i = row_1; i <= row_2; i++){
                    for(int j = column_1; j <= column_2; j++){
                        result += cells[i][j].getDoubleValue();
                    }
                    
                }
                return result;
                
            }

            else if(formula == "AVER"){
                double total_cells_value = 0.0;
                for(int i = row_1; i <= row_2; i++){
                    for(int j = column_1; j <= column_2; j++){
                    total_cells_value += cells[i][j].getDoubleValue();
                    }
                }
                return  total_cells_value / double(total_cells);
            }
            else if(formula == "STDDEV"){
                double total_cells_value = 0.0;
                double avg_cells = 0.0;
                double temp = 0.0;
                for(int i = row_1; i <= row_2; i++){
                    for(int j = column_1; j <= column_2; j++){
                    total_cells_value += cells[i][j].getDoubleValue();
                    }
                }
                avg_cells =  total_cells_value / double(total_cells);

                for(int i = row_1; i <= row_2;i++){
                    for(int j = column_1; j <= column_2; j++){
                    temp += pow((cells[i][j].getDoubleValue() - avg_cells),2);
                    }
                }
                return sqrt(temp/double((total_cells)));
                
            }
            else if(formula == "MAX"){
                double max = cells[row_1][column_1].getDoubleValue();
                for(int i = row_1; i <= row_2;i++){
                    for(int j = column_1; j <= column_2; j++){
                    if(cells[i][j].getDoubleValue() > max){
                        max = cells[i][j].getDoubleValue();
                    }
                    }
                }
                return max;
            }
            else if(formula == "MIN"){
                double min = cells[row_1][column_1].getDoubleValue();
                for(int i = row_1; i <= row_2;i++){
                    for(int j = column_1; j <= column_2; j++){
                    if(cells[i][j].getDoubleValue() < min){
                        min = cells[i][j].getDoubleValue();
                    }
                    }
                }
                return min;
            }
            return -999.0;



        
    
}
}

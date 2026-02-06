
#ifndef FORMULAPARSER_H
#define FORMULAPARSER_H

#include <string>
#include <unordered_map>
#include <vector>
#include "Cell.h"
namespace formulaparserspace{
class FormulaParser {
public:
    FormulaParser();  // Constructor
    int ColumnLetterToInt(std::string letter);
    // Parses a formula and evaluates the result

    std::string getFirstCell(std::string value_str)const;
    std::string getSecondCell(std::string value_str)const;
    bool isNumeric(const std::string& str);
    

    std::string getColumn(std::string cell_vaue);
    int getRow(std::string cell_value);
    //int evaluateFormula(const std::string& formula, const std::vector<std::vector<Cell>>& cells);
    double evaluateFormulaTwoCell(const std::string&formula, std::string value_str ,const std::vector<std::vector<cellspace::Cell>> &cells);
    double evaluateFormulaInRange(const std::string&formula, std::string value_str,const std::vector<std::vector<cellspace::Cell>> &cells);
private:
   
   

  
    
   
    
};
}
#endif // FORMULAPARSER_H

#include "Cell.h"
#include <iostream>
namespace cellspace{
// Constructor
Cell::Cell(std::string value) : value(value), formula(""), isFormulaCell(false){}

// Getter for formula
std::string Cell::getFormula() const {
    return formula; 
}


// Set value and check if it's a formula
// Sets the value of the cell to the specified string.
void Cell::setValue(const std::string& value) {

    if (!value.empty() && value[0] == '=') {
        //before added  != std::string::npos it value.find returns big number and it was positive all time.
        //so now compare with npos if not equal its return 0 
        this -> value = value;
        if(value.find("SUM")!= std::string::npos){
           
            formula = "SUM";
        }
        else if(value.find("AVER")!= std::string::npos){
            formula = "AVER";
        }
        else if(value.find("STDDEV")!= std::string::npos){
            formula = "STDDEV";
        }
        else if(value.find("MAX")!= std::string::npos){
            formula = "MAX";
        }
        else if(value.find("MIN")!= std::string::npos){
            formula = "MIN";
        } 


        else if(value.find('+')!= std::string::npos){
            formula = '+';
        }
        else if(value.find('-')!= std::string::npos){
            formula = '-';
        }
        else if(value.find('/')!= std::string::npos){
            formula = '/';
        }
        else if(value.find('*')!= std::string::npos){
            formula = '*';
        }   

        isFormulaCell = true;



    } 
    else {
        // Not a formula, store as a direct value
        formula = "";
        isFormulaCell = false;
        this->value = value;
    }
}

// Set formula directly (for FormulaParser to use)
// Sets the formula for the cell.
void Cell::setFormula(const std::string& formula) {
    this->formula = formula;
    isFormulaCell = true;
}

// Getter for value
std::string Cell::getValue() const {
    return value;
}

// Check if the cell contains a formula
bool Cell::isFormula() const {
    return isFormulaCell;
}

}

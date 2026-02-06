#ifndef CELL_H
#define CELL_H

#include <string>
namespace cellspace{
class Cell {
public:
    Cell(std::string value = "");  // Constructor declaration
    std::string getFormula()const;
    void setValue(const std::string& value);  // Declare the setter for the cell value
    std::string getValue() const;             // Declare the getter for the cell value
    double getDoubleValue()const{return double_value;}
    void setFormula(const std::string& formula);
    void setDoubleValue(double n){double_value = n;}

    bool isFormula() const;                   // Declare the function to check if the cell contains a formula

private:
    std::string value;         // Stores the text or numeric value of the cell
    std::string formula;       // Stores the formula if the cell contains one
    double double_value;
    bool isFormulaCell;        // Indicates whether the cell contains a formula
    
};
}
#endif // CELL_H

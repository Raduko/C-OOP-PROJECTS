#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include <string>
#include <vector>

namespace spreadsheetspace {
    class Spreadsheet;  // Forward declaration
}
namespace filemanagerspace{



class FileManager {
    public:
        FileManager();
        void saveToFile(const std::string& filename, const spreadsheetspace::Spreadsheet& spreadsheet);
        spreadsheetspace::Spreadsheet loadFromFile(const std::string& filename);

    private:
        std::string currentFilePath;
        std::vector<std::string> tempData;
};
}
#endif // FILEMANAGER_H

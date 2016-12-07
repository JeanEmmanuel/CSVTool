#ifndef INTERFACE_H
#define INTERFACE_H

#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>

#include "CSVReader.h"

class Interface
{
public:
    Interface();
    /*
     *  Used to show start up message and ask for first user input.
     */
    void Initialization();
    
    /*
     *  Command line interface, handles the menu and parses requests.
     */
    void CmdLine();
    const std::string& GetVersion(){
        static std::string version("CSVTool v1.0");
        return version;
    }
    
    /*
     *  Used to cout a delimiter on the screen, static to avoid initializing it every time.
     */
    const std::string& GetDelimiter(){
        static std::string delimiter("---------------------------------------------------------------------");
        return delimiter;
    }

    /*
     *  Operators list
     */
    const std::vector<std::string>& GetOperators(){
        static std::vector<std::string> operators = {"+", "-", "/", "*", "inner", "outer"};
        return operators;
    }


private:
    /*
     *  Handles the CSV input, and file check.
     */
    void AskForCsvInput();
    
    /*
     *  Handles the "help" user request
     */
    void fHelp();
    
    /*
     *  Handles the "version" user request
     */
    void fVersion();
    
    /*
     *  Handles the "exit" user request
     */
    void fExit();
    
    /*
     *  Handles the "new" user request
     */
    void fNewCsv();
    
    /*
     *  Handles the "save" user request
     */
    void fSave();
    
    /*
     *  Handles the "+/-/... [headers]" user request
     */
    void fOperations();
    
    /*
     *  Handles the "operators" user request
     */
    void fOperators();
    
    /*
     *  Handles the "statistics [header]" user request
     */
    void fStatistics();
    
    /*
     *  Handles the "headers" user request
     */
    void fHeaders();
    
    /*
     *  Handles the "size" user request
     */
    void fSize();
    
    /*
     *  Handles any unknown user request
     */
    void fUnknown();
    
    /*
     *  Warns the user during an operation error
     */
    void WrongOperationRequest();
    
    /*
     *  Warns the user during a statistic error
     */
    void WrongStatisticsRequest();
    
    /*
     *  Saves the CSV to the path
     */
    void SaveToPath(const std::string & pathToFile);
    
    /*
     *  Makes operation between column1 and column2
     */
    void operation(const std::string & op, const std::string & col1, const std::string & col2);

    /*
     *  Transforms the user input in integer (enum UserCommands)
     */
    int CmdParser();

    /*
     *  Get user input and passes it
     */
    std::string GetUserInput();

    /*
     *  Returns true if an operator is found in the cmd line
     */
    bool foundOperatorInCmd(const std::string & cmd);
    
    /*
     *  Returns true if the "statistics" is found in the cmd line
     */
    bool foundStatisticsInCmd(const std::string & cmd);
    
public:
private:
    CSVReader *         m_CSVReader;
    std::vector<char>   v_Operators;
    std::string         m_CmdRequest;
    std::string         m_PathToFile;
};

enum UserCommands{
    cHelp = 0,
    cVersion,
    cExit,
    cNewCSV,
    cSave,
    cOperations,
    cOperators,
    cStatistics,
    cHeaders,
    cSize,
    cUnknown
};

#endif // INTERFACE_H

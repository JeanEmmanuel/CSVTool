#include "Interface.h"

#include <cstdlib>
#include <string>
#include <algorithm>

// /Users/jean-emmanuellosi/Desktop/Investment strategy/100best_pairs.csv

using namespace std;

Interface::Interface()
{

}

void Interface::Initialization()
{
    cout << "*******************************************************************************"<<endl;
    cout << "*************************** Welcome to "+GetVersion()+" ***************************"<<endl;
    cout << "*******************************************************************************"<<endl;

    AskForCsvInput();
}

void Interface::CmdLine()
{
    while(1)
    {
        cout<<GetDelimiter()<<endl;
        cout<<"What would you like to do ?"<<endl;

        int UserRequest = CmdParser();
        switch(UserRequest){
        case cHelp:
            fHelp();
            break;
        case cVersion:
            fVersion();
            break;
        case cExit:
            fExit();
            break;
        case cNewCSV:
            fNewCsv();
            break;
        case cSave:
            fSave();
            break;
        case cStatistics:
            fStatistics();
            break;
        case cOperators:
            fOperators();
            break;
        case cOperations:
            fOperations();
            break;
        case cHeaders:
            fHeaders();
            break;
        case cSize:
            fSize();
            break;
        case cUnknown:
            fUnknown();
            break;
        default:
            fUnknown();
        }
    }
}

int Interface::CmdParser()
{
    m_CmdRequest = GetUserInput();

    if(m_CmdRequest == "help"){
        return cHelp;
    }
    else if(m_CmdRequest == "version"){
        return cVersion;
    }
    else if(m_CmdRequest == "exit"){
        return cExit;
    }
    else if(m_CmdRequest == "new"){
        return cNewCSV;
    }
    else if(m_CmdRequest == "save"){
        return cSave;
    }
    else if(foundStatisticsInCmd(m_CmdRequest)){
        return cStatistics;
    }
    else if(m_CmdRequest == "operators"){
        return cOperators;
    }
    else if(m_CmdRequest == "headers"){
        return cHeaders;
    }
    else if(m_CmdRequest == "size"){
        return cSize;
    }
    else if(foundOperatorInCmd(m_CmdRequest)){
        return cOperations;
    }
    else{
        return cUnknown;
    }
}

void Interface::AskForCsvInput()
{
    m_PathToFile = "";
    string extension = "";
    size_t pos;

    while(1)
    {
        cout << endl;
        cout << "Please drag and drop the CSV file, or type full address, to start analyzing: ";
        m_PathToFile = GetUserInput();
        m_PathToFile.erase(remove(m_PathToFile.begin(), m_PathToFile.end(), '\"'), m_PathToFile.end());
        m_PathToFile.substr(0,m_PathToFile.find_last_not_of(' ')); //Removes trailing space when drag drop from macOS
        cout<<"PATH=["<<m_PathToFile<<"]"<<endl;
        
        pos = m_PathToFile.find_last_of(".");
        if (pos == string::npos){
            //If there is not '.' to find the extension
            cout << "Invalid file name. Make sure to include the extension.";
        }
        else{
            //If we found it, check if the extension is a csv
            extension = m_PathToFile.substr(pos+1);

            if( extension != "csv" ){
                cout<<extension<<endl;
                cout<<"The file found at this address is not a csv. Please input a CSV file."<<endl;
            }
            else{
                //If the file is a CSV, try to open it.
                ifstream * fileStream = new ifstream(m_PathToFile.c_str());

                if( ! fileStream->is_open() ) {
                    cout <<"Something went wrong. Failed to open the file." << endl;
                }
                else{
                    cout <<"Opening file..." << endl;
                    bool containsHeader;
                    while(1)
                    {
                        cout <<"Does this data contains a header ? y/n" << endl;
                        string containsHeaderStr = GetUserInput();
                        if(containsHeaderStr == "y"){
                            containsHeader = 1;
                            break;
                        }
                        else if(containsHeaderStr == "n"){
                            containsHeader = 0;
                            break;
                        }
                    }
                    m_CSVReader = new CSVReader(fileStream,containsHeader);
                    if(m_CSVReader->Initialization()){
                        cout <<"CSVTool ready to use !" << endl;
                        break;
                    }
                    else{
                        cout <<"CSVTool failed to parse the file." << endl;
                    }
                }
            }
        }
    }
}

string Interface::GetUserInput()
{
    string input;
    getline(cin, input);
    return input;
}

void Interface::fHelp()
{
    cout<<"help                             print all available command line operations."<<endl;
    cout<<"version                          print the current application's version."<<endl;
    cout<<"exit                             stops the application."<<endl;
    cout<<"new                              input a new csv file."<<endl;
    cout<<"save                             saves the file in the executable's directory."<<endl;
    cout<<"statistics [header]              output some statistics about the selected column."<<endl;
    cout<<"[operator] [lhs_header] [rhs_header]   selected operator on [col1] and [col2]."<<endl;
    cout<<"operators                        print all available operators on columns."<<endl;
    cout<<"headers                          print all available column names."<<endl;
    cout<<"size                             returns the size of the dataset."<<endl;
}

void Interface::fVersion()
{
    cout<<GetVersion()<<endl;
}

void Interface::fExit()
{
    exit(0);
}

void Interface::fNewCsv()
{
    delete m_CSVReader;
    AskForCsvInput();
}

void Interface::fSave()
{
    cout<<GetDelimiter()<<endl;
    cout<<"Where would you like to save the file ? [overwrite/default/path]"<<endl;
    cout<<"overwrite                overwrite the input file."<<endl;
    cout<<"copy                     write at the same location as the input file, with copy suffix added."<<endl;
    cout<<"path                     write at the path input by the user."<<endl;
    
    string input = GetUserInput();
    string path;
    if(input == "overwrite"){
        path = m_PathToFile;
    }
    else if(input == "copy"){
        size_t extensionPosition = m_PathToFile.find_last_of('.');
        path = m_PathToFile.substr(0, extensionPosition)+"-copy"+m_PathToFile.substr(extensionPosition);
    }
    else if(input == "path"){
        path = GetUserInput();
    }
    else{
        fSave();
    }
    SaveToPath(path);
}

void Interface::SaveToPath(const std::string & pathToFile)
{
    ofstream fileToWrite;
    vector<vector<string>> CSVToWrite;
    vector<int> columnsToWrite;
    cout<<"Opening file located at ["<<pathToFile<<"]..."<<endl;
    fileToWrite.open(pathToFile.c_str(), std::ofstream::out | std::ofstream::trunc); //clears the file before opening it.

    if(! fileToWrite.is_open() ){
        cout<<"Cannot write at this location."<<endl;
        fSave();
        return;
    }
    else{
        string input = "";
        while(input==""){
            cout<<GetDelimiter()<<endl;
            cout<<"Which colums would you like to save ? Options are:"<<endl;
            cout<<"all                                  write all columns to the selected location."<<endl;
            cout<<"[header1] [header2] ... [header n]   write the n selected columns to the location."<<endl;
            input = GetUserInput();
        }
        if(input == "all"){
            CSVToWrite = m_CSVReader->GetCsv();
            for(int i = 0; i<CSVToWrite.size(); ++i){
                columnsToWrite.push_back(i);
            }
        }
        else{
            //parse list of header
            while(input != "")
            {
                size_t first_not_space = input.find_first_not_of(' '); //ignores first spaces input by the user.
                input = input.substr(first_not_space);
                size_t next_space = input.find(' ');
                
                string header = input.substr(0,next_space);
                int posInHeaders = m_CSVReader->PositionInHeader(header);
                if( posInHeaders == -1){
                    cout<<"["<<header<<"] is not a valid header name."<<endl;
                    SaveToPath(pathToFile);
                    return;
                }
                columnsToWrite.push_back(posInHeaders);
                input = input.substr(next_space+1);
                if(next_space==string::npos)
                    break;
            }
            cout<<"Writing...";
            vector<vector<string>> FullCsv = m_CSVReader->GetCsv();
            for(int ColumnIndex : columnsToWrite){
                if(FullCsv[ColumnIndex].size() != FullCsv[0].size()){
                    //if the column does not have the right number of elements (inner joint), add null at the end to avoir bad_access
                    for(size_t missing = FullCsv[ColumnIndex].size(); missing < FullCsv[0].size(); ++missing){
                        FullCsv[ColumnIndex].push_back("null");
                    }
                }
                CSVToWrite.push_back(FullCsv[ColumnIndex]);
                }
        }
        
        size_t nCol = CSVToWrite.size();
        size_t nRow = 0;
        if(nCol > 0){
            nRow = CSVToWrite.at(0).size();
        }
        if(nRow==0){
            cout<<"Something went wrong."<<endl;
            return;
        }
        
        while(1)
        {
            //Does the user wants to include the headers (custom if available, otherwise default)
            cout<<"Do you want to put headers in the dataset ? [y/n]";
            string wantHeader = GetUserInput();
            if(wantHeader == "y"){
                
                vector<string> HeaderList = m_CSVReader->GetHeader();
                if(HeaderList.size()==0){
                    break;
                }
                else{
                    fileToWrite<<HeaderList[0];
                    for(int index = 1; index<columnsToWrite.size(); ++index){
                        fileToWrite<<",";
                        fileToWrite<<HeaderList[index];
                    }
                    fileToWrite<<"\n";
                    break;
                }
            }
            else if(wantHeader == "n"){
                break;
            }
        }
        //write the CSV at the stream location
        for (int row=0; row<CSVToWrite.at(0).size(); ++row)
        {
            fileToWrite << CSVToWrite[0][row];
            
            for (int col=1; col<CSVToWrite.size(); ++col)
            {
                fileToWrite << "," << CSVToWrite[col][row];
            }
            fileToWrite << "\n";
        }
        fileToWrite.close();
    }
}

void Interface::fOperations()
{
    string op;
    string col1;
    string col2;

    string StringLeftToConsume = m_CmdRequest.substr(m_CmdRequest.find_first_not_of(' '));

    size_t firstSpace = StringLeftToConsume.find(' ');
    if(StringLeftToConsume.at(firstSpace) != ' '){
        cout<<"Spacing error after operator. ";
        WrongOperationRequest();
        return;
    }
    else if(! foundOperatorInCmd(StringLeftToConsume.substr(0,firstSpace))){
        cout<<"["<<StringLeftToConsume.substr(0,firstSpace)<<"] Operator not valid. ";
        WrongOperationRequest();
        return;
    }
    else{
        op = StringLeftToConsume.substr(0,firstSpace);
        StringLeftToConsume = StringLeftToConsume.substr(firstSpace+1);

        size_t secondSpace = StringLeftToConsume.find(' ');
        if(secondSpace == string::npos){
            cout<<"Spacing between columns not found. ";
            WrongOperationRequest();
            return;
        }
        else if(! m_CSVReader->isValidColumnName(StringLeftToConsume.substr(0,secondSpace))){
            cout<<"First column name not valid. Received ["<<StringLeftToConsume.substr(0,secondSpace)<<"]. ";
            WrongOperationRequest();
            return;
        }
        else{

            col1 = StringLeftToConsume.substr(0,secondSpace);
            StringLeftToConsume = StringLeftToConsume.substr(secondSpace+1);

            size_t thirdSpace = StringLeftToConsume.find(' ');

            if(thirdSpace == string::npos){
                if(! m_CSVReader->isValidColumnName(StringLeftToConsume)){
                    cout<<"Second column name not valid. Received ["<<StringLeftToConsume<<"]. ";
                    WrongOperationRequest();
                    return;
                }
                else{
                    col2 = StringLeftToConsume;
                    operation(op,col1,col2);
                }
            }
            else{
                if(! m_CSVReader->isValidColumnName(StringLeftToConsume.substr(0,thirdSpace))){
                    cout<<"Second column name not valid. Received ["<<StringLeftToConsume.substr(0,thirdSpace)<<"]. ";
                    WrongOperationRequest();
                    return;
                }
                else{
                    col2 = StringLeftToConsume.substr(0,thirdSpace);
                    operation(op,col1,col2);
                }
            }
        }
    }
}

void Interface::fOperators()
{
    cout<<"The available operators on columns are:"<<endl;
    for(const string & op : GetOperators()){
        cout<<op<<" ";
    }
    cout<<endl;
}


void Interface::fStatistics()
{
    size_t IndexOfSpace = m_CmdRequest.find(' ');
    if(IndexOfSpace==string::npos){
        WrongStatisticsRequest();
        return;
    }
    size_t IndexOfSecondSpace = m_CmdRequest.substr(IndexOfSpace+1).find(' ');
    string header = m_CmdRequest.substr(IndexOfSpace+1, IndexOfSecondSpace);
    if(! m_CSVReader->isValidColumnName(header)){
        cout<<"Header chosen not valid. Received ["<<header<<"]."<<endl;
        WrongStatisticsRequest();
        return;
    }
    
    int positionColumn = m_CSVReader->PositionInHeader(header);
    if(positionColumn == -1){
        cout<<"Header chosen not valid. Received ["<<header<<"]."<<endl;
        WrongStatisticsRequest();
        return;
    }
    
    m_CSVReader->statistics(positionColumn);
    
}

void Interface::fUnknown()
{
    cout<<"This command is unknown. Here are all the commands available:"<<endl;
    fHelp();
}

void Interface::fHeaders()
{
    m_CSVReader->PrintAvailableHeaders();
}

void Interface::fSize()
{
    m_CSVReader->GetSize();
}

bool Interface::foundOperatorInCmd(const string & cmd)
{
    for(const string & op : GetOperators()){
        size_t pos = cmd.find(op);
        if(pos != std::string::npos)
            return 1;
    }
    return 0;
}

bool Interface::foundStatisticsInCmd(const std::string & cmd)
{
    size_t pos = cmd.find("statistics");
    if(pos != std::string::npos)
        return 1;
    return 0;
}

void Interface::WrongOperationRequest()
{
    cout<<"Wrong operation syntax. Please use the following:"<<endl;
    cout<<"[operator] [lhs_header] [rhs_header]"<<endl;
    cout<<"operators    will give you the list of available operators"<<endl;
    cout<<"headers      will give you the list of available headers"<<endl;
    cout<<"Example: \"+ c1 c2\""<<endl;
}

void Interface::WrongStatisticsRequest()
{
    cout<<"Wrong statistics syntax. Please use the following:"<<endl;
    cout<<"statistics [header]"<<endl;
    cout<<"headers      will give you the list of available headers"<<endl;
    cout<<"Example: \"statistics c1\""<<endl;
}


void Interface::operation(const std::string & op, const std::string & col1, const std::string & col2)
{
    int positionColumn1 = m_CSVReader->PositionInHeader(col1);
    int positionColumn2 = m_CSVReader->PositionInHeader(col2);
    if(positionColumn1 == -1 || positionColumn2 == -1)
    {
        cout<<"Something went wrong. Column name not found in header list. Unable to perform the operations."<<endl;
        return;
    }
    m_CSVReader->operation(op, positionColumn1, positionColumn2);
}




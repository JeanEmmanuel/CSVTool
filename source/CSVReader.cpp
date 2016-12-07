#include "CSVReader.h"
#include <iostream>
#include <sstream>

using namespace std;

CSVReader::CSVReader(std::ifstream * stream, bool header)
: m_FileStream(stream), m_hasHeader(header)
{
    
}

bool CSVReader::Initialization()
{
    bool success;
    if(ParseFileStream())
        success = 1;
    else
        success = 0;
    //close all connections
    
    if(m_FileStream!=NULL){
        m_FileStream->close();
        delete m_FileStream;
        m_FileStream = NULL;
    }
    
    return success;
}

CSVReader::~CSVReader()
{
    ///To avoid memory leaks
    delete m_FileStream;
    m_FileStream = NULL;
}

bool CSVReader::ParseFileStream()
{
    int index = 0;
    CSVArray CsvArray;
    m_Header.clear();
    m_DefaultHeader.clear();
    ///In case the user wants to parse the stream again (let's say he forgot the file had headers).
    ///ParseFileStream() can be called several times.
    
    cout<<"Parsing CSV...";
    size_t csvLineSize = -1;
    if(m_FileStream == NULL){
        cout<<"Failed !"<<endl;
        return 0;
    }
    else{
        string line;
        string cell;
        
        if(m_hasHeader){
            ///creates the custom header name
            getline(*m_FileStream,line);
            stringstream  lineStream(line.c_str());
            while(getline(lineStream,cell,','))
            {
                m_Header.push_back(cell);
            }
            ++index;
        }
        while(getline(*m_FileStream,line))
        {
            //Parses the data
            CSVLine csvLine;
            stringstream  lineStream(line.c_str());
            while(getline(lineStream,cell,','))
            {
                csvLine.push_back(cell);
            }
            if (!lineStream && cell.empty())
            {
                /// If there was a trailing comma then add an empty element.
                csvLine.push_back("");
            }
            if(csvLineSize == -1){
                ///Check size of new row
                csvLineSize = csvLine.size();
                CsvArray.push_back(csvLine);
            }
            
            else{
                if(csvLine.size() != csvLineSize){
                    cout<<"Error: a row does not contain the right number of values. Skipping the row number "<<index<<endl;
                    cout<<"Has "<<csvLine.size()<<" instead of "<<csvLineSize<<endl;
                }
                else{
                    CsvArray.push_back(csvLine);
                }
            }
            ++index;
        }
        string defaultHeaderChar = "c";
        for(int i = 0; i<csvLineSize ; ++i){
            ///creates the default header name
            m_DefaultHeader.push_back(defaultHeaderChar+std::to_string(i));
        }
        m_CSVTransposedArray = TransposeCsv(CsvArray);
        cout<<"Done !"<<endl;
        return 1;
    }
}

bool CSVReader::isValidColumnName(const string & colName)
{
    for(const string & header : m_DefaultHeader){
        if(header == colName)
            return 1;
    }
    for(const string & header : m_Header){
        if(header == colName)
            return 1;
    }
    
    return 0;
}

int CSVReader::PositionInHeader(const std::string &columnName)
{
    for(int i = 0; i<m_DefaultHeader.size(); ++i){
        if(columnName == m_DefaultHeader[i])
            return i;
    }
    for(int i = 0; i<m_Header.size(); ++i){
        if(columnName == m_Header[i])
            return i;
    }
    return -1;
}


void CSVReader::PrintAvailableHeaders()
{
    if(m_hasHeader){
        cout<<"Custom headers are (without brackets):"<<endl;
        for(const string & header : m_Header){
            cout<<"["<<header<<"] ";
        }
        cout<<endl;
        cout<<"Defaults headers are (without brackets):"<<endl;
    }
    else{
        cout<<"Your dataset does not have any header. Defaults headers are (without brackets):"<<endl;
    }
    
    for(const string & header : m_DefaultHeader){
        cout<<"["<<header<<"] ";
    }
    cout<<endl;
    
}

void CSVReader::GetSize()
{
    if(m_CSVTransposedArray.size()==0 || m_CSVTransposedArray.at(0).size()==0){
        cout<<"Your dataset is empty."<<endl;
    }
    else{
        long nColumns = m_CSVTransposedArray.size();
        long nRows = 0;
        for(const vector<string> & column : m_CSVTransposedArray){
            if(column.size() > nRows)
                nRows = column.size();
        }
        cout<<"Your dataset has ["<<nColumns<<"] columns, and ["<<nRows<<"] rows."<<endl;
    }
}

CSVReader::CSVTransposedArray CSVReader::TransposeCsv(const CSVArray & dataset)
{
    CSVTransposedArray transposedArray;
    if(dataset.size() != 0 && dataset.at(0).size() != 0){
        size_t nCols = dataset.at(0).size();
        size_t nRows = dataset.size();
        for(int i = 0; i < nCols; ++i){
            CSVColumn NewColumn;
            
            for(int j = 0; j<nRows; ++j){
                NewColumn.push_back(dataset.at(j).at(i));
            }
            transposedArray.push_back(NewColumn);
        }
    }
    return transposedArray;
}

void CSVReader::operation(const std::string & op, int column1, int column2)
{
    if(column1 >= m_CSVTransposedArray.size() || column2 >= m_CSVTransposedArray.size()){
        cout<<"Unable to perform operation. Column out of bounds."<<endl;
        return;
    }
    if(m_CSVTransposedArray[column1].size() != m_CSVTransposedArray[column2].size()){
        cout<<"Unable to perform operation. Columns do not have the same length."<<endl;
        return;
    }
    vector<string> dataColumn1 = m_CSVTransposedArray[column1];
    vector<string> dataColumn2 = m_CSVTransposedArray[column2];
    
    vector<vector<string>> newColumns;
    if(op == "+"){
        newColumns.push_back(operationPlus(dataColumn1,dataColumn2));
    }
    else if(op == "-"){
        newColumns.push_back(operationMinus(dataColumn1,dataColumn2));
    }
    else if(op == "*"){
        newColumns.push_back(operationMultiply(dataColumn1,dataColumn2));
    }
    else if(op == "/"){
        newColumns.push_back(operationDivide(dataColumn1,dataColumn2));
    }
    else if(op == "inner"){
        newColumns.push_back(operationInner(dataColumn1,dataColumn2));
    }
    else if(op == "outer"){
        newColumns=(operationOuter(dataColumn1,dataColumn2));
    }
    else{
        cout<<"Unable to perform operation. Wrong operator."<<endl;
    }
    if(newColumns.size()==0){
        cout<<"Operation failed."<<endl;
    }
    else if(newColumns.size()==1){
        if(newColumns[0].size() == 0){
            cout<<"Operation failed."<<endl;
        }
        else{
            cout<<"Operation successful, new column available with header [c"<<m_DefaultHeader.size()<<"]. ";
            string header = "";
            if(m_hasHeader){
                GetValidHeaderName(header, 1);
            }
            addNewDefaultHeader(header); //push back new header on header list.
            m_CSVTransposedArray.push_back(newColumns[0]);
            cout<<"Done !"<<endl;
        }
    }
    else if(newColumns.size() == 2){
        if(newColumns[0].size() == 0 || newColumns[1].size() == 0){
            cout<<"Operation failed."<<endl;
        }
        else{
            cout<<"Operation successful, new columns available with header [c"<<m_DefaultHeader.size()<<"] and [c"
            <<m_DefaultHeader.size()+1<<"]. ";
            string header1 = "";
            string header2 = "";
            if(m_hasHeader){
                GetValidHeaderName(header1, 1);
                addNewDefaultHeader(header1); //push back new header on header list.
                
                GetValidHeaderName(header2, 2);
                addNewDefaultHeader(header2); //push back new header on header list.
            }
            else{
                addNewDefaultHeader(header1);
                addNewDefaultHeader(header2);
            }
            m_CSVTransposedArray.push_back(newColumns[0]);
            m_CSVTransposedArray.push_back(newColumns[1]);
            cout<<"Done !"<<endl;
        }
    }
    else{
        cout<<"Something went wrong. Sorry, please try again."<<endl;
    }
}

std::vector<string> CSVReader::operationPlus(const std::vector<std::string> & column1, const std::vector<std::string> & column2)
{
    vector<string> newColumn;
    size_t lengthColumn = column1.size();
    
    for(int i = 0; i < lengthColumn; ++i)
    {
        try{
            double resultOfOperation;
            double lhs = stod(column1[i]);
            double rhs = stod(column2[i]);
            resultOfOperation = lhs + rhs;
            newColumn.push_back(to_string(resultOfOperation));
        }
        catch(...){
            cout<<"Unable to perform operation because some data line ["<<i
            <<"] could not be transformed in double. Value found are respectively ["
            <<column1[i]<<"] and ["
            <<column2[i]<<"]."
            <<endl;
            vector<string> badOperation;
            return badOperation;
        }
    }
    return newColumn;
}

std::vector<string> CSVReader::operationMinus(const std::vector<std::string> & column1, const std::vector<std::string> & column2)
{
    vector<string> newColumn;
    size_t lengthColumn = column1.size();
    
    for(int i = 0; i < lengthColumn; ++i)
    {
        try{
            double resultOfOperation;
            double lhs = stod(column1[i]);
            double rhs = stod(column2[i]);
            resultOfOperation = lhs - rhs;
            newColumn.push_back(to_string(resultOfOperation));
        }
        catch(...){
            cout<<"Unable to perform operation because some data line ["<<i
            <<"] could not be transformed in double. Value found are respectively ["
            <<column1[i]<<"] and ["
            <<column2[i]<<"]."<<endl;
            vector<string> badOperation;
            return badOperation;
        }
    }
    return newColumn;
    
}

std::vector<string> CSVReader::operationMultiply(const std::vector<std::string> & column1, const std::vector<std::string> & column2)
{
    vector<string> newColumn;
    size_t lengthColumn = column1.size();
    
    for(int i = 0; i < lengthColumn; ++i)
    {
        try{
            double resultOfOperation;
            double lhs = stod(column1[i]);
            double rhs = stod(column2[i]);
            resultOfOperation = lhs * rhs;
            newColumn.push_back(to_string(resultOfOperation));
        }
        catch(...){
            cout<<"Unable to perform operation because some data line ["<<i
            <<"] could not be transformed in double. Value found are respectively ["
            <<column1[i]<<"] and ["
            <<column2[i]<<"]."<<endl;
            vector<string> badOperation;
            return badOperation;
        }
    }
    return newColumn;
}

std::vector<string> CSVReader::operationDivide(const std::vector<std::string> & column1, const std::vector<std::string> & column2)
{
    vector<string> newColumn;
    size_t lengthColumn = column1.size();
    
    for(int i = 0; i < lengthColumn; ++i)
    {
        try{
            double resultOfOperation;
            double lhs = stod(column1[i]);
            double rhs = stod(column2[i]);
            if(rhs == 0){
                cout<<"This operation tried to divide row ["<<i<<"] by 0. The value at the output row will be changed to 0 instead."<<endl;
                resultOfOperation = 0;
            }
            else{
                resultOfOperation = lhs / rhs;
            }
            newColumn.push_back(to_string(resultOfOperation));
        }
        catch(...){
            cout<<"Unable to perform operation because some data line ["<<i
            <<"] could not be transformed in double. Value found are respectively ["
            <<column1[i]<<"] and ["
            <<column2[i]<<"]."<<endl;
            vector<string> badOperation;
            return badOperation;
        }
    }
    return newColumn;
}

std::vector<std::string> CSVReader::operationInner(const std::vector<std::string> & column1, const std::vector<std::string> & column2)
{
    vector<string> outputColumn;
    vector<double> sortedColumn1 = sortAndCheck(column1);
    vector<double> sortedColumn2 = sortAndCheck(column2);
    
    size_t sizeSortedColumn1 =sortedColumn1.size();
    size_t sizeSortedColumn2 =sortedColumn2.size();
    if(sizeSortedColumn1==0 || sizeSortedColumn2==0){
        vector<string> badOperation;
        return badOperation;
    }
    
    long indexColumn1 = 0;
    long indexColumn2 = 0;
    while(indexColumn1<sizeSortedColumn1 && indexColumn2<sizeSortedColumn2)
    {
        if(sortedColumn1[indexColumn1] == sortedColumn2[indexColumn2])
        {
            outputColumn.push_back(to_string(sortedColumn1[indexColumn1]));
            ++indexColumn1;
            ++indexColumn2;
        }
        else if(sortedColumn1[indexColumn1] < sortedColumn2[indexColumn2])
        {
            ++indexColumn1;
        }
        else if(sortedColumn1[indexColumn1] > sortedColumn2[indexColumn2])
        {
            ++indexColumn2;
        }
    }
    
    if(outputColumn.size() == 0){
        cout<<"No common values.";
    }
    
    return outputColumn;
}

vector< vector<string> > CSVReader::operationOuter(const std::vector<std::string> & column1, const std::vector<std::string> & column2)
{
    vector<vector<string>> outputColumn(2);
    vector<double> sortedColumn1 = sortAndCheck(column1);
    vector<double> sortedColumn2 = sortAndCheck(column2);
    
    size_t sizeSortedColumn1 =sortedColumn1.size();
    size_t sizeSortedColumn2 =sortedColumn2.size();
    if(sizeSortedColumn1==0 || sizeSortedColumn2==0){
        vector<vector<string>> badOperation;
        return badOperation;
    }
    
    long indexColumn1 = 0;
    long indexColumn2 = 0;
    while(indexColumn1<sizeSortedColumn1 && indexColumn2<sizeSortedColumn2)
    {
        if(sortedColumn1[indexColumn1] == sortedColumn2[indexColumn2])
        {
            outputColumn[0].push_back(to_string(sortedColumn1[indexColumn1]));
            outputColumn[1].push_back(to_string(sortedColumn1[indexColumn1]));
            ++indexColumn1;
            ++indexColumn2;
        }
        else if(sortedColumn1[indexColumn1] < sortedColumn2[indexColumn2])
        {
            outputColumn[0].push_back("null");
            ++indexColumn1;
        }
        else if(sortedColumn1[indexColumn1] > sortedColumn2[indexColumn2])
        {
            outputColumn[1].push_back("null");
            ++indexColumn2;
        }
    }
    
    if(outputColumn.size() == 0){
        cout<<"No common values.";
    }
    
    return outputColumn;
}

void CSVReader::addNewDefaultHeader(string customHeader)
{
    string newHeader = "c"+to_string(m_DefaultHeader.size());
    m_DefaultHeader.push_back(newHeader);
    
    if(m_hasHeader){
        if(customHeader == ""){
            customHeader = newHeader;
        }
        m_Header.push_back(customHeader);
    }
}

void CSVReader::statistics(int column)
{
    if(column >= m_CSVTransposedArray.size()){
        cout<<"Unable to perform statistics. Column out of bounds."<<endl;
        return;
    }
    
    vector<string> dataColumn = m_CSVTransposedArray[column];
    double min;
    double max;
    double average;
    double median;
    
    vector<double> sortedColumn = sortAndCheck(dataColumn);
    size_t sizeSortedColumn =sortedColumn.size();
    if(sizeSortedColumn==0)
        return;
    
    min = sortedColumn[0];
    max = sortedColumn[sizeSortedColumn-1];
    median = sizeSortedColumn%2==0 ?(sortedColumn[sizeSortedColumn/2-1]+sortedColumn[sizeSortedColumn/2])/2 : sortedColumn[sizeSortedColumn/2];
    for(double value : sortedColumn)
        average += value;
    average /= sizeSortedColumn;
    
    cout<<"Statistics on the selected column successful."<<endl;
    cout<<"Minimum    =    "<<min<<endl;
    cout<<"Maximum    =    "<<max<<endl;
    cout<<"Median     =    "<<median<<endl;
    cout<<"Average    =    "<<average<<endl;
}

vector<double> CSVReader::sortAndCheck(const vector<string> & dataColumn)
{
    //Performs bubble sort and check if data is double.
    //output empty vector if some data is not double in the set.
    size_t sizeDataset = dataColumn.size();
    vector<double> doubleColumn;
    
    for(long i = 0; i<sizeDataset; ++i)
    {
        try{
            doubleColumn.push_back(stod(dataColumn[i]));
        }
        catch(...){
            cout<<"Unable to perform operation because some data line ["<<i
            <<"] could not be transformed in double. Value found is ["
            <<dataColumn[i]<<"]."<<endl;
            vector<double> badOperation;
            return badOperation;
        }
    }
    
    mergeSort(doubleColumn.begin(),doubleColumn.end());
    return doubleColumn;
}

void CSVReader::mergeOperation(vector<double>::iterator startPoint, vector<double>::iterator midPoint, vector<double>::iterator endPoint)
{
    vector<double> tempVector(make_move_iterator(startPoint), make_move_iterator(endPoint));
    
    vector<double>::iterator beginAlt = tempVector.begin();
    vector<double>::iterator endAlt = tempVector.end();
    vector<double>::iterator midAlt = next(beginAlt, distance(startPoint, midPoint));
    
    vector<double>::iterator left = beginAlt;
    vector<double>::iterator right = midAlt;
    vector<double>::iterator index = startPoint;
    
    while(left<midAlt && right<endAlt)
    {
        *index = move((*left < *right) ? *left++ : *right++);
        ++index;
    }
    while(left<midAlt)
    {   *index = move(*left++);
        ++index;
    }
    while(right<endAlt)
    {   *index = move(*right++);
        ++index;
    }
}

void CSVReader::mergeSort(vector<double>::iterator beginPoint, vector<double>::iterator endPoint)
{
    size_t length = distance(beginPoint, endPoint);
    if (length < 2)
        return;
    size_t midPos = length/2;
    vector<double>::iterator midPoint = next(beginPoint, midPos);
    
    mergeSort(beginPoint, midPoint);
    mergeSort(midPoint, endPoint);
    mergeOperation(beginPoint, midPoint, endPoint);
}

void CSVReader::GetValidHeaderName(string & header, int col){
    while(1){
        cout<<"Please choose a new header for the "<<col<<" new column: ";
        getline(cin, header);
        if(isValidColumnName(header)){
            cout<<"This header is already taken."<<endl;
        }
        else if(header==""){
            cout<<"Please input a valid name (not empty string)."<<endl;
        }
        else{
            break;
        }
    }

}

vector<string> CSVReader::GetHeader()
{
    if(m_hasHeader)
    {
        return m_Header;
    }
    return m_DefaultHeader;
}







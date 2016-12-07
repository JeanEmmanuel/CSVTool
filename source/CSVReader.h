#ifndef CSVREADER_H
#define CSVREADER_H

#include <fstream>
#include <vector>

class CSVReader
{
public:
    typedef std::vector<std::string>            CSVLine;
    typedef std::vector<CSVLine>                CSVArray;

    typedef std::vector<std::string>            CSVColumn;
    typedef std::vector<CSVColumn>              CSVTransposedArray;

    CSVReader(){}
    CSVReader(std::ifstream *stream, bool header);
    ~CSVReader();
    
    /*
     *  Handles the parsing of CSV stream and destruction of heap allocated stream on success.
     */
    bool Initialization();
    
    /*
     *  Reads the stream and parses in vector<vector<string>>. Transposes the array at the end for easier processing.
     */
    bool ParseFileStream();
    
    /*
     *  Returns true if the header input is valid (exists in the list of custom or default headers)
     */
    bool isValidColumnName(const std::string &colName);
    
    /*
     *  Returns the position of the input header in the header list. This position is the position of column having the same name
     *  in m_CSVTransposedArray
     */
    int PositionInHeader(const std::string &columnName);
    
    /*
     *  Prints all available headers, default only if user said that the dataset had no header, otherwise default + custom
     */
    void PrintAvailableHeaders();
    
    /*
     *  Prints the maximum width and length of the CSV.
     */
    void GetSize();
    
    /*
     *  Calls the adequate function depending on the operator input by user.
     */
    void operation(const std::string & op, int column1, int column2);
    
    /*
     *  Performs and print statistics on a given column. Input has to be numeric only. Merge sort inside.
     */
    void statistics(int column);
    
    /*
     *  Add a new header. If data has no header, will only add default header, otherwise will add a default header and the custom header
     *  input by the user.
     */
    void addNewDefaultHeader(std::string customHeader);
    
    /*
     *  Returns the list of header available.
     */
    std::vector<std::string> GetHeader();
    
    /*
     *  Getter. Returns the CSV stored in the class.
     */
    CSVTransposedArray GetCsv() {return m_CSVTransposedArray;}
    
private:
    
    /*
     *  Returns the new column after performing "+" operation on the 2 chosen columns. Columns have to be both numeric only.
     */
    std::vector<std::string> operationPlus(const std::vector<std::string> & column1, const std::vector<std::string> & column2);
    
    /*
     *  Returns the new column after performing "-" operation on the 2 chosen columns. Columns have to be both numeric only.
     */
    std::vector<std::string> operationMinus(const std::vector<std::string> & column1, const std::vector<std::string> & column2);
    
    /*
     *  Returns the new column after performing "*" operation on the 2 chosen columns. Columns have to be both numeric only.
     */
    std::vector<std::string> operationMultiply(const std::vector<std::string> & column1, const std::vector<std::string> & column2);
    
    /*
     *  Returns the new column after performing "/" operation on the 2 chosen columns. Columns have to be both numeric only.
     */
    std::vector<std::string> operationDivide(const std::vector<std::string> & column1, const std::vector<std::string> & column2);
    
    /*
     *  Returns the new column after performing inner join operation on the 2 chosen columns. Columns have to be both numeric only.
     *  Merge sort inside.
     */
    std::vector<std::string> operationInner(const std::vector<std::string> & column1, const std::vector<std::string> & column2);
    
    /*
     *  Returns the 2 new columns after performing outer join operation on the 2 chosen columns. Columns have to be both numeric only.
     *  Merge sort inside.
     */
    std::vector< std::vector<std::string> > operationOuter(const std::vector<std::string> & column1, const std::vector<std::string> & column2);
    
    /*
     *  Checks if all values are numeric. If so, calls merge sort on the input column and output the result of the merge sort, as vector<double>
     */
    std::vector<double> sortAndCheck(const std::vector<std::string> & dataColumn);
    
    /*
     *  Asks user to input a header, and checks its validity (does not exist already). Value input by user replaces header (reference).
     */
    void GetValidHeaderName(std::string & header, int col);

    /*
     *  Merging operation of the merge sort algorithm. Will modify the iterators.
     */
    void mergeOperation(std::vector<double>::iterator beginPoint, std::vector<double>::iterator midPoint, std::vector<double>::iterator endPoint);
    
    /*
     *  Splitter in merge sort algorithm.
     */
    void mergeSort(std::vector<double>::iterator beginPoint, std::vector<double>::iterator endPoint);

    
    /*
     *  Transposes the input dataset and output the results.
     */
    CSVTransposedArray TransposeCsv(const CSVArray & dataset);
    
private:

    std::ifstream *             m_FileStream = NULL;
    std::vector<std::string>    m_Header;
    std::vector<std::string>    m_DefaultHeader;
    CSVTransposedArray          m_CSVTransposedArray;
    bool                        m_hasHeader;
    
};

#endif // CSVREADER_H

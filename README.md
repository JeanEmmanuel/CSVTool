--------------------------------------------------

# CSVTool v1.0
Jean-Emmanuel Losi, jean.e.losi@gmail.com

--------------------------------------------------

This tool performs some simple operations on a CSV.

help                             print all available command line operations.
version                          print the current application's version.
exit                             stops the application.
new                              input a new csv file.
save                             saves the file in the executable's directory.
statistics [header]              output some statistics about the selected column.
[operator] [lhs_header] [rhs_header]   selected operator on [lhs_header] and [rhs_header].
operators                        print all available operators on columns.
headers                          print all available column names.
size                             returns the size of the dataset.

--------------------------------------------------

A test CSV has been included, this data is the daily prices of the AAPL stock during the last 35 years, where each column has been concatenated several time with itself.
This data is meaningless and only for test purposes, to test the speed of the algorithms<
Number of rows of this dataset is around 100k lines.

--------------------------------------------------

This tool needs to be build with C++11.

On MacOSX with clang++ (tested with the latest MacOSX and Xcode updates):
clang++ -std=c++11 -stdlib=libc++ *.cpp -o CSVTool

On windows with g++ (tested on g++ (i686-posix-dwarf-rev0, Built by MinGW-W64 project) 5.3.0):
g++ -std=c++11 *.cpp -o CSVTool

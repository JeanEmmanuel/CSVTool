/*
 * CSVTool v1.0
 * Command line app to do simple operations on a CSV file, such as column operations, and some statistics.
 * Creator Jean-Emmanuel Losi, jean.e.losi@gmail.com
 */

// /Users/jean-emmanuellosi/anaconda/pkgs/matplotlib-1.4.3-np19py27_1/lib/python2.7/site-packages/matplotlib/mpl-data/sample_data/aapl.csv
// /Users/jean-emmanuellosi/Desktop/Investment strategy/Intro to maths fi/75f2a00816031277.csv
// /Users/jean-emmanuellosi/Downloads/table.csv

#include <iostream>
#include <string>
#include "Interface.h"

using namespace std;

int main(int argc, char *argv[])
{
    Interface interface;
    interface.Initialization();
    interface.CmdLine();
    return 0;
}

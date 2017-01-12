/*
 * CSVTool v1.0
 * Command line app to do simple operations on a CSV file, such as column operations, and some statistics.
 * Creator Jean-Emmanuel Losi, jean.e.losi@gmail.com
 */

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

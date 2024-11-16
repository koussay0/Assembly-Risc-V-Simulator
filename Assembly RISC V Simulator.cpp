#include "simulator.h"
#include "Radix.h"
#include <iostream>

using namespace std;

int main()
{   
    simulator riscsimulator;
    string file_name;
    cout << "please enter name of the file that will be tested : \n";
    cin >> file_name;
    cout << "\n";
    riscsimulator.loadProgram(file_name);

    return 0;
}
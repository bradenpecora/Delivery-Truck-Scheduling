#include <iostream>
#include "classes.h"
#include <fstream>

using std::cin;
using std::cout;
using std::endl;
using std::ifstream;


int main () {

    Route list;
    
    // while (true)
    // {
    //     cin >> x >> y;
    //     cout << "Entered Address: (" << x << "," << y << ")" << endl;
    //     if(x != 0 || y != 0){
    //         list.addAddress(x,y);
    //     }
    //     else{
    //         break;
    //     }
    // }
    


    //multiOpt2 Demonstration
    Route input;

    ifstream file("multiOpt2TestCase.txt");
    float x,y;
    while(file >> x >> y){
        if(x != 0 || y != 0){
            input.addAddress(x,y);
        }
        else{
            break;
        }
    }
    
    bool manhattan = false;
 
    vector<Route> origSplit = input.splitRoute(2);
    origSplit.at(0).opt2Route();
    origSplit.at(1).opt2Route();
    vector<Route> split = input.twoTruckOpt2(manhattan);

    cout << "Original Route:" << endl;
    origSplit.at(0).print(); cout << origSplit.at(0).length(manhattan) << endl;
    origSplit.at(1).print(); cout << origSplit.at(1).length(manhattan) << endl;
    cout << "Original Length: " << list.twoTruckLength(origSplit, manhattan) << endl << endl <<endl;

    cout << "New Route:" << endl;
    split.at(0).print(); cout << split.at(0).length(manhattan) << endl;
    split.at(1).print(); cout << split.at(1).length(manhattan) << endl;
    cout << "New Length: " << list.twoTruckLength(split, manhattan) << endl;

    // cout << endl;
    // opt2.print();
    // cout << endl;
    // cout << greedy.length(manhattan) << endl;
    // cout << opt2.length(manhattan) << endl;

    return 0;

}
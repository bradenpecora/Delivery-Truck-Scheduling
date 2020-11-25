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
    
    ifstream file("test.txt");
    float x,y;
    while(file >> x >> y){
        if(x != 0 || y != 0){
            list.addAddress(x,y);
        }
        else{
            break;
        }
    }
    

    list.print();
    cout << endl;
    Route greedy = list.greedyRoute();
    Route opt2 = list.opt2Route();
    greedy.print();
    cout << endl;
    opt2.print();
    cout << endl;
    cout << greedy.length() << endl;
    cout << opt2.length() << endl;

    return 0;

}
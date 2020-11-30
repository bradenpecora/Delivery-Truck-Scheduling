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
    
    ifstream file("data.txt");
    float x,y;
    while(file >> x >> y){
        if(x != 0 || y != 0){
            list.addAddress(x,y);
        }
        else{
            break;
        }
    }
    

    // list.print();
    cout << endl;
    bool manhattan = false;
    // Route greedy = list.greedyRoute(manhattan);
    // Route opt2 = list.opt2Route(manhattan);
    // double oldLength;
    // do{
    //     oldLength = opt2.length(manhattan);
    //     cout << opt2.length(manhattan) << endl;
    //     opt2 = opt2.opt2Route(manhattan);
    // }while(oldLength > opt2.length(manhattan));
    // greedy.print();
    vector<Route> split = list.multiOpt2(manhattan);
    cout << endl;
    // opt2.print();
    // cout << endl;
    // cout << greedy.length(manhattan) << endl;
    // cout << opt2.length(manhattan) << endl;

    return 0;

}
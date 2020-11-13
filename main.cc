#include <iostream>
#include "classes.h"

using std::cin;
using std::cout;
using std::endl;

int main () {

    int x,y;
    Route list;
    
    while (true)
    {
        cin >> x >> y;
        cout << "Entered Address: (" << x << "," << y << ")" << endl;
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
    
    
    cout << greedy.length() << endl;

    return 0;

}
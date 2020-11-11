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
        if(x != 0 && y != 0){
            Address newHouse(x,y);
            list.addAddress(newHouse);
        }
        else{
            break;
        }
    }

    list.print();
    
    cout << list.length() << endl;

    return 0;

}
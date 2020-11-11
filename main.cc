#include <iostream>
using std::cin;
using std::cout;
using std::endl;

class Address{
    private:
        int x, y; // Cartesian coordinates of address
    public:
        Address(int i, int j): x(i), y(j){};
        
        int getX(){
            return x;
        }
        
        int getY(){
            return y;
        }

        double distance(Address house){
            return (x-house.getX()) + (y-house.getY());
        }
};

int main () {

    Address start(0,0);
    Address one(3,4);
    cout << one.distance(start) << endl;

    return 0;

}
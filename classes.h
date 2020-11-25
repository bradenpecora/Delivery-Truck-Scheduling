#ifndef CLASSES
#define CLASSES

#include <limits>
#include <vector>
//#include <list>
#include <iostream>
#include <cmath>
using std::cin;
using std::cout;
using std::endl;
using std::vector;
//using std::list;
using std::numeric_limits;


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

        /**
         * Finds the distance between the current Address and house.
         * Can be Manhattan distance (sum of x distance and y distance)
         * or direct distance (distance formula)
         * 
         * @param house Address to be compared to
         * @param manhattan flag that indicates type of distance to return
         * @return distance between current Address and house
         */
        double distance(Address house, bool manhattan=true){
            if(manhattan){
                return (abs(x-house.getX())) + (abs(y-house.getY()));
            }
            else{
                return sqrt(pow(x-house.getX(),2) + pow(y-house.getY(),2));
            }
            return 0;
        }

        void print () {
            cout << "(" << getX() << "," << getY() << ") ";
        }

        
};

class AddressList{
    protected:
        vector<Address> addresses;
    public:
        AddressList(){};

        void addAddress(Address newHouse){
            bool isEqual = false;
            for(auto house : addresses){
                if (house.distance(newHouse) == 0){
                    isEqual = true;
                    break;
                }
            }
            if(!isEqual) {
                addresses.push_back(newHouse);
            }

        }

        void addAddress(int x, int y){
            Address newHouse(x,y);
            addAddress(newHouse);
        }

        void removeAddress(int i){
            addresses.erase(addresses.begin() + i);
        }

        void swapAddresses(int i,int j){
            Address temp = addresses.at(i);
            addresses.at(i) = addresses.at(j);
            addresses.at(j) = temp;
        }

        int addressesSize(){
            return addresses.size();
        }

        Address at(int i){
            return addresses.at(i);
        }

        /**
         * Finds the distance traveled by a truck that visits
         * each Address in addresses, in order.
         * 
         * @return the total distance 
         */
        double length(bool manhattan=true){
            double result = 0;
            for(int i = 0; i < addresses.size()-1; i++){
                result += addresses.at(i).distance(addresses.at(i+1), manhattan);
            }
            return result;
        }

        /**
         * Finds the index of the Address in addresses that is closest
         * to house.
         * 
         * @param house the address that is being compared to 
         * @return the index of Address in addresses 
         */
        int indexClosestTo(Address house, bool manhattan=true){
            double minDist = numeric_limits<int>::max();
            int index = 0;
            for(int i = 0; i < addresses.size(); i++){
                if (house.distance(addresses.at(i), manhattan) < minDist){
                    index = i;
                    minDist = house.distance(addresses.at(i), manhattan);
                }
            }
            return index;
        }

        void print () {
            for (auto house : addresses){
                house.print();
            }
        }
};

class Route : public AddressList{
    private:
        Address depot;
    public:
        Route(): depot(0,0) {
            addresses.push_back(depot);
        }
        
        double length(bool manhattan=true){
            return AddressList::length(manhattan) + depot.distance(addresses.back(), manhattan);
        }

        void print(){
            AddressList::print();
            depot.print();
        }

        Route greedyRoute(bool manhattan=true){
            Route copy = *this;
            Route greedy;
            Address currentLoc = depot;
            int i;
            while(copy.addressesSize() > 0){
                i = copy.indexClosestTo(currentLoc, manhattan);
                currentLoc = copy.at(i);
                greedy.addAddress(currentLoc);
                copy.removeAddress(i);
            }
            return greedy;
        }
        
        Route opt2Route(bool manhattan=true){
            Route opt2 = greedyRoute();
            for(int i = 0; i < addresses.size()-1; i++){
                Route test = opt2;
                test.swapAddresses(i, i+1);
                if(test.length(manhattan) < opt2.length(manhattan)){
                    opt2 = test;
                }
            }
            return opt2;
        }
        
};


#endif
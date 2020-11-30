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
                
        void replaceAddress(int i, Address toSwap){
            addresses.at(i) = toSwap;
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
            Route opt2 = greedyRoute(manhattan);
            int swapcount = 0;
            for(int i = 0; i < addresses.size()-1; i++){
                // Route test = opt2;
                // test.swapAddresses(i, i+1);
                // if(test.length(manhattan) < opt2.length(manhattan)){
                //     opt2 = test;
                //     swapcount++;
                // }
                double origLength = opt2.length(manhattan);
                opt2.swapAddresses(i, i+1);
                if (!(opt2.length(manhattan) < origLength))
                {
                    opt2.swapAddresses(i, i + 1);
                    
                }
                else{
                    swapcount++;
                }
            }
            //cout << swapcount << endl;
            return opt2;
        }
        /**
         * Splits this AddressList into n routes, each of size addresses.size()/n
         * 
         * @param n - number of trucks
         * @param manhattan 
         * @return Route 
         */
        vector<Route> splitRoute(int n){
            vector<Route> routes(n);
            int i = 0;
            for(Address house: addresses){
                routes[i].addAddress(house);
                i++;
                i = i%n;
            }

            return routes;
        }
        double twoTruckLength(vector<Route> truckPaths, bool manhattan=true){
            return truckPaths[0].length(manhattan) + truckPaths[1].length(manhattan);
        }
        
        void directSwap(vector<Route> &truckPaths, int i){
            Address temp = truckPaths[0].at(i);
            truckPaths[0].replaceAddress(i, truckPaths[1].at(i));
            truckPaths[1].replaceAddress(i, temp);
        }
        
        void diagonalSwap1(vector<Route> &truckPaths, int i){
            Address temp = truckPaths[0].at(i);
            truckPaths[0].replaceAddress(i, truckPaths[1].at(i+1));
            truckPaths[1].replaceAddress(i+1, temp);
        }

        void diagonalSwap2(vector<Route> &truckPaths, int i){
            Address temp = truckPaths[1].at(i);
            truckPaths[1].replaceAddress(i, truckPaths[0].at(i+1));
            truckPaths[0].replaceAddress(i+1, temp);
        }
        
        vector<Route> multiOpt2(bool manhattan=true){
            vector<Route> truckPaths = splitRoute(2);
            for(Route truckPath: truckPaths){
                truckPath.opt2Route(manhattan);
            }
            truckPaths[0].print();
            cout << truckPaths[0].length(manhattan);
            cout << endl;
            truckPaths[1].print();
            cout << truckPaths[1].length(manhattan);
            cout << endl;
            for(int i = 1; i < addresses.size()/2; i++){
                int shortestPath = 0;
                double origLength = twoTruckLength(truckPaths, manhattan);
                double shortestLength = origLength;

                Address temp = truckPaths[0].at(i);

                directSwap(truckPaths,i);
                double directSwapLength = twoTruckLength(truckPaths, manhattan);
                if (directSwapLength < shortestLength){
                    shortestLength = directSwapLength;
                    shortestPath = 1;
                }

                // revert to original routes
                truckPaths[1].replaceAddress(i, truckPaths[0].at(i));
                truckPaths[0].replaceAddress(i, temp);

                if(i+1 < truckPaths[1].addressesSize()){
                    diagonalSwap1(truckPaths, i);
                    double diagonalSwap1 = twoTruckLength(truckPaths, manhattan);

                    if (diagonalSwap1 < shortestLength){
                        shortestLength = diagonalSwap1;
                        shortestPath = 2;
                    }

                    // revert to original routes
                    truckPaths[1].replaceAddress(i+1, truckPaths[0].at(i));
                    truckPaths[0].replaceAddress(i, temp);
                }
                if(i+1 < truckPaths[0].addressesSize()){
                    temp = truckPaths[1].at(i);
                    diagonalSwap2(truckPaths, i);
                    double diagonalSwap2 = twoTruckLength(truckPaths, manhattan);

                    if (diagonalSwap2 < shortestLength){
                        shortestLength = diagonalSwap2;
                        shortestPath = 3;
                    }
                
                    // revert to original routes
                    truckPaths[0].replaceAddress(i+1, truckPaths[1].at(i));
                    truckPaths[1].replaceAddress(i, temp);
                    if(i+1 < truckPaths[1].addressesSize()){
                        temp = truckPaths[0].at(i+1);
                        directSwap(truckPaths, i+1);
                        double directSwap2 = twoTruckLength(truckPaths, manhattan);

                        if (directSwap2 < shortestLength){
                            shortestLength = directSwap2;
                            shortestPath = 4;
                        }

                        // revert to original routes
                        truckPaths[1].replaceAddress(i+1, truckPaths[0].at(i+1));
                        truckPaths[0].replaceAddress(i+1, temp);
                    }
                }
                
                switch(shortestPath){
                    case 0:
                        break;
                    case 1:
                        directSwap(truckPaths,i);
                        break;
                    case 2:
                        diagonalSwap1(truckPaths,i);
                        break;
                    case 3:
                        diagonalSwap2(truckPaths,i);
                        break;
                    case 4:
                        directSwap(truckPaths,i+1);
                        break;
                }
            }

            cout << endl;
            truckPaths[0].print();
            cout << truckPaths[0].length(manhattan);
            cout << endl;
            truckPaths[1].print();
            cout << truckPaths[1].length(manhattan);
            cout << endl;

            return truckPaths;
        }
        
};


#endif
#ifndef CLASSES
#define CLASSES

#include <limits>
#include <vector>
//#include <list>
#include <iostream>
#include <cmath>
#include <algorithm>
using std::cin;
using std::cout;
using std::endl;
using std::vector;
using std::reverse;
//using std::list;
using std::numeric_limits;
using std::max;
using std::min;

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
        
        void reverseAddresses(int start, int numToSwap){
            reverse(addresses.begin()+start, addresses.begin()+start+numToSwap);
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
            int maxToSwap = max((int)floor(0.1*opt2.addressesSize()), 1);
            // cout << maxToSwap << endl;
            for(int numToSwap = 1; numToSwap < maxToSwap; numToSwap++){
                for(int i = 1; i < addresses.size()-numToSwap; i++){
                    Route test = opt2;
                    test.reverseAddresses(i, numToSwap);
                    if(test.length(manhattan) < opt2.length(manhattan)){
                        opt2 = test;
                        swapcount++;
                    }
                }
                // cout << numToSwap << endl;
            }
            // cout << endl << swapcount << endl;
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
                routes.at(i).addAddress(house);
                i++;
                i = i%n;
            }

            return routes;
        }

        double twoTruckLength(vector<Route> truckPaths, bool manhattan=true){
            return truckPaths.at(0).length(manhattan) + truckPaths.at(1).length(manhattan);
        }

        void swapTwoPortions(vector<Route> &truckPaths, int start1, int start2, int numToSwap, bool manhattan = true){
            Route copy1 = truckPaths.at(0);
            for(int i = 0; i < numToSwap; i++){
                truckPaths.at(0).replaceAddress(start1 + i, truckPaths.at(1).at(start2 + i));
                truckPaths.at(1).replaceAddress(start2 + i, copy1.at(start1 + i));
            }
        }

        vector<Route> twoTruckOpt2(bool manhattan = true){
            vector<Route> truckPaths = splitRoute(2);
            for(Route truckPath: truckPaths){
                truckPath.opt2Route(manhattan);
            }
            
            int maxToSwap = max((int)floor(0.1*addresses.size()/2), 2);
            cout << "MaxToSwap: " << maxToSwap << endl;
            double origLength = twoTruckLength(truckPaths, manhattan);
            for(int lengthToSwap = maxToSwap; lengthToSwap > 0; lengthToSwap--){
                for(int path1 = 1; path1 <= truckPaths.at(0).addressesSize()-lengthToSwap; path1++){
                    for(int path2 = 1; path2 <= truckPaths.at(1).addressesSize()-lengthToSwap; path2++){
                        vector<Route> swapCopy = truckPaths;
                        vector<Route> reverseFirst = truckPaths;
                        vector<Route> reverseSecond = truckPaths;
                        vector<Route> reverseBoth(2);

                        reverseFirst.at(0).reverseAddresses(path1, lengthToSwap);
                        reverseSecond.at(1).reverseAddresses(path2, lengthToSwap);
                        reverseBoth.at(0) = reverseFirst.at(0);
                        reverseBoth.at(1) = reverseSecond.at(1);

                        swapTwoPortions(swapCopy, path1, path2, lengthToSwap, manhattan);
                        swapTwoPortions(reverseFirst, path1, path2, lengthToSwap, manhattan);
                        swapTwoPortions(reverseSecond, path1, path2, lengthToSwap, manhattan);
                        swapTwoPortions(reverseBoth, path1, path2, lengthToSwap, manhattan);

                        double swapCopyLength = twoTruckLength(swapCopy,manhattan);
                        double reverseFirstLength = twoTruckLength(reverseFirst, manhattan);
                        double reverseSecondLength = twoTruckLength(reverseSecond, manhattan);
                        double reverseBothLength = twoTruckLength(reverseBoth, manhattan);

                        double minLength = min({swapCopyLength, reverseFirstLength, reverseSecondLength, reverseBothLength, origLength});
                        if(minLength == swapCopyLength){
                            truckPaths = swapCopy;
                        }else if (minLength == reverseFirstLength){
                            truckPaths = reverseFirst;
                        }else if (minLength == reverseSecondLength){
                            truckPaths = reverseSecond;
                        }else if (minLength == reverseBothLength){
                            truckPaths = reverseBoth;
                        }
                    }
                }
            }
            // for(Route truckPath: truckPaths){
            //     truckPath.opt2Route(manhattan);
            // }
            return truckPaths;
        }

        
};


#endif
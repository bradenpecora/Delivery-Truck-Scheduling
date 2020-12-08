#ifndef CLASSES
#define CLASSES

#include <limits>
#include <vector>
//#include <list>
#include <iostream>
#include <cmath>
#include <algorithm>
#include <fstream>
#include <string>
using std::cin;
using std::cout;
using std::endl;
using std::vector;
using std::reverse;
//using std::list;
using std::numeric_limits;
using std::max;
using std::min;
using std::string;
using std::ofstream;

class Route;
double twoTruckLength(vector<Route> truckPaths, bool manhattan);
void twoRouteOutput(vector<Route> truckPaths, bool manhattan);

class Address{
    protected:
        int x, y; // Cartesian coordinates of address
        bool prime;
    public:
        Address(int i, int j): x(i), y(j), prime(false){};
        Address(int i, int j, bool isPrime): x(i), y(j), prime(isPrime){};
        
        int getX(){
            return x;
        }
        
        int getY(){
            return y;
        }
        
        bool isPrime(){
            return prime;
        }

        void changePrimeTo(bool isPrime){
            prime = isPrime;
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

        void printToFile(ofstream &output, bool printPrime = false) {
            output << getX() << " " << getY();
            if(printPrime){
                output << " " << isPrime();
            }
            output << endl;
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

        void addAddress(int x, int y, bool prime){
            Address newHouse(x,y,prime);
            addAddress(newHouse);
        }

        void insertAddress(int index, Address newAddress){
            addresses.insert(addresses.begin() + index, newAddress);
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
        
        Route opt2Route(bool manhattan=true, float percentToReverse=0.15){
            Route opt2 = greedyRoute(manhattan);
            int swapcount = 0;
            int maxToReverse = max((int)floor(percentToReverse*opt2.addressesSize()), 1);
            for(int numToReverse = maxToReverse; numToReverse > 0; numToReverse--){
                for(int i = 1; i < addresses.size()-numToReverse; i++){
                    Route test = opt2;
                    test.reverseAddresses(i, numToReverse);
                    if(test.length(manhattan) < opt2.length(manhattan)){
                        opt2 = test;
                        swapcount++;
                    }
                }
            }
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

        void swapTwoPortions(vector<Route> &truckPaths, int start1, int start2, int numToSwap, bool manhattan = true){
            Route copy1 = truckPaths.at(0);
            for(int i = 0; i < numToSwap; i++){
                if(!truckPaths.at(0).at(start1 + i).isPrime() && !truckPaths.at(1).at(start2 + i).isPrime()){
                    truckPaths.at(0).replaceAddress(start1 + i, truckPaths.at(1).at(start2 + i));
                    truckPaths.at(1).replaceAddress(start2 + i, copy1.at(start1 + i));
                }
            }
        }

        vector<Route> twoTruckOpt2(bool manhattan = true, float percentToSwap = 0.15){
            vector<Route> truckPaths = splitRoute(2);
            for(int truckPath = 0; truckPath < truckPaths.size(); truckPath++){
                truckPaths.at(truckPath) = truckPaths.at(truckPath).opt2Route(manhattan, percentToSwap);
            }
            
            int maxToSwap = max((int)floor(percentToSwap*addresses.size()/2), 1);
            for(int lengthToSwap = maxToSwap; lengthToSwap > 0; lengthToSwap--){
                vector<Route> minPath = truckPaths;
                for(int path1 = 1; path1 <= truckPaths.at(0).addressesSize()-lengthToSwap; path1++){
                    vector<Route> minTestPath = minPath;
                    for(int path2 = 1; path2 <= truckPaths.at(1).addressesSize()-lengthToSwap; path2++){
                        double origLength = twoTruckLength(truckPaths, manhattan);

                        vector<Route> swap = truckPaths;
                        vector<Route> reverseFirst = truckPaths;
                        vector<Route> reverseSecond = truckPaths;
                        vector<Route> reverseBoth(2);

                        reverseFirst.at(0).reverseAddresses(path1, lengthToSwap);
                        reverseSecond.at(1).reverseAddresses(path2, lengthToSwap);
                        reverseBoth.at(0) = reverseFirst.at(0);
                        reverseBoth.at(1) = reverseSecond.at(1);

                        swapTwoPortions(swap, path1, path2, lengthToSwap, manhattan);
                        swapTwoPortions(reverseFirst, path1, path2, lengthToSwap, manhattan);
                        swapTwoPortions(reverseSecond, path1, path2, lengthToSwap, manhattan);
                        swapTwoPortions(reverseBoth, path1, path2, lengthToSwap, manhattan);

                        double swapLength = twoTruckLength(swap,manhattan);
                        double reverseFirstLength = twoTruckLength(reverseFirst, manhattan);
                        double reverseSecondLength = twoTruckLength(reverseSecond, manhattan);
                        double reverseBothLength = twoTruckLength(reverseBoth, manhattan);

                        double minLength = min({swapLength, reverseFirstLength, reverseSecondLength, reverseBothLength, origLength});
                        //cout << minLength << endl;
                        if(minLength == swapLength){
                            minTestPath = swap;
                        }else if (minLength == reverseFirstLength){
                            minTestPath = reverseFirst;
                        }else if (minLength == reverseSecondLength){
                            minTestPath = reverseSecond;
                        }else if (minLength == reverseBothLength){
                            minTestPath = reverseBoth;
                        }
                    }
                    if(twoTruckLength(minPath, manhattan) > twoTruckLength(minTestPath, manhattan)){
                        minPath = minTestPath;
                    }
                }
                if(twoTruckLength(truckPaths, manhattan) > twoTruckLength(minPath, manhattan)){
                    truckPaths = minPath;
                }
            }
            for(int truckPath = 0; truckPath < truckPaths.size(); truckPath++){
                truckPaths.at(truckPath) = truckPaths.at(truckPath).opt2Route(manhattan, percentToSwap);
            }
            
            return truckPaths;
        }

        vector<Route> addBeforeSplittingRoutes(AddressList newAddresses, bool manhattan = true, double percentToReverse = 0.15){
            for(int i = 0; i < newAddresses.addressesSize(); i++){
                addresses.push_back(newAddresses.at(i));
            }
            return twoTruckOpt2(manhattan, percentToReverse);
        }

        void saveRouteToFile(string fileName, bool printPrime = false) {
            fileName = "routes/" + fileName + ".txt";
            ofstream output;
            output.open(fileName);
            for(int i = 0; i < addresses.size(); i++){
                addresses.at(i).printToFile(output, printPrime);
            }
            depot.printToFile(output,printPrime);
            output.close();
        }

        
};

vector<Route> addToExistingRoutes(vector<Route> paths, AddressList newAddresses, bool manhattan = true, double percentToReverse = 0.15){
    for(int i = 0; i < newAddresses.addressesSize(); i++){
        Address house = newAddresses.at(i);
        Route route1 = paths.at(0);
        Route route2 = paths.at(1);
        double origDist1 = route1.length(manhattan);
        double origDist2 = route2.length(manhattan);

        int index1 = route1.indexClosestTo(house);
        int index2 = route2.indexClosestTo(house);
        route1.insertAddress(index1, house);
        route2.insertAddress(index2, house);

        route1 = route1.opt2Route(manhattan, percentToReverse);
        route2 = route2.opt2Route(manhattan, percentToReverse);
        double newDist1 = route1.length(manhattan);
        double newDist2 = route2.length(manhattan);

        if(newDist1 - origDist1 < newDist2 - origDist2){
        // if(newDist1 < newDist2 ){
            paths.at(0) = route1;
        }else{
            paths.at(1) = route2;
        }
    }
    return paths;
}

double twoTruckLength(vector<Route> truckPaths, bool manhattan=true){
            return truckPaths.at(0).length(manhattan) + truckPaths.at(1).length(manhattan);
        }

void twoRouteOutput(vector<Route> truckPaths, bool manhattan=true){
    truckPaths.at(0).print(); cout << "Length: " << truckPaths.at(0).length(manhattan) << endl;
    truckPaths.at(1).print(); cout << "Length: " << truckPaths.at(1).length(manhattan) << endl;
    cout << "Total Length: " << twoTruckLength(truckPaths, manhattan) << endl << endl;
}

void saveTwoRoutesToFile(vector<Route> paths, string fileName, bool printPrime = false){
            string path1 =  fileName + "1";
            string path2 =  fileName + "2";
            paths.at(0).saveRouteToFile(path1, printPrime);
            paths.at(1).saveRouteToFile(path2, printPrime);
        }

#endif
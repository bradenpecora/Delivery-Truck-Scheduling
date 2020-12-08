#ifndef CLASSES
#define CLASSES

#include <limits>
#include <vector>
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
        bool prime; //Amazon 'Prime' status of an address. Defaults to false if not specified.
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

        /**
         * @brief Prints an address to a file in the format:
         * 
         * x y prime
         * 
         * 'prime' is not printed if printPrime is false.
         * 
         * @param output - output file stream of file.
         * @param printPrime - The Amazon 'Prime' status of an addresses is printed if true.
         */
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

/**
 * Derived from Address list. Accounts for the fact that a route must start and end at the
 * Depot, which is located at (0,0). Methods that account for the depot, methods that
 * can optimize the route, and methods that can split the route are contained within 
 * this class.
 * 
 */
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

        /**
         * @brief Quickly creates a 'greedy construction' of a route.
         * 
         * Starting with a 'currentLoc' (current Location) at depot ((0,0)), a new Route is incrementally 
         * generated by finding the address in a copy of *this closest to the currentLoc. As an address is 
         * added to the new Route, it is removed from the copy of *this and and established as the currentLoc.
         * This proccess is continued until there are no longer any addrresses in the copy of *this.
         * 
         * @param manhattan 
         * @return Route 
         */
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
        
        /**
         * @brief Optimizes a route based on the 'opt2' heuristic method: A path that 'crosses' itself 
         * can be made shorter by reversing part of it.
         * 
         * Method checks to see if reversing a number of addresses foward from all possible addresses 
         * in the route decreases the length of the route. If doing so decreases the length of the route,
         * those addresses are reversed in the master route.
         * 
         * The method checks all integer 'number of routes to reverse' from the percentToReverse*addressSize to 1.
         * 
         * Method begins with a greedy construction of the addresses to serve as a starting point.
         * 
         * @param manhattan 
         * @param percentToReverse - the maximum percent of the list that will be swapped. 
         * Increasing percentToReverse increases runtime.
         * @return Route 
         */
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
         * @return routes - A vector containing n routes
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

        /**
         * @brief Exchanges numToSwap addresses between the first route and second routes in truckPaths
         * 
         * Addresses start1 through start1 + numToSwap in the first route are exchanged with 
         * addresses start2 through start2 + numToSwap in the second route.
         * Addresses are not swapped if the address has Amazon 'Prime' status.
         * 
         * @param truckPaths - input vector that contains two routes
         * @param start1 - starting index for the first route in the vector
         * @param start2 - starting index for the second route in the vector
         * @param numToSwap - number of addresses to be swapped between routes.
         * @param manhattan 
         */
        void swapTwoPortions(vector<Route> &truckPaths, int start1, int start2, int numToSwap, bool manhattan = true){
            Route copy1 = truckPaths.at(0);
            for(int i = 0; i < numToSwap; i++){
                if(!truckPaths.at(0).at(start1 + i).isPrime() && !truckPaths.at(1).at(start2 + i).isPrime()){
                    truckPaths.at(0).replaceAddress(start1 + i, truckPaths.at(1).at(start2 + i));
                    truckPaths.at(1).replaceAddress(start2 + i, copy1.at(start1 + i));
                }
            }
        }

        /**
         * @brief Exchanges several portions of a route in an attempt to optimize the two routes.
         * 
         * The method swaps a portion of one route with another for every possible combination of length of the route to swap (lengthToSwap)
         * and possible starting indicies for each route with which the swap is made foward from (path1 and path2, respectively). If a given
         * combination decreases the length of the route, the respective swap is applied to the master vector of routes.
         * 
         * The maximum lengthToSwap is based on the perctange of the route the user wishes to swap. This value is defauled to 15%.
         * 
         * The method assumes that the addresses within the instance vector are in a random order. The method begins by assigning
         * every other address in the instance vector to one route and the rest to the other.
         * 
         * After the addresses are split into two routes, the opt2Route method is run on each route to serve as a starting point
         * for this method. 
         * 
         * @param manhattan 
         * @param percentToSwap - Determines the maximum amount of addresses that can be swapped between addresses.
         * Increasing this value increases runtime.
         * @return vector<Route> - Contains the two optimized routes.
         */
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

        /**
         * @brief Adds the addresses within 'newAddresses' to the instance vector 'addresses' that are intended to be split between two routes.
         * The twoTruckOpt2 method is ran to create and optimize the two routes with the appended addresses.
         * 
         * @param newAddresses - an AddressList containing additional addresses
         * @param manhattan 
         * @param percentToSwap - see twoTruckOpt2 method.
         * @return vector<Route> 
         */
        vector<Route> addBeforeSplittingRoutes(AddressList newAddresses, bool manhattan = true, double percentToSwap = 0.15){
            for(int i = 0; i < newAddresses.addressesSize(); i++){
                addresses.push_back(newAddresses.at(i));
            }
            return twoTruckOpt2(manhattan, percentToSwap);
        }

        /**
         * @brief Prints each address in a route to a file. See printToFile in Address class.
         * 
         * @param fileName - the name of the output file, without the ".txt"
         * @param printPrime - Outputs 'prime' status of addresses to file if true. 
         */
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

/**
 * @brief Adds newAddresses to paths that have already been optimized. 
 * 
 * For each address in newAddresses, the index of the closest address in each route of paths is found.
 * The new address is inserted at the respective index in a copy of each route in paths.
 * The 'opt2Route' method is ran on each route copy. The increase in length of each route copy is compared,
 * and the copy that increases the length of the route the least is kept.
 * 
 * @param paths - input vector containing two pre-optimized routes.
 * @param newAddresses - list of addresses to be added to routes
 * @param manhattan 
 * @param percentToReverse - see 'opt2Route' method in Route class.
 * @return vector<Route> 
 */
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

/**
 * @brief Returns the combined length of the two routes
 * 
 * @param truckPaths - A vector containing two routes
 * @param manhattan 
 * @return double - the combined length of the routes
 */
double twoTruckLength(vector<Route> truckPaths, bool manhattan=true){
    return truckPaths.at(0).length(manhattan) + truckPaths.at(1).length(manhattan);
}

/**
 * @brief Prints the first and second routes and their lengths. Prints out the combined length of the route.
 * 
 * @param truckPaths - A vector containing two routes.
 * @param manhattan 
 */
void twoRouteOutput(vector<Route> truckPaths, bool manhattan=true){
    truckPaths.at(0).print(); cout << "Length: " << truckPaths.at(0).length(manhattan) << endl;
    truckPaths.at(1).print(); cout << "Length: " << truckPaths.at(1).length(manhattan) << endl;
    cout << "Total Length: " << twoTruckLength(truckPaths, manhattan) << endl << endl;
}

/**
 * @brief Prints the first and second routes in paths to two separate text files.
 * 
 * The first path will be printed into "filename"1.txt
 * The second path will be printed into "filename"2.txt
 * 
 * @param paths - Input vector of routes to be printed.
 * @param fileName - Name of output file w/o ".txt" 
 * @param printPrime - Prints Amazon "Prime" status of address if true.
 */
void saveTwoRoutesToFile(vector<Route> paths, string fileName, bool printPrime = false){
    string path1 =  fileName + "1";
    string path2 =  fileName + "2";
    paths.at(0).saveRouteToFile(path1, printPrime);
    paths.at(1).saveRouteToFile(path2, printPrime);
}

#endif
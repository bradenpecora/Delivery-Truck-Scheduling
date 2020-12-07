#include <iostream>
#include "classes.h"
#include <fstream>

using std::cin;
using std::cout;
using std::endl;
using std::ifstream;


int main () {

    bool manhattan = false;
    float x,y;
    bool prime;


    /////////////////////////////////////////////////////////////////////////
    //55.3
    cout << "**************************************************" << endl;
    cout << "Exercise 55.3: 'Greedy Route' Optimization: " << endl << endl;
    
    Route deliveries;
    deliveries.addAddress(Address(0,5));
    deliveries.addAddress(Address(5,0));
    deliveries.addAddress(Address(5,5));

    cout << "Original Route:" << endl; 
    deliveries.print(); cout << endl;
    cout << "Length: " << deliveries.length(manhattan) << endl << endl;
    deliveries.saveRouteToFile("nongreedy");

    Route greedyDeliveries = deliveries.greedyRoute(manhattan);
    cout << "Greedy Route:" << endl;
    greedyDeliveries.print(); cout << endl;
    cout << "Length: " << greedyDeliveries.length(manhattan) << endl << endl;
    greedyDeliveries.saveRouteToFile("greedy");    
    /////////////////////////////////////////////////////////////////////////
    //55.4
    cout << endl << "**************************************************" << endl;
    cout << "Exercise 55.4: opt2 Heuristic Optimization: " << endl << endl;

    Route sampleTour;

    ifstream opt2file("data/opt2TestCase.txt");
    while(opt2file >> x >> y){
        if(x != 0 || y != 0){
            sampleTour.addAddress(x,y);
        }
    }

    Route greedyList = sampleTour.greedyRoute(manhattan);
    Route opt2List = sampleTour.opt2Route(manhattan,0.75);

    cout << "For a list of 150 random addresses (x,y from [-100,100]):" << endl;
    cout << "Original List Length: " << sampleTour.length(manhattan) << endl;
    cout << "Greedy List Length: " << greedyList.length(manhattan) << endl;
    cout << "opt2 List Length: " << opt2List.length(manhattan) << endl << endl;

    greedyList.saveRouteToFile("Greedy 150");
    opt2List.saveRouteToFile("'Opt2' 150");
    
    /////////////////////////////////////////////////////////////////////////
    //55.7
    cout << endl << "**************************************************" << endl;
    cout << "Exercise 55.7: Multi-path opt2 Heuristic:" << endl << endl;

    Route multiOpt2input;

    ifstream multOpt2File("data/multiOpt2TestCase.txt");
    while(multOpt2File >> x >> y){
        if(x != 0 || y != 0){
            multiOpt2input.addAddress(x,y);
        }
    }
 
    vector<Route> origSplit = multiOpt2input.splitRoute(2);
    origSplit.at(0).opt2Route();
    origSplit.at(1).opt2Route();
    vector<Route> multiOpt2Split = multiOpt2input.twoTruckOpt2(manhattan,0.5);

    cout << "**Original Route:" << endl;
    twoRouteOutput(origSplit, manhattan);

    cout << "**New Route:" << endl;
    twoRouteOutput(multiOpt2Split, manhattan);

    saveTwoRoutesToFile(origSplit,"pre_multopt2_");
    saveTwoRoutesToFile(multiOpt2Split,"post_multopt2_");

    /////////////////////////////////////////////////////////////////////////
    //55.8
    cout << endl << "**************************************************" << endl;
    cout << "Exercise 55.8: Amazon Prime:" << endl << endl;

    
    cout << "**Few-Addresses Prime Routes:" << endl;
    Route noPrimeTruck;
    ifstream fewPrimeListFileNoPrime("data/fewPrimeTestCase.txt");
    while(fewPrimeListFileNoPrime >> x >> y >> prime){
        if(x != 0 || y != 0){
            noPrimeTruck.addAddress(x,y);
        }
    }

    Route primeTruck;
    ifstream fewPrimeListFile("data/fewPrimeTestCase.txt");
    while(fewPrimeListFile >> x >> y >> prime){
        if(x != 0 || y != 0){
            primeTruck.addAddress(x,y,prime);
        }
    }

    AddressList primeAddresses;
    for(int i = 1; i < primeTruck.addressesSize(); i++){
        if(primeTruck.at(i).isPrime()){
            primeAddresses.addAddress(primeTruck.at(i));
        }
    }
    cout << "The Prime addresses that can not be swapped are:" << endl;
    primeAddresses.print(); cout << endl;

    vector<Route> noOrg = primeTruck.splitRoute(2);
    cout << "*Routes as inputted: " << endl;
    twoRouteOutput(noOrg, manhattan);

    vector<Route> noPrimeTwoTruck = noPrimeTruck.twoTruckOpt2(manhattan);
    cout << "*Multi-opt2 Optimized Routes without accounting for 'Prime' status:" << endl;
    twoRouteOutput(noPrimeTwoTruck, manhattan);

    vector<Route> primeTwoTruck = primeTruck.twoTruckOpt2(manhattan);
    cout << "*Multi-opt2 Optimized Routes where Prime addresses can not be swapped:" << endl;
    twoRouteOutput(primeTwoTruck, manhattan);

    saveTwoRoutesToFile(noOrg, "noOpt", true);
    saveTwoRoutesToFile(noPrimeTwoTruck, "noPrimeSmall", true);
    saveTwoRoutesToFile(primeTwoTruck, "PrimeSmall", true);

    cout << "**Many-Address Prime Routes:" << endl;
    Route primeList;

    ifstream manyPrimeListFile("data/manyPrimeTestCase.txt");
    while(manyPrimeListFile >> x >> y){
        if(x != 0 || y != 0){
            primeList.addAddress(x,y);
        }
    }

    vector<Route> noPrime = primeList.twoTruckOpt2(manhattan, 0.5);

    double primePercent = 0.40;
    int primeCounter = 0;
    for (;primeCounter < floor(primePercent*primeList.addressesSize()); primeCounter++){
        Address toPrime = primeList.at(primeCounter); 
        toPrime.changePrimeTo(true);
        primeList.replaceAddress(primeCounter,toPrime);
    }
    cout << primeCounter << " out of " << primeList.addressesSize() << " addresses are prime and can not be swapped." << endl;

    vector<Route> withPrime = primeList.twoTruckOpt2(manhattan, 0.5);

    cout << "Length of two routes optimized without accounting for Amazon Prime status: ";
    cout << twoTruckLength(noPrime, manhattan) << endl;

    cout << "Length of two route optimized when Prime addresses can not be swapped: ";
    cout << twoTruckLength(withPrime, manhattan) << endl;

    saveTwoRoutesToFile(noPrime, "noPrimeLarge");
    saveTwoRoutesToFile(withPrime, "withPrimeLarge", true);

    /////////////////////////////////////////////////////////////////////////
    //55.9
    cout << endl << "**************************************************" << endl;
    cout << "Exercise 55.9:" << endl << endl;
    
    Route dynamic;

    ifstream existingDeliveries("data/opt2TestCase.txt");
    while(existingDeliveries >> x >> y){
        if(x != 0 || y != 0){
            dynamic.addAddress(x,y);
        }

    }
    int originalRouteSize = dynamic.addressesSize() - 1;
    
    ifstream newDeliveries("data/newAddresses.txt");
    AddressList newAddresses;
    while(newDeliveries >> x >> y){
        if(x != 0 || y != 0){
            newAddresses.addAddress(x,y);
        }
    }
    
    vector<Route> opt2paths = dynamic.twoTruckOpt2();
    opt2paths = addToExistingRoutes(opt2paths, newAddresses, manhattan);
    vector<Route> multiOpt2paths = dynamic.addBeforeSplittingRoutes(newAddresses, manhattan);

    cout << "Adding " << newAddresses.addressesSize() << " addresses to " << originalRouteSize;
    cout << " addresses split among two trucks." << endl << endl;

    cout << "Adding new addresses to existing routes:" << endl;
    cout << "Length: " << twoTruckLength(opt2paths, manhattan) << endl;
    
    cout << "Adding new addresses before optimization/splitting routes: " << endl;
    cout << "Length: " << twoTruckLength(multiOpt2paths, manhattan) << endl;

    saveTwoRoutesToFile(opt2paths, "dynamic_opt2_");
    saveTwoRoutesToFile(multiOpt2paths, "dynamic_multi_");

    return 0;
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

}
#include <iostream>
#include "classes.h"
#include <fstream>

using std::cin;
using std::cout;
using std::endl;
using std::ifstream;


int main () {

    bool manhattan = false;

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

    Route greedyDeliveries = deliveries.greedyRoute(manhattan);
    cout << "Greedy Route:" << endl;
    greedyDeliveries.print(); cout << endl;
    cout << "Length: " << greedyDeliveries.length(manhattan) << endl << endl;

    /////////////////////////////////////////////////////////////////////////
    //55.4
    cout << endl << "**************************************************" << endl;
    cout << "Exercise 55.4: opt2 Heuristic Optimization: " << endl << endl;

    Route sampleTour;

    ifstream opt2file("opt2TestCase.txt");
    int x,y;
    while(opt2file >> x >> y){
        if(x != 0 || y != 0){
            sampleTour.addAddress(x,y);
        }
        else{
            break;
        }
    }

    Route greedyList = sampleTour.greedyRoute(manhattan);
    Route opt2List = sampleTour.opt2Route(manhattan);

    cout << "For a list of 150 random addresses (x,y from [-100,100]):" << endl;
    cout << "Original List Length: " << sampleTour.length(manhattan) << endl;
    cout << "Greedy List Length: " << greedyList.length(manhattan) << endl;
    cout << "opt2 List Length: " << opt2List.length(manhattan) << endl << endl;
    
    /////////////////////////////////////////////////////////////////////////
    //55.7
    cout << endl << "**************************************************" << endl;
    cout << "Exercise 55.7: Multi-path opt2 Heuristic:" << endl << endl;

    Route multiOpt2input;

    ifstream multOpt2File("multiOpt2TestCase.txt");
    while(multOpt2File >> x >> y){
        if(x != 0 || y != 0){
            multiOpt2input.addAddress(x,y);
        }
        else{
            break;
        }
    }
 
    vector<Route> origSplit = multiOpt2input.splitRoute(2);
    origSplit.at(0).opt2Route();
    origSplit.at(1).opt2Route();
    vector<Route> split = multiOpt2input.twoTruckOpt2(manhattan);

    cout << "**Original Route:" << endl;
    origSplit.at(0).print(); cout << "Length: " << origSplit.at(0).length(manhattan) << endl;
    origSplit.at(1).print(); cout << "Length: " << origSplit.at(1).length(manhattan) << endl;
    cout << "Total Length: " << multiOpt2input.twoTruckLength(origSplit, manhattan) << endl << endl;

    cout << "**New Route:" << endl;
    split.at(0).print(); cout << "Length: " << split.at(0).length(manhattan) << endl;
    split.at(1).print(); cout << "Length: " << split.at(1).length(manhattan) << endl;
    cout << "New Total Length: " << multiOpt2input.twoTruckLength(split, manhattan) << endl << endl;

    /////////////////////////////////////////////////////////////////////////
    //55.8
    cout << endl << "**************************************************" << endl;
    cout << "Exercise 55.8: Amazon Prime:" << endl << endl;

    
    cout << "**Few-Addresses Prime Routes:" << endl;
    Route noPrimeTruck;
    ifstream fewPrimeListFileNoPrime("fewPrimeTestCase.txt");
    bool prime;
    while(fewPrimeListFileNoPrime >> x >> y >> prime){
        if(x != 0 || y != 0){
            noPrimeTruck.addAddress(x,y);
        }
        else{
            break;
        }
    }

    Route primeTruck;
    ifstream fewPrimeListFile("fewPrimeTestCase.txt");
    while(fewPrimeListFile >> x >> y >> prime){
        if(x != 0 || y != 0){
            primeTruck.addAddress(x,y,prime);
        }
        else{
            break;
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

    vector<Route> noOrg = noPrimeTruck.splitRoute(2);
    cout << "*Routes as inputted: " << endl;
    noOrg.at(0).print(); cout << "Length: " << noOrg.at(0).length(manhattan) << endl;
    noOrg.at(1).print(); cout << "Length: " << noOrg.at(1).length(manhattan) << endl;
    cout << "Total Length: " << noPrimeTruck.twoTruckLength(noOrg, manhattan) << endl;

    vector<Route> noPrimeTwoTruck = noPrimeTruck.twoTruckOpt2(manhattan);
    cout << "*opt2 Optimized Routes without accounting for 'Prime' status:" << endl;
    noPrimeTwoTruck.at(0).print(); cout << "Length: " << noPrimeTwoTruck.at(0).length(manhattan) << endl;
    noPrimeTwoTruck.at(1).print(); cout << "Length: " << noPrimeTwoTruck.at(1).length(manhattan) << endl;
    cout << "Total Length: " << noPrimeTruck.twoTruckLength(noPrimeTwoTruck, manhattan) << endl;

    vector<Route> primeTwoTruck = primeTruck.twoTruckOpt2(manhattan);
    cout << "*opt2 Optimized Routes where Prime addresses can not be swappeed:" << endl;
    primeTwoTruck.at(0).print(); cout << "Length: " << primeTwoTruck.at(0).length(manhattan) << endl;
    primeTwoTruck.at(1).print(); cout << "Length: " << primeTwoTruck.at(1).length(manhattan) << endl;
    cout << "Total Length: " << primeTruck.twoTruckLength(primeTwoTruck, manhattan) << endl << endl;





    cout << "**Many-Address Prime Routes:" << endl;
    double primePercent = 0.40;
    Route primeList;

    ifstream manyPrimeListFile("manyPrimeTestCase.txt");
    while(manyPrimeListFile >> x >> y){
        if(x != 0 || y != 0){
            primeList.addAddress(x,y);
        }
        else{
            break;
        }
    }

    vector<Route> noPrime = primeList.twoTruckOpt2(manhattan);

    for (int i = 1; i <= floor(primePercent*primeList.addressesSize()); i++){
        Address toPrime = primeList.at(i);
        toPrime.changePrimeTo(true);
        primeList.replaceAddress(i,toPrime);
    }
    int counter = 0;
    for (int i = 0 ; i < primeList.addressesSize(); i ++) {
        if (primeList.at(i).isPrime()){
            counter++;
        }
    }
    cout << counter << " out of " << primeList.addressesSize() << " addresses are prime and can not be swapped." << endl;

    vector<Route> withPrime = primeList.twoTruckOpt2(manhattan);

    cout << "Length of two routes optimized without accounting for Amazon Prime status: ";
    cout << primeList.twoTruckLength(noPrime, manhattan) << endl;

    cout << "Length of two route optimized when Prime addresses can not be swapped: ";
    cout << primeList.twoTruckLength(withPrime, manhattan) << endl;

    /////////////////////////////////////////////////////////////////////////
    //55.9
    cout << endl << "**************************************************" << endl;
    cout << "Exercise 55.9:" << endl << endl;

    // vector<Route> primeTest = origSplit;
    // for (int i =1; i <= 1; i++) {
    //     Address toPrime = primeTest.at(0).at(i);
    //     toPrime.changePrimeTo(true);
    //     primeTest.at(0).replaceAddress(i,toPrime);
    // }
    // cout << endl;
    // cout << multiOpt2input.twoTruckLength(origSplit, manhattan) << endl;
    // origSplit.at(0).print(); cout << endl;
    // origSplit.at(1).print();cout << endl;

    // cout << primeList.twoTruckLength(primeTest,manhattan) << endl;
    // primeTest.at(0).print(); cout << endl;
    // primeTest.at(1).print(); cout << endl;
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
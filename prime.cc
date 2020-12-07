#include <iostream>
#include "classes.h"
#include <fstream>
#include <string>

using std::cin;
using std::cout;
using std::endl;
using std::ifstream;
using std::string;
using std::to_string;


int main () {

    bool manhattan = false;
    float x,y;
    bool prime;
    double avgLengthChange = 0;

    int numberOfFiles = 5;
    double percentToSwap = 0.25;
    double primePercent = 0.3;
    string fileName = "test";

    vector<Route> primeLists(numberOfFiles);

    for(int i = 0; i < numberOfFiles; i++){
        string number = to_string(i+1);
        string name = fileName + number + ".txt";
        ifstream opt2file(name);
        while(opt2file >> x >> y){
            if(x != 0 || y != 0){
                primeLists.at(i).addAddress(x,y);
            }
        }

        vector<Route> noPrime = primeLists.at(i).twoTruckOpt2(manhattan, percentToSwap);
        double noPrimeLength = twoTruckLength(noPrime, manhattan);

        for (int primeCounter = 0; primeCounter < floor(primePercent*primeLists.at(i).addressesSize()); primeCounter++){
            Address toPrime = primeLists.at(i).at(primeCounter); 
            toPrime.changePrimeTo(true);
            primeLists.at(i).replaceAddress(primeCounter,toPrime);
        }
        vector<Route> withPrime = primeLists.at(i).twoTruckOpt2(manhattan, percentToSwap);
        double primeLength = twoTruckLength(withPrime, manhattan);

        double percentIncrease = ((primeLength - noPrimeLength)/noPrimeLength)*100;

        cout << i + 1 << ":" << endl;
        cout << "No prime: " << noPrimeLength << endl; 
        cout << "With prime: " << primeLength << endl;
        cout << "% Increase: " << percentIncrease << "%" << endl << endl;

        avgLengthChange += percentIncrease;

    }

    avgLengthChange = avgLengthChange/numberOfFiles;
    cout << "Avg: " << avgLengthChange << "%" << endl;

    return 0;
}
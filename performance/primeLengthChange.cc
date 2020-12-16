#include <iostream>
#include "../classes.h"
#include <fstream>
#include <string>

using std::cin;
using std::cout;
using std::endl;
using std::ifstream;
using std::string;
using std::to_string;


int main () {

    //Measure the change in length that occurs when the 'Prime' status of an addresses is
    //account for. Prime indicates that the address can not be swapped between routes.

    float x,y;
    bool prime;
    double avgLengthChange = 0;

    bool manhattan = false;
    int numberOfFiles = 5; //Number of files. Files must be named with a number; i.e. timer1.txt, timer2.txt
    double percentToSwap = 0.25; //Percent to swap for opt2 method
    vector<double> primePercents = {.01,.05,.1,.25,.5,.75};//Percents of addresses that are prime to be tested
    string fileName = "test";//File name wihout #.txt  Files must be in this directory


    for (double primePercent : primePercents){
        cout <<  "***************************" << endl;
        cout << "Prime Percent: " << 100 * primePercent << "%:" << endl << endl;
        
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

            // cout << "File: " << i + 1 << ", (" << 100*primePercent << "%):" << endl;
            // cout << "No prime: " << noPrimeLength << endl; 
            // cout << "With prime: " << primeLength << endl;
            // cout << "% Increase: " << percentIncrease << "%" << endl << endl;

            avgLengthChange += percentIncrease;

        }

        avgLengthChange = avgLengthChange/numberOfFiles;
        cout << "Avg: " << avgLengthChange << "%" << endl;
        
        
    }
    return 0;
}
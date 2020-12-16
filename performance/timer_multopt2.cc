#include <iostream>
#include "../classes.h"
#include <fstream>
#include <chrono>
#include <string>

using namespace std::chrono;

using std::cin;
using std::cout;
using std::endl;
using std::ifstream;
using std::ios_base;
using std::string;
using std::to_string;


int main () {

    //Finds average runtime increase and length decrease for the multiple opt2 method
    //in comparison to the single opt2 method.

    float x,y;
    double timeAvg = 0;
    double mult2Length, optLength;
    double changeInLength;
    double percentLength;
    double lengthAvg = 0;

    //Change the following variables as needed
    bool manhattan = false;
    int numberOfFiles = 5;//Number of files. Files must be named with a number; i.e. timer1.txt, timer2.txt
    int timeTrials = 5;//Number of times each instance is run to find average runtime
    vector<double> percentToSwaps = {.15};//Different values to be tested
    string fileName = "test";//File name without #.txt  Files must be in this directory


    for (double percentToSwap : percentToSwaps){
        cout << "*************************" << endl;
        cout << "Percent to Swap: " << percentToSwap * 100 << "%" << endl << endl;
        
        vector<Route> sampleTours(numberOfFiles);

        for(int i = 1; i <= numberOfFiles; i++){
            string number = to_string(i);
            string name = fileName + number + ".txt";
            ifstream mult2file(name);
            while(mult2file >> x >> y){
                if(x != 0 || y != 0){
                    sampleTours.at(i-1).addAddress(x,y);
                }
            }
        }   

        for(int j = 0; j<numberOfFiles; j++){
            double timeIncrease = 0;
            for(int i = 0; i<timeTrials; i++){
                auto optStart = high_resolution_clock::now();
                ios_base::sync_with_stdio(false);
                vector<Route> optList = sampleTours.at(j).splitRoute(2);
                optList.at(0) = optList.at(0).opt2Route(manhattan);
                optList.at(1) = optList.at(1).opt2Route(manhattan);
                auto optEnd = high_resolution_clock::now();
                auto optTime = duration_cast<microseconds>(optEnd - optStart);

                auto multStart = high_resolution_clock::now();
                ios_base::sync_with_stdio(false);
                vector<Route> mult2List = sampleTours.at(j).twoTruckOpt2(manhattan,percentToSwap);
                auto multEnd = high_resolution_clock::now();
                auto multTime = duration_cast<microseconds>(multEnd - multStart);

                timeIncrease += ((multTime.count() / optTime.count())-1)*100;

                // cout << "opt2 time " << optTime.count() << endl;
                // cout << "mult time " << multTime.count() << endl;
                // cout << "time increase " << timeIncrease << endl << endl;

                optLength = twoTruckLength(optList, manhattan);
                mult2Length = twoTruckLength(mult2List, manhattan);

                changeInLength = optLength - mult2Length;
                percentLength = (changeInLength / optLength) * 100;
            }
            timeIncrease = timeIncrease/timeTrials;
            timeAvg += timeIncrease;

            lengthAvg += percentLength;

            // cout << "File " << j+1 << ": " << endl;
            // cout << "Change in Length: " << changeInLength << endl;
            // cout << "Percent decrease in Length: " << percentLength << "%" << endl << endl;
        }
        timeAvg = timeAvg/numberOfFiles;
        lengthAvg = lengthAvg/numberOfFiles;
        
        cout << "For " << numberOfFiles << " files at a percent to swap of " << percentToSwap* 100 << "%:" << endl;
        cout << "Average runtime Increase: " << timeAvg << "%" << endl;
        cout << "Average length Decrease: " << lengthAvg << "%" << endl;


    }
    return 0;

}    
#include <iostream>
#include "classes.h"
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

    bool manhattan = false;
    float x,y;
    double timeAvg = 0;

    int numberOfFiles = 5;
    int timeTrials = 10;
    double percentToReverse = 0.15;
    string fileName = "test";

    vector<Route> sampleTours(numberOfFiles);

    for(int i = 1; i <= numberOfFiles; i++){
        string number = to_string(i);
        string name = fileName + number + ".txt";
        ifstream opt2file(name);
        while(opt2file >> x >> y){
            if(x != 0 || y != 0){
                sampleTours.at(i-1).addAddress(x,y);
            }
        }
    }   

    for(int j = 0; j<numberOfFiles; j++){
        double timeIncrease = 0;
        for(int i = 0; i<timeTrials; i++){
            auto greedyStart = high_resolution_clock::now();
            ios_base::sync_with_stdio(false);
            Route greedyList = sampleTours.at(j).greedyRoute(manhattan);
            auto greedyEnd = high_resolution_clock::now();
            auto greedyTime = duration_cast<microseconds>(greedyEnd - greedyStart);

            auto optStart = high_resolution_clock::now();
            ios_base::sync_with_stdio(false);
            Route opt2List = sampleTours.at(j).opt2Route(manhattan,percentToReverse);
            auto optEnd = high_resolution_clock::now();
            auto optTime = duration_cast<microseconds>(optEnd - optStart);

            timeIncrease += ((optTime.count() / greedyTime.count())-1)*100;
        }
        timeIncrease = timeIncrease/timeTrials;
        timeAvg += timeIncrease;

    }
    timeAvg = timeAvg/numberOfFiles;
    cout << timeAvg << endl;

    return 0;

}    
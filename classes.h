#ifndef CLASSES
#define CLASSES

#include <limits>
#include <vector>
#include <iostream>
#include <cmath>
using std::cin;
using std::cout;
using std::endl;
using std::vector;
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
            //TODO: Non-manhattan distance
            return 0;
        }

        void print () {
            cout << "(" << getX() << "," << getY() << ") " << endl;
        }

        
};

class AddressList{
    protected:
        vector<Address> list;
    public:
        AddressList(){};

        void addAddress(Address newHouse){
            bool isEqual = false;
            for(auto house : list){
                if (house.distance(newHouse) == 0){
                    isEqual = true;
                    break;
                }
            }
            if(!isEqual) {
                list.push_back(newHouse);
            }

        }

        /**
         * Finds the distance traveled by a truck that visits
         * each Address in list, in order.
         * 
         * @return the total distance 
         */
        double length(){
            double result = 0;
            for(int i = 0; i < list.size()-1; i++){
                result += list.at(i).distance(list.at(i+1));
            }
            return result;
        }

        /**
         * Finds the index of the Address in list that is closest
         * to house.
         * 
         * @param house the address that is being compared to 
         * @return the index of Address in list 
         */
        int indexClosestTo(Address house){
            double minDist = numeric_limits<double>::max();
            int index = 0;
            for(int i = 0; i < list.size(); i++){
                if (house.distance(list.at(i)) < minDist){
                    index = i;
                }
            }
            return index;
        }

        void print () {
            for (auto house : list){
                house.print();
            }
        }
};

class Route : public AddressList{
    private:
        Address depot;
    public:
        Route(): depot(0,0) {
            list.push_back(depot);
        }
        
        double length(){
            return AddressList::length() + depot.distance(list.back());
        }

        void print(){
            AddressList::print();
            depot.print();
        }
        
};


#endif
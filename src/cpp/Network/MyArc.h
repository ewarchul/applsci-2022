#ifndef NETWORKGENERATOR_MYARC_H
#define NETWORKGENERATOR_MYARC_H

#include <cmath>
#include <string>
#include <vector>
#include "MyVertex.h"

using namespace std;

class MyArc {
private:
    int id;
    string code;
    MyVertex source;
    MyVertex target;
    int length;
    vector<int> travel_cost;  // koszt dla każdego typu pojazdu
    int temp_cost = 0;

public:
    MyArc(int id, MyVertex source, MyVertex target);
    MyArc(int id, const MyVertex &source, const MyVertex &target, int length);

    int getId() const;
    const MyVertex &getSource() const;
    const MyVertex &getTarget() const;
    MyVertex getSource2() const;
    MyVertex getTarget2() const;
    int getLength() const;
    const string &getCode() const;

    void setLength(int length);
    void setTravelCost(const vector<int> &travelCost);

    static int calculateLength(MyVertex src, MyVertex dest);
    void change_temp_cost(int cost);
};


#endif //NETWORKGENERATOR_MYARC_H
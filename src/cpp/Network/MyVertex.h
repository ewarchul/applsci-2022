#ifndef NETWORKGENERATOR_CITY_H
#define NETWORKGENERATOR_CITY_H

#include <string>
#include <vector>

using namespace std;

class MyVertex {
private:
    int id;
    string name;
    string code;
    double x_coor;
    double y_coor;
    vector<int> arcs_entering;
    vector<int> arcs_leaving;

public:
    MyVertex(int id, const string &name, double xCoor, double yCoor);

    int getId() const;
    const string &getName() const;
    double getXCoor() const;
    double getYCoor() const;
    const vector<int> &getArcsEntering() const;
    const vector<int> &getArcsLeaving() const;
    const string &getCode() const;

    void addArcse(int);
    void addArcsl(int);
    void removeArcse(int);
    void removeArcsl(int);
};

#endif //NETWORKGENERATOR_CITY_H

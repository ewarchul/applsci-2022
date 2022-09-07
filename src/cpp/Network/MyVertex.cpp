#define ARCS_LIMIT 5
#include "MyVertex.h"

using namespace std;

MyVertex::MyVertex(int id, const string &name, double xCoor, double yCoor) : id(id), name(name), x_coor(xCoor), y_coor(yCoor)
{
    char a = name[1];
    a = toupper(a);
    char b = name[2];
    b = toupper(b);

    code = name[0];
    code = code + a + b;

    arcs_entering.reserve(ARCS_LIMIT);
    arcs_leaving.reserve(ARCS_LIMIT);
}

int MyVertex::getId() const {
    return id;
}

const string &MyVertex::getName() const {
    return name;
}

double MyVertex::getXCoor() const {
    return x_coor;
}

double MyVertex::getYCoor() const {
    return y_coor;
}

const vector<int> &MyVertex::getArcsEntering() const {
    return arcs_entering;
}

const vector<int> &MyVertex::getArcsLeaving() const {
    return arcs_leaving;
}

void MyVertex::addArcse(int index)
{
    this->arcs_entering.push_back(index);
}

void MyVertex::addArcsl(int index)
{
    this->arcs_leaving.push_back(index);
}

void MyVertex::removeArcse(int index)
{
    for (int i = 0; i < this->arcs_entering.size(); i++)
    {
        if (this->arcs_entering[i] == index)
            this->arcs_entering.erase(this->arcs_entering.begin() + i);
    }
}

void MyVertex::removeArcsl(int index)
{
    for (int i = 0; i < this->arcs_leaving.size(); i++)
    {
        if (this->arcs_leaving[i] == index)
            this->arcs_leaving.erase(this->arcs_leaving.begin() + i);
    }
}

const string &MyVertex::getCode() const {
    return code;
}
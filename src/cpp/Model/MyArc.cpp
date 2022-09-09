#include "MyArc.h"

using namespace std;

MyArc::MyArc(int id, MyVertex source, MyVertex target)
    : id(id), source(source), target(target) {
  code = source.getCode() + "_" + target.getCode();

  length = calculateLength(source, target);
}

int MyArc::calculateLength(MyVertex source, MyVertex target) {
  int r = 6367;

  double source_lat = source.getXCoor();
  double source_lon = source.getYCoor();
  double target_lat = target.getXCoor();
  double target_lon = target.getYCoor();

  double rlat1 = source_lat * 3.14 / 180;
  double rlon1 = source_lon * 3.14 / 180;
  double rlat2 = target_lat * 3.14 / 180;
  double rlon2 = target_lon * 3.14 / 180;
  double drlat = (rlat2 - rlat1);
  double drlon = (rlon2 - rlon1);

  double init =
      (sin(drlat / 2.)) * (sin(drlat / 2.)) +
      (cos(rlat1)) * (cos(rlat2)) * ((sin(drlon / 2.)) * (sin(drlon / 2.)));

  int result = int(2.0 * r * asin(min(1., sqrt(init))));
  return result;
}

int MyArc::getId() const { return id; }

const MyVertex &MyArc::getSource() const { return source; }

const MyVertex &MyArc::getTarget() const { return target; }

MyVertex MyArc::getSource2() const { return source; }

MyVertex MyArc::getTarget2() const { return target; }

int MyArc::getLength() const { return length; }

const string &MyArc::getCode() const { return code; }

void MyArc::setLength(int length) { MyArc::length = length; }

MyArc::MyArc(int id, const MyVertex &source, const MyVertex &target, int length)
    : id(id), source(source), target(target), length(length) {}

void MyArc::setTravelCost(const vector<int> &travelCost) {
  for (int i = 0; i < travelCost.size(); i++)
    this->travel_cost.push_back(travelCost[i]);
}

void MyArc::change_temp_cost(int cost) { temp_cost += cost; }

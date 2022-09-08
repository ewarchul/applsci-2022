#ifndef NETWORKGENERATOR_NETWORK_H
#define NETWORKGENERATOR_NETWORK_H

#define VEHICLES_NUMBER 3
#define PERIODS_NUMBER 7

#include "Country.h"
#include "DFS.h"
#include "MyArc.h"
#include "MyRealisation.h"
#include <cmath>
#include <ctime>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

class Network {
private:
  Country country;
  vector<MyVertex> vertices;
  vector<MyArc> arcs;
  vector<MyRealisation> realisations;

  double vehicle_coefficients[VEHICLES_NUMBER] = {0.3, 0.5, 0.9};
  double time_coefficients[VEHICLES_NUMBER] = {0.35, 0.4, 0.5};

  string vehicles[VEHICLES_NUMBER] = {"RENAULT", "FIAT", "TIR"};
  string dim[2] = {"M", "V"};
  string log_fun[3] = {"NONE", "WER", "PER"};
  string delays[4] = {"D3", "D2", "D1", "D0"};
  string periods[PERIODS_NUMBER] = {"MON", "TUE", "WED", "THU",
                                    "FRI", "SAT", "SUN"};

  vector<vector<MyArc>> arcs_entering;
  vector<vector<MyArc>> arcs_leaving;
  vector<vector<MyArc>> arcs_for_vehicle;
  vector<vector<string>> vehicles_for_vertex;
  vector<vector<int>> streams_sizes;
  vector<MyVertex> realisation_source;
  vector<MyVertex> realisation_target;
  vector<vector<int>> travel_cost;
  vector<vector<int>> travel_time;
  int capacity[6] = {25, 25, 50, 50, 100, 100};
  vector<vector<vector<int>>> function_cost;
  vector<vector<vector<int>>> function_time;
  vector<vector<MyRealisation>> realisation_gathered;
  vector<int> vertex_open;
  vector<int> vertex_closed;
  vector<int> realisation_available;
  vector<int> realisation_delivery;
  vector<string> realisation_max_delay;
  vector<int> period_time;

public:
  Network(const Country &country, double, double, double, double);
  Network();

  void generate_arcs(double coefficient, int max_vertices);
  void generate_realisations(int max_value, const Country &country);
  static bool check_crossroads(const MyVertex &src, const MyVertex &dst,
                               const MyVertex &srcBis, const MyVertex &dstBis);
  static double multiplication(const MyVertex &, const MyVertex &,
                               const MyVertex &);
  static bool lies_between(const MyVertex &, const MyVertex &,
                           const MyVertex &);
  vector<MyRealisation> divideRandomRealisation(int realIndex);
  void divideRealisations();

  const vector<MyVertex> &getVertices() const;
  const vector<MyArc> &getArcs() const;
  const vector<MyRealisation> &getRealisations() const;
  const string *getPeriods() const;
  const double *getVehicleCoefficients() const;
  const double *getTimeCoefficients() const;
  const string *getVehicles() const;
  const string *getDim() const;
  const string *getLogFun() const;
  const string *getDelays() const;
  const vector<vector<MyArc>> &getArcsEntering() const;
  const vector<vector<MyArc>> &getArcsLeaving() const;
  const vector<vector<MyArc>> &getArcsForVehicle() const;
  const vector<vector<string>> &getVehiclesForVertex() const;
  const vector<vector<int>> &getStreamsSizes() const;
  const vector<MyVertex> &getRealisationSource() const;
  const vector<MyVertex> &getRealisationTarget() const;
  const vector<vector<int>> &getTravelCost() const;
  const vector<vector<int>> &getTravelTime() const;
  const int *getCapacity() const;

  const vector<vector<vector<int>>> &getFunctionCost() const;
  const vector<vector<vector<int>>> &getFunctionTime() const;
  const vector<vector<MyRealisation>> &getRealisationGathered() const;
  const vector<int> &getVertexOpen() const;
  const vector<int> &getVertexClosed() const;
  const vector<int> &getRealisationAvailable() const;
  const vector<int> &getRealisationDelivery() const;
  const vector<string> &getRealisationMaxDelay() const;
  const vector<int> &getPeriodTime() const;
  const int getCity(int index);
};

#endif // NETWORKGENERATOR_NETWORK_H

#ifndef NETWORKGENERATOR_COUNTRY_H
#define NETWORKGENERATOR_COUNTRY_H

#include "MyVertex.h"
#include <vector>

using namespace std;

class Country {
private:
  vector<MyVertex> cities;

public:
  Country();

  const vector<MyVertex> &getCities() const;
};

#endif // NETWORKGENERATOR_COUNTRY_H

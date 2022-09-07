#ifndef NETWORKGENERATOR_WRITER_H
#define NETWORKGENERATOR_WRITER_H

#include <vector>
#include <fstream>
#include "Network.h"

using namespace std;

class Writer {
private:

public:
    void createExtendedDat(Network network);

    void createFirstModelDat(Network network, int demand_max_value);
};


#endif //NETWORKGENERATOR_WRITER_H

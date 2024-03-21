#ifndef reModule_h
#define reModule_h

#include "nodeInformation.h"

namespace reMod {

void put(string key,string value,NodeInformation &nodeInfo);
std::string get(string key,NodeInformation nodeInfo);
void del(string key, NodeInformation nodeInfo);

}

#endif

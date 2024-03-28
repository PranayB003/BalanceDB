#ifndef reModule_h
#define reModule_h

#include <string>
#include "nodeInformation.h"

namespace reMod {

long long int hashN(string key, int funcNum);
int getNumChoices();
void setNumChoices(int newNumChoices);

void put(string key,string value,NodeInformation &nodeInfo);
std::string get(string key,NodeInformation nodeInfo);
void del(string key, NodeInformation nodeInfo);

}

#endif

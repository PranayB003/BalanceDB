#include <algorithm>

#include "reModule.h"
#include "headers.h"
#include "helperClass.h"
#include "M.h"

namespace reMod {

int numChoices = 1;
HelperFunctions help = HelperFunctions();

long long int hashN(string key, int funcNum) {
    unsigned long long int a = (funcNum + 1) * A;
    unsigned long long int b = B;
    unsigned long long int p = P;
    unsigned long long int m = pow(2, M);
    
    unsigned long long int hashValue = 0;
    for (char c : key) {
        hashValue = (a * hashValue + c) % p;
    }
    return (hashValue + b) % m;
}


int getNumChoices() {
    return numChoices;
}

void setNumChoices(int newNumChoices) {
    numChoices = newNumChoices;
}

void put(string key, string value, NodeInformation &nodeInfo) {
    vector<lli> keyHash(numChoices);
    for (int i = 0; i < numChoices; i++) {
        keyHash[i] = hashN(key, i);
    }

    vector<int> nodeCapacity(numChoices);
    vector<pair<pair<string, int>, lli>> node(numChoices);
    for (int i = 0; i < numChoices; i++) {
        node[i] = nodeInfo.findSuccessor(keyHash[i]);
        nodeCapacity[i] = help.getNumKeysFromNode(node[i]);
    }
    int index = min_element(nodeCapacity.begin(), nodeCapacity.end()) - nodeCapacity.begin();

    help.sendKeyToNode(node[index], keyHash[index], value);
}

std::string get(string key, NodeInformation nodeInfo) {
    vector<lli> keyHash(numChoices);
    for (int i = 0; i < numChoices; i++) {
        keyHash[i] = hashN(key, i);
    }

    vector<pair<pair<string, int>, lli>> node(numChoices);
    for (int i = 0; i < numChoices; i++) {
        node[i] = nodeInfo.findSuccessor(keyHash[i]);
    }

    for (int i = 0; i < numChoices; i++) {
        string val = help.getKeyFromNode(node[i], to_string(keyHash[i]));
        if (val.size() > 0) {
            return val;
        }
    }
    return "";
}

void del(string key, NodeInformation nodeInfo) {
    vector<lli> keyHash(numChoices);
    for (int i = 0; i < numChoices; i++) {
        keyHash[i] = hashN(key, i);
    }

    vector<pair<pair<string, int>, lli>> node(numChoices);
    for (int i = 0; i < numChoices; i++) {
        node[i] = nodeInfo.findSuccessor(keyHash[i]);
        help.delKeyFromNode(node[i], to_string(keyHash[i]));
    }
}

}

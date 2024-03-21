#include "reModule.h"
#include "helperClass.h"

namespace reMod {

HelperFunctions help = HelperFunctions();

void put(string key, string value, NodeInformation &nodeInfo) {
    lli keyHash = help.getHash(key);
    pair< pair<string,int> , lli > node = nodeInfo.findSuccessor(keyHash);
    help.sendKeyToNode(node,keyHash,value);
}
std::string get(string key, NodeInformation nodeInfo) {
    lli keyHash = help.getHash(key);
    pair< pair<string,int> , lli > node = nodeInfo.findSuccessor(keyHash);
    string val = help.getKeyFromNode(node,to_string(keyHash));
    return val;
}
void del(string key, NodeInformation nodeInfo) {
    lli keyHash = help.getHash(key);
    pair< pair<string, int>, lli> node = nodeInfo.findSuccessor(keyHash);
    help.delKeyFromNode(node, to_string(keyHash));
}

}

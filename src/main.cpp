#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <cstdlib>
#include <cstring>
#include <thread>

#include "httpServer.h"
#include "port.h"
#include "functions.h"
#include "helperClass.h"
#include "nodeInformation.h"
#include "reModule.h"

using namespace std;

std::vector<std::string> splitString(std::string& str, char delimiter);
void initialize(std::map<std::string, std::string> &args);

int main(int argc, char* argv[]){
    std::map<std::string, std::string> arguments;

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        size_t pos = arg.find('=');

        if (pos != std::string::npos) {
            std::string key = arg.substr(0, pos);
            std::string value = arg.substr(pos + 1);
            arguments[key] = value;
        } else {
            std::cerr << "Invalid argument format: " << arg << std::endl;
        }
    }

    initialize(arguments);  //init.cpp
}

std::vector<std::string> splitString(std::string& str, char delimiter) {
    std::vector<std::string> tokens;
    std::string token;
    std::size_t pos = 0;

    while ((pos = str.find(delimiter)) != std::string::npos) {
        token = str.substr(0, pos);
        tokens.push_back(token);
        str.erase(0, pos + 1);
    }

    // Add the last token after the last delimiter
    tokens.push_back(str);

    return tokens;
}

void initialize(std::map<std::string, std::string> &args){
    NodeInformation nodeInfo = NodeInformation();

    // open a socket to listen to other nodes
    int given_port = 0;
    if (args.find("port") != args.end()) {
        given_port = std::stoi(args["port"]);
    }
    nodeInfo.sp.specifyPortServer(given_port);

    // join or create a chord ring based on CLI args
    if (args.find("mode") != args.end()) {
        std::vector<std::string> mode_params = splitString(args["mode"], ',');
        if (mode_params.size() == 1 && mode_params[0] == "create") {
            if (args.find("choices") != args.end()) {
                reMod::setNumChoices(stoi(args["choices"]));
            }
            thread first(create,ref(nodeInfo));
            first.detach();
        }
        else if (mode_params.size() == 3 && mode_params[0] == "join") {
            join(nodeInfo, mode_params[1], mode_params[2]);
        }
        else {
            std::cout << "Invalid usage. Try: mode=create OR mode=join,IP,PORT\n";
        }
    }

    // start an HTTP server to listen for incoming GET and PUT requests
    if (args.find("http") != args.end()) {
        auto callback = [&nodeInfo](string method, string data) {
            std::string response;
            if(nodeInfo.getStatus() == false){
                response = "This node is not in the ring\n";
            } else if (method == "PUT") {
                auto delim_pos  = data.find("=");
                std::string key = data.substr(0, delim_pos);
                std::string val = data.substr(delim_pos + 1);
                response = putWrapper(key, val, nodeInfo);
            } else if (method == "GET") {
                if (data == "__NUM_KEYS__") {
                    response = to_string(nodeInfo.getNumKeys());
                } else {
                    response = getWrapper(data, nodeInfo);
                }
            } else if (method == "DELETE") {
                if (data == "__LOCAL_ALL__") {
                    nodeInfo.clearKeys();
                    response = "OK";
                } else {
                    response = delWrapper(data, nodeInfo);
                }
            } else {
                response = "Invalid Request. Try `GET key` or `PUT key=val` instead.";
            }
            return response;
        };

        thread serv(start_http_server, stoi(args["http"]), callback);
        serv.detach();
    }

    cout<<"Now listening at port number "<<nodeInfo.sp.getPortNumber()<<endl;
    cout<<"Type help to know more\n";

    // accept shell commands
    string command;
    while(1){
        cout << "> ";
        getline(cin,command);

        /* find space in command and seperate arguments*/
        HelperFunctions help = HelperFunctions();
        vector<string> arguments = help.splitCommand(command);

        string arg = arguments[0];
        if(arguments.size() == 1){
            /* creates */
            if(arg == "create"){
                if(nodeInfo.getStatus() == true){
                    cout<<"Sorry but this node is already on the ring\n";
                }
                else{
                    if (args.find("choices") != args.end()) {
                        reMod::setNumChoices(stoi(args["choices"]));
                    }
                    thread first(create,ref(nodeInfo));
                    first.detach();
                }
            }
            /* prints */
            else if(arg == "printstate"){
                if(nodeInfo.getStatus() == false){
                    cout<<"Sorry this node is not in the ring\n";
                }
                else
                    printState(nodeInfo);
            }
            /* leaves */
            else if(arg == "leave"){
                leave(nodeInfo);
                nodeInfo.sp.closeSocket();
                return;
            }
            /* print current port number */
            else if(arg == "port"){
                cout<<nodeInfo.sp.getPortNumber()<<endl;
            }
            /* print keys present in this node */
            else if(arg == "print"){
                if(nodeInfo.getStatus() == false){
                    cout<<"Sorry this node is not in the ring\n";
                }
                else
                    nodeInfo.printKeys();
            }
            else if (arg == "clear"){
                nodeInfo.clearKeys();
            }
            else if (arg == "numkeys"){
                cout << "Number of keys on this node: " << nodeInfo.getNumKeys() << '\n';
            }
            else if(arg == "help"){
                showHelp();
            }
            else{
                cout<<"Invalid Command\n";
            }
        }
        else if(arguments.size() == 2){
            if(arg == "port"){
                if(nodeInfo.getStatus() == true){
                    cout<<"Sorry you can't change port number now\n";
                }
                else{
                    int newPortNo = atoi(arguments[1].c_str());
                    nodeInfo.sp.changePortNumber(newPortNo);
                }
            }
            else if(arg == "get"){
                if(nodeInfo.getStatus() == false){
                    cout<<"Sorry this node is not in the ring\n";
                }
                else
                    cout << getWrapper(arguments[1],nodeInfo) << "\n";
            }
            else if (arg == "delete") {
                if(nodeInfo.getStatus() == false){
                    cout<<"Sorry this node is not in the ring\n";
                }
                else
                    cout << delWrapper(arguments[1],nodeInfo) << "\n";
            }
            else{
                cout<<"Invalid Command\n";
            }
        }
        else if(arguments.size() == 3){
            if(arg == "join"){
                if(nodeInfo.getStatus() == true){
                    cout<<"Sorry but this node is already on the ring\n";
                }
                else
                    join(nodeInfo,arguments[1],arguments[2]);
            }
            /* puts the entered key and it's value to the necessary node*/
            else if(arg == "put"){
                if(nodeInfo.getStatus() == false){
                    cout<<"Sorry this node is not in the ring\n";
                }
                else
                    cout << putWrapper(arguments[1],arguments[2],nodeInfo) << "\n";
            }
            else{
                cout<<"Invalid Command\n";
            }
        }
        else{
            cout<<"Invalid Command\n";
        }
    }
}

#include <iostream>
#include <vector>
#include "init.h"
using namespace std;

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
            // Handle invalid argument format here
            std::cerr << "Invalid argument format: " << arg << std::endl;
            // Optionally, you may exit the program or handle the error differently
        }
    }

    // Print out the contents of the map
    for (const auto& pair : arguments) {
        std::cout << "Key: " << pair.first << ", Value: " << pair.second << std::endl;
    }

    initialize(&arguments);  //init.cpp
}

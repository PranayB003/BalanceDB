#ifndef httpserv_h
#define httpserv_h

#include <iostream>
#include <string>
#include <cstring>
#include <functional>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

void handle_request(int client_socket, const std::function<std::string(std::string, std::string)> &callback);
void start_http_server(int given_port, const std::function<std::string(std::string, std::string)> &callback);

#endif

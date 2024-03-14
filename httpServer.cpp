#include "httpServer.h"

void handle_request(int client_socket, const std::function<std::string(std::string, std::string)> &callback) {
    // Read the HTTP request from the client
    char buffer[1024] = {0};
    read(client_socket, buffer, 1024);

    // Check if it's a PUT or GET request
    std::string request(buffer), response;
    size_t start = request.find("\r\n\r\n") + 4;
    std::string data = request.substr(start);
    if (request.find("PUT") != std::string::npos) {
        // Handle PUT request
        response = callback("PUT", data);
    } else if (request.find("GET") != std::string::npos) {
        // Handle GET request
        response = callback("GET", data);
    }
    std::string http_response = "HTTP/1.1 200 OK\r\nContent-Length: " + std::to_string(response.size()) + "\r\n\r\n" + response;
    send(client_socket, http_response.c_str(), http_response.size(), 0);

    // Close the client socket
    close(client_socket);
}

void start_http_server(int given_port, const std::function<std::string(std::string, std::string)> &callback) {
    // Create a socket
    int server_socket = socket(AF_INET, SOCK_STREAM, 0);
    int yes=1;
    if (setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof yes) == -1) {
        std::cerr << "Error: Could not ret REUSEADDR option for HTTP server socket" << std::endl;
        return;
    }
    if (server_socket == -1) {
        std::cerr << "Error: Could not create socket for HTTP server" << std::endl;
        return;
    }

    // Bind the socket to the given port
    sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_port = htons(given_port);

    if (bind(server_socket, (struct sockaddr *)&server_address, sizeof(server_address)) < 0) {
        std::cerr << "Error: Bind failed for HTTP server" << std::endl;
        close(server_socket);
        return;
    }

    // Listen for incoming connections
    if (listen(server_socket, 5) < 0) {
        std::cerr << "Error: Listen failed for HTTP server" << std::endl;
        close(server_socket);
        return;
    }

    std::cout << "HTTP Server listening on port " << given_port << std::endl;

    // Accept incoming connections and handle requests
    while (true) {
        int client_socket = accept(server_socket, nullptr, nullptr);
        if (client_socket < 0) {
            std::cerr << "Error: Accept failed for HTTP server" << std::endl;
            close(server_socket);
            return;
        }

        // Handle the request from the client
        handle_request(client_socket, callback);
    }

    // Close the server socket
    close(server_socket);
}

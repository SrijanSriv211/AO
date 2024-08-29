#include <cstring>
#include <winsock2.h>
#include <ws2tcpip.h>

#include "aopch.h"
#include "entrypoint.h"
#include "ao.h"

#include "console/console.h"
#include "nlohmann/json.hpp"

// link with the Ws2_32.lib library (Winsock library)
// this directive tells the linker to include the Winsock 2.2 library
#pragma comment(lib, "Ws2_32.lib")
using json = nlohmann::json;

//* NOTE: I don't understand everything that's going on in here
// https://youtu.be/VlUO6ERf1TQ
void start_server()
{
    AO::clear_console();

    WSADATA wsaData; // structure to hold Winsock data
    SOCKET server_socket; // server socket descriptor
    sockaddr_in server_addr; // structure to hold server address information

    // initialize Winsock and exit if Winsock initialization fails
    // MAKEWORD(2, 2) specifies version 2.2 of Winsock
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
    {
        console::print("WSAStartup failed.", console::color::LIGHT_RED);
        return;
    }

    // create a socket for listening and exit if socket creation fails
    // AF_INET: IPv4, SOCK_STREAM: TCP, 0: default protocol
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == INVALID_SOCKET)
    {
        console::print("Socket creation failed.", console::color::LIGHT_RED);
        WSACleanup();
        return;
    }

    // set up the sockaddr_in structure for the server address
    server_addr.sin_family = AF_INET; // IPv4 address family
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1"); // bind to localhost
    server_addr.sin_port = htons(8000); // bind to port 8000 (convert to network byte order)

    // bind the socket to the IP address and port & exit if bind fails
    if (bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) == SOCKET_ERROR)
    {
        console::print("Bind failed.", console::color::LIGHT_RED);
        closesocket(server_socket);
        WSACleanup();
        return;
    }

    // start listening for incoming connections & exit if listen fails
    // the second argument specifies the maximum number of pending connections (backlog)
    if (listen(server_socket, 10) == SOCKET_ERROR)
    {
        console::print("Listen failed.", console::color::LIGHT_RED);
        closesocket(server_socket);
        WSACleanup();
        return;
    }

    console::print("> ", console::color::GRAY, false);
    console::print("Server listening on 127.0.0.1:8000", console::color::LIGHT_WHITE);

    // enter an infinite loop to accept and handle multiple client connections
    while (true)
    {
        SOCKET client_socket; // client socket descriptor
        sockaddr_in client_addr; // structure to hold client address information
        int client_addr_size = sizeof(client_addr); // size of client address structure

        // accept an incoming client connection & exit if accept fails
        client_socket = accept(server_socket, (struct sockaddr*)&client_addr, &client_addr_size);
        if (client_socket == INVALID_SOCKET)
        {
            console::print("Accept failed.", console::color::LIGHT_RED);
            closesocket(server_socket);
            WSACleanup();
            return;
        }

        // receive data from the client
        // buffer to store incoming data (adjust size as needed)
        char buffer[4096];
        // receive data from the client socket
        int recv_size = recv(client_socket, buffer, sizeof(buffer) - 1, 0); // leave space for null terminator
        if (recv_size > 0)
        {
            buffer[recv_size] = '\0'; // null-terminate the received data
            std::string request(buffer); // convert received data to std::string

            // simple HTTP Request Parsing (very basic)
            // check if the request is a POST request
            if (request.find("POST") == 0)
            {
                // find the end of the HTTP headers (indicated by two consecutive CRLFs)
                size_t header_end = request.find("\r\n\r\n");
                if (header_end != std::string::npos)
                {
                    // extract the body of the request
                    std::string body = request.substr(header_end + 4); // skip past "\r\n\r\n"
                    AO::print_prompt();
                    std::cout << body << std::endl;

                    // create a JSON object for the response
                    json response_json;
                    response_json["input"] = body;
                    response_json["status"] = "success";
                    response_json["code"] = 200;

                    // serialize the JSON object to a string
                    std::string json_response = response_json.dump(4);

                    // construct the full HTTP response with headers and JSON body
                    std::string http_response = "HTTP/1.1 200 OK\r\nContent-Type: application/json\r\n"
                                            "Content-Length: " + std::to_string(json_response.length()) + "\r\n\r\n" + json_response;

                    // send the HTTP response to the client
                    int send_result = send(client_socket, http_response.c_str(), http_response.length(), 0);
                    if (send_result == SOCKET_ERROR)
                        console::print("Send failed with error: " + WSAGetLastError(), console::color::LIGHT_RED);
                }

                else
                {
                    // malformed HTTP request (no header-body separator)
                    console::print("Malformed HTTP request: No header-body separator found.", console::color::LIGHT_RED);
                    // optionally, send a 400 Bad Request response
                    std::string bad_request = "HTTP/1.1 400 Bad Request\r\n"
                                            "Content-Type: application/json\r\n\r\n"
                                            "{\"error\": \"Malformed HTTP request\"}";
                    send(client_socket, bad_request.c_str(), bad_request.length(), 0);
                }
            }

            else
            {
                console::print("Unsupported HTTP method.", console::color::LIGHT_RED);
                // optionally, send a 405 Method Not Allowed response
                std::string method_not_allowed = "HTTP/1.1 405 Method Not Allowed\r\n"
                                                "Content-Type: application/json\r\n\r\n"
                                                "{\"error\": \"Method Not Allowed\"}";
                send(client_socket, method_not_allowed.c_str(), method_not_allowed.length(), 0);
            }
        }

        // connection was gracefully closed by the client
        else if (recv_size == 0)
            std::cout << "Connection closed by client." << std::endl;

        // recv failed with an error
        else
            console::print("recv failed with error: " + WSAGetLastError(), console::color::LIGHT_RED);

        // close the client socket after handling the request
        closesocket(client_socket);
    }

    // cleanup server socket and Winsock
    closesocket(server_socket);
    WSACleanup();
}

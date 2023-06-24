#include "Response/Get.hpp"
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

// Helper function to read the content of a file
std::vector<char> readFile(const std::string& filename) {
    std::ifstream file(filename.c_str(), std::ios::binary | std::ios::ate);
    if (!file) {
        throw std::runtime_error("Failed to open file");
    }

    std::ifstream::pos_type fileSize = file.tellg();
    std::vector<char> fileData(fileSize);
    file.seekg(0, std::ios::beg);
    file.read(&fileData[0], fileSize);
    return fileData;
}

// Function to send an HTTP response with the image data
void sendImageResponse(int clientSocket) {
	Get response;
	response.generate_response();
	send(clientSocket, response.get_head().c_str(), response.get_head().length(), 0);
	std::vector<char> res = response.get_body();
	send(clientSocket, &res[0], res.size(), 0);
}

int main() {
    int listenSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (listenSocket == -1) {
        std::cerr << "Failed to create socket" << std::endl;
        return 1;
    }

    struct sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(8080); // Change the port number if needed
    serverAddress.sin_addr.s_addr = htonl(168495372);

    if (bind(listenSocket, (struct sockaddr*) &serverAddress, sizeof(serverAddress)) == -1) {
        std::cerr << "Failed to bind socket" << std::endl;
        close(listenSocket);
        return 1;
    }

    if (listen(listenSocket, 5) == -1) {
        std::cerr << "Failed to listen on socket" << std::endl;
        close(listenSocket);
        return 1;
    }

    std::cout << "Listening on port 8080..." << std::endl;

    while (true) {
        struct sockaddr_in clientAddress;
        socklen_t clientAddressLength = sizeof(clientAddress);

        int clientSocket = accept(listenSocket, (struct sockaddr*) &clientAddress, &clientAddressLength);
        if (clientSocket == -1) {
            std::cerr << "Failed to accept client connection" << std::endl;
            continue;
        }

        std::cout << "Accepted client connection" << std::endl;

        sendImageResponse(clientSocket);

        close(clientSocket);
        std::cout << "Closed client connection" << std::endl;
    }

    close(listenSocket);
   
}
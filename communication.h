#ifndef COMMUNICATION_H
#define COMMUNICATION_H

#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <fstream>

using namespace std;
class Communication {
public:
    Communication(const char* ip_address, int port); // Constructeur
    ~Communication(); // Destructeur
    bool connectToServer();
    bool sendData(const string& data);
    string receiveData();

private:
    int m_socket;
    struct sockaddr_in m_serv_addr;
    bool connected;
};

#endif // COMMUNICATION_H

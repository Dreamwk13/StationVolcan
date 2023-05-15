#include <iostream>
#include <string>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>

const std::string SERVER_IP = "your.server.ip.address"; // Remplacez par l'adresse IP de votre serveur
const int SERVER_PORT = 12345; // Remplacez par le numero de port du serveur

int main()
{
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
    {
        std::cerr << "Erreur lors de la creation du socket" << std::endl;
        return 1;
    }

    sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    inet_pton(AF_INET, SERVER_IP.c_str(), &server_addr.sin_addr);

    if (connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
    {
        std::cerr << "Erreur lors de la connexion au serveur" << std::endl;
        close(sockfd);
        return 1;
    }

    std::string data = "Hello, Server!";
    ssize_t bytes_sent = send(sockfd, data.c_str(), data.size(), 0);
    if (bytes_sent < 0)
    {
        std::cerr << "Erreur lors de l'envoi des donnees" << std::endl;
    }

    close(sockfd);
    return 0;
}

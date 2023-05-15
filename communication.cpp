#include "communication.h"
using namespace std;




Communication::Communication(const char* ip_address, int port) : m_socket(0), connected(false) {
    // création du socket
    if ((m_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        cerr << "Erreur de création de socket" << endl;
        return;
    }

    // configuration de l'adresse IP et du port du serveur
    m_serv_addr.sin_family = AF_INET;
    m_serv_addr.sin_port = htons(port);

    // convertir l'adresse IP en notation point-virgule
    if(inet_pton(AF_INET, ip_address, &m_serv_addr.sin_addr)<=0) {
       cerr << "Adresse IP non valide" << endl;
        return;
    }
}

Communication::~Communication() {
    close(m_socket);
}

bool Communication::connectToServer() {
    // test de connexion
    if (connect(m_socket, (struct sockaddr *)&m_serv_addr, sizeof(m_serv_addr)) == 0) {
        connected = true;
        cout << "Connexion réussie" << endl;
        return true;
    }
    else {
        cerr << "Echec de la connexion" << endl;
        return false;
    }
}

bool Communication::sendData(const std::string& data) {
    if (connected) {
        if (send(m_socket, data.c_str(), data.length(), 0) < 0) {
            std::cerr << "Erreur lors de l'envoi de données" << std::endl;
            return false;
        }
        cout << "Données envoyées" << endl;
        return true;
    }
    else {
        cerr << "Non connecté au serveur" << endl;
        return false;
    }
}

std::string Communication::receiveData() {
    if (connected) {
        char buffer[1024] = {0};
        int valread = read(m_socket, buffer, 1024);
        if (valread < 0) {
            std::cerr << "Erreur lors de la réception de données" << std::endl;
            return "";
        }
        cout << "Données reçues : " << buffer << endl;
        return buffer;
    }
    else {
        cerr << "Non connecté au serveur" << endl;
        return "";
    }
}

#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <thread>
#include <fstream>
using namespace std; 

void handle_client(int new_socket, fstream& fs)
{
    string message;

    // Receive a message from the client
    char buffer[2000];
    if (recv(new_socket, buffer, 2000, 0) < 0)
    {
         cout << "Receive failed" << endl;
        return;
    }
    message = buffer;
     cout << "Message received: " << message << endl;

    // Send a response to the client
   message = "j'ai bien reçu"; 
    write(new_socket, message.c_str(), message.length());

    close(new_socket);
}





int main()
{
    int socket_desc, new_socket, c;
    struct sockaddr_in server, client;
    fstream fs;


    // Create the socket
    socket_desc = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_desc == -1)
    {
         cout << "Impossible de créer le socket" << endl;
         return 1;
    }

    // Prepare the socket
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(8888);

    // Bind the socket
    if (bind(socket_desc, (struct sockaddr *)&server, sizeof(server)) < 0)
    {
         cout << "Liaison échouée" << endl;
        return 1;
    }
     cout << "Socket liée" << endl;

    // Listen for connections
    listen(socket_desc, 3);

    // Accept incoming connections
     cout << "Attente de connections..." << endl;
    c = sizeof(struct sockaddr_in);
    while (true) {
        new_socket = accept(socket_desc, (struct sockaddr *)&client, (socklen_t*)&c);
        if (new_socket < 0)
        {
             cout << "Connexion échouée" << endl;
            continue;
        }
        cout << "Nouvelle connexion acceptée" << endl;

        // Create a new thread to handle the client
        thread t(handle_client, new_socket,ref(fs));
        t.detach();
    }

    close(socket_desc);

    return 0;
}

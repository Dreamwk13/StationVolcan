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
    string message,messageretour;

    // message reçu
    char buffer[2000];
    if (recv(new_socket, buffer, 2000, 0) < 0)
    {
         cout << "Receive failed" << endl;
        return;
    }
    message = buffer;
     cout << "Message received: " << message << endl;

    close(new_socket);
   
fs.open("resultat.txt",fstream::out | fstream::app);
fs << message <<endl;
fs.close();



}





int main()
{
    int socket_desc, new_socket, c;
    struct sockaddr_in server, client;
    fstream fs;


    // Creation du socket
    socket_desc = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_desc == -1)
    {
         cout << "Impossible de créer le socket" << endl;
         return 1;
    }

    // Preparatin socket
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(8888);

    // liaison socket
    if (bind(socket_desc, (struct sockaddr *)&server, sizeof(server)) < 0)
    {
         cout << "Liaison échouée" << endl;
        return 1;
    }
     cout << "Socket liée" << endl;

    // Ecoute
    listen(socket_desc, 3);

    // attente connexion 
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

        // Creation thread pour chaques clients
        thread t(handle_client, new_socket,ref(fs));
        t.detach();
    }

    close(socket_desc);
    return 0;
}

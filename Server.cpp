/*
Name:Yair Shlomo
ID: 308536150
Name:Gal Eini
ID: 305216962
*/

#include <sstream>
#include "Server.h"
#include "HandleClient.h"

using namespace std;
#define MAX_CONNECTED_CLIENTS 10
map <string,int> Game;
Server::Server(int port): port(port), serverSocket(0),games() {
    cout << "Server" << endl;
}
void Server::start() {
    char* name;
    // Create a socket point
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket < 0) {
        throw "Error opening socket";
    }
    struct sockaddr_in serverAddress;
    // Assign a local address to the socket
    memset(&serverAddress, 0, sizeof(serverAddress));
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = INADDR_ANY;
    serverAddress.sin_port = htons(port);
    // Define the client socket's structures

    //struct sockaddr_in clientAddress2;
    if (bind(serverSocket, (struct sockaddr *) &serverAddress, sizeof(serverAddress)) == -1) {
        throw "Error on binding";
    }
    // Start listening to incoming connections
    if (listen(serverSocket, MAX_CONNECTED_CLIENTS) < 0) {
        throw "Error listening to socket";
    }

   // socklen_t clientAddressLen2 = sizeof(clientAddress);
    CommandsManager* commandsManager=new CommandsManager(games);
    ThreadArgs* threadArgs = new ThreadArgs;
    threadArgs->serverSocket=serverSocket;
    threadArgs->commandsManager=commandsManager;
    vector<int> clients;
    threadArgs->clients=clients;
    pthread_t thread;
    int rc = pthread_create(&thread, NULL, listening, (void *)threadArgs);
    cin >> name;
    if (strcmp(name,"exit")==0) {
        for (vector<int>::iterator it = threadArgs->clients.begin() ; it != threadArgs->clients.end(); ++it)
            close(*it);
            //cout <<it  ;
        cout << '\n';
    }
    /* while (true) {
        //int rc = pthread_create(&threads[0], NULL, printHello, NULL);

        cout << "Waiting for client connections..." << endl;
        // Accept a new client connection
        int clientSocket = accept(serverSocket, (struct
                sockaddr *) &clientAddress, &clientAddressLen);
        cout << "Client connected" << endl;
        if (clientSocket == -1) {
            throw "Error accepting client";
        }

       cout << "Waiting for another client connections..." << endl;
        char approval[2] = {'1'};
        int sendApproval = send(clientSocket, &approval[0], 1, 0);
        if (sendApproval < 0) {
            throw "error sending to client";
        }
        int clientSocket2 = accept(serverSocket, (struct
                sockaddr *) &clientAddress2, &clientAddressLen2);
        if (clientSocket2 == -1) {
            throw "Error accepting client";
        }
        cout << "2 clients connected" << endl;
        char buffer[2] = {'1', '2'};
        int sendByte = send(clientSocket, &buffer[0], 1, 0);
        if (sendByte < 0) {
            throw "error sending to client";
        }
        sendByte = send(clientSocket2, &buffer[1], 1, 0);
        if (sendByte < 0) {
            throw "error sending to client";
        }
        handleClient(clientSocket);
        close(clientSocket);
        i++;
    }*/
   // pthread_exit(NULL);
}
void* Server::listening(void *threadArgs) {
    struct sockaddr_in clientAddress;
    //long serversocket = (long)sSocket;
    ThreadArgs* threadArgs1=(ThreadArgs*)threadArgs;
    socklen_t clientAddressLen = sizeof(clientAddress);
    vector<pthread_t> gameThreads;

    while (true) {
        cout << "Waiting for client connections..." << endl;
        // Accept a new client connection
        int clientSocket = accept(threadArgs1->serverSocket, (struct
                sockaddr *) &clientAddress, &clientAddressLen);
        threadArgs1->clients.push_back(clientSocket);
        HandleClient handleClient(threadArgs1->commandsManager);
        handleClient.run(clientSocket);
    }
}




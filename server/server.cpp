#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <sys/select.h>
#include <vector>
#include <fstream>
#include <chrono>
#include <ctime>

int main() {
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == -1) {
        std::cerr << "Erreur lors de la création de la socket." << std::endl;
        return 1;
    }

    // Configuration de l'adresse et du port d'écoute
    struct sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = INADDR_ANY;
    serverAddress.sin_port = htons(12345);

    // Liaison de la socket à l'adresse et au port
    if (bind(serverSocket, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) == -1) {
        std::cerr << "Erreur lors de la liaison de la socket." << std::endl;
        close(serverSocket);
        return 1;
    }

    // Écoute de nouvelles connexions
    if (listen(serverSocket, 5) == -1) {
        std::cerr << "Erreur lors de l'écoute des connexions." << std::endl;
        close(serverSocket);
        return 1;
    }

	std::ofstream csvFile("data.csv");
	csvFile << "Heure,Num_capteur,Température,Hygrométrie" << std::endl;

    std::cout << "Serveur en attente de connexions sur le port 12345..." << std::endl;

    std::vector<int> clientSockets;
    fd_set readfds;

    while (true) {
        FD_ZERO(&readfds);
        FD_SET(serverSocket, &readfds);
        int maxfd = serverSocket;

        for (const int &clientSocket : clientSockets) {
            FD_SET(clientSocket, &readfds);
            if (clientSocket > maxfd) {
                maxfd = clientSocket;
            }
        }

        int activity = select(maxfd + 1, &readfds, nullptr, nullptr, nullptr);

        if (activity == -1) {
            std::cerr << "Erreur lors de l'appel à select." << std::endl;
            continue;
        }

        // Nouvelle connexion entrante
        if (FD_ISSET(serverSocket, &readfds)) {
            struct sockaddr_in clientAddress;
            socklen_t clientAddressLength = sizeof(clientAddress);
            int clientSocket = accept(serverSocket, (struct sockaddr *)&clientAddress, &clientAddressLength);

            if (clientSocket != -1) {
                std::cout << "Nouvelle connexion acceptée." << std::endl;
                clientSockets.push_back(clientSocket);
            }
        }

        for (auto it = clientSockets.begin(); it != clientSockets.end();) {
            int clientSocket = *it;
            if (FD_ISSET(clientSocket, &readfds)) {
                char buffer[1024];
                int bytesRead = recv(clientSocket, buffer, sizeof(buffer), 0);

                if (bytesRead > 0) {
                    // Get current time and format it
                    auto now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
                    std::string currentTime = std::ctime(&now);
                    currentTime.pop_back(); // Remove trailing newline

                    std::cout << "Données reçues : " << currentTime << "," << std::string(buffer, bytesRead) << std::endl;

                    // Add the formatted time to the CSV file
                    csvFile << currentTime << "," << std::string(buffer, bytesRead) << std::endl;

                    // Envoyer une réponse (par exemple, un message de confirmation)
                } else if (bytesRead == 0) {
                    std::cout << "La connexion a été fermée par le client." << std::endl;
                    close(clientSocket);
                    it = clientSockets.erase(it);
                } else {
                    std::cerr << "Erreur lors de la réception des données." << std::endl;
                    close(clientSocket);
                    it = clientSockets.erase(it);
                }
            } else {
                ++it;
            }
        }
    }

    // Fermer la socket du serveur (le code ci-dessous ne sera pas atteint dans ce cas)
    close(serverSocket);

    return 0;
}

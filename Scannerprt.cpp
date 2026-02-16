#include <iostream>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <string>
#include <vector>
#include <thread>
#include <mutex>
#include <conio.h>

#pragma comment(lib, "Ws2_32.lib")

std::mutex console_mutex;


void scannerport(std::string ip, int port) {
    SOCKET u = socket(AF_INET, SOCK_STREAM, 0);
    if (u == INVALID_SOCKET) return;
    u_long mode = 1;
    ioctlsocket(u, FIONBIO, &mode);

    sockaddr_in pill;
    pill.sin_family = AF_INET;
    pill.sin_port = htons(port);
    inet_pton(AF_INET, ip.c_str(), &pill.sin_addr);
    

    fd_set tir;
    FD_ZERO(&tir);
    FD_SET(u, &tir);

    timeval tps;
    tps.tv_sec = 0;
    tps.tv_usec = 300000;

    int cnx = connect(u, (sockaddr*)&pill, sizeof(pill));
    int result = select(0, NULL, &tir, NULL, &tps);

    
    if (result > 0) {
        std::lock_guard<std::mutex> lock(console_mutex);
        std::cout << "[+] " << ip << ":" << port << std::endl;
    }
    
    

    closesocket(u);
}

int main() {
    SetConsoleOutputCP(CP_UTF8);
    WSADATA wsa;
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
        std::cout << "Echec du scanner" << std::endl;
        return 1;
    }
    else {
        std::cout << "Initialisation du scanner réussie" << std::endl;
    }


    std::string target;
    std::cout << "Entrez une IP" << std::endl;
    std::cin >> target;
    sockaddr_in testy;
    if (inet_pton(AF_INET, target.c_str(), &testy.sin_addr) != 1) {
        std::cout << "Entrée invalide. Exemple d'IP correcte: 127.0.0.1." << std::endl;
        WSACleanup();
        _getch();
        return 1;
    }
    int maxthr = 1000;
    std::vector<std::thread> pool;

    for (int i = 1; i <= 65535; ++i) {
        
        pool.push_back(std::thread(scannerport, target, i));

        
        if (pool.size() >= maxthr) {
            for (auto& thrd : pool) {
                if (thrd.joinable()) {
                    thrd.join(); 
                }
            }
            pool.clear(); 
        }
    }

    
    for (auto& thrd : pool) {
        if (thrd.joinable()) thrd.join();
    }

    std::cout << "Scan sur l'IP " << target << " terminé." << std::endl;

    WSACleanup();
    std::cout << "Appuyez sur n'importe quelle touche pour quitter" << std::endl;
    _getch();
    return 0;
}

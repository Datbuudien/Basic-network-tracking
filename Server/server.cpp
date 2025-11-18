#include <iostream>
#include <string>
#include <winsock2.h>
#include <ws2tcpip.h>
#include<fstream>
#include<iomanip>
#include<ctime>
#pragma comment(lib, "ws2_32.lib")
using namespace std;
string getCurrentTime(){
    time_t now = time(0);
    tm *ltm  = localtime(&now);
    char buffer[80];
    strftime(buffer,sizeof(buffer),"%Y-%m-%d %H:%M:%S",ltm);
    return std::string(buffer);
}

int main() {
    // 1. Khởi tạo Winsock
    ofstream file("client_log.txt");
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "Loi khoi tao Winsock\n";
        return 1;
    }

    // 2. Tạo Socket
    SOCKET serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    
    // 3. Cấu hình địa chỉ IP và Cổng (Port)
    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY; // Lắng nghe mọi IP của máy này
    serverAddr.sin_port = htons(54000);      // Chọn cổng 54000 (cổng cao cho đỡ trùng)

    // 4. Bind (Gắn cổng vào socket)
    bind(serverSocket, (sockaddr*)&serverAddr, sizeof(serverAddr));

    // 5. Listen (Bắt đầu chờ kết nối)
    listen(serverSocket, SOMAXCONN);
    
    cout << "======================================\n";
    cout << "SERVER DANG CHAY TAI CONG 54000\n";
    cout << "Hay mo CMD go 'ipconfig' de xem IP may nay.\n";
    cout << "Dang cho may Client ket noi...\n";
    cout << "======================================\n";

    // 6. Accept (Chấp nhận kết nối khi có người vào)
    sockaddr_in clientAddr;
    int clientSize = sizeof(clientAddr);
    SOCKET clientSocket = accept(serverSocket, (sockaddr*)&clientAddr, &clientSize);

    if (clientSocket == INVALID_SOCKET) {
        std::cerr << "Loi ket noi!\n";
    } else {
        std::cout << ">> MOT MAY KHACH DA KET NOI THANH CONG!\n";
    }

    // 7. Vòng lặp nhận tin nhắn
    char buffer[4096];
    while (true) {
        ZeroMemory(buffer, 4096); // Xóa sạch bộ nhớ đệm
        
        // Nhận dữ liệu
        int bytesReceived = recv(clientSocket, buffer, 4096, 0);
        
        if (bytesReceived > 0) {
            string current = getCurrentTime();
            std::cout << "Client noi: " << current << " " << std::string(buffer, 0, bytesReceived) << "\n";          
            file << current << " " << std::string(buffer,0,bytesReceived) << "\n";
            file.flush();
        } else {
            std::cout << "Client da ngat ket noi.\n";
            break;
        }
    }

    // 8. Dọn dẹp
    file.close();
    closesocket(clientSocket);
    closesocket(serverSocket);
    WSACleanup();
    return 0;
}
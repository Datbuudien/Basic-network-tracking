// ================= CLIENT_TRACK_ACTIVE_TAB.cpp =================
#define _WINSOCK_DEPRECATED_NO_WARNINGS 
#include <iostream>
#include <string>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>
#include <tlhelp32.h> 

#pragma comment(lib, "ws2_32.lib")

// Ham chuyen doi Unicode sang UTF-8 (de hien tieng Viet)
std::string WCharToString(const WCHAR* wstr) {
    if (!wstr) return "";
    std::wstring ws(wstr);
    int size_needed = WideCharToMultiByte(CP_UTF8, 0, &ws[0], (int)ws.size(), NULL, 0, NULL, NULL);
    std::string strTo(size_needed, 0);
    WideCharToMultiByte(CP_UTF8, 0, &ws[0], (int)ws.size(), &strTo[0], size_needed, NULL, NULL);
    return strTo;
}

// Ham lay ten file .exe tu PID (Vi du: chrome.exe)
std::string GetProcessNameByPID(DWORD pid) {
    HANDLE hProcessSnap;
    PROCESSENTRY32W pe32; 
    hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hProcessSnap == INVALID_HANDLE_VALUE) return "Unknown";

    pe32.dwSize = sizeof(PROCESSENTRY32W);
    if (Process32FirstW(hProcessSnap, &pe32)) {
        do {
            if (pe32.th32ProcessID == pid) {
                std::string name = WCharToString(pe32.szExeFile);
                CloseHandle(hProcessSnap);
                return name;
            }
        } while (Process32NextW(hProcessSnap, &pe32));
    }
    CloseHandle(hProcessSnap);
    return "Unknown";
}

int main() {
    // Hien thi tieng Viet trong Console
    SetConsoleOutputCP(65001); 

    // --- KET NOI SERVER ---
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);
    SOCKET clientSocket = socket(AF_INET, SOCK_STREAM, 0);

    std::string ipAddress;
    std::cout << "Nhap IP Server: ";
    std::getline(std::cin, ipAddress);

    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(54000);
    serverAddr.sin_addr.s_addr = inet_addr(ipAddress.c_str());

    if (connect(clientSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        std::cout << "Ket noi that bai!\n";
        return 1;
    }
    std::cout << ">> DA KET NOI! Dang theo doi cua so ban dang xem...\n";

    // Bien luu trang thai cu de so sanh
    std::string lastTitle = "";
    std::string lastExe = "";

    while (true) {
        // 1. Lay Handle cua cua so dang ACTIVE (Cua so ban dang dung)
        HWND hwnd = GetForegroundWindow();

        if (hwnd != NULL) {
            // 2. Lay Tieu de cua so (Window Title)
            WCHAR titleBuffer[1024];
            if (GetWindowTextW(hwnd, titleBuffer, 1024) > 0) {
                std::string currentTitle = WCharToString(titleBuffer);
                
                // 3. Lay PID va Ten file EXE cua cua so do
                DWORD pid;
                GetWindowThreadProcessId(hwnd, &pid);
                std::string currentExe = GetProcessNameByPID(pid);

                // --- KIEM TRA SU THAY DOI ---
                // Chi gui neu Tieu de thay doi HOAC Ung dung thay doi
                if (currentTitle != lastTitle || currentExe != lastExe) {
                    
                    // Bo qua neu title rong hoac la "Program Manager" (rac he thong)
                    if (!currentTitle.empty() && currentTitle != "Program Manager") {
                        
                        std::string msg = "[DANG XEM] " + currentExe + " | Noi dung: " + currentTitle;
                        
                        std::cout << msg << "\n";
                        send(clientSocket, msg.c_str(), msg.size() + 1, 0); // Gui ve Server
                        
                        // Cap nhat trang thai cu
                        lastTitle = currentTitle;
                        lastExe = currentExe;
                    }
                }
            }
        }

        // Ngu 1 giay roi quet tiep (De do ton CPU)
        Sleep(1000);
    }

    closesocket(clientSocket);
    WSACleanup();
    return 0;
}
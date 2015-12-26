#include <stdio.h>
#include <windows.h>
#include <winuser.h>
#include <winsock2.h>
#pragma comment(lib, "ws2_32.lib")

void setData(char *val);

int main() {
	WSADATA wsa;
    SOCKET s, new_socket;
    int c, recv_size;
    struct sockaddr_in server, client;
    char reply[2000];

    puts("Starting Winsock..");
    if (WSAStartup(MAKEWORD(2,2), &wsa) != 0)
        puts("ERR, :(");
    puts("Started!");
    
    if((s = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET)
        puts("failed");
    puts("Socket init!");

    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_family = AF_INET;
    server.sin_port = htons(8080);

    if (bind(s,(struct sockaddr *)&server, sizeof(server)) == SOCKET_ERROR)
    	puts("Bind failed");
    puts("SUCCESS");

    listen(s, 3);

    c = sizeof(struct sockaddr_in);
    new_socket = accept(s, (struct sockaddr *)&client, &c);
    if (new_socket == INVALID_SOCKET)
    	puts("FAILED");
    puts("Got connection!");
    
    if ((recv_size = recv(new_socket, reply, 2000, 0)) == SOCKET_ERROR)
        printf("failed to recv %d", WSAGetLastError());
    reply[recv_size] = '\0';
    puts(reply);
    setData(reply); 
    puts("Copied to Clipboard")
    closesocket(s);
    WSACleanup();
	return 0;
}

void setData(char *val) {
	if (OpenClipboard(NULL) == 0)
        puts("No such luck!");
    HGLOBAL hMem = GlobalAlloc(GMEM_MOVEABLE, (const size_t) strlen(val)+1);
    memcpy(GlobalLock(hMem), val, (const size_t) strlen(val)+1);
    GlobalUnlock(hMem);
    EmptyClipboard();
    SetClipboardData(CF_TEXT, hMem);
    CloseClipboard();
}

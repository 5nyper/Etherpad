#include <stdio.h>
#include <windows.h>
#include <winuser.h>
#include <winsock2.h>
#pragma comment(lib, "ws2_32.lib")

int setData(char *val);

int main() {
	system("title SERVER");
	WSADATA wsa;
    SOCKET s, new_socket;
    int c, recv_size;
    struct sockaddr_in server, client;
    char reply[2000];

    puts("Starting Winsock..");
    if (WSAStartup(MAKEWORD(2,2), &wsa) != 0) {
        MessageBox(NULL,
                "Unable to start WinSock!",
                "Failure",
                MB_OK |
                MB_ICONWARNING);
        return 1;
    }
    puts("Started!");
    
    if((s = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET){
        MessageBox(NULL,
                "Unable to start Socket!",
                "Failure",
                MB_OK |
                MB_ICONWARNING);
        return 1;
    }

    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_family = AF_INET;
    server.sin_port = htons(8080);

    if (bind(s,(struct sockaddr *)&server, sizeof(server)) == SOCKET_ERROR) {
    	MessageBox(NULL,
                "Unable to bind to socket!",
                "Failure",
                MB_OK |
                MB_ICONWARNING);
        return 1;
    }
    puts("SUCCESS");
	while(1) {
	    listen(s, 3);

	    c = sizeof(struct sockaddr_in);
	    new_socket = accept(s, (struct sockaddr *)&client, &c);
	    if (new_socket == INVALID_SOCKET) {
	    	MessageBox(NULL,
                "Unable to Unable to accept new connection!",
                "Failure",
                MB_OK |
                MB_ICONWARNING);
            return 1;
        }
	    puts("Got connection!");
	    if ((recv_size = recv(new_socket, reply, 2000, 0)) == SOCKET_ERROR){
	        MessageBox(NULL,
                "Unable to recieve data!",
                "Failure",
                MB_OK |
                MB_ICONWARNING);
            return 1;
        }
	    reply[recv_size] = '\0';
	    puts(reply);
	    if (setData(reply) != 0) {
             MessageBox(NULL,
                "Unable to set data to Clipboard!",
                "Failure",
                MB_OK |
                MB_ICONWARNING);
        }
        else {
	    MessageBox(NULL,
                "New data copied to Clipboard from peer!",
                "Attention!",
                MB_OK |
                MB_ICONWARNING);
        }
	}
    closesocket(s);
    WSACleanup();
    getchar();
	return 0;
}

int setData(char *val) {
	if (OpenClipboard(NULL) == 0){
         MessageBox(NULL,
                "Unable to OpenClipboard!",
                "Failure",
                MB_OK |
                MB_ICONWARNING);
        return 1;
    }
    HGLOBAL hMem = GlobalAlloc(GMEM_MOVEABLE, (const size_t) strlen(val)+1);
    memcpy(GlobalLock(hMem), val, (const size_t) strlen(val)+1);
    GlobalUnlock(hMem);
    EmptyClipboard();
    SetClipboardData(CF_TEXT, hMem);
    CloseClipboard();
    return 0;
}

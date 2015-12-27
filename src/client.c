#include <stdio.h>
#include <windows.h>
#include <winuser.h>
#include <winsock2.h>
#pragma comment(lib, "ws2_32.lib")

size_t strlen_s(const char *s, size_t maxlen);
int sendData();

void Stealth();

char *getData();

int main() {
    char *current = getData();
    printf("%s\n", current);
    system("title CLIENT");
    //Stealth();
    while(1) {
        if (strcmp(current, getData()) != 0) {
            printf("%s\n", current);
            printf("%s\n", getData());
            if (MessageBox(NULL,
                "Send recently copied text to other PC?",
                "Send Authorization",
                MB_YESNO |
                MB_ICONQUESTION) == IDYES) {
                strcpy(current, getData());
                sendData();
            }
        }
        SleepEx(1,1); 
    } 
    
    return 0;
}

char *getData() {
    if (OpenClipboard(NULL) == 0)
        puts("No such luck!");
    HGLOBAL var = GetClipboardData(CF_TEXT);
    char *data = GlobalLock(var);
    int size = GlobalSize(var);
    char *result = malloc(strlen_s(data, size+1));
    memcpy(result, data, size);
    GlobalUnlock(var);
    return result;
}

int sendData() {
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
        MessageBox(NULL,
                "Couldn't connect",
                "Failure",
                MB_OK |
                MB_ICONWARNING);
        return 1;
    }

    puts("CONNECTED");

    clipboard = (char *) getData();
    puts(clipboard);
    if (send(s, clipboard, strlen(clipboard), 0) < 0)
        puts("send failed");
    puts("data sent!");
    MessageBox(NULL,
                "Sent!",
                "Success!",
                MB_OK |
                MB_ICONINFORMATION);
    return 0;
}

void Stealth(){
    HWND Stealth;
    AllocConsole();
    Stealth = FindWindowA("ConsoleWindowClass", NULL);
    ShowWindow(Stealth,0);
}
size_t strlen_s(const char *str, size_t maxlen)
{
     size_t i;
     for(i = 0; i < maxlen && str[i]; i++);
     return i;
}

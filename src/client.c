#include <stdio.h>
#include <windows.h>
#include <winuser.h>
#include <winsock2.h>

typedef enum {
    FAILURE,
    SUCCESS,
} RESULT;

size_t strlen_s(const char *s, size_t maxlen);
RESULT sendData();
void Stealth();
char *getData();

int main() {
    char *current = getData();
    printf("%s\n", current);
    system("title CLIENT");
    Stealth();
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
                if(sendData() != SUCCESS){
                    MessageBox(NULL,
                            "Unable to Send!",
                            "Failure",
                            MB_OK |
                            MB_ICONWARNING);
                }
                else {
                 MessageBox(NULL,
                            "Sent!",
                            "Success!",
                            MB_OK |
                            MB_ICONINFORMATION);
                }
            }
            else 
                strcpy(current, getData());
        }
        SleepEx(1,1); 
    } 
    
    return 0;
}

char *getData() {
    if (OpenClipboard(NULL) == 0){
        MessageBox(NULL,
                "Unable to OpenClipboard!",
                "Failure",
                MB_OK |
                MB_ICONWARNING);
        return "(null)";
    }
    HGLOBAL var = GetClipboardData(CF_TEXT);
    char *data = GlobalLock(var);
    int size = GlobalSize(var);
    char *result = malloc(strlen_s(data, size)+1);
    result[size] = '\0';
    memcpy(result, data, size);
    GlobalUnlock(var);
    return result;
}

RESULT sendData() {
    if (OpenClipboard(NULL) == 0){
        MessageBox(NULL,
                "Unable to OpenClipboard!",
                "Failure",
                MB_OK |
                MB_ICONWARNING);
        return FAILURE;
    }
    WSADATA wsa;
    SOCKET s;
    struct sockaddr_in server;
    char *clipboard;

    puts("Starting Winsock..");
    if (WSAStartup(MAKEWORD(2,2), &wsa) != 0) {
        MessageBox(NULL,
                "Unable to start WinSock!",
                "Failure",
                MB_OK |
                MB_ICONWARNING);
        return FAILURE;
    }
    puts("Started!");
    
    if((s = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET){
        MessageBox(NULL,
                "Unable to start Socket!",
                "Failure",
                MB_OK |
                MB_ICONWARNING);
        return FAILURE;
    }
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
        return FAILURE;
    }

    puts("CONNECTED");

    clipboard = (char *) getData();
    puts(clipboard);
    if (send(s, clipboard, strlen(clipboard), 0) < 0) {
        MessageBox(NULL,
                "Unable to send data!",
                "Failure",
                MB_OK |
                MB_ICONWARNING);
        return FAILURE;
    }
    puts("data sent!");
    return SUCCESS;
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

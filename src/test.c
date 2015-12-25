#include <stdio.h>
#include <windows.h>
#include <winuser.h>

HANDLE getData();
void getData(char *val);

int main() {
    if (OpenClipboard(NULL) == 0)
        puts("No such luck!");
    printf("%s\n", getData());
    setData("Hello World!");
    OpenClipboard(NULL);    //reopen because setData sets Clipboard ownership to NULL after Emptying
    printf("%s\n", getData());
    CloseClipboard();
	return 0;
}

HANDLE getData() {
    return GetClipboardData(CF_TEXT);
}

void setData(char *val) {
    HGLOBAL hMem = GlobalAlloc(GMEM_MOVEABLE, (const size_t) strlen(val)+1);
    memcpy(GlobalLock(hMem), val, (const size_t) strlen(val)+1);
    GlobalUnlock(hMem);
    EmptyClipboard();
    SetClipboardData(CF_TEXT, hMem);
    CloseClipboard();
}

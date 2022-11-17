#include <windows.h>
#include <time.h>

#define uint unsigned int

char project_name[] = "AaP-Course-work";

int main() {
    PROCESS_INFORMATION pinfo = {0};
    STARTUPINFOW sinfo = {0};
    sinfo.cb = sizeof(sinfo);
    clock_t start = clock();
    if (CreateProcess(NULL, "gcc.exe main.c functions.c -o main_new.exe", NULL, NULL, FALSE, REALTIME_PRIORITY_CLASS, NULL, NULL, &sinfo, &pinfo))
        printf("C.EXE: Compilation started\n");
    else {
        printf("C.EXE: Can't start compiling (0x%08x)\n", GetLastError());
        return 1;
    }
    WaitForSingleObject(pinfo.hProcess, INFINITE);
    clock_t end = clock();
    DWORD res;
    GetExitCodeProcess(pinfo.hProcess, &res);
    if (!res) printf("C.EXE: \"%s\" compiled with exit code %d (ellapsed %d ms)\n", project_name, res, end-start);
    else {
        printf("C.EXE: Error while compiling \"%s\" (0x%08x)\n", project_name, res);
        return 1;
    }
    printf("C.EXE: Press any key to start \"%s\"\n", project_name);
    _getch();
    if (CreateProcess("./main_new.exe", "./main_new.exe", NULL, NULL, FALSE, REALTIME_PRIORITY_CLASS, NULL, NULL, &sinfo, &pinfo))
        printf("C.EXE: \"%s\" started\n", project_name);
    else {
        printf("C.EXE: Can't start \"%s\" (0x%08x)\n", project_name, GetLastError());
        return 1;
    }
    WaitForSingleObject(pinfo.hProcess, INFINITE);
    printf("C.EXE: \"%s\" finished\n", project_name);
    return 0;
}

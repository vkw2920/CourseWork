#include <windows.h>
#include <time.h>
#include <stdio.h>
#include <assert.h>

#define uint unsigned int

char project_name[] = "AaP-Course-work";
char input_file_list[] = "./main.c ./functions.c";
char output_file_name[] = "./main_new.exe";

char info[] = "\x1b[0;32m[I]\x1b[0m";
char warn[] = "\x1b[0;33m[W]\x1b[0m";
char error[] = "\x1b[0;31m[E]\x1b[0m\a";
char critical[] = "\x1b[0;91m[C]\x1b[0m\a";

DWORD priorities[4] = {
    IDLE_PRIORITY_CLASS,
    NORMAL_PRIORITY_CLASS,
    HIGH_PRIORITY_CLASS,
    REALTIME_PRIORITY_CLASS
};

const char* priorities_str[4] =  {
    "IDLE_PRIORITY_CLASS",
    "NORMAL_PRIORITY_CLASS",
    "HIGH_PRIORITY_CLASS",
    "REALTIME_PRIORITY_CLASS"
};

char** str_split(char* a_str, const char a_delim) {
    char** result    = 0;
    size_t count     = 0;
    char* tmp        = a_str;
    char* last_comma = 0;
    char delim[2] = {a_delim, 0};
    while (*tmp) {
        if (a_delim == *tmp) count++, last_comma = tmp;
        tmp++;
    }
    count += last_comma < (a_str + strlen(a_str) - 1);
    result = (char**)calloc(++count, sizeof(char*));

    if (result) {
        size_t idx  = 0;
        char* token = strtok(a_str, delim);
        while (token) assert(idx < count), *(result + idx++) = strdup(token), token = strtok(0, delim);
        assert(idx == count - 1);
        *(result + idx) = 0;
    }
    return result;
}

int main(int argc, char* argv[]) {
    DWORD compile_priority = priorities[3];
    DWORD run_priority = priorities[3];
    char** exe_path = str_split(argv[0], '\\');
    char** exe_fname = exe_path;
    while (*(exe_fname+1)) exe_fname++;
    printf("Using: %s [<PRIORITY_1> [<PRIORITY_2>]]\n    Priority has a one char (I, N, H or R) or one number (0, 1, 2 or 3)\n\n", *exe_fname);
    int number = 0;
    if (argc == 1) {
        printf("Launcher: %s Using %s for compilation and run.\n", info, priorities_str[3]);
    } else if (argc == 2) {
        if (strlen(argv[1]) == 1) {
                if (argv[1][0] == '0' || argv[1][0] == 'I' || argv[1][0] == 'i') compile_priority = run_priority = priorities[0], number = 0;
            else if (argv[1][0] == '1' || argv[1][0] == 'N' || argv[1][0] == 'n') compile_priority = run_priority = priorities[1], number = 1;
            else if (argv[1][0] == '2' || argv[1][0] == 'H' || argv[1][0] == 'h') compile_priority = run_priority = priorities[2], number = 2;
            else if (argv[1][0] == '3' || argv[1][0] == 'R' || argv[1][0] == 'r') compile_priority = run_priority = priorities[3], number = 3;
            else printf("Launcher: %s Can't recognize priority class.\n", warn);
            printf("Launcher: %s Using %s for compilation and run.\n", info, priorities_str[number]);
        } else {
                 if (argv[1][0] == '0' || argv[1][0] == 'I' || argv[1][0] == 'i') compile_priority = priorities[0], number = 0;
            else if (argv[1][0] == '1' || argv[1][0] == 'N' || argv[1][0] == 'n') compile_priority = priorities[1], number = 1;
            else if (argv[1][0] == '2' || argv[1][0] == 'H' || argv[1][0] == 'h') compile_priority = priorities[2], number = 2;
            else if (argv[1][0] == '3' || argv[1][0] == 'R' || argv[1][0] == 'r') compile_priority = priorities[3], number = 3;
            else printf("Launcher: %s Can't recognize priority class.\n", warn);
            printf("Launcher: %s Using %s for compilation.\n", info, priorities_str[number]);
                if (argv[1][1] == '0' || argv[1][1] == 'I' || argv[1][1] == 'i') run_priority = priorities[0], number = 0;
            else if (argv[1][1] == '1' || argv[1][1] == 'N' || argv[1][1] == 'n') run_priority = priorities[1], number = 1;
            else if (argv[1][1] == '2' || argv[1][1] == 'H' || argv[1][1] == 'h') run_priority = priorities[2], number = 2;
            else if (argv[1][1] == '3' || argv[1][1] == 'R' || argv[1][1] == 'r') run_priority = priorities[3], number = 3;
            else printf("Launcher: %s Can't recognize priority class.\n", warn);
            printf("Launcher: %s Using %s for run.\n", info, priorities_str[number]);
        }
    } else if (argc == 3) {
             if (argv[1][0] == '0' || argv[1][0] == 'I' || argv[1][0] == 'i') compile_priority = priorities[0], number = 0;
        else if (argv[1][0] == '1' || argv[1][0] == 'N' || argv[1][0] == 'n') compile_priority = priorities[1], number = 1;
        else if (argv[1][0] == '2' || argv[1][0] == 'H' || argv[1][0] == 'h') compile_priority = priorities[2], number = 2;
        else if (argv[1][0] == '3' || argv[1][0] == 'R' || argv[1][0] == 'r') compile_priority = priorities[3], number = 3;
        else printf("Launcher: %s Can't recognize priority class.\n", warn);
        printf("Launcher: %s Using %s for compilation.\n", info, priorities_str[number]);
             if (argv[2][0] == '0' || argv[2][0] == 'I' || argv[2][0] == 'i') run_priority = priorities[0], number = 0;
        else if (argv[2][0] == '1' || argv[2][0] == 'N' || argv[2][0] == 'n') run_priority = priorities[1], number = 1;
        else if (argv[2][0] == '2' || argv[2][0] == 'H' || argv[2][0] == 'h') run_priority = priorities[2], number = 2;
        else if (argv[2][0] == '3' || argv[2][0] == 'R' || argv[2][0] == 'r') run_priority = priorities[3], number = 3;
        else printf("Launcher: %s Can't recognize priority class.\n", warn);
        printf("Launcher: %s Using %s for run.\n", info, priorities_str[number]);
    } else {
        printf("Launcher: %s You have used unsupported arguments.\n", critical);
        return 1;
    }
    PROCESS_INFORMATION pinfo = {0};
    STARTUPINFOW sinfo = {0};
    sinfo.cb = sizeof(sinfo);
    clock_t start = clock();
    char compile_cmd[1024] = {0};
    sprintf(compile_cmd, "gcc.exe %s -o %s", input_file_list, output_file_name);
    if (CreateProcess(NULL, compile_cmd, NULL, NULL, FALSE, compile_priority, NULL, NULL, &sinfo, &pinfo))
        printf("Launcher: %s Compilation started\n", info);
    else {
        printf("Launcher: %s Can't start compiling (0x%08x)\n", error, GetLastError());
        return 1;
    }
    WaitForSingleObject(pinfo.hProcess, INFINITE);
    clock_t end = clock();
    DWORD res;
    GetExitCodeProcess(pinfo.hProcess, &res);
    if (!res) printf("Launcher: %s \"%s\" compiled with exit code %d (ellapsed %d ms)\n", info, project_name, res, end-start);
    else {
        printf("Launcher: %s Error while compiling \"%s\" (0x%08x)\n", error, project_name, res);
        return res;
    }
    printf("Launcher: %s Starting \"%s\"...\n", info, project_name);
    // _getch();
    sinfo.lpTitle = project_name;
    if (CreateProcess(output_file_name, output_file_name, NULL, NULL, FALSE, run_priority | CREATE_NEW_CONSOLE, NULL, NULL, &sinfo, &pinfo))
        printf("Launcher: %s \"%s\" started\n", info, project_name);
    else {
        printf("Launcher: %s Can't start \"%s\" (0x%08x)\n", error, project_name, GetLastError());
        return 1;
    }
    WaitForSingleObject(pinfo.hProcess, INFINITE);
    res = 0;
    GetExitCodeProcess(pinfo.hProcess, &res);
    if (!res) printf("Launcher: %s \"%s\" finished\n", info, project_name);
    else {
        char err_type[48] = "\0";
        if (res == 0xC0000005) strcpy(err_type, "Segmentation fault");
        if (err_type[0]) printf("Launcher: %s \"%s\" finished with error (0x%08x / %s)\n", error, project_name, res, err_type);
        else printf("Launcher: %s \"%s\" finished with error (0x%08x)\n", error, project_name, res);
    }
    return 0;
}

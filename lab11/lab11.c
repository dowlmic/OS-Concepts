#include <Windows.h>
#include <stdio.h>

char* getState(DWORD state) {
        if (state == 0x1000) {
                return "commit";
        }
        else if (state == 0x10000) {
                return "free";
        }
        else {
                return "reserve";
        }
}

int main() {
        SYSTEM_INFO info;
        GetSystemInfo(&info);
        printf("Page Size: %d\n", info.dwPageSize);

        MEMORY_BASIC_INFORMATION memInfo;
        void* var = malloc(1048576); //1 mil bytes
        VirtualQuery(var, &memInfo, sizeof(MEMORY_BASIC_INFORMATION));
        DWORD state1 = memInfo.State;

        free(var);
        VirtualQuery(var, &memInfo, sizeof(MEMORY_BASIC_INFORMATION));
        DWORD state2 = memInfo.State;

        char *s1 = getState(state1);
        char* s2 = getState(state2);
        printf("State of malloced mem: %s\n", s1);
        printf("State of freed mem: %s\n", s2);
}

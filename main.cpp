#include <windows.h>
#include <iostream>
#include <string>
#include <thread>
#include <mutex>
#include <chrono>  // if using time-based functions

using namespace std;
void monitorChanges(HANDLE dirHandle);
HANDLE startMonitoring(const string& path);
 // Function prototype
int main() {
    std::string path;
    std::cout << "Enter directory to monitor: ";
    std::getline(std::cin, path);

    HANDLE dirHandle = startMonitoring(path);
    if (dirHandle == NULL) {
        std::cerr << "Failed to open directory for monitoring." << std::endl;
        return 1;
    }

    std::cout << "Monitoring started. Press Ctrl+C to exit..." << std::endl;
    monitorChanges(dirHandle);

    CloseHandle(dirHandle);
    return 0;
}

void monitorChanges(HANDLE dirHandle) {
    const DWORD bufferSize = 8192;
    char buffer[bufferSize];
    DWORD bytesReturned;

    while (true) {
        BOOL success = ReadDirectoryChangesW(
            dirHandle,
            buffer,
            bufferSize,
            TRUE, // TRUE = monitor subdirectories as well
            FILE_NOTIFY_CHANGE_FILE_NAME | 
            FILE_NOTIFY_CHANGE_DIR_NAME |
            FILE_NOTIFY_CHANGE_ATTRIBUTES |
            FILE_NOTIFY_CHANGE_SIZE |
            FILE_NOTIFY_CHANGE_LAST_WRITE,
            &bytesReturned,
            NULL,
            NULL
        );

        if (!success) {
            std::cerr << "Error reading directory changes. Code: " << GetLastError() << std::endl;
            break;
        }

        FILE_NOTIFY_INFORMATION* info = (FILE_NOTIFY_INFORMATION*)buffer;
        do {
            std::wstring fileName(info->FileName, info->FileNameLength / sizeof(WCHAR));
            switch (info->Action) {
                case FILE_ACTION_ADDED:
                    std::wcout << L"File created: " << fileName << std::endl;
                    break;
                case FILE_ACTION_REMOVED:
                    std::wcout << L"File deleted: " << fileName << std::endl;
                    break;
                case FILE_ACTION_MODIFIED:
                    std::wcout << L"File modified: " << fileName << std::endl;
                    break;
                case FILE_ACTION_RENAMED_OLD_NAME:
                    std::wcout << L"File renamed from: " << fileName << std::endl;
                    break;
                case FILE_ACTION_RENAMED_NEW_NAME:
                    std::wcout << L"File renamed to: " << fileName << std::endl;
                    break;
                default:
                    std::wcout << L"Other action: " << fileName << std::endl;
            }
            if (info->NextEntryOffset == 0) break;
            info = (FILE_NOTIFY_INFORMATION*)((BYTE*)info + info->NextEntryOffset);
        } while (true);
    }
}


HANDLE startMonitoring(const string& path) {
    HANDLE dirHandle = CreateFileA(
        path.c_str(),
        FILE_LIST_DIRECTORY,
        FILE_SHARE_READ | FILE_SHARE_WRITE|FILE_SHARE_DELETE,
        NULL,
        OPEN_EXISTING,
        FILE_FLAG_BACKUP_SEMANTICS,
        NULL
    );

    if (dirHandle == INVALID_HANDLE_VALUE) {
        cerr << "Error opening directory. Code: " << GetLastError() << endl;
        return NULL;
    }
    return dirHandle;
}

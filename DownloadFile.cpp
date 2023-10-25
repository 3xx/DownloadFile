#include <iostream>
#include <windows.h>
#include <urlmon.h>
#pragma comment(lib, "urlmon.lib")
using namespace std;

BOOL FileExistsStatus(LPCSTR path)
{
    DWORD dwAttribute = GetFileAttributesA(path);
    if (dwAttribute == 0xFFFFFFFF) return false;
    return true;
}

BOOL DownloadFiles(LPCSTR url, LPCSTR downloadPath)
{
    
    char fileName[MAX_PATH];
    const char* lastSlash = strrchr(url, '/');
    if (lastSlash) {
        strcpy(fileName, lastSlash + 1);
    }
    else {
        cout << "Invalid URL." << endl;
        return false;
    }

    
    char completePath[MAX_PATH];
    strcpy(completePath, downloadPath);
    strcat(completePath, "\\");
    strcat(completePath, fileName);

    HRESULT hr = URLDownloadToFileA(NULL, url, completePath, 0, NULL);
    if (hr == S_OK && FileExistsStatus(completePath)) {
        
        WIN32_FILE_ATTRIBUTE_DATA fileInfo;
        if (GetFileAttributesExA(completePath, GetFileExInfoStandard, &fileInfo)) {
            double fileSizeKB = static_cast<double>(fileInfo.nFileSizeLow) / 1024;
            cout << "Downloaded: " << fileName << " (" << fileSizeKB << " KB)" << endl;
            return true;
        }
        else {
            cout << "Error getting file size for: " << fileName << endl;
            return false;
        }
    }
    else {
        cout << "Error downloading: " << fileName << endl;
        return false;
    }
}

int main()
{
    LPCSTR url;
    char link[2048];
    cout << "Enter the download link: ";
    cin.getline(link, sizeof(link));
    url = link;

    LPCSTR downloadPath = ".";

    if (DownloadFiles(url, downloadPath)) {
        cout << "Download successful!" << endl;
    }
    else {
        cout << "Download failed!" << endl;
    }

    system("pause");
    return 0;
}

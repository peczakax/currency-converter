#include "CurrencyConverter.h"

#include <windows.h>
#include <wininet.h>
#include <string>
#include <stdexcept>

#pragma comment(lib, "wininet.lib")

std::string CurrencyConverter::getHttpResponse(const std::string& url) {
    HINTERNET hInternet = InternetOpen(L"CurrencyConverter", INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);
    if (!hInternet) {
        throw std::runtime_error("InternetOpen failed");
    }

    HINTERNET hConnect = InternetOpenUrl(hInternet, std::wstring(url.begin(), url.end()).c_str(), NULL, 0, INTERNET_FLAG_RELOAD, 0);
    if (!hConnect) {
        InternetCloseHandle(hInternet);
        throw std::runtime_error("InternetOpenUrl failed");
    }

    char buffer[4096];
    DWORD bytesRead;
    std::string response;

    while (InternetReadFile(hConnect, buffer, sizeof(buffer), &bytesRead) && bytesRead != 0) {
        response.append(buffer, bytesRead);
    }

    InternetCloseHandle(hConnect);
    InternetCloseHandle(hInternet);

    return response;
}

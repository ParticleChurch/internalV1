#include "GUI/HTTP.hpp"

std::string HTTP::URLEncode(const std::string& value) {
    std::ostringstream escaped;
    escaped.fill('0');
    escaped << std::hex;

    for (std::string::const_iterator i = value.begin(), n = value.end(); i != n; ++i) {
        std::string::value_type c = (*i);

        // Keep alphanumeric and other accepted characters intact
        if (isalnum(c) || c == '-' || c == '_' || c == '.' || c == '~') {
            escaped << c;
            continue;
        }

        // Any other characters are percent-encoded
        escaped << std::uppercase;
        escaped << '%' << std::setw(2) << int((unsigned char)c);
        escaped << std::nouppercase;
    }

    return escaped.str();
}

std::string HTTP::GET(std::string Host, std::string Directory, std::string URLArguments)
{
    Directory += "?" + URLArguments;

    // configure and open connection
    HINTERNET hInternet = InternetOpenA("InetURL/1.0", INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);
    HINTERNET hConnection = InternetConnectA(hInternet, Host.c_str(), INTERNET_DEFAULT_HTTPS_PORT, 0, 0, INTERNET_SERVICE_HTTP, 0, 0);
    HINTERNET hData = HttpOpenRequestA(hConnection, "GET", Directory.c_str(), 0, 0, 0, INTERNET_FLAG_SECURE | INTERNET_FLAG_RELOAD, 0);
    if (!HttpSendRequestA(hData, NULL, 0, NULL, 0))
        std::cout << "HTTP GET failed w/ winerror: " << GetLastError() << std::endl;

    // read data
    std::string HTML = "";
    
    char Buffer[2048];
    DWORD ChunkSize = 0;
    DWORD BytesRead = 0;
    while (InternetReadFile(hData, Buffer, 2047, &ChunkSize) && ChunkSize > 0)
    {
        // append this chunk to tracked total
        Buffer[ChunkSize] = 0;
        HTML += Buffer;
        BytesRead += ChunkSize;
    }

    if (!InternetCloseHandle(hInternet))
        std::cout << "Failed to close hInternet w/ winerror:" << GetLastError() << std::endl;
    
    // return pointer to first byte in the vector
    return HTML;
}

// wrapper for POST so we dont have to keep typing the same url
std::string HTTP::API(std::string CMD, Arguments Arguments)
{
    Arguments.Add("cmd", CMD);
    return GET("www.a4g4.com", "/CSGO/api.php", Arguments.String);
}
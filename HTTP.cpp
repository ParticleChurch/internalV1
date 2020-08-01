#include "HTTP.hpp"

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

std::string HTTP::POST(std::string Host, std::string Directory, std::string URLArguments)
{
    Directory += "?" + URLArguments;

    // configure and open connection
    HINTERNET hInternet = InternetOpenA("InetURL/1.0", INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);
    HINTERNET hConnection = InternetConnectA(hInternet, Host.c_str(), 80, " ", " ", INTERNET_SERVICE_HTTP, 0, 0);
    HINTERNET hData = HttpOpenRequestA(hConnection, "POST", Directory.c_str(), 0, 0, 0, 0, 0);
    HttpSendRequestA(hData, NULL, 0, NULL, 0);

    // read data
    std::string HTML;

    char buffer[2048];
    DWORD bytesRead = 0;
    while (InternetReadFile(hData, buffer, 2047, &bytesRead) && bytesRead > 0)
    {
        // the buffer is filled with 2047 chars or less of data from the request
        // now null terminate to validate the cstr
        buffer[bytesRead] = 0;

        // append this chunk to tracked total
        HTML += buffer;
    }

    return HTML;
}

// wrapper for POST so we dont have to keep typing the same url
std::string HTTP::API(std::string CMD, Arguments Arguments)
{
    Arguments.Add("cmd", CMD);
    return POST("www.a4g4.com", "/CSGO/api.php", Arguments.String);
}
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

// read binary data from a website into T output
// returns bool succes
template <class T>
bool HTTP::StructuredGET(std::string Host, std::string Directory, std::string URLArguments, T* Output)
{
    // host/directory?arg=value
    Directory += "?" + URLArguments;

    // configure and open connection
    HINTERNET hInternet = InternetOpenA("InetURL/1.0", INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);
    HINTERNET hConnection = InternetConnectA(hInternet, Host.c_str(), INTERNET_DEFAULT_HTTPS_PORT, 0, 0, INTERNET_SERVICE_HTTP, 0, 0);
    HINTERNET hData = HttpOpenRequestA(hConnection, "GET", Directory.c_str(), 0, 0, 0, INTERNET_FLAG_SECURE | INTERNET_FLAG_RELOAD, 0);
    if (!HttpSendRequestA(hData, NULL, 0, NULL, 0))
    {
        std::cout << "StructuredGET.HttpSendRequest failed w/ winerror: " << GetLastError() << std::endl;
        return false;
    }


    DWORD BytesToRead = sizeof(T);
    DWORD BytesRead;
    if (!InternetReadFile(hData, Output, BytesToRead + 1 /* add one to know if there is more than requested */, &BytesRead))
    {
        std::cout << "InternetReadFile.HttpSendRequest failed w/ winerror: " << GetLastError() << std::endl;
        return false;
    }

    if (BytesRead != BytesToRead)
    {
        std::cout << "StructuredGET - server gave bad response byte size (got " << BytesRead << " but expected " << BytesToRead << ")\n";
        return false;
    }
    else
        return true;
}

// wrap StructuredGET
template <class T>
bool HTTP::APIGET(std::string Cmd, Arguments Data, T* Output)
{
    Data.Add("cmd", Cmd);
    return HTTP::StructuredGET<T>("www.a4g4.com", "/API/api.php", Data.String, Output);
}


APIResponseFormat_LoginAttempt HTTP::APILogin(std::string Email, std::string Password)
{
    APIResponseFormat_LoginAttempt Response{};

    Arguments Data;
    Data.Add("email", Email);
    Data.Add("passwd", Password);

    if (!APIGET<APIResponseFormat_LoginAttempt>("login", Data, &Response))
         Response.Flags = 0;

    return Response;
}
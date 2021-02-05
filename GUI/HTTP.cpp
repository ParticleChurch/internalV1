#include "HTTP.hpp"
#include "../Logger.hpp"

namespace HTTP
{
    std::string userAgent = "WinHTTP/1.0";
    INTERNET_PORT port = INTERNET_DEFAULT_HTTPS_PORT; // 443
    DWORD requestFlags = INTERNET_FLAG_SECURE | defaultNoCacheFlags | defaultBaseFlags;
    std::string contentType = "application/x-www-form-urlencoded";

    byte* Post(std::string URL, std::string input, DWORD* bytesRead)
    {
        L::Log(("HTTP::Post to url " + URL).c_str());


        // split string into host and directory
        // [ garbage ][     host part    ][     directory part     ]
        //   https://    www.example.com     /subdirectory/api.php

        // remove protocol from url
        if (URL.rfind("http://") == 0)
        {
            URL = URL.substr(7);
        }
        else if (URL.rfind("https://") == 0)
        {
            URL = URL.substr(8);
        }

        // split host and directory
        int directoryIndex = URL.find("/");
        std::string host = URL;
        std::string directory = "";
        if (directoryIndex >= 0)
        {
            host = URL.substr(0, directoryIndex);
            directory = URL.substr(directoryIndex + 1);
        }
        L::Log(("Determined that host = \"" + host + "\", and directory = \"" + directory + "\"").c_str());

        HINTERNET hInternet = InternetOpen(
            userAgent.c_str(),
            INTERNET_OPEN_TYPE_PRECONFIG,
            NULL, NULL,
            0
        );
        if (!hInternet)
        {
            L::Log(("Failed to open hInternet w/ error: " + std::to_string(GetLastError())).c_str());
            return nullptr;
        }

        HINTERNET hConnection = InternetConnect(
            hInternet,
            host.c_str(),
            INTERNET_DEFAULT_HTTPS_PORT,
            NULL, NULL,
            INTERNET_SERVICE_HTTP,
            0, 0
        );
        if (!hConnection)
        {
            L::Log(("Failed to open hConnection w/ error: " + std::to_string(GetLastError())).c_str());
            return nullptr;
        }

        // "Long pointer to a null-terminated array of string pointers indicating content types accepted by the client"
        LPCSTR acceptContentTypes[]{ "*/*", 0 };

        HINTERNET hRequest = HttpOpenRequest(hConnection,
            "POST",
            directory.c_str(),
            "HTTP/1.1",
            0,
            acceptContentTypes,
            requestFlags,
            0
        );
        if (!hRequest)
        {
            L::Log(("Failed to open hRequest w/ error: " + std::to_string(GetLastError())).c_str());
            return nullptr;
        }

        bool headersAdded = HttpAddRequestHeaders(
            hRequest,
            ("Content-Type: " + contentType + "\r\n").c_str(),
            48,
            HTTP_ADDREQ_FLAG_REPLACE
        );

        bool requestSuccess = HttpSendRequest(hRequest,
            // headers and length, not supported yet
            NULL, 0,
            // post data and post length
            (void*)input.c_str(), input.size()
        );

        if (requestSuccess)
            L::Log("Request completed successfully");
        else
            L::Log(("Request failed, error: " + std::to_string(GetLastError())).c_str());

        if (!requestSuccess)
            return nullptr;

        // dynamic buffer to hold file
        byte* fileBuffer = NULL;
        DWORD fileSize = 0;

        // buffer for each kb of the file
        byte* chunkBuffer = (byte*)malloc(1024);
        DWORD chunkSize = 1024;
        DWORD chunkBytesRead = 0;
        if (!chunkBuffer)
        {
            L::Log("failed to malloc for chunk buffer");
            free(fileBuffer);
            return nullptr;
        }

        do
        {
            chunkBytesRead = 0;
            InternetReadFile(hRequest, chunkBuffer, chunkSize, &chunkBytesRead);

            byte* re = (byte*)realloc(fileBuffer, fileSize + chunkBytesRead);
            if (!re)
            {
                L::Log("failed to realloc dynamic buffer");
                free(fileBuffer);
                free(chunkBuffer);
                return nullptr;
            }
            else
            {
                fileBuffer = re;
            }
            memcpy(fileBuffer + fileSize, chunkBuffer, chunkBytesRead);
            fileSize += chunkBytesRead;

        } while (chunkBytesRead > 0);
        free(chunkBuffer);

        if (fileSize == 0)
        {
            L::Log(("Got no bytes in response w/ error: " + std::to_string(GetLastError())).c_str());
            free(fileBuffer);
            return nullptr;
        }
        if (bytesRead)
            *bytesRead = fileSize;

        return fileBuffer;
    }
}
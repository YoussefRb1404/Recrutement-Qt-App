#ifndef SMS_H
#define SMS_H
#define CURL_STATICLIB

#include <string>
#include <curl/curl.h>
#include <QString>

class SMS {
public:
    SMS(std::string apiKey, std::string apiSecret, std::string fromNumber);
    ~SMS();
    bool sendSMS(std::string toNumber, QString message);
    static size_t writecallback(void* ptr, size_t size, size_t nmemb, void* userdata);
    QString replaceSpacesWithURLFormat(const QString& inputString);

private:
    std::string m_apiKey;
    std::string m_apiSecret;
    std::string m_fromNumber;
};

#endif // SMS_H

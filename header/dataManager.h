#ifndef DATAMANAGER_H
#define DATAMANAGER_H

#include <iostream>
#include <curl/curl.h>
#include <curl/curlver.h> 
#include <string>
#include <vector>

class DataManager{
    public:
    DataManager();

    private:

    
    static size_t writeCallback(char *content, size_t size, size_t nmemb, std::string* userData);
    struct hl
    {
        std::string headline;
        std::string headlineUrl;
    };


};
#endif
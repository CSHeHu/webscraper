#include <iostream>
#include <curl/curl.h>
#include <curl/curlver.h> 
#include <string>


size_t writeCallback(char *content, size_t size, size_t nmemb, std::string* userData){
    size_t realSize = size * nmemb;
    userData -> append((char*)content, realSize);
    return realSize;
}

int main() {

    CURLcode res;
    CURL *curl;
    std::string responseData;

    curl = curl_easy_init();

    if (curl){
    curl_easy_setopt(curl, CURLOPT_URL, "https://www.iltalehti.fi");
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &writeCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &responseData); 
    
    res = curl_easy_perform(curl);

    std::cout << responseData;

    curl_easy_cleanup(curl);
    curl = NULL;
    }



    return 0;
}
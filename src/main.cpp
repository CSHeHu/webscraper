#include <iostream>
#include <curl/curl.h>
#include <curl/curlver.h> 
#include <string>
#include <vector>


size_t writeCallback(char *content, size_t size, size_t nmemb, std::string* userData){
    size_t realSize = size * nmemb;
    userData -> append((char*)content, realSize);
    return realSize;
}


int main() {

    CURLcode res;
    CURL *curl;
    std::string responseData;
    
    // sw and lastchar for headlinesearch, url for destination
    std::string searchWord = "class=\"front-title\">";
    std::string lastChar = "<";
    const char* url = "https://www.iltalehti.fi";
    size_t lastPos = 0;
    std::vector<std::string> headlines;

 

    curl = curl_easy_init();

    if (curl){
        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &writeCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &responseData); 
        
        res = curl_easy_perform(curl);

        // go trough response and save every headline to vector
        while ( lastPos != std::string::npos){
            lastPos = responseData.find(searchWord, lastPos);

            if (lastPos == std::string::npos){
                continue;
            }   
            size_t lastCharPos = responseData.find(lastChar, lastPos); 
            
            headlines.push_back(responseData.substr(lastPos + searchWord.size(), lastCharPos - (lastPos + searchWord.size())));
            lastPos += searchWord.size();
            
    }


    for (const auto &i : headlines){
        std::cout << i << std::endl;
    }

    curl_easy_cleanup(curl);
    curl = NULL;
    }



    return 0;
}
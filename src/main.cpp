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

size_t findPos(std::string& data, size_t lastPos, std::string& delimiter){

}

int main() {

    CURLcode res;
    CURL *curl;
    std::string responseData;
    
    std::vector<std::string> headlines;
    std::string searchWord = "class=\"front-title\">";
    size_t lastPos = 0;
    unsigned int headlineLength = 35;


    curl = curl_easy_init();

    if (curl){
    curl_easy_setopt(curl, CURLOPT_URL, "https://www.iltalehti.fi");
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &writeCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &responseData); 
    
    res = curl_easy_perform(curl);


    //std::cout << responseData;
    //lastPos = responseData.find(searchWord, lastPos);
    //std::cout << responseData.substr(lastPos, 50);

    while ( lastPos != std::string::npos){
        lastPos = responseData.find(searchWord, lastPos);

        if (lastPos == std::string::npos){
            continue;
        }    
        headlines.push_back(responseData.substr(lastPos + searchWord.size(), headlineLength));
        lastPos += searchWord.size();
          
    }


    std::cout << headlines.size(); 

    for (const auto &i : headlines){
        std::cout << i << std::endl;
    }

    curl_easy_cleanup(curl);
    curl = NULL;
    }



    return 0;
}
#include "dataManager.h"

DataManager::DataManager()
{


}

void DataManager::updateData()
{

    CURLcode res;
    CURL *curl;
    std::string responseData;
    
    // sw and lastchar for headlinesearch, url for destination
    const char* url = "https://www.iltalehti.fi";
    std::string titleBegin = "class=\"front-title\">";
    std::string titleEnd = "<";
    size_t lastPos = 0;

    curl = curl_easy_init();

    if (curl){
        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &responseData); 
        
        res = curl_easy_perform(curl);

        // go trough response and save every headline to vector
        while ( lastPos != std::string::npos){
            lastPos = responseData.find(titleBegin, lastPos);

            if (lastPos == std::string::npos){
                continue;
            }   

            // find headlines
            size_t titleEndPos = responseData.find(titleEnd, lastPos);
            std::string hlTemp = responseData.substr(lastPos + titleBegin.size(), titleEndPos - (lastPos + titleBegin.size()));
            
            // below search for headline url
            size_t urlStartPos = responseData.rfind("<a href=\"", lastPos);
            size_t urlEndPos = responseData.find("\"", urlStartPos + 10);
            std::string hlUrlTemp = responseData.substr(urlStartPos + 10, urlEndPos - (urlStartPos + 10));
            
            hl tmpHeadline = {hlTemp, hlUrlTemp};
            headlines.push_back(tmpHeadline);
            
            lastPos += titleBegin.size();
            
    }
    
    curl_easy_cleanup(curl);
    curl = NULL;
    }
}

std::vector<hl> DataManager::getHeadlines()
{
    return headlines;
}

size_t DataManager::writeCallback(char *content, size_t size, size_t nmemb, std::string* userData){
    size_t realSize = size * nmemb;
    userData -> append((char*)content, realSize);
    return realSize;
}


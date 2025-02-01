#include "dataManager.h"
#include <regex>

DataManager::DataManager() : headlines()
{


}

void DataManager::updateData()
{

    headlines.clear();
    CURLcode res;
    CURL *curl;
    std::string responseData;
    
    // sw and lastchar for headlinesearch, url for destination
    const char* url = "https://www.iltalehti.fi";
    std::string titleBegin = "class=\"front-title\">";
    std::string titleEnd = "<";
    std::string urlBegin = "<a href=\"";
    std::string urlEnd = "\"";
    size_t lastPos = 0;

    curl = curl_easy_init();


    if (curl){
        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &responseData); 
        
        res = curl_easy_perform(curl);

        // validate response
        if (res != CURLE_OK) {
            std::cerr << "CURL error: " << curl_easy_strerror(res) << std::endl;
            curl_easy_cleanup(curl);
            curl = NULL;
            return;
        }
        
        // go trough response and save every headline to vector
        while ( lastPos != std::string::npos){
            lastPos = responseData.find(titleBegin, lastPos);

            if (lastPos == std::string::npos){
                continue;
            }   

            // find headlines
            size_t titleEndPos = responseData.find(titleEnd, lastPos);
            std::string hlTemp = responseData.substr(lastPos + titleBegin.size(), titleEndPos - (lastPos + titleBegin.size()));
            
            // search for headline url
            size_t urlStartPos = responseData.rfind(urlBegin, lastPos);
            size_t urlEndPos = responseData.find(urlEnd, urlStartPos + urlBegin.size());
            std::string hlUrlTemp = url + responseData.substr(urlStartPos + urlBegin.size(), urlEndPos - (urlStartPos + urlBegin.size()));

            //get caption
            std::string hlCaptionTemp = updateCaption(hlUrlTemp.c_str());

            hl tmpHeadline = {hlTemp, hlUrlTemp, hlCaptionTemp};
            headlines.push_back(tmpHeadline);
            
            lastPos += titleBegin.size();
            
    }
    
    curl_easy_cleanup(curl);
    curl = NULL;
    }
}

std::vector<DataManager::hl> DataManager::getHeadlines()
{
    return headlines;
}

size_t DataManager::writeCallback(char *content, size_t size, size_t nmemb, std::string* userData){
    size_t realSize = size * nmemb;
    userData -> append((char*)content, realSize);
    return realSize;
}

std::string DataManager::updateCaption(const char* headlineURL)
{
    
    CURLcode res;
    CURL *curl;
    std::string responseData;
    std::string hlTemp = "";

    // sw and lastchar for headlinesearch, url for destination
    const char* url = headlineURL;
    std::string titleBegin = "itemProp=\"articleBody\"><p class=\"paragraph\">";
    std::string titleEnd = "</div>";
    size_t lastPos = 0;

    curl = curl_easy_init();
if (curl){


        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &responseData); 
        
        res = curl_easy_perform(curl);

        // validate response
        if (res != CURLE_OK) {
            std::cerr << "CURL error: " << curl_easy_strerror(res) << std::endl;
            curl_easy_cleanup(curl);
            curl = NULL;
            return hlTemp;
        }
        
        lastPos = responseData.find(titleBegin, lastPos);

        if (lastPos == std::string::npos){
            curl_easy_cleanup(curl);
            curl = NULL;
            return hlTemp;
        }   

        // find caption
        size_t titleEndPos = responseData.find(titleEnd, lastPos);
        hlTemp = responseData.substr(lastPos + titleBegin.size(), titleEndPos - (lastPos + titleBegin.size()));
        
    }    
    
    
    curl_easy_cleanup(curl);
    curl = NULL;
    clearTags(hlTemp);

    return hlTemp;
}

void DataManager::clearTags(std::string &origCaption){
    // Regular expression to match HTML tags
    std::regex html_tag_pattern("<[^>]*>");
    // Replace HTML tags with an empty string
    origCaption = std::regex_replace(origCaption, html_tag_pattern, ""); 

}


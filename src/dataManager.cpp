#include "dataManager.h"

DataManager::DataManager() : headlines()
{
    providerInfo iltalehti;
    iltalehti.name = "Iltalehti";
    iltalehti.url = "https://www.iltalehti.fi";
    iltalehti.titleBegin = "class=\"front-title\">";
    iltalehti.titleEnd = "<";
    iltalehti.urlBegin = "<a href=\"";
    iltalehti.urlEnd = "\"";
    providers.insert({iltalehti.name, iltalehti});

}

void DataManager::updateData()
{
    std::string testParam = "Iltalehti"; //REMOVE
    providerInfo tmpProvider = providers.at(testParam);
        
    headlines.clear();
    CURLcode res;
    CURL *curl;
    std::string responseData;
    size_t lastPos = 0;

    curl = curl_easy_init();


    if (curl){
        curl_easy_setopt(curl, CURLOPT_URL, tmpProvider.url);
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
            lastPos = responseData.find(tmpProvider.titleBegin, lastPos);

            if (lastPos == std::string::npos){
                continue;
            }   

            // find headlines
            size_t titleEndPos = responseData.find(tmpProvider.titleEnd, lastPos);
            std::string hlTemp = responseData.substr(lastPos + tmpProvider.titleBegin.size(), titleEndPos - (lastPos + tmpProvider.titleBegin.size()));
            
            // search for headline url
            size_t urlStartPos = responseData.rfind(tmpProvider.urlBegin, lastPos);
            size_t urlEndPos = responseData.find(tmpProvider.urlEnd, urlStartPos + tmpProvider.urlBegin.size());
            std::string hlUrlTemp = tmpProvider.url + responseData.substr(urlStartPos + tmpProvider.urlBegin.size(), urlEndPos - (urlStartPos + tmpProvider.urlBegin.size()));

            //get caption
            std::string hlCaptionTemp = updateCaption(hlUrlTemp.c_str());

            hl tmpHeadline = {hlTemp, hlUrlTemp, hlCaptionTemp};
            headlines.push_back(tmpHeadline);
            
            lastPos += tmpProvider.titleBegin.size();
            
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


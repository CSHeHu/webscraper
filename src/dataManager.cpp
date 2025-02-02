#include "dataManager.h"

DataManager::DataManager() : headlines()
{
    providerInfo iltalehti;
    iltalehti.name = "Iltalehti";
    iltalehti.url = "https://www.iltalehti.fi/rss/rss.xml";
    iltalehti.titleBegin = "<title>";
    iltalehti.titleEnd = "</title>";
    iltalehti.urlBegin = "<link>";
    iltalehti.urlEnd = "</link>";
    iltalehti.captionBegin = "<description>";
    iltalehti.captionEnd = "</description>";
    providers.insert({iltalehti.name, iltalehti});

    providerInfo iltasanomat;
    iltasanomat.name = "Iltasanomat";
    iltasanomat.url = "https://www.is.fi/rss/tuoreimmat.xml";
    iltasanomat.titleBegin = "<title><![CDATA[";
    iltasanomat.titleEnd = "]]></title>";
    iltasanomat.urlBegin = "<link>";
    iltasanomat.urlEnd = "</link>";
    iltasanomat.captionBegin = "<description><![CDATA[";
    iltasanomat.captionEnd = "]]></description>";
    providers.insert({iltasanomat.name, iltasanomat});
    
    selectedProvider = iltalehti.name;
}

void DataManager::updateData()
{
    providerInfo tmpProvider = providers.at(selectedProvider);
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
            if (titleEndPos == std::string::npos) {
                lastPos = std::string::npos;
                continue;
            }
            std::string hlTemp = responseData.substr(lastPos + tmpProvider.titleBegin.size(), titleEndPos - lastPos - tmpProvider.titleBegin.size());
        

            // search for headline url
            size_t urlStartPos = responseData.find(tmpProvider.urlBegin, lastPos);
            size_t urlEndPos = responseData.find(tmpProvider.urlEnd, urlStartPos);
            if (urlStartPos == std::string::npos || urlEndPos == std::string::npos) {
                lastPos = std::string::npos;
                continue; 
            }
            std::string hlUrlTemp = responseData.substr(urlStartPos + tmpProvider.urlBegin.size(), urlEndPos - urlStartPos - tmpProvider.urlBegin.size());

            //get caption
            size_t captionStartPos = responseData.find(tmpProvider.captionBegin, lastPos);
            size_t captionEndPos = responseData.find(tmpProvider.captionEnd, captionStartPos);
            if (captionStartPos == std::string::npos || captionEndPos == std::string::npos) {
                lastPos = std::string::npos;
                continue; 
            }
            std::string hlCaptionTemp = responseData.substr(captionStartPos + tmpProvider.captionBegin.size(), captionEndPos - captionStartPos- tmpProvider.captionBegin.size());


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

std::string DataManager::getSelectedProvider()
{
    return selectedProvider;    
}

void DataManager::changeProvider(const std::string &name)
{
    selectedProvider = name;
}

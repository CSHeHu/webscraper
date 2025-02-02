#ifndef DATAMANAGER_H
#define DATAMANAGER_H

#include <iostream>
#include <string>
#include <vector>
#include <curl/curlver.h> 
#include <curl/curl.h>
#include <unordered_map>

/**
 * @brief Manages data fetching and headline extraction.
 */
class DataManager {
public:
    /**
     * @brief Represents a headline with its URL and caption.
     */
    struct hl {
        std::string headline;    
        std::string headlineUrl;
        std::string headlineCaption;
    };

    /**
     * @brief Default constructor to initialize the DataManager.
     */
    DataManager();

    /**
     * @brief Fetches and updates the latest data.
     * 
     * This method retrieves headlines, URLs, and captions from the selected provider.
     */
    void updateData();

    /**
     * @brief Gets the list of extracted headlines.
     * 
     * @return A vector containing all the headlines, URLs, and captions.
     */
    std::vector<hl> getHeadlines();

    /**
     * @brief Changes the provider to fetch data from.
     * 
     * @param name The name of the provider to switch to.
     */
    void changeProvider(std::string name);

private:
    /**
     * @brief Holds provider details for fetching data.
     */
    struct providerInfo {
        std::string name;
        const char* url; 
        std::string titleBegin;
        std::string titleEnd; 
        std::string urlBegin;
        std::string urlEnd;
        std::string captionBegin; 
        std::string captionEnd;
    };

    std::unordered_map<std::string, providerInfo> providers;
    std::vector<hl> headlines;
    std::string selectedProvider;

    /**
     * @brief Appends received data to a string.
     * 
     * @param content Pointer to the received data.
     * @param size Size of each data element.
     * @param nmemb Number of data elements.
     * @param userData Pointer to the string where data will be appended.
     * @return The total number of bytes written.
     */
    static size_t writeCallback(char *content, size_t size, size_t nmemb, std::string* userData);
};

#endif // DATAMANAGER_H

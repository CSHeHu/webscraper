#ifndef DATAMANAGER_H
#define DATAMANAGER_H

#include <iostream>
#include <string>
#include <vector>
#include <regex>
#include <curl/curlver.h> 
#include <curl/curl.h>

/**
 * @brief A class to manage data fetching and processing.
 * 
 * This class handles retrieving data from the web using CURL and
 * processing it to extract headlines and their URLs.
 */
class DataManager {
public:
/**
 * @brief Struct to represent a headline and its associated URL.
 */
struct hl {
    std::string headline;    
    std::string headlineUrl;
    std::string headlineCaption;
};

    DataManager();
    void updateData();
    std::vector<hl> getHeadlines();


private:
    /**
     * @brief Callback function for handling CURL data writes.
     * 
     * This function appends the data received by CURL to a user-provided string.
     * 
     * @param content Pointer to the received data.
     * @param size Size of each data element.
     * @param nmemb Number of data elements.
     * @param userData Pointer to the user-provided string where data will be appended.
     * @return The total number of bytes written.
     */
    static size_t writeCallback(char *content, size_t size, size_t nmemb, std::string* userData);
    std::string updateCaption(const char* headlineURL);
    std::vector<hl> headlines;
    void clearTags(std::string &origCaption);
};

#endif // datamanager_h

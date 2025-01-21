#ifndef DATAMANAGER_H
#define DATAMANAGER_H

#include <iostream>
#include <string>
#include <vector>
#include <regex>
#include <curl/curlver.h> 
#include <curl/curl.h>
/**
 * @brief Struct to represent a headline and its associated URL.
 */
struct hl {
    std::string headline;    /**< The text of the headline. */
    std::string headlineUrl; /**< The URL associated with the headline. */
    std::string headlineCaption;
};

/**
 * @brief A class to manage data fetching and processing.
 * 
 * This class handles retrieving data from the web using CURL and
 * processing it to extract headlines and their URLs.
 */
class DataManager {
public:
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
    std::vector<hl> headlines; /**< A vector to store the extracted headlines and their URLs. */
    void clearTags(std::string &origCaption);
};

#endif // DATAMANAGER_H

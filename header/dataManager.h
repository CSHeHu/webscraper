#ifndef DATAMANAGER_H
#define DATAMANAGER_H

#include <iostream>
#include <curl/curl.h>
#include <curl/curlver.h> 
#include <string>
#include <vector>

/**
 * @brief Struct to represent a headline and its associated URL.
 */
struct hl {
    std::string headline;    /**< The text of the headline. */
    std::string headlineUrl; /**< The URL associated with the headline. */
};

/**
 * @brief A class to manage data fetching and processing.
 * 
 * This class handles retrieving data from the web using CURL and
 * processing it to extract headlines and their URLs.
 */
class DataManager {
public:
    /**
     * @brief Constructs a new DataManager object.
     */
    DataManager();

    /**
     * @brief Updates the internal data by fetching new content from the web.
     */
    void updateData();

    /**
     * @brief Retrieves the list of headlines.
     * 
     * @return A vector containing headlines and their URLs.
     */
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

    std::vector<hl> headlines; /**< A vector to store the extracted headlines and their URLs. */
};

#endif // DATAMANAGER_H

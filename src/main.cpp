#include <iostream>
#include <curl/curl.h>
#include <curl/curlver.h>

int main() {
    curl_global_init(CURL_GLOBAL_DEFAULT);

    curl_version_info_data *version_info = curl_version_info(CURLVERSION_NOW);
    std::cout << "libcurl version: " << version_info->version << std::endl;
   
    curl_global_cleanup();
    return 0;
}
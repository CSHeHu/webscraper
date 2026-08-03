#ifndef DATAMANAGER_H
#define DATAMANAGER_H

#include <curl/curl.h>
#include <curl/curlver.h>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

class DataManager {
public:
  struct hl {
    std::string headline;
    std::string headlineUrl;
    std::string headlineCaption;
  };

  DataManager();

  void updateData(const std::string &filterString = "");

  std::vector<hl> *getHeadlines();

  void changeProvider(const std::string &name);
  void filterHeadlines(const std::string &filterString);

private:
  struct providerInfo {
    std::string name;
    const char *url;
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
  static size_t writeCallback(char *content, size_t size, size_t nmemb,
                              std::string *userData);
  std::string toLowerHeadline(std::string str);
};

#endif // DATAMANAGER_H

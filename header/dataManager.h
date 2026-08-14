#ifndef DATAMANAGER_H
#define DATAMANAGER_H

#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QObject>
#include <string>
#include <unordered_map>
#include <vector>

class DataManager : public QObject {
  Q_OBJECT

public:
  struct hl {
    std::string headline;
    std::string headlineUrl;
    std::string headlineCaption;
  };

  DataManager(QObject *parent = nullptr);

  void updateData(const std::string &filterString = "");

  std::vector<hl> *getHeadlines();

  void changeProvider(const std::string &name);
  void filterHeadlines(const std::string &filterString);

signals:
  void headlinesReady();
  void fetchFailed(const QString &message);

private slots:
  void onReplyFinished();

private:
  struct providerInfo {
    std::string name;
    std::string url;
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
  std::string pendingFilter;
  QNetworkAccessManager *networkManager;
  QNetworkReply *currentReply = nullptr;
  void parseResponse(const std::string &responseData);
  void readConfigFile();
  std::string toLowerHeadline(std::string str);
};

#endif // DATAMANAGER_H

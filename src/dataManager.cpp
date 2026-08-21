#include "dataManager.h"
#include <QCoreApplication>
#include <QDateTime>
#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonParseError>
#include <QNetworkRequest>
#include <QStringList>
#include <QUrl>
#include <algorithm>

DataManager::DataManager(QObject *parent)
    : QObject(parent), headlines(),
      networkManager(new QNetworkAccessManager(this)) {
  readConfigFile();
  if (!providers.empty()) {
    selectedProvider = providers.begin()->first;
  }
}

void DataManager::updateData(const std::string &filterString) {
  if (currentReply) {
    currentReply->abort();
  }

  providerInfo tmpProvider = providers.at(selectedProvider);
  headlines.clear();
  pendingFilter = filterString;

  QNetworkRequest request(QUrl(QString::fromStdString(tmpProvider.url)));
  currentReply = networkManager->get(request);
  connect(currentReply, &QNetworkReply::finished, this,
          &DataManager::onReplyFinished);
}

void DataManager::onReplyFinished() {
  QNetworkReply *reply = currentReply;
  currentReply = nullptr;

  if (reply->error() != QNetworkReply::NoError) {
    QString errorMessage = reply->errorString();
    reply->deleteLater();
    if (reply->error() != QNetworkReply::OperationCanceledError) {
      emit fetchFailed(errorMessage);
    }
    return;
  }

  std::string responseData = reply->readAll().toStdString();
  reply->deleteLater();

  parseResponse(responseData);

  if (!pendingFilter.empty()) {
    filterHeadlines(pendingFilter);
  }

  emit headlinesReady();
}

void DataManager::parseResponse(const std::string &responseData) {
  providerInfo tmpProvider = providers.at(selectedProvider);
  size_t lastPos = 0;

  while (lastPos != std::string::npos) {
    lastPos = responseData.find(tmpProvider.titleBegin, lastPos);
    if (lastPos == std::string::npos) {
      continue;
    }

    size_t titleEndPos = responseData.find(tmpProvider.titleEnd, lastPos);
    if (titleEndPos == std::string::npos) {
      lastPos = std::string::npos;
      continue;
    }
    std::string hlTemp = responseData.substr(
        lastPos + tmpProvider.titleBegin.size(),
        titleEndPos - lastPos - tmpProvider.titleBegin.size());

    size_t urlStartPos = responseData.find(tmpProvider.urlBegin, lastPos);
    size_t urlEndPos = responseData.find(tmpProvider.urlEnd, urlStartPos);
    if (urlStartPos == std::string::npos || urlEndPos == std::string::npos) {
      lastPos = std::string::npos;
      continue;
    }
    std::string hlUrlTemp = responseData.substr(
        urlStartPos + tmpProvider.urlBegin.size(),
        urlEndPos - urlStartPos - tmpProvider.urlBegin.size());

    size_t captionStartPos =
        responseData.find(tmpProvider.captionBegin, lastPos);
    size_t captionEndPos =
        responseData.find(tmpProvider.captionEnd, captionStartPos);
    if (captionStartPos == std::string::npos ||
        captionEndPos == std::string::npos) {
      lastPos = std::string::npos;
      continue;
    }
    std::string hlCaptionTemp = responseData.substr(
        captionStartPos + tmpProvider.captionBegin.size(),
        captionEndPos - captionStartPos - tmpProvider.captionBegin.size());

    size_t pubDateStartPos =
        responseData.find(tmpProvider.pubDateBegin, lastPos);
    size_t pubDateEndPos =
        responseData.find(tmpProvider.pubDateEnd, pubDateStartPos);
    if (pubDateStartPos == std::string::npos ||
        pubDateEndPos == std::string::npos) {
      lastPos = std::string::npos;
      continue;
    }
    std::string hlPubDateTemp = responseData.substr(
        pubDateStartPos + tmpProvider.pubDateBegin.size(),
        pubDateEndPos - pubDateStartPos - tmpProvider.pubDateBegin.size());

    hl tmpHeadline = {hlTemp, hlUrlTemp, hlCaptionTemp, hlPubDateTemp};
    headlines.push_back(tmpHeadline);

    lastPos += tmpProvider.titleBegin.size();
  }
}

std::vector<DataManager::hl> *DataManager::getHeadlines() { return &headlines; }

void DataManager::changeProvider(const std::string &name) {
  selectedProvider = name;
}

void DataManager::filterHeadlines(const std::string &filterString) {
  std::string lowerFilterString = toLowerHeadline(filterString);
  std::vector<hl> filteredHeadlines;

  for (const auto &headline : headlines) {
    std::string lowerHeadline = toLowerHeadline(headline.headline);
    std::string lowerheadlineCaption =
        toLowerHeadline(headline.headlineCaption);
    if (lowerHeadline.find(lowerFilterString) != std::string::npos ||
        lowerheadlineCaption.find(lowerFilterString) != std::string::npos) {
      filteredHeadlines.push_back(headline);
    }
  }
  headlines = filteredHeadlines;
}

std::string DataManager::toLowerHeadline(std::string str) {
  std::transform(str.begin(), str.end(), str.begin(),
                 [](unsigned char c) { return std::tolower(c); });
  return str;
}

void DataManager::readConfigFile() {
  const QStringList candidates = {
#ifdef APP_CONFIG_DIR
      QStringLiteral(APP_CONFIG_DIR "/config.json"),
#endif
      QCoreApplication::applicationDirPath() +
          QStringLiteral("/config/config.json"),
  };

  QByteArray fileData;
  for (const QString &path : candidates) {
    QFile configFile(path);
    if (configFile.open(QIODevice::ReadOnly)) {
      fileData = configFile.readAll();
      configFile.close();
      break;
    }
  }
  if (fileData.isEmpty()) {
    return;
  }

  QJsonParseError parseError;
  QJsonDocument doc = QJsonDocument::fromJson(fileData, &parseError);
  if (parseError.error != QJsonParseError::NoError || !doc.isArray()) {
    return;
  }

  std::unordered_map<std::string, providerInfo> loadedProviders;

  for (const QJsonValue &entry : doc.array()) {
    QJsonObject obj = entry.toObject();

    providerInfo provider;
    provider.name = obj.value("name").toString().toStdString();
    provider.url = obj.value("url").toString().toStdString();
    provider.titleBegin = obj.value("titleBegin").toString().toStdString();
    provider.titleEnd = obj.value("titleEnd").toString().toStdString();
    provider.urlBegin = obj.value("urlBegin").toString().toStdString();
    provider.urlEnd = obj.value("urlEnd").toString().toStdString();
    provider.captionBegin = obj.value("captionBegin").toString().toStdString();
    provider.captionEnd = obj.value("captionEnd").toString().toStdString();
    provider.pubDateBegin = obj.value("pubDateBegin").toString().toStdString();
    provider.pubDateEnd = obj.value("pubDateEnd").toString().toStdString();

    if (provider.name.empty() || provider.url.empty()) {
      continue;
    }

    loadedProviders.insert({provider.name, provider});
  }

  if (!loadedProviders.empty()) {
    providers = loadedProviders;
  }
}

const std::unordered_map<std::string, DataManager::providerInfo> &
DataManager::getProviders() const {
  return providers;
}

// Convert a pubDate string to milliseconds since epoch, handling named time
// zones
static qint64 pubDateToMSecs(const std::string &pubDate) {
  static const std::vector<std::pair<QString, QString>> namedZones = {
      {QStringLiteral("GMT"), QStringLiteral("+0000")},
      {QStringLiteral("UTC"), QStringLiteral("+0000")},
      {QStringLiteral("UT"), QStringLiteral("+0000")},
      {QStringLiteral("EDT"), QStringLiteral("-0400")},
      {QStringLiteral("EST"), QStringLiteral("-0500")},
      {QStringLiteral("CDT"), QStringLiteral("-0500")},
      {QStringLiteral("CST"), QStringLiteral("-0600")},
      {QStringLiteral("MDT"), QStringLiteral("-0600")},
      {QStringLiteral("MST"), QStringLiteral("-0700")},
      {QStringLiteral("PDT"), QStringLiteral("-0700")},
      {QStringLiteral("PST"), QStringLiteral("-0800")},
  };

  QString text = QString::fromStdString(pubDate).trimmed();
  for (const auto &[name, offset] : namedZones) {
    if (text.endsWith(name)) {
      text.replace(text.size() - name.size(), name.size(), offset);
      break;
    }
  }

  const QDateTime pubDateTime = QDateTime::fromString(text, Qt::RFC2822Date);
  return pubDateTime.isValid() ? pubDateTime.toMSecsSinceEpoch() : 0;
}

void DataManager::sortHeadlines(sortingMode mode) {
  std::stable_sort(
      headlines.begin(), headlines.end(),
      [this, mode](const hl &a, const hl &b) {
        switch (mode) {
        case DATE:
          return sortedDate
                     ? pubDateToMSecs(a.pubDate) > pubDateToMSecs(b.pubDate)
                     : pubDateToMSecs(a.pubDate) < pubDateToMSecs(b.pubDate);
        case TITLE:
          return sortedTitle ? a.headline > b.headline
                             : a.headline < b.headline;
        case CAPTION:
          return sortedCaption ? a.headlineCaption > b.headlineCaption
                               : a.headlineCaption < b.headlineCaption;
        }
        return false;
      });
  if (mode == DATE) {
    sortedDate = !sortedDate;
  } else if (mode == TITLE) {
    sortedTitle = !sortedTitle;
  } else if (mode == CAPTION) {
    sortedCaption = !sortedCaption;
  }
  emit headlinesReady();
}

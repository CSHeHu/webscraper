#include "dataManager.h"
#include <QNetworkRequest>
#include <QUrl>
#include <algorithm>

DataManager::DataManager(QObject *parent)
    : QObject(parent), headlines(),
      networkManager(new QNetworkAccessManager(this)) {
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

void DataManager::updateData(const std::string &filterString) {
  if (currentReply) {
    currentReply->abort();
  }

  providerInfo tmpProvider = providers.at(selectedProvider);
  headlines.clear();
  pendingFilter = filterString;

  QNetworkRequest request(
      QUrl(QString::fromStdString(tmpProvider.url)));
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

    hl tmpHeadline = {hlTemp, hlUrlTemp, hlCaptionTemp};
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

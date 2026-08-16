#include "dataManager.h"
#include "mainwindow.h"
#include <QApplication>
#include <QCommandLineParser>

#ifndef APP_NAME
#define APP_NAME "newscraper"
#endif
#ifndef APP_VERSION
#define APP_VERSION "unknown"
#endif

int main(int argc, char *argv[]) {
  QApplication a(argc, argv);
  QApplication::setApplicationName(QStringLiteral(APP_NAME));
  QApplication::setApplicationVersion(QStringLiteral(APP_VERSION));

  QCommandLineParser parser;
  parser.setApplicationDescription(
      QStringLiteral("Read news headlines from RSS feeds."));
  parser.addHelpOption();
  parser.addVersionOption();
  parser.process(a);

  MainWindow w;
  w.show();
  return a.exec();
}

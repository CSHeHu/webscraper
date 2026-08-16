#include "mainwindow.h"
#include "dataManager.h"
#include <QApplication>
#include <qlabel.h>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), data(new DataManager(this))

{
  this->setWindowTitle("Uutiset");

  createGui();
  connect(data, &DataManager::headlinesReady, this, &MainWindow::updateGui);
  connect(data, &DataManager::fetchFailed, this,
          [this](const QString &message) { newsInfoLabel->setText(message); });
  data->updateData();

  QApplication *a = qApp;
  lightPalette = a->palette();

  darkPalette.setColor(QPalette::Window, QColor(53, 53, 53));
  darkPalette.setColor(QPalette::WindowText, Qt::white);
  darkPalette.setColor(QPalette::Base, QColor(25, 25, 25));
  darkPalette.setColor(QPalette::AlternateBase, QColor(53, 53, 53));
  darkPalette.setColor(QPalette::ToolTipBase, Qt::white);
  darkPalette.setColor(QPalette::ToolTipText, Qt::white);
  darkPalette.setColor(QPalette::Text, Qt::white);
  darkPalette.setColor(QPalette::Button, QColor(53, 53, 53));
  darkPalette.setColor(QPalette::ButtonText, Qt::white);
  darkPalette.setColor(QPalette::BrightText, Qt::red);
  darkPalette.setColor(QPalette::Link, QColor(42, 130, 218));
  darkPalette.setColor(QPalette::Highlight, QColor(42, 130, 218));
  darkPalette.setColor(QPalette::HighlightedText, Qt::black);
}

MainWindow::~MainWindow() {}

bool MainWindow::eventFilter(QObject *obj, QEvent *event) {
  QPushButton *button = qobject_cast<QPushButton *>(obj);
  if (event->type() == QEvent::Enter) {
    newsInfoLabel->setText(button->property("caption").toString());
    return true;
  } else if (event->type() == QEvent::Leave) {
    return true;
  } else {
    return QMainWindow::eventFilter(obj, event);
  }
}

void MainWindow::createGui() {
  QWidget *mainWidget = new QWidget(this);
  setCentralWidget(mainWidget);
  mainWidget->setMinimumWidth(1024);

  QHBoxLayout *mainLayout = new QHBoxLayout(mainWidget);
  mainWidget->setLayout(mainLayout);

  QScrollArea *scrollArea = new QScrollArea(mainWidget);
  scrollArea->setWidgetResizable(true);

  centralWidget = new QWidget(scrollArea);
  scrollArea->setWidget(centralWidget);

  buttonLayout = new QVBoxLayout(centralWidget);
  centralWidget->setLayout(buttonLayout);

  QWidget *sideWidget = new QWidget(mainWidget);
  sideWidget->setFixedWidth(300);

  QVBoxLayout *sidePanelLayout = new QVBoxLayout(sideWidget);
  sideWidget->setLayout(sidePanelLayout);

  newsInfoLabel = new QLabel("", sideWidget);
  newsInfoLabel->setWordWrap(true);
  sidePanelLayout->addWidget(newsInfoLabel);

  mainLayout->addWidget(scrollArea);
  mainLayout->addWidget(sideWidget);

  toolBar = addToolBar(tr("Menu"));

  QAction *updateAction = new QAction("Update", this);
  connect(updateAction, &QAction::triggered, this,
          [this]() { data->updateData(); });

  toolBar->addAction(updateAction);

  QAction *quitAction = new QAction("Quit", this);
  connect(quitAction, &QAction::triggered, QCoreApplication::quit);
  toolBar->addAction(quitAction);

  const std::unordered_map<std::string, DataManager::providerInfo>
      &tmpProviders = data->getProviders();
  auto it = tmpProviders.begin();
  while (it != tmpProviders.end()) {
    QAction *tmpAction =
        new QAction(QString::fromStdString(it->second.name), this);
    connect(tmpAction, &QAction::triggered, this, [this, it]() {
      data->changeProvider(it->second.name);
      data->updateData();
    });
    toolBar->addAction(tmpAction);
    it++;
  }

  searchLineEdit = new QLineEdit(this);
  searchLineEdit->setPlaceholderText("Search...");
  toolBar->addWidget(searchLineEdit);

  auto doSearch = [this]() {
    search();
    searchLineEdit->clear();
  };

  QAction *searchAction = new QAction("Search", this);
  connect(searchAction, &QAction::triggered, this, doSearch);
  toolBar->addAction(searchAction);

  connect(searchLineEdit, &QLineEdit::returnPressed, this, doSearch);

  QAction *toggleThemeAction = new QAction("Toggle Theme", this);
  connect(toggleThemeAction, &QAction::triggered, this,
          &MainWindow::toggleTheme);
  toolBar->addAction(toggleThemeAction);
}

void MainWindow::updateGui() {
  while (QLayoutItem *item = buttonLayout->takeAt(0)) {
    if (QWidget *widget = item->widget()) {
      widget->deleteLater();
    }
    delete item;
  }

  std::vector<DataManager::hl> *headlines = data->getHeadlines();

  for (const auto &headline : *headlines) {
    if (headline.headline.size() == 0) {
      continue;
    }

    QPushButton *button = new QPushButton(
        QString::fromStdString(headline.headline), centralWidget);
    button->setProperty("url", QString::fromStdString(headline.headlineUrl));
    connect(button, &QPushButton::clicked, this, &MainWindow::openUrl);

    button->setProperty("caption",
                        QString::fromStdString(headline.headlineCaption));

    button->installEventFilter(this);
    buttonLayout->addWidget(button);
  }
}

void MainWindow::openUrl() {
  QPushButton *button = qobject_cast<QPushButton *>(sender());
  if (button) {
    QString url = button->property("url").toString();
    QDesktopServices::openUrl(QUrl(url));
  }
}

void MainWindow::search() {
  QString searchText = searchLineEdit->text().trimmed();
  data->updateData(searchText.toStdString());
}

void MainWindow::toggleTheme() {
  QApplication *a = qApp;
  (a->palette() == lightPalette) ? a->setPalette(darkPalette)
                                 : a->setPalette(lightPalette);
}

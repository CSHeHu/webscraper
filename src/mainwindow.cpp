
#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "dataManager.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("Uutiset");
    
    centralWidget = new QWidget(this);
    buttonLayout = new QVBoxLayout(centralWidget);
    centralWidget->setLayout(buttonLayout);
    setCentralWidget(centralWidget);

    DataManager data;
    data.updateData();


    // Fetch headlines from DataManager
    std::vector<hl> headlines = data.getHeadlines();

    // Create buttons for each headline
    for (const auto &headline : headlines) {
        QPushButton *button = new QPushButton(QString::fromStdString(headline.headline), centralWidget);
        button->setProperty("url", QString::fromStdString(headline.headlineUrl)); // Store URL as property
        connect(button, &QPushButton::clicked, this, &MainWindow::openUrl);
        buttonLayout->addWidget(button); // Add button to the layout
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::openUrl()
{
    // Get the sender button
    QPushButton *button = qobject_cast<QPushButton*>(sender());
    if (button) {
        // Retrieve the URL from the button property
        QString url = button->property("url").toString();
        QDesktopServices::openUrl(QUrl(url)); // Open the URL in the default browser
    } 
}
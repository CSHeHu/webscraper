
#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "dataManager.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("Uutiset");
    this-> setMinimumSize(600,800);

    createGui();
    updateData();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::createGui()
{
    // Create a scroll area
    QScrollArea *scrollArea = new QScrollArea(this);
    scrollArea->setWidgetResizable(true);
    setCentralWidget(scrollArea);
    
    // Create the central widget 
    centralWidget = new QWidget(scrollArea);
    scrollArea->setWidget(centralWidget);
    
    // create button layout 
    buttonLayout = new QVBoxLayout(centralWidget);
    centralWidget->setLayout(buttonLayout);

    // Create toolbar
    toolBar = addToolBar(tr("Menu"));
    QPushButton *button = new QPushButton("Update", toolBar);
    connect(button, &QPushButton::clicked, this, &MainWindow::updateData);
    toolBar->addWidget(button); // Add button to the layout

    button = new QPushButton("Quit", toolBar);
    connect(button, &QPushButton::clicked, QCoreApplication::quit);
    toolBar->addWidget(button); // Add button to the layout
}

void MainWindow::updateData()
{   
    
    while (QLayoutItem *item = buttonLayout->takeAt(0)){
        if (QWidget *widget = item->widget()) {
            widget->deleteLater();
        }
        delete item;
    }
    
    DataManager data;
    data.updateData();


    // Fetch headlines from DataManager
    std::vector<hl> headlines = data.getHeadlines();

    // Create buttons for each headline
    for (const auto &headline : headlines) {

        // skipping empty headlines if any
        if (headline.headline.size() == 0){
            continue;
        }

        QPushButton *button = new QPushButton(QString::fromStdString(headline.headline), centralWidget);
        button->setProperty("url", QString::fromStdString(headline.headlineUrl)); // Store URL as property
        connect(button, &QPushButton::clicked, this, &MainWindow::openUrl);
        buttonLayout->addWidget(button); // Add button to the layout
    }


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
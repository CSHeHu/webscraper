
#include "mainwindow.h"
#include "dataManager.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent) 
    , data(new DataManager)


{
    this->setWindowTitle("Uutiset");
    this-> setMinimumSize(800,800);

    createGui();
    updateData();
}

MainWindow::~MainWindow()
{
    delete data;
}

bool MainWindow::eventFilter(QObject *obj, QEvent *event)
{
    QPushButton *button = qobject_cast<QPushButton *>(obj);
        if (event->type() == QEvent::Enter) 
        {
            std::cout << "päällä" << std::endl;
            return true;
        } 
        else if (event->type() == QEvent::Leave) 
        {
           std::cout << "pois" << std::endl;
           return true; 
        }
        else 
        {
            // pass event to parent class
            return QMainWindow::eventFilter(obj, event);
        }
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
    
    data->updateData();


    // Fetch headlines from DataManager
    std::vector<hl> headlines = data->getHeadlines();

    // Create buttons for each headline
    for (const auto &headline : headlines) {

        // skipping empty headlines if any
        if (headline.headline.size() == 0){
            continue;
        }

        QPushButton *button = new QPushButton(QString::fromStdString(headline.headline), centralWidget);
        button->setProperty("url", QString::fromStdString(headline.headlineUrl)); // Store URL as property
        connect(button, &QPushButton::clicked, this, &MainWindow::openUrl);
        button->installEventFilter(this);
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
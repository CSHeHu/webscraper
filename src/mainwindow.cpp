#include "mainwindow.h"
#include "dataManager.h"
#include <qlabel.h>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent) 
    , data(new DataManager)


{
    this->setWindowTitle("Uutiset");
    
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
            newsInfoLabel->setText(button->property("caption").toString());
            return true;
        } 
        else if (event->type() == QEvent::Leave) 
        {
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
    // Create a main widget that will hold everything
    QWidget *mainWidget = new QWidget(this);
    setCentralWidget(mainWidget);
    mainWidget->setMinimumWidth(1024);
      
    // Create a layout for the main window (horizontal layout)
    QHBoxLayout *mainLayout = new QHBoxLayout(mainWidget);
    mainWidget->setLayout(mainLayout);

    // Create a scroll area for the central widget
    QScrollArea *scrollArea = new QScrollArea(mainWidget);
    scrollArea->setWidgetResizable(true);
    
    // Create the central widget (the area where buttons are)
    centralWidget = new QWidget(scrollArea);
    scrollArea->setWidget(centralWidget);
    
    // Create the button layout inside the central widget
    buttonLayout = new QVBoxLayout(centralWidget);
    centralWidget->setLayout(buttonLayout);

    // Create a side panel widget (for additional content)
    QWidget *sideWidget = new QWidget(mainWidget);
    sideWidget->setFixedWidth(300);

    // Create layout for the side widget (a vertical layout)
    QVBoxLayout *sidePanelLayout = new QVBoxLayout(sideWidget);
    sideWidget->setLayout(sidePanelLayout);

    // Test label for side panel
    newsInfoLabel = new QLabel("", sideWidget);
    newsInfoLabel->setWordWrap(true);
    sidePanelLayout->addWidget(newsInfoLabel);
    
    // Add the scroll area and side widget to the main layout
    mainLayout->addWidget(scrollArea);  
    mainLayout->addWidget(sideWidget);

    // Create toolbar
    toolBar = addToolBar(tr("Menu"));
    
    // Create toolbar actions 
    QAction *updateAction = new QAction("Update", this);
    connect(updateAction, &QAction::triggered, this, &MainWindow::updateData);
    toolBar->addAction(updateAction);

    QAction *quitAction = new QAction("Quit", this);
    connect(quitAction, &QAction::triggered, QCoreApplication::quit);
    toolBar->addAction(quitAction);
    
    QAction *is = new QAction("Iltasanomat", this);
    connect(is, &QAction::triggered,this, [this]() {
        data->changeProvider("Iltasanomat");
        updateData();     
    });
    toolBar->addAction(is);
    
    QAction *il = new QAction("Iltalehti", this);
    connect(il, &QAction::triggered,this, [this]() {
        data->changeProvider("Iltalehti");
        updateData();     
    });
    toolBar->addAction(il); 
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
    std::vector<DataManager::hl>* headlines = data->getHeadlines();

    // Create buttons for each headline
    for (const auto &headline : *headlines) {

        if (headline.headline.size() == 0){
            continue;
        }

        QPushButton *button = new QPushButton(QString::fromStdString(headline.headline), centralWidget);
        button->setProperty("url", QString::fromStdString(headline.headlineUrl)); 
        connect(button, &QPushButton::clicked, this, &MainWindow::openUrl);
        
        button->setProperty("caption", QString::fromStdString(headline.headlineCaption));

        button->installEventFilter(this);
        buttonLayout->addWidget(button);
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

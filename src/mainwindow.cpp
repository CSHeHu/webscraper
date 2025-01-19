
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
    // Create a main widget that will hold everything
    QWidget *mainWidget = new QWidget(this);
    setCentralWidget(mainWidget);

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
    
    // Create layout for the side widget (a vertical layout)
    QVBoxLayout *sidePanelLayout = new QVBoxLayout(sideWidget);
    sideWidget->setLayout(sidePanelLayout);

    // Test label for side panel
    QLabel *label = new QLabel("More content here", sideWidget);
    sidePanelLayout->addWidget(label);

    // Add the scroll area and side widget to the main layout
    mainLayout->addWidget(scrollArea);  
    mainLayout->addWidget(sideWidget);

    // Create toolbar
    toolBar = addToolBar(tr("Menu"));
    
    // Create toolbar actions (instead of buttons)
    QAction *updateAction = new QAction("Update", this);
    connect(updateAction, &QAction::triggered, this, &MainWindow::updateData);
    toolBar->addAction(updateAction); // Add action to toolbar

    QAction *quitAction = new QAction("Quit", this);
    connect(quitAction, &QAction::triggered, QCoreApplication::quit);
    toolBar->addAction(quitAction); // Add action to toolbar    
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
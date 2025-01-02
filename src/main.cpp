#include "dataManager.h"
#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{

    DataManager data;
    data.updateData();

    for (const auto &i : data.getHeadlines())
    {
        std::cout << i.headline << " - " << i.headlineUrl << std::endl;
    }

    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();



}
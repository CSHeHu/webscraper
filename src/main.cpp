#include "dataManager.h"

int main(int argc, char *argv[])
{
/*
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
*/

DataManager data;
data.updateData();

for (const auto &i : data.getHeadlines())
{
    std::cout << i.headline << " - " << i.headlineUrl << std::endl;
}

}
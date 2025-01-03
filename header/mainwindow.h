#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QVBoxLayout>
#include <QUrl>
#include <QDesktopServices>
#include <QUrl>
#include <QScrollArea>
#include <QToolBar>
#include "dataManager.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:

private slots:
    void openUrl(); 

private:
    Ui::MainWindow *ui;
    QVBoxLayout *buttonLayout;  // Layout to hold buttons
    QWidget *centralWidget;     // Central widget for the layout
    DataManager dataManager;    // Instance of DataManager to fetch headlines
    QToolBar *toolBar;

    void createGui();
    void updateData();
};
#endif // MAINWINDOW_H

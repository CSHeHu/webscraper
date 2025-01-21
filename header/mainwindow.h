#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QVBoxLayout>
#include <QUrl>
#include <QScrollArea>
#include <QToolBar>
#include <QLabel>
#include <QCoreApplication>
#include "dataManager.h"
#include <QDesktopServices>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    bool eventFilter(QObject *obj, QEvent *event) override;

private slots:
    void openUrl(); 

private:
    QVBoxLayout *buttonLayout;  // Layout to hold buttons
    QWidget *centralWidget;     // Central widget for the layout
    DataManager *data;    // Instance of DataManager to fetch headlines
    QToolBar *toolBar;
    QLabel *newsInfoLabel;

    void createGui();
    void updateData();
};
#endif // MAINWINDOW_H

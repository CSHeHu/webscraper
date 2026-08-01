#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "dataManager.h"
#include <QCoreApplication>
#include <QDesktopServices>
#include <QLabel>
#include <QLineEdit>
#include <QMainWindow>
#include <QPushButton>
#include <QScrollArea>
#include <QToolBar>
#include <QUrl>
#include <QVBoxLayout>
#include <qlineedit.h>

class MainWindow : public QMainWindow {
  Q_OBJECT

public:
  MainWindow(QWidget *parent = nullptr);
  ~MainWindow();

protected:
  bool eventFilter(QObject *obj, QEvent *event) override;

private slots:
  void openUrl();

private:
  QVBoxLayout *buttonLayout; // Layout to hold buttons
  QWidget *centralWidget;    // Central widget for the layout
  DataManager *data;         // Instance of DataManager to fetch headlines
  QToolBar *toolBar;
  QLabel *newsInfoLabel;
  QLineEdit *searchLineEdit;

  void createGui();
  void updateData();
  void search();
};
#endif // MAINWINDOW_H

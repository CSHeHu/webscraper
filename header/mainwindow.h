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
  QVBoxLayout *buttonLayout;
  QWidget *centralWidget;
  DataManager *data;
  QToolBar *toolBar;
  QLabel *newsInfoLabel;
  QLineEdit *searchLineEdit;
  QPalette lightPalette;
  QPalette darkPalette;

  void createGui();
  void updateGui();
  void search();
  void toggleTheme();
};
#endif // MAINWINDOW_H

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDebug>
#include <QPixmap>
#include <QPainter>
#include <QKeyEvent>
#include "maze.h"
namespace Ui {
  class MainWindow;
}

class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  explicit MainWindow(QWidget *parent = 0);


  ~MainWindow();
public slots:
  void showMaze();
  void whetherShowPath();
  void handleSlider();
  void generateNewMaze();
private:
  Ui::MainWindow *ui;
  Maze maze;
  bool showPath;
  int tempWidth;
  int tempHeight;
  const int texSize=20;
  const int fixX=20;
  const int fixY=20;
protected:
  void paintEvent(QPaintEvent* e);
  void keyPressEvent(QKeyEvent* e);
};

#endif // MAINWINDOW_H

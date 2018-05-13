#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::MainWindow)
{
  ui->setupUi(this);
  connect(ui->genarate_button,SIGNAL(clicked(bool)),this,SLOT(generateNewMaze()));
  connect(ui->findPathCheck,SIGNAL(clicked(bool)),this,SLOT(whetherShowPath()));
  connect(ui->XSlider,SIGNAL(valueChanged(int)),this,SLOT(handleSlider()));
  connect(ui->YSlider,SIGNAL(valueChanged(int)),this,SLOT(handleSlider()));
  this->setFocus();
  int tempHeight=15;
  int tempWidth=21;
  ui->XSlider->setValue(11);
  ui->YSlider->setValue(8);
  ui->label->setText(tr("宽:")+QString::number(tempWidth)+tr("*高:")+QString::number(tempHeight));
  generateNewMaze();
}
void MainWindow::handleSlider(){
  tempWidth=ui->XSlider->value()*2-1;
  tempHeight=ui->YSlider->value()*2-1;
  ui->label->setText(tr("宽:")+QString::number(tempWidth)+tr("*高:")+QString::number(tempHeight));
}

MainWindow::~MainWindow()
{
  delete ui;
}
void MainWindow::showMaze(){
  update();
}
void MainWindow::generateNewMaze(){
  maze.generate(tempWidth,tempHeight);
  ui->XSlider->setGeometry(fixX+tempWidth*texSize+fixX,
                           ui->XSlider->geometry().y(),
                           ui->XSlider->width(),
                           ui->XSlider->height());
  ui->YSlider->setGeometry(fixX+tempWidth*texSize+fixX+25,
                           ui->YSlider->geometry().y(),
                           ui->YSlider->width(),
                           ui->YSlider->height());
  ui->genarate_button->setGeometry(fixX+tempWidth*texSize+fixX,
                                   ui->genarate_button->geometry().y(),
                                   ui->genarate_button->width(),
                                   ui->genarate_button->height());
  ui->findPathCheck->setGeometry(fixX+tempWidth*texSize+fixX,
                                 ui->findPathCheck->geometry().y(),
                                 ui->findPathCheck->width(),
                                 ui->findPathCheck->height());
  ui->label->setGeometry(fixX+tempWidth*texSize+fixX,
                         ui->label->geometry().y(),
                         ui->label->width(),
                         ui->label->height());
  this->setGeometry(this->geometry().x(),
                    this->geometry().y(),
                    fixX+tempWidth*texSize+fixX+ui->genarate_button->width()+fixX,
                    fixY+tempHeight*texSize+fixY*2);
  update();
}

void MainWindow::whetherShowPath(){
  if(ui->findPathCheck->checkState()==Qt::Checked)
    showPath=true;
  else
    showPath=false;
  update();
}

void MainWindow::paintEvent(QPaintEvent *e){
  QPixmap wallImg(":/image/brick.jpg");
  QPixmap huajige(":/image/smallHJ.png");
  QPainter painter(this);
  painter.setPen(Qt::green);
  for(int i=0;i<maze.width;++i)
    for(int j=0;j<maze.height;++j){
        if(maze.maze[i][j]==mazeStatus::wall)
        painter.drawPixmap(texSize*i+fixX,texSize*j+fixY,texSize,texSize,wallImg.scaled(texSize,texSize));
      }
  painter.drawPixmap(texSize*maze.currentPoint.x+fixX,texSize*maze.currentPoint.y+fixY,texSize,texSize,huajige.scaled(texSize,texSize));
  if(showPath){
      maze.findPath();
      painter.setBrush(QBrush(Qt::green));
      for(auto a:maze.currentToEndPath){
          painter.drawRect(texSize*a.x+fixX,texSize*a.y+fixY,texSize,texSize);
        }
    }
  if(maze.currentPoint!=maze.endPoint){
      QPixmap outImg(":/image/out.png");
      painter.drawPixmap(texSize*maze.endPoint.x+fixX,texSize*maze.endPoint.y+fixY,texSize,texSize,outImg.scaled(texSize,texSize));
    }
}
void MainWindow::keyPressEvent(QKeyEvent *e){
  switch (e->key()) {
    case Qt::Key_Up:
      if(maze.currentPoint.y-1>=0&&maze.maze[maze.currentPoint.x][maze.currentPoint.y-1]!=mazeStatus::wall)
        --maze.currentPoint.y;
      break;
    case Qt::Key_Down:
      if(maze.currentPoint.y+1<maze.height&&maze.maze[maze.currentPoint.x][maze.currentPoint.y+1]!=mazeStatus::wall)
        ++maze.currentPoint.y;
      break;
    case Qt::Key_Right:
      if(maze.currentPoint.x+1<maze.width&&maze.maze[maze.currentPoint.x+1][maze.currentPoint.y]!=mazeStatus::wall)
        ++maze.currentPoint.x;
      break;
    case Qt::Key_Left:
      if(maze.currentPoint.x-1>=0&&maze.maze[maze.currentPoint.x-1][maze.currentPoint.y]!=mazeStatus::wall)
        --maze.currentPoint.x;
      break;
    default:
      break;
    }
  update();
}

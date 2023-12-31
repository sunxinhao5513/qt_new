#include "game.h"
#include "ui_game.h"
#include <QPainter>
#include <QPaintEvent>
#include <QSoundEffect>
#include <QComboBox>
#include "helpwindow.h"
#include "gameinit.h"
#include "changewindow.h"
#include <QPalette>
#include <QLayout>
#include <QTimer>
#include "mainwindow.h"
#include "timeout.h"

game::game(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::game)
{
    setAutoFillBackground(true);
    ui->setupUi(this);
    this->resize(QSize(1000,700));
    music->setSource(QUrl::fromLocalFile("://music/Flappy-Bird-Theme-Song.wav"));
    music->setLoopCount(music->Infinite);
    music->play();
    setLayout(gameLayout);
    changewind->setWindowFlags(Qt::Dialog);
    helpwind->setWindowFlags(Qt::Dialog);
    timer->start(20*60*1000);
    connect(timer,SIGNAL(timeout()),this,SLOT(handletle()));
}
void game::paintEvent(QPaintEvent *event){
    QPainter p(this);
    p.drawPixmap(0,0,this->width(),this->height(),QPixmap(":/resource/images/765395.jpg"));
}

game::~game()
{
    delete ui;
    delete music;
    if(window != nullptr) delete window;
}


void game::on_helpbutton_clicked()
{
    helpwind->show();
}

void game::on_quitbutton_clicked()
{
    this->close();
}

void game::on_startbutton_clicked()
{
    window = new MainWindow(nullptr, skin, diff);
    connect(window, &MainWindow::returnTo, this, &game::handleReturnTo);
    this->hide();
    window->show();
}

void game::on_pushButton_clicked()
{
    changewind->music = music;
    changewind->show();
}

void game::handleReturnTo(){
    this->show();
    window->close();
    delete window;
}

void game::changeToSkin0(bool checked){
    if(checked == true)
        skin = 0;
}

void game::changeToSkin1(bool checked){
    if(checked == true)
        skin = 1;
}

void game::changeToDiff0(bool checked){
    if(checked == true)
        diff = 0;
}

void game::changeToDiff1(bool checked){
    if(checked == true)
        diff = 1;
}
void game::handletle(){
    disconnect(timer,SIGNAL(timeout()),this,SLOT(handletle()));
    timeout *page=new timeout();
    page->show();
    this->close();
}


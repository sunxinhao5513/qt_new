#include "timeout.h"
#include "ui_timeout.h"
#include<QTimer>
#include<QPainter>
#include<QString>

timeout::timeout(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::timeout)
{
    ui->setupUi(this);
    this->setWindowTitle("健康游戏忠告");
    QTimer *t=new QTimer();
    connect(t,SIGNAL(timeout()),this,SLOT(handletimeout()));
    t->start(5000);
    QString s="你已经游戏20分钟，休息一下吧";
    ui->textEdit->setTextInteractionFlags(Qt::NoTextInteraction);
    ui->textEdit->setTextColor(Qt::yellow);
    ui->textEdit->setText(s);
}
void timeout::handletimeout(){
    this->close();
}

void timeout::paintEvent(QPaintEvent *event){
    QPainter p(this);
    p.drawPixmap(0,0,this->width(),this->height(),QPixmap(":/resource/images/765395.jpg"));
}

timeout::~timeout()
{
    delete ui;
}

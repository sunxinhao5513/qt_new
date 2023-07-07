#include "map.h"
#include "ui_map.h"
#include <QPainter>

map::map(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::map)
{
    ui->setupUi(this);
}

void map::paintEvent(QPaintEvent *event){
    QPainter p(this);
    p.drawPixmap(0,0,this->width(),this->height(),QPixmap(":/resource/images/765395.jpg"));
}

map::~map()
{
    delete ui;
}

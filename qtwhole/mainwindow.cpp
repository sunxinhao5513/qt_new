#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "bird.h"
#include "ghost.h"
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QPixmap>
#include <QTimer>
#include <QTextEdit>
#include <QDataStream>
#include <QTextStream>
#include <QFile>
#include "stoppage.h"
#include <QApplication>
MainWindow::MainWindow(QWidget *parent, int idx, int _diff)
    : QMainWindow(parent), ui(new Ui::MainWindow), scene(new QGraphicsScene(this)), bird(new PacbirdClass(idx)),
      dropTimer(new QTimer)
{
    setWindowTitle("game");
    diff = _diff;
    init();
    action();
}

void MainWindow::init(){
    eatDot->setSource(QUrl::fromLocalFile(":/music/mixkit-arcade-game-jump-coin-216.wav"));
    ui->setupUi(this);
    scene->setSceneRect(0, 0, 800, 600);
    episode=max_episode;
    QPixmap backgroundPixmap(":/resource/images/background.png");
    scene->setBackgroundBrush(QBrush(backgroundPixmap));
    ui->graphicsView->setScene(scene);
    //结束界面隐藏
    ui->textEdit->setTextInteractionFlags(Qt::NoTextInteraction);
//结束界面隐藏
    ui->textEdit_2->hide();
    ui->textEdit_3->hide();
    ui->textEdit_4->hide();
    ui->textEdit_5->hide();
    ui->textEdit_6->hide();
    ui->button1->hide();
    ui->button2->hide();




    updatescore();
    life = true;

    scene->addItem(bird);
    connect(this, &MainWindow::lose, this, &MainWindow::Lose);
    connect(this, &MainWindow::win, this, &MainWindow::Win);
    for(int i = 0; i < 4; ++i){
        topGhost[i] = new GhostClass(i, 0, diff);
        bottomGhost[i] = new GhostClass(i, 1, diff);
        scene->addItem(bottomGhost[i]);
        scene->addItem(topGhost[i]);
    }
    for(int i = 0; i < 8; ++i){
        dotClass* newDot = new dotClass;
        scene->addItem(newDot);
        dots.push_back(newDot);
    }
}

void MainWindow::action(){
    connect(dropTimer, SIGNAL(timeout()), this, SLOT(advance()));
    dropTimer->start(30);
}

void MainWindow::detectLose(){
    QPointF birdPos = bird->pos();
    if((birdPos.y() <= -30 || birdPos.y() >= 650) && life == true) emit lose();
    for(int i = 0; i < 4; ++i)
        if(((detectCollide(bird, topGhost[i]) || detectCollide(bird, bottomGhost[i])) && bird->isSuper() == 0) && life == true)
            emit lose();
}

void MainWindow::detectWin(){
    if(score>=passscore){
        emit win();
    }
}

void MainWindow::keyPressEvent(QKeyEvent* event){
    if (event->key() == Qt::Key_Space) {
        bird->flap();
    }
    if(event->key()==Qt::Key_S){
        disconnect(dropTimer, SIGNAL(timeout()), this, SLOT(advance()));
        stop->show();
    }
}
void MainWindow::keyReleaseEvent(QKeyEvent *event){
    if(event->key()==Qt::Key_C){
        connect(dropTimer, SIGNAL(timeout()), this, SLOT(advance()));
        delete stop;
    }
}

void MainWindow::mousePressEvent(QMouseEvent *event){
    if(event->button() == Qt::LeftButton && !life){
        this->close();
        emit returnTo();
    }
}

bool MainWindow::detectCollide(const QGraphicsPixmapItem* a, const QGraphicsPixmapItem* b){
    QRectF aBox = a->boundingRect();
    aBox = a->mapRectToScene(aBox);
    QRectF bBox = b->boundingRect();
    bBox = b->mapRectToScene(bBox);
    if(aBox.intersects(bBox)) return 1;
    else return 0;
}

void MainWindow::updatescore(){

    ui->textEdit->setTextColor(QColor(255, 241, 67));

    QString s = "Score:";
    s.append(QString::number(score, 10));
    ui->textEdit->setText(s);
}

void MainWindow::advance(){

    updatescore();
    updatePosition();
    detectLose();
    detectWin();
    processDots();
}

void MainWindow::updatePosition(){
    bird->updatePosition();
    if(life == true){
        for(int i = 0; i < 4; ++i){
            if(topGhost[i]->checkpos()) score++;
            topGhost[i]->updatePosition();
            bottomGhost[i]->updatePosition();
        }
        for(auto& dot :dots)
            dot->updatePosition();
    }
}


void MainWindow::processDots(){
    for(auto& dot :dots){
        while(true){
            bool flag = 1;
            for(int i = 0; i < 4; ++i)
                if(detectCollide(dot, topGhost[i]) || detectCollide(dot, bottomGhost[i])){
                    dot->reset();
                    flag = 0;
                    break;
                }
            if(flag) break;
        }
        for(int i = 0; i < 4; ++i)
            while(detectCollide(dot, topGhost[i]) || detectCollide(dot, bottomGhost[i])){
                dot->reset();
            }
        if(detectCollide(dot, bird)){
            eatDot->play();
            if(dot->isCapsule()){
                bird->onSuper();
                for(int i = 0; i < 4; ++i){
                    topGhost[i]->onSuper();
                    bottomGhost[i]->onSuper();
                }
                score += 5;
            }
            score += 1;
            dot->reset();
        }
    }
}

void MainWindow::read_maxscore()
{
    QFile file("D:/code/final/score.txt");
    bool isOK = file.open(QIODevice::ReadOnly);
    if(isOK == true){
        QTextStream stream(&file);
        stream >> max_score;

    }

    if(max_score < score) max_score = score;
    file.close();
}

void MainWindow::write_maxscore()
{
    QFile file("D:/code/final/score.txt");
    bool isOK = file.open(QIODevice::WriteOnly);
    if(isOK == true){
        QTextStream stream(&file);
        stream << max_score;
    }

    file.close();
}

void MainWindow::read_maxepisode()
{
    QFile file("D:/qtwhole/episode.txt");
    bool isOK = file.open(QIODevice::ReadOnly);
    if(isOK == true){
        QTextStream stream(&file);
        stream >> max_episode;

    }

    if(max_episode < score) max_episode = episode;
    file.close();
}

void MainWindow::write_maxepisode()
{
    QFile file("D:/qtwhole/episode.txt");
    bool isOK = file.open(QIODevice::WriteOnly);
    if(isOK == true){
        QTextStream stream(&file);
        stream << max_episode;
    }

    file.close();
}
void MainWindow::Lose()
{
    //disconnect(dropTimer, SIGNAL(timeout()), this, SLOT(advance()));
    life = false;

    read_maxscore();
    write_maxscore();

    ui->textEdit_2->setTextInteractionFlags(Qt::NoTextInteraction);
    ui->textEdit_3->setTextInteractionFlags(Qt::NoTextInteraction);
    ui->textEdit_4->setTextInteractionFlags(Qt::NoTextInteraction);
    ui->textEdit_5->setTextInteractionFlags(Qt::NoTextInteraction);



    ui->textEdit_4->moveCursor(QTextCursor::End);
    ui->textEdit_5->moveCursor(QTextCursor::End);
    ui->textEdit_4->insertPlainText(QString::number(score));
    ui->textEdit_5->insertPlainText(QString::number(max_score));

    ui->textEdit_2->show();
    ui->textEdit_3->show();
    ui->textEdit_4->show();
    ui->textEdit_5->show();
}

void MainWindow::Win(){
    life=false;
    read_maxepisode();
    write_maxepisode();
    QString s1="恭喜通过第"+QString::number(max_episode)+"关";
    ui->textEdit_6->setText(s1);
    ui->textEdit_6->setTextColor(Qt::yellow);
    ui->textEdit_6->setTextInteractionFlags(Qt::NoTextInteraction);
    ui->textEdit_6->show();
    ui->button1->show();
    ui->button2->show();
    episode++;
}

MainWindow::~MainWindow()
{
    delete ui;
    for(int i = 0; i < 4; ++i){
        delete topGhost[i];
        delete bottomGhost[i];
    }
    delete bird;
    delete scene;
    delete dropTimer;
    delete stop;
}

void MainWindow::on_button1_clicked()
{
    emit returnTo();
}


void MainWindow::on_button2_clicked()
{
    this->close();
    MainWindow*newepisode=new MainWindow();
    newepisode->show();
}


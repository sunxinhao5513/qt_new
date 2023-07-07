#ifndef TIMEOUT_H
#define TIMEOUT_H

#include <QWidget>

namespace Ui {
class timeout;
}

class timeout : public QWidget
{
    Q_OBJECT

public:
    explicit timeout(QWidget *parent = nullptr);
    ~timeout();
public slots:
    void handletimeout();

private:
    Ui::timeout *ui;

    // QWidget interface
protected:
    void paintEvent(QPaintEvent *event);
};

#endif // TIMEOUT_H

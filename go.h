#ifndef GO_H
#define GO_H

#include <QMainWindow>
#include <QPainter>
#include<QTcpSocket>
#include<QMessageBox>
#include"table.h"
namespace Ui {
class Go;
}

class Go : public QMainWindow
{
    Q_OBJECT

public:
    explicit Go(QWidget *parent = 0);
    ~Go();
    void paintEvent(QPaintEvent *event);
private slots:
    void on_pushButton_clicked();
    //收到消息触发槽
    void ClientRecvData();
    void on_pushButton_3_clicked();
    void on_moveButton_clicked();
    void on_pushButton_2_clicked();

private:

    Table *table;
    Ui::Go *ui;
    int mode=0;
    QTcpSocket * clientSocket;
    bool first=0;
};

#endif // GO_H

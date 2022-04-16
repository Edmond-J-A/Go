#include "go.h"
#include "ui_go.h"
static int roun=1;
Go::Go(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Go)
{
    ui->setupUi(this);
    repaint();
    buttonSound=new QSoundEffect();
    buttonSound->setSource(QUrl::fromLocalFile(":/res/down.WAV"));
    buttonSound->setLoopCount(1);
    ui->label->setVisible(0);
    ui->label_2->setVisible(0);
    ui->label_3->setVisible(0);
    table=new Table(this);
    ui->pushButton_2->setVisible(0);
}

Go::~Go()
{
    delete ui;
    delete clientSocket;
    delete table;
    delete buttonSound;
}

void Go::paintEvent(QPaintEvent *event){
    QPainter paint(this);
    if(mode==0){
        paint.setPen(Qt::NoPen);
        paint.setBrush(Qt::black);
        paint.drawRect(rect());
    }else if(mode==1||mode==2){
        paint.setPen(Qt::NoPen);
        paint.setBrush(Qt::black);
        paint.drawRect(rect());
        paint.drawPixmap(0,0,QPixmap(":/res/table.png"));
        if(first){
            ui->label_2->setStyleSheet(
                        "image: url(:/res/black.png);"
                        "background-color: rgb(0, 100, 255);"
                        );
            ui->label->setStyleSheet(
                        "image: url(:/res/white.png);"
                        "background-color: rgb(0, 100, 255);"
                        );
        }else{
            ui->label_2->setStyleSheet(
                        "image: url(:/res/white.png);"
                        "background-color: rgb(0, 100, 255);"
                        );
            ui->label->setStyleSheet(
                        "image: url(:/res/black.png);"
                        "background-color: rgb(0, 100, 255);"
                        );
        }
        //table->ShowButton();
    }
}

void Go::on_pushButton_clicked()
{
    clientSocket = new QTcpSocket();
    clientSocket->connectToHost("localhost", 23334);//1.15.140.139
    if(!clientSocket->waitForConnected(3000))
    {
        QMessageBox::information(this, "Error", "连接服务端失败,请检查网络！");
        this->close();
    }
    connect(clientSocket, SIGNAL(readyRead()), this, SLOT(ClientRecvData()));

}

void Go::ClientRecvData()
{
    qDebug()<<"MainWindow";
    QByteArray result = clientSocket->readAll();
    for(int i = 0;i < result.size();i++)
    {
        if(result[i] == '\n')
        {
            result.resize(i);
            break;
        }
    }
    qDebug()<<"result"<<result;
    std::string get=result.toStdString();
    if(mode==0&&get[0]=='d'){
        mode=2;
        first=get[1]-'0';
        ui->pushButton->setVisible(0);
        ui->pushButton_3->setVisible(0);
        ui->label->setVisible(1);
        ui->label_2->setVisible(1);
        ui->label_3->setVisible(1);
        table->ShowButton();
        repaint();
    }else if(mode==2&&(get[0]=='M'||get[0]=='m')){
        int p;
        if(get[0]=='M'){
            p=1;
        }else{
            p=0;
        }
        int x=0,y=0;
        int i=1;
        for(;i<get.size();i++){
            if(get[i]==' '){
                break;
            }
            x=x*10+get[i]-'0';
        }
        i++;
        for(;i<get.size();i++){
            y=y*10+get[i]-'0';
        }
        qDebug()<<x;
        qDebug()<<y;
        table->Move(x,y);
    }
}

void Go::on_pushButton_3_clicked()
{
    mode=1;
    first=1;
    ui->pushButton->setVisible(0);
    ui->pushButton_3->setVisible(0);
    ui->label->setVisible(1);
    ui->label_2->setVisible(1);
    ui->label_3->setVisible(1);
    table->ShowButton();
    ui->pushButton_2->setVisible(1);
    repaint();
}

void Go::on_moveButtonclicked(){
    if(mode==1){//本地
        QPushButton *pb=qobject_cast<QPushButton *>(sender());
        std::string getName=pb->accessibleName().toStdString();
        int x=0,y=0;
        int i=0;
        for(;i<getName.size();i++){
            if(getName[i]==' '){
                break;
            }
            x=x*10+getName[i]-'0';
        }
        i++;
        for(;i<getName.size();i++){
            y=y*10+getName[i]-'0';
        }
        if(table->Move(x,y)){           //完成后需判定
            buttonSound->play();
            std::vector<int> aiMovement=ai.AIMove(x,y,table->GetTable(),table->Getlwx(),table->Getlwy());
            if(aiMovement.size()!=0){
                table->Move(aiMovement[0],aiMovement[1]);
                buttonSound->play();
            }else{
                qDebug()<<"empty";
                QMessageBox::information(this,"message","end");
            }
        }
    }else if(mode==2){
        if(table->GetRound()%2==first){
            QPushButton *pb=qobject_cast<QPushButton *>(sender());
            std::string getName=pb->accessibleName().toStdString();
            int x=0,y=0;
            int i=0;
            for(;i<getName.size();i++){
                if(getName[i]==' '){
                    break;
                }
                x=x*10+getName[i]-'0';
            }
            i++;
            for(;i<getName.size();i++){
                y=y*10+getName[i]-'0';
            }
            if(table->Move(x,y)){
                QString mess="m"+QString::number(x)+" "+QString::number(y)+"\n";
                const char* ch = mess.toStdString().c_str();
                clientSocket->write(ch);
            }
        }
    }

}

void Go::on_pushButton_2_clicked()
{
    table->Clean();
}


#ifndef TABLE_H
#define TABLE_H
#include<vector>
#include<QPushButton>
#include<QWidget>
#include<queue>
#include<QObject>
#define XSTART 81
#define YSTART 68
#define WIDTH 33
class Table
{
    std::vector<std::vector<int>>map;
    std::vector<std::vector<QPushButton*>>block;
    int round=1;

public:
    Table(QWidget * parent) {
        map=std::vector<std::vector<int>>(19,std::vector<int>(19,0));
        for(int i=0;i<19;i++){
            std::vector<QPushButton*>temp;
            for(int j=0;j<19;j++){
                QPushButton *newbutton=new QPushButton(parent);
                newbutton->resize(WIDTH,WIDTH);
                newbutton->setAccessibleName(QString::number(i)+" "+QString::number(j));
                newbutton->setStyleSheet(
                            "QPushButton { "
                            "background-color:rgba(0, 0, 0,0);"
                            "}"

                            "QPushButton:hover { "
                            "color:rgba(0, 0, 0,125);"
                            "background-image: url(:/res/black.png);"
                            "}"
                            );
                QObject::connect(newbutton,SIGNAL(clicked()),parent,SLOT(on_moveButton_clicked()));
                newbutton->move(3+XSTART-0.5*39+i*39,3+YSTART-0.5*39+j*39);
                newbutton->setCursor(Qt::PointingHandCursor);
                newbutton->setVisible(0);
                temp.push_back(newbutton);
            }
            block.push_back(temp);
        }
    }

    ~Table(){
        for(int i=0;i<19;i++){
            for(int j=0;j<19;j++){
                delete block[i][j];
            }
        }
    }

    void ShowButton(){
        for(int i=0;i<19;i++){
            for(int j=0;j<19;j++){
                block[i][j]->setVisible(1);
            }
        }
    }
    void RoundChange();
    bool Move(int player,int x,int y);
    bool Check(int x,int y);
    void Undo();
    void Clean();
    void Eat(int x,int y);
    std::vector<std::vector<int>> GetTable();
};
#endif // TABLE_H

#include"table.h"
bool Table::Move(int x,int y){
    if(Check(x,y)){
        map[x][y]=round;
        if(round%2==0){                     //白棋
            lastwx=x;
            lastwy=y;
            block[x][y]->setStyleSheet(
                        "background-color:rgba(0, 0, 0,0);"
                        "background-image: url(:/res/white.png);"
                        );
        }else{                              //黑棋
            lastbx=x;
            lastby=y;
            block[x][y]->setStyleSheet(
                        "background-color:rgba(0, 0, 0,0);"
                        "background-image: url(:/res/black.png);"
                        );
        }
        RoundChange();
        return true;
    }
    return false;
}

void Table::RoundChange(){
    round++;
    for(int i=0;i<19;i++){
        for(int j=0;j<19;j++){
            if(map[i][j]==0){
                if(round%2==0){                     //白棋
                    block[i][j]->setStyleSheet(
                                "QPushButton { "
                                "background-color:rgba(0, 0, 0,0);"
                                "}"

                                "QPushButton:hover { "
                                "color:rgba(0, 0, 0,125);"
                                "background-image: url(:/res/white.png);"
                                "}"
                                );
                }else{                              //黑棋
                    block[i][j]->setStyleSheet(
                                "QPushButton { "
                                "background-color:rgba(0, 0, 0,0);"
                                "}"

                                "QPushButton:hover { "
                                "color:rgba(0, 0, 0,125);"
                                "background-image: url(:/res/black.png);"
                                "}"
                                );
                }
            }
        }
    }
}

void Table::Undo(){

}

void Table::Clean(){
    for(int i=0;i<19;i++){
        for(int j=0;j<19;j++){
            map[i][j]=0;
            block[i][j]->setStyleSheet(
                        "QPushButton { "
                        "background-color:rgba(0, 0, 0,0);"
                        "}"

                        "QPushButton:hover { "
                        "color:rgba(0, 0, 0,125);"
                        "background-image: url(:/res/black.png);"
                        "}"
                        );
        }
    }
    round=1;

}

void Table::Eat(int x,int y){
    std::stack<std::pair<int,int>> s;
    std::vector<std::pair<int,int>> temp;
    int direction[4][2]={{1,0},{0,1},{-1,0},{0,-1}};
    for(auto d : direction){
        std::vector<std::vector<bool>>visitied(19,std::vector<bool>(19,0));
        visitied[x][y]=1;
        if(0<=x+d[0]&&x+d[0]<19&&y+d[1]<19&&0<=y+d[1]&&map[x+d[0]][y+d[1]]%2!=round%2){
            s.push(std::pair<int,int>(x+d[0],y+d[1]));
            temp.push_back(std::pair<int,int>(x+d[0],y+d[1]));
        }
        while(s.size()!=0){               //dfs
            int x2=s.top().first;
            int y2=s.top().second;
            s.pop();
            for(auto dir : direction){
                if(0<=x2+dir[0]&&x2+dir[0]<19&&0<=y2+dir[1]&&y2+dir[1]<19&&visitied[x2+dir[0]][y2+dir[1]]==0){
                    int x1=x2+dir[0];
                    int y1=y2+dir[1];

                    if(map[x1][y1]==0){
                        s=std::stack<std::pair<int,int>>();
                        temp.clear();
                        break;
                    }else if(map[x1][y1]%2!=round%2){
                        s.push(std::pair<int,int>(x1,y1));
                        temp.push_back(std::pair<int,int>(x1,y1));
                        visitied[x1][y1]=1;
                    }
                }
            }
        }
        for(int i=0;i<temp.size();i++){
            map[temp[i].first][temp[i].second]=0;
        }
    }
}

bool Table::Check(int x,int y){
    if(x<0||x>=19||y<0||y>=19){
        return false;
    }
    if(round%2==1&&lastbx==x&&lastby==y){
        return false;
    }else if(round%2==0&&lastwx==x&&lastwy==y){
        return false;
    }
    if(map[x][y]==0){
        Eat(x,y);
        int direction[4][2]={{1,0},{0,1},{-1,0},{0,-1}};
        std::vector<std::vector<bool>>visitied(19,std::vector<bool>(19,0));
        std::queue<std::pair<int,int>> q;
        q.emplace(std::pair<int,int>(x,y));
        visitied[x][y]=1;
        bool flag=0;
        while(q.size()!=0){                         //bfs
            int x2=q.front().first;
            int y2=q.front().second;
            q.pop();
            for(auto dir : direction){
                if(0<=x2+dir[0]&&x2+dir[0]<19&&0<=y2+dir[1]&&y2+dir[1]<19&&visitied[x2+dir[0]][y2+dir[1]]==0){
                    int x1=x2+dir[0];
                    int y1=y2+dir[1];
                    visitied[x1][y1]=1;
                    if(map[x1][y1]==0){
                        q=std::queue<std::pair<int,int>>();
                        flag=1;
                        break;
                    }else if(map[x1][y1]%2==round%2){
                        q.emplace(std::pair<int,int>(x1,y1));
                    }
                }
            }
        }
        if(flag){
            return true;
        }else{
            return false;
        }
    }else{
        return false;
    }
}

std::vector<std::vector<int>> Table::GetTable(){
    return map;
}


#include"ai.h"
std::vector<int> AI::AIMove(int x,int y,std::vector<std::vector<int>>m,int lx,int ly){
    int getscore=0;
    map=m;
    std::vector<int> bestx, besty;
    for(int i=0;i<19;i++){
        for(int j=0;j<19;j++){
            if(i==lx&&j==ly){
                continue;
            }
            if(map[i][j]==0){
                std::vector<std::vector<int>>smap = map;
                int nowEat=EatForAI(i,j,smap,1);
                if(nowEat>getscore)
                {
                    getscore=nowEat;
                    bestx.clear();
                    besty.clear();
                    bestx.push_back(i);
                    besty.push_back(j);
                }else if(nowEat==getscore){
                    bestx.push_back(i);
                    besty.push_back(j);
                }
            }
        }
    }
    if(getscore>0){
        int get = rand() % bestx.size();
        qDebug()<<"攻击"<<bestx[get]<< besty[get];
        return {bestx[get], besty[get]};
    }
    bestx.clear();
    besty.clear();
    getscore=0;
    for(int i=0;i<19;i++){
        for(int j=0;j<19;j++){
            if(i==lx&&j==ly){
                continue;
            }
            std::vector<std::vector<int>>smap = map;
            if(map[i][j]==0&&CheckForAI(i,j,smap,1)!=-1&&!isAnEye(i,j,smap)){//只剩一口气情况分类讨论未完成
                int nowEat=EatForAI(i,j,smap,0);
                if(nowEat>getscore)
                {
                    getscore=nowEat;
                    bestx.clear();
                    besty.clear();
                    bestx.push_back(i);
                    besty.push_back(j);
                }else if(nowEat==getscore){
                    bestx.push_back(i);
                    besty.push_back(j);
                }
            }
        }
    }
    if(getscore>0){
        int get = rand() % bestx.size();
        qDebug()<<"保护"<<bestx[get]<< besty[get];
        return {bestx[get], besty[get]};
    }


    std::vector<std::vector<int>>smap = map;
    bestx.clear();
    besty.clear();
    int  max = -100000;
    for (int i = x-4; i < x+4; i++) {
        for (int j = y-4; j < y+4; j++) {
            if(i==lx&&j==ly){
                continue;
            }
            if (i>=0&&j>=0&&j<19&&i<19&&smap[i][j] == 0&&CheckForAI(i,j,smap,1)!=-1&&!isAnEye(i,j,smap))
            {
                int temp = GetScore(i, j, 1, smap, 1, 0);
                if (temp > max) {
                    bestx.clear();
                    besty.clear();
                    bestx.push_back(i);
                    besty.push_back(j);
                    max = temp;
                }
                else if (temp == max) {
                    bestx.push_back(i);
                    besty.push_back(j);
                }
            }
        }
    }
    if(bestx.size()>0){
        int get = rand() % bestx.size();
        qDebug()<<"搜索"<<bestx[get]<< besty[get];
        return {bestx[get], besty[get]};
    }
    return {};
}

int AI::GetScore(int x,int y,int step,std::vector<std::vector<int>>&smap,bool ab,int nowscore){
    smap[x][y]=step+1;
    int getscore=0;
    std::vector<int> bestx, besty;
    for(int i=0;i<19;i++){
        for(int j=0;j<19;j++){
            if(smap[i][j]==0){
                std::vector<std::vector<int>>ssmap = smap;
                int nowEat=EatForAI(i,j,ssmap,1);
                if(nowEat>getscore)
                {
                    getscore=nowEat;
                    bestx.clear();
                    besty.clear();
                    bestx.push_back(i);
                    besty.push_back(j);
                }else if(nowEat==getscore){
                    bestx.push_back(i);
                    besty.push_back(j);
                }
            }
        }
    }
    if(getscore>0){
        return -getscore*2;
    }
    bestx.clear();
    besty.clear();
    getscore=0;
    for(int i=0;i<19;i++){
        for(int j=0;j<19;j++){
            if(smap[i][j]==0){
                std::vector<std::vector<int>>ssmap = smap;
                int nowEat=EatForAI(i,j,ssmap,0);
                if(nowEat>getscore)
                {
                    getscore=nowEat;
                    bestx.clear();
                    besty.clear();
                    bestx.push_back(i);
                    besty.push_back(j);
                }else if(nowEat==getscore){
                    bestx.push_back(i);
                    besty.push_back(j);
                }
            }
        }
    }
    if(getscore>0){
        return -getscore;
    }
    return 0;
}

int AI::EatForAI(int x,int y,std::vector<std::vector<int>>&smap,bool ab){
    std::stack<std::pair<int,int>> s;
    std::vector<std::pair<int,int>> temp;
    int res=0;
    int direction[4][2]={{1,0},{0,1},{-1,0},{0,-1}};
    for(auto d : direction){
        std::vector<std::vector<bool>>visitied(19,std::vector<bool>(19,0));
        visitied[x][y]=1;
        if(0<=x+d[0]&&x+d[0]<19&&y+d[1]<19&&0<=y+d[1]&&smap[x+d[0]][y+d[1]]%2==int(ab)){
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

                    if(smap[x1][y1]==0){
                        s=std::stack<std::pair<int,int>>();
                        temp.clear();
                        break;
                    }else if(smap[x1][y1]%2==int(ab)){
                        s.push(std::pair<int,int>(x1,y1));
                        temp.push_back(std::pair<int,int>(x1,y1));
                        visitied[x1][y1]=1;
                    }
                }
            }
        }
        for(int i=0;i<temp.size();i++){
            smap[temp[i].first][temp[i].second]=0;
        }
        res+=temp.size();
    }
    return res;
}

int AI::CheckForAI(int x,int y,std::vector<std::vector<int>>&smap,bool ab){
    if(smap[x][y]==0){
        int temp=EatForAI(x,y,smap,ab)>0;
        if(temp>0){
            return temp;
        }
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
                    if(smap[x1][y1]==0){
                        q=std::queue<std::pair<int,int>>();
                        flag=1;
                        break;
                    }else if(smap[x1][y1]%2!=int(ab)){
                        q.emplace(std::pair<int,int>(x1,y1));
                    }
                }
            }
        }
        if(flag){
            return 0;
        }else{
            return -1;
        }
    }else{
        return -1;
    }
}

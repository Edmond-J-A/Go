#ifndef AI_H
#define AI_H

#include<queue>
#include<stack>
#include<time.h>
#include <stdlib.h>
#include<QDebug>
class AI{
    std::vector<std::vector<int>>map;
public:
    AI(){
        srand(time(0));
    }

    std::vector<int> AIMove(int x,int y,std::vector<std::vector<int>>m,int lx,int ly);
    int GetScore(int x,int y,int step,std::vector<std::vector<int>>&smap,bool ab,int nowscore);
    int CheckForAI(int x,int y,std::vector<std::vector<int>>&smap,bool ab);
    int EatForAI(int x,int y,std::vector<std::vector<int>>&smap,bool ab);
    bool isAnEye(int x,int y,std::vector<std::vector<int>>&smap){
        int direction[4][2]={{1,0},{0,1},{-1,0},{0,-1}};
            for(auto dir : direction){
                if(0<=x+dir[0]&&x+dir[0]<19&&0<=y+dir[1]&&y+dir[1]<19){
                   if(smap[x+dir[0]][y+dir[1]]==0||smap[x+dir[0]][y+dir[1]]%2==1){
                       return false;
                   }
                }
            }
            return true;
    }
};


#endif // AI_H

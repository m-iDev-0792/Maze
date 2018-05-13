#include "maze.h"

//把选中的红色块周围的块变成蓝色
void Maze::turnBlue(int x, int y){
    maze[x][y]=mazeStatus::visitedRoad;
    //check up
    if(y-1!=0){
        if(maze[x][y-1]==mazeStatus::wall){
            blues.push_back(Point(x,y-1));
            maze[x][y-1]=mazeStatus::standbyArea;
        }
    }
    //check down
    if(y+1!=height-1){
        if(maze[x][y+1]==mazeStatus::wall){
            blues.push_back(Point(x, y+1));
            maze[x][y+1]=mazeStatus::standbyArea;
        }

    }
    //check left
    if(x-1!=0){
        if(maze[x-1][y]==mazeStatus::wall){
            blues.push_back(Point(x-1, y));
            maze[x-1][y]=mazeStatus::standbyArea;
        }
    }
    //check right
    if(x+1!=width-1){
        if(maze[x+1][y]==mazeStatus::wall){
            blues.push_back(Point(x+1,y));
            maze[x+1][y]=mazeStatus::standbyArea;
        }
    }
}
//判断一个蓝色块是否能够联通两个"房间",即一边是1(没有走过的房间)另一边是3(走过的房间)
bool Maze::goodBlue(int bx,int by,Point &linkedRoom){
    //left ==1 right==3
    if(maze[bx-1][by]==mazeStatus::road&&maze[bx+1][by]==mazeStatus::visitedRoad){
        linkedRoom.x=bx-1;
        linkedRoom.y=by;
        return true;
    }
    //left ==3 right==1
    if(maze[bx-1][by]==mazeStatus::visitedRoad&&maze[bx+1][by]==mazeStatus::road){
        linkedRoom.x=bx+1;
        linkedRoom.y=by;
        return true;
    }
    //up ==1 down ==3
    if(maze[bx][by+1]==mazeStatus::road&&maze[bx][by-1]==mazeStatus::visitedRoad){
        linkedRoom.x=bx;
        linkedRoom.y=by+1;
        return true;
    }
    //up ==3 down==1
    if(maze[bx][by+1]==mazeStatus::visitedRoad&&maze[bx][by-1]==mazeStatus::road){
        linkedRoom.x=bx;
        linkedRoom.y=by-1;
        return true;
    }
    else return false;

}
void Maze::generate(){
    //初始化maze
    for(int i=0;i<height;++i)
        for(int j=0;j<width;++j)
            if(i%2&&j%2)
                maze[j][i]=mazeStatus::road;
            else
                maze[j][i]=mazeStatus::wall;
    blues.clear();
    //get random
    std::default_random_engine random(time(nullptr));
    const int n=(width-1)/2;
    std::uniform_int_distribution<int> dist(0, n-1);
    //生成迷宫的起始点
    int startX=2*dist(random)+1;
    int startY=height-2;
    turnBlue(startX, startY);
    while(!blues.empty()){
        srand((unsigned)time(nullptr));
        std::uniform_int_distribution<int> dist2(0, blues.size()-1);
        int choosed=dist2(random);//rand()%blues.size();
        //        cout<<"choosed:"<<choosed<<"size:"<<blues.size()<<endl;
        Point choosedBlue=blues.at(choosed);

        //如果该蓝色块联通着两个房间,把该蓝色块变为3(已访问),然后我们把为1的房间标记为3已访问的房间,再把该房间周围的0块变成蓝色的2块
        Point linkedRoom;
        if(goodBlue(choosedBlue.x, choosedBlue.y,linkedRoom)){
            maze[choosedBlue.x][choosedBlue.y]=mazeStatus::visitedRoad;
            maze[linkedRoom.x][linkedRoom.y]=mazeStatus::visitedRoad;
            turnBlue(linkedRoom.x, linkedRoom.y);

        }else{//如果该蓝色块没有联通任何两个房间,则直接把这个蓝色块变成墙壁0
            maze[choosedBlue.x][choosedBlue.y]=mazeStatus::wall;
        }

        //完成操作,将这个随机选择的蓝色块删除
        for(auto k=blues.begin();k!=blues.end();++k){
            if(*k==choosedBlue){
                blues.erase(k);
                break;
            }
        }

    }
    startPoint.x=startPoint.y=endPoint.y=0;
    endPoint.x=width-1;
    //设置迷宫的起点
    for(;;){
        srand((unsigned)time(nullptr));
        int i=rand()%height;
        if(maze[1][i]==mazeStatus::visitedRoad){
            startPoint.y=i;
            maze[0][i]=mazeStatus::visitedRoad;
            break;
        }
    }
    //顺便把当前的位置设为起点
    currentPoint=startPoint;
    //设置迷宫的终点
    for(;;){
        srand((unsigned)time(nullptr));
        int i=rand()%height;
        if(maze[width-2][i]==mazeStatus::visitedRoad){
            endPoint.y=i;
            maze[width-1][i]=mazeStatus::visitedRoad;
            break;
        }
    }
}
void Maze::showMazeMatrix(){
    for(int i=0;i<height;++i){
        for(int j=0;j<width;++j)
            if(maze[j][i]==mazeStatus::visitedRoad)cout<<" ";else cout<<"X";
        //cout<<maze[j][i];
        cout<<endl;
    }
}
void Maze::generate(int w, int h){
    width=w;
    height=h;
    generate();
}
void Maze::findPath(){
    //开始自动寻路功能
    currentToEndPath.clear();
    vector<node> visited;
    vector<node> candidate;
    mapToList map[width][height];
    for(int i=0;i<height;++i){
        for(int j=0;j<width;++j)
            map[j][i].status=mapStatus::unreached;
    }
    node temp;//公用临时变量
//    map[currentPoint.x][currentPoint.y].status=mapStatus::inVisitedList;
//    temp.location=currentPoint;
//    temp.startToMe=0;
//    temp.totalCost=Point::distance(currentPoint, endPoint);
//    temp.meToEnd=temp.totalCost;
//    temp.parent=currentPoint;
//    visited.push_back(temp);

    map[currentPoint.x][currentPoint.y].status=mapStatus::inCandidateList;
    temp.parent=currentPoint;
    temp.startToMe=0;
    temp.meToEnd=Point::distance(currentPoint, endPoint);
    temp.totalCost=temp.meToEnd;
    temp.location=currentPoint;
    candidate.push_back(temp);

    for(;;){
        int currentIndex=node::getMinNode(candidate);
        Point current=candidate.at(currentIndex).location;
        if(current==endPoint)break;
        //下面开始搜索minIndex所在位置四周的点
        //上
        //如果是墙壁或者已经访问了则不访问
        if(!(current.y-1<0||maze[current.x][current.y-1]==mazeStatus::wall||map[current.x][current.y-1].status==mapStatus::inVisitedList)){
            //如果不在任何一个表中的话,把他加进待定表中
            if(map[current.x][current.y-1].status==mapStatus::unreached){
                map[current.x][current.y-1].status=mapStatus::inCandidateList;
                temp.location=Point(current.x,current.y-1);
                temp.parent=current;
                temp.startToMe=candidate.at(currentIndex).startToMe+1;
                temp.meToEnd=Point::distance(temp.location, endPoint);
                temp.totalCost=temp.startToMe+temp.meToEnd;
                candidate.push_back(temp);
                map[current.x][current.y-1].status=mapStatus::inCandidateList;
                map[current.x][current.y-1].index=candidate.size()-1;
            }else{//如果在的话,更新该节点的数据
                int nearIndex=map[current.x][current.y-1].index;
                if(candidate.at(nearIndex).startToMe>candidate.at(currentIndex).startToMe+1){
                    candidate.at(nearIndex).startToMe=candidate.at(currentIndex).startToMe+1;
                    candidate.at(nearIndex).totalCost=
                    candidate.at(nearIndex).startToMe+candidate.at(nearIndex).meToEnd;
                    candidate.at(nearIndex).parent=current;
                }
            }
        }
        //下
        //如果是墙壁或者已经访问了则不访问
        if(!(current.y+1>=height||maze[current.x][current.y+1]==mazeStatus::wall||map[current.x][current.y+1].status==mapStatus::inVisitedList)){
            //如果不在任何一个表中的话,把他加进待定表中
            if(map[current.x][current.y+1].status==mapStatus::unreached){
                map[current.x][current.y+1].status=mapStatus::inCandidateList;
                temp.location=Point(current.x,current.y+1);
                temp.parent=current;
                temp.startToMe=candidate.at(currentIndex).startToMe+1;
                temp.meToEnd=Point::distance(temp.location, endPoint);
                temp.totalCost=temp.startToMe+temp.meToEnd;
                candidate.push_back(temp);
                map[current.x][current.y+1].status=mapStatus::inCandidateList;
                map[current.x][current.y+1].index=candidate.size()-1;
            }else{//如果在的话,更新该节点的数据
                int nearIndex=map[current.x][current.y+1].index;
                if(candidate.at(nearIndex).startToMe>candidate.at(currentIndex).startToMe+1){
                    candidate.at(nearIndex).startToMe=candidate.at(currentIndex).startToMe+1;
                    candidate.at(nearIndex).totalCost=
                    candidate.at(nearIndex).startToMe+candidate.at(nearIndex).meToEnd;
                    candidate.at(nearIndex).parent=current;
                }
            }
        }
        //左
        //如果是墙壁或者已经访问了则不访问
        if(!(current.x-1<0||maze[current.x-1][current.y]==mazeStatus::wall||map[current.x-1][current.y].status==mapStatus::inVisitedList)){
            //如果不在任何一个表中的话,把他加进待定表中
            if(map[current.x-1][current.y].status==mapStatus::unreached){
                map[current.x-1][current.y].status=mapStatus::inCandidateList;
                temp.location=Point(current.x-1,current.y);
                temp.parent=current;
                temp.startToMe=candidate.at(currentIndex).startToMe;
                temp.meToEnd=Point::distance(temp.location, endPoint);
                temp.totalCost=temp.startToMe+temp.meToEnd;
                candidate.push_back(temp);
                map[current.x-1][current.y].status=mapStatus::inCandidateList;
                map[current.x-1][current.y].index=candidate.size()-1;
            }else{//如果在的话,更新该节点的数据
                int nearIndex=map[current.x-1][current.y].index;
                if(candidate.at(nearIndex).startToMe>candidate.at(currentIndex).startToMe){
                    candidate.at(nearIndex).startToMe=candidate.at(currentIndex).startToMe;
                    candidate.at(nearIndex).totalCost=
                    candidate.at(nearIndex).startToMe+candidate.at(nearIndex).meToEnd;
                    candidate.at(nearIndex).parent=current;
                }
            }
        }
        //右
        //如果是墙壁或者已经访问了则不访问
        if(!(current.x+1>=width||maze[current.x+1][current.y]==mazeStatus::wall||map[current.x+1][current.y].status==mapStatus::inVisitedList)){
            //如果不在任何一个表中的话,把他加进待定表中
            if(map[current.x+1][current.y].status==mapStatus::unreached){
                map[current.x+1][current.y].status=mapStatus::inCandidateList;
                temp.location=Point(current.x+1,current.y);
                temp.parent=current;
                temp.startToMe=candidate.at(currentIndex).startToMe;
                temp.meToEnd=Point::distance(temp.location, endPoint);
                temp.totalCost=temp.startToMe+temp.meToEnd;
                candidate.push_back(temp);
                map[current.x+1][current.y].status=mapStatus::inCandidateList;
                map[current.x+1][current.y].index=candidate.size()-1;
            }else{//如果在的话,更新该节点的数据
                int nearIndex=map[current.x+1][current.y].index;
                if(candidate.at(nearIndex).startToMe>candidate.at(currentIndex).startToMe){
                    candidate.at(nearIndex).startToMe=candidate.at(currentIndex).startToMe;
                    candidate.at(nearIndex).totalCost=
                    candidate.at(nearIndex).startToMe+candidate.at(nearIndex).meToEnd;
                    candidate.at(nearIndex).parent=current;
                }
            }
        }
        //访问完毕,更新candidate表的索引值,

        //把当前节点加入visited表中
        vector<node>::iterator minP=candidate.begin()+currentIndex;
        map[minP->location.x][minP->location.y].status=mapStatus::inVisitedList;
        visited.push_back(*minP);
        map[minP->location.x][minP->location.y].index=visited.size()-1;

        //从candidate表中移除选择的元素,然后更新candidate表的索引
        candidate.erase(minP);
        for(int i=0;i<candidate.size();++i){
            map[candidate.at(i).location.x][candidate.at(i).location.y].index=i;
        }
    }

    //////////////////////////////
    Point travel=visited.at(visited.size()-1).location;
    Point lastTravel=endPoint;
    while(travel!=currentPoint){
        #ifdef MAZE_DEBUG
        //For Debug Use
        if(lastTravel.x==travel.x)
            maze[travel.x][travel.y]='|';
        else
            maze[travel.x][travel.y]='-';
        #endif
        currentToEndPath.push_back(travel);
        lastTravel=travel;
        travel=visited.at(map[travel.x][travel.y].index).parent;
    }

    //显示带通道的迷宫
    #ifdef MAZE_DEBUG
    //For Debug Use
    cout<<endl<<endl;
    for(int i=0;i<height;++i){
        for(int j=0;j<width;++j)
            if(maze[j][i]==mazeStatus::visitedRoad)cout<<" ";
            else if(maze[j][i]==mazeStatus::wall) cout<<"X";
            else cout<<(char)maze[j][i];

        //cout<<maze[j][i];
        cout<<endl;
    }
    #endif
}

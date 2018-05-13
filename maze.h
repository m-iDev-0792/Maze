#ifndef MAZE_H
#define MAZE_H


//
//  main.cpp
//  mazeClass
//
//  Created by 何振邦 on 2017/10/1.
//  Copyright © 2017年 何振邦. All rights reserved.
//

#include <iostream>
#include <vector>
#include <random>
//#define MAZE_DEBUG
using namespace std;
//Point类
class Point{
public:
    int x;
    int y;
    Point(int xx,int yy):x(xx),y(yy){}
    Point(){};
    bool operator == (Point &p){
        return (x==p.x)&&(y==p.y);
    }
    bool operator !=(Point &p){
        return !(*this==p);
    }
    Point& operator = (const Point &p){

        x=p.x;
        y=p.y;
        return *this;
    }
    static int distance(Point p1,Point p2){
        return abs(p1.x-p2.x)+abs(p1.y-p2.y);
    }
};


//用于A*寻路算法中储存各节点的信息
class node{
public:
    Point parent;
    int startToMe;
    int meToEnd;
    int totalCost;
    Point location;
    node()=default;
    node(Point p,int s,int e,int t,Point l):parent(p),startToMe(s),meToEnd(e),totalCost(t),location(l){}
    static int getMinNode(vector<node>& v){
        int min=INT_MAX;
        int index;
        for(int i=0;i<v.size();++i){
            if(v.at(i).totalCost<min){
                min=v.at(i).totalCost;
                index=i;
            }
        }
        return index;
    }
};
enum mapStatus{
    unreached,
    inCandidateList,
    inVisitedList
};
//用于A*寻路算法 记录坐标到表的映射
class mapToList{
public:
    int index;//在表中的位置
    int status;//在哪个表中? 0 什么表都不在 1在待定表中 2在已访问表中
};

enum mazeStatus{
    wall,
    road,//初始的room(红色)
    standbyArea,//蓝色块
    visitedRoad//道路(红色)
};
//主体---迷宫类
class Maze{
public:
    //公用
    int width;//  \
              //   >这两个必须是奇数
    int height;// /
    int maze[51][51];
    void generate();
    void generate(int w,int h);
    void findPath();
    Point startPoint;
    Point endPoint;
    Point currentPoint;
    vector<Point> currentToEndPath;
    void showMazeMatrix();
    //创建迷宫用
    vector<Point> blues;
    Maze(){
      generate(21,15);
    }

    Maze(int w,int h){
      generate(w,h);
    }
private:
    void turnBlue(int x,int y);
    bool goodBlue(int bx,int by,Point &linkedRoom);
};

#endif // MAZE_H

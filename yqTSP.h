#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<math.h>
#include<float.h>
#define N 108/*定义城市数，城市标识为0到N-1*/
#define Ant 50/*蚂蚁数*/
#define arf 1/*信息素的重要程度*/
#define beta 5/*启发因子的重要程度*/
#define Q 100/*信息素更新中的常数Q*/
#define N_MAX 150/*迭代的最大次数*/
int jinjishu=0;/*城市之间不可通行的道路数量*/
double rou=0.8;/*信息素的挥发因子*/


double citycoordinate[N][2];/*储存城市坐标*/
double  Sbest=DBL_MAX;/*记录全局最优解（距离），初值设置为double值的MAX*/
double Antbest=DBL_MAX;/*记录循环中的最短距离*/
int Waybest[N]; /*最优路线*/ 
double Pheromone[N][N];/*信息素矩阵*/
double Distance_Matrix[N][N];/*城市间距离矩阵*/
double antdistance;/*储存每只蚂蚁的路径长度*/
double dertaPheromone[N][N];
int Initialize_distance_matrix(FILE*);
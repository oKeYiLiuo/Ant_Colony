#include"yqTSP.h"

void Initialize_pheromone(void);
void MOVE(void);
void Update_pheromone(void);
void OutputToFile(void);/*打印数据函数，将最终路径坐标打印*/


int Initialize_distance_matrix(FILE* fp)
{
    int j = 0;
    while(!feof(fp))   //读文件，直到文件末尾
    {
        fscanf(fp, "%lf %lf", &citycoordinate[j][0],&citycoordinate[j][1]);  //将文件内容附给flag
        j++;
    }
    if (j != N)
    {
        printf("citys numbers %d != %d\n please check file. ", j, N);
        return 0;
    }
    else{
        int jinji[N][N]={0};
        printf("Are there any cities that are not passable? Enter 1 to indicate presence, otherwise enter 0:");
        int jinjibiaoshi=0;/*城市之间是否可以同行，0代表可以，1代表不行*/
        scanf("%d",&jinjibiaoshi);
        getchar();
        while(jinjibiaoshi)
        {
            int i,j;
            printf("Please enter a non passable city label(0-%d). Such as 1 2, if 2 1 is not passable, you need to enter 2 1 again:",N-1);
            scanf("%d %d",&i,&j);
            getchar;
            jinji[i][j]=1;
            jinjishu++;
            printf("Are there any cities that are not passable? Enter 1 to indicate presence, otherwise enter 0 to exit:");
            scanf("%d",&jinjibiaoshi);
            getchar();
        }
        double distance;
        for(int i=0;i<N;i++)/*计算城市i到j的距离*/
        {
            for(int j=0;j<N;j++)
            {
                if(!jinji[i][j])
                {
                distance=sqrt(pow(citycoordinate[i][0]-citycoordinate[j][0],2)+pow(citycoordinate[i][1]-citycoordinate[j][1],2));
                Distance_Matrix[i][j]=distance;
                }
                else Distance_Matrix[i][j]=0.0;
            }
        }
        fclose(fp);

        return 1;
    }
    
    
}

void Initialize_pheromone(void)
{
    int ways=N*(N-1)-jinjishu;
    for(int i=0;i<N;i++)
    {
        for(int j=0;j<N;j++)
        {
            if(Distance_Matrix[i][j])
            Pheromone[i][j]=(double)(1.0)/(ways);
        }
    }
}

void MOVE(void)/*蚂蚁的移动函数*/
{
        srand((unsigned)time(NULL)+(unsigned)rand());
        int suijishucity=rand()%N;
        int city[N]={0};/*禁忌表，标识城市有没有走过，0代表没走过，1代表走过*/
        int Now=suijishucity;/*起始（当前）城市标志*/
        int antway[N]={antway[0]=Now};/*蚂蚁路径记录表*/
        int di=1;/*已经走了几个城市*/
        antdistance=0.0;/*初始化全局变量距离为0*/
        Antbest=DBL_MAX;/*初始化全局变量为最大值，以保证存储的是每轮迭代期间的最优路径长度*/
        do/*求整个路径*/
        {   if(di==(N))
            {
                city[suijishucity]=1;
                antdistance=antdistance+Distance_Matrix[antway[N-1]][suijishucity];
            }
            else
            {
            int Next=0;/*城市标志,从0开始遍历*/
            double total=0;/*轮盘赌信息素的总和，用于做节点选择公式的分母*/
            double lunpan[N];/*轮盘赌工具*/
            int panbiaoshi=0;/*轮盘标识*/
            int PCSBS[N];/*轮盘赌城市标识*/
            while (Next<N)/*遍历所有城市,寻找可能的城市，可能的城市数量为panbiaoshi*/
            {
                if((!city[Next]&&Distance_Matrix[Now][Next])&&(Next!=suijishucity))/*如果第next个城市可以被选择，没到过且可以到的且没有走遍所有城市时不允许回到起始点城市标志（距离不为0）*/
                {
                    total+=pow(Pheromone[Now][Next],arf)*pow((1/Distance_Matrix[Now][Next]),beta);
                    lunpan[panbiaoshi]=total;
                    PCSBS[panbiaoshi]=Next;
                    panbiaoshi++;
                }
                Next++;
            }
            for(int k=0;k<panbiaoshi;k++)
            {
                lunpan[k]=lunpan[k]/total;
            }
            srand((unsigned)time(NULL)+(unsigned)rand());
            double suijishu=1.0*rand()/RAND_MAX;
            for(int kk=0,bb=1;kk<panbiaoshi&&bb;kk++)/*寻找下一步要走的城市pcsbs[kk]*/
            {
                if(suijishu<=lunpan[kk])
                {
                    antway[di]=PCSBS[kk];
                    antdistance=antdistance+Distance_Matrix[Now][PCSBS[kk]];
                    bb=0;
                    Now=PCSBS[kk];
                    city[PCSBS[kk]]=1;
                    di++;
                    
                }
            }
            }            
        } while (!city[suijishucity]);
        for(int antwayPheromone=0;antwayPheromone<N-1;antwayPheromone++)/*记录每只蚂蚁走过路径的信息素，加到全局变量derta中，用于全部蚂蚁行走完以后更新信息素*/
        {
            dertaPheromone[antway[antwayPheromone]][antway[antwayPheromone+1]]=dertaPheromone[antway[antwayPheromone]][antway[antwayPheromone+1]]+rou*Q/antdistance;
        }
        dertaPheromone[antway[N-1]][antway[0]]=dertaPheromone[antway[N-1]][antway[0]]+rou*Q/antdistance;
        if(antdistance<Antbest)/*更新当前迭代期间的最优路径*/
        {
            Antbest=antdistance;
        }
        if(antdistance<Sbest)/*更新最优解*/
        {
            for(int ss=0;ss<N;ss++)
            {
                Waybest[ss]=antway[ss];/*由于蚂蚁路线是局部变量，因此要在此时更新最优路线*/
            }
        }
        for(int i=0;i<N-1;i++)/*局部动态更新信息素*/
        {
            Pheromone[antway[i]][antway[i+1]]=(1-rou)*Pheromone[antway[i]][antway[i+1]]+rou*dertaPheromone[antway[i]][antway[i+1]];
        }
            Pheromone[antway[N-1]][antway[0]]=(1-rou)*Pheromone[antway[N-1]][antway[0]]+rou*dertaPheromone[antway[N-1]][antway[0]];
        
}

void Update_pheromone(void)/*所有蚂蚁行动结束后进行全局动态更新信息素，已知最优路线Waybest[N]以及当前信息素矩阵*/
{
    for(int zuiyouPheromone=0;zuiyouPheromone<N-1;zuiyouPheromone++)
    {
        Pheromone[Waybest[zuiyouPheromone]][Waybest[zuiyouPheromone+1]]=(1-rou)*Pheromone[Waybest[zuiyouPheromone]][Waybest[zuiyouPheromone+1]]+rou*(Antbest-Sbest)/Sbest;
    }
    Pheromone[Waybest[N-1]][Waybest[0]]=(1-rou)*Pheromone[Waybest[N-1]][Waybest[0]]+rou*(Antbest-Sbest)/Sbest;
    if(Antbest<Sbest)/*每轮迭代结束后更新全局最优路径长度*/
    {
        Sbest=Antbest;
    }
}

void OutputToFile(void)/*打印函数，将结果打印*/
{
    double x[N];
    double y[N];
    for(int i=0;i<N;i++)
    {
        x[i]=citycoordinate[Waybest[i]][0];
    }
    for(int i=0;i<N;i++)
    {
        y[i]=citycoordinate[Waybest[i]][1];
    }
    FILE *fp = NULL ;
	fp = fopen("result.xlsx","a");
    for (int i=0 ;i<N ;i++)
		fprintf(fp,"%d\t%f\t%f\n",Waybest[i],x[i],y[i] ) ;
	fclose(fp);
}
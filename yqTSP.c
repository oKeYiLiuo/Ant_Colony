#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<math.h>
#include<float.h>
#define N 108/*��������������б�ʶΪ0��N-1*/
#define Ant 50/*������*/
#define arf 1/*��Ϣ�ص���Ҫ�̶�*/
#define beta 5/*�������ӵ���Ҫ�̶�*/
#define Q 100/*��Ϣ�ظ����еĳ���Q*/
#define N_MAX 150/*������������*/
int jinjishu=0;/*����֮�䲻��ͨ�еĵ�·����*/
double rou=0.8;/*��Ϣ�صĻӷ�����*/


double citycoordinate[N][2];/*�����������*/
double  Sbest=DBL_MAX;/*��¼ȫ�����Ž⣨���룩����ֵ����Ϊdoubleֵ��MAX*/
double Antbest=DBL_MAX;/*��¼ѭ���е���̾���*/
int Waybest[N]; /*����·��*/ 
double Pheromone[N][N];/*��Ϣ�ؾ���*/
double Distance_Matrix[N][N];/*���м�������*/
double antdistance;/*����ÿֻ���ϵ�·������*/
double dertaPheromone[N][N];

int Initialize_distance_matrix(FILE*);
void Initialize_pheromone(void);
void MOVE(void);
void Update_pheromone(void);
void OutputToFile(void);/*��ӡ���ݺ�����������·�������ӡ*/

int main(int argc , char* argv[])
{
    char* file_name = argv[1];
    FILE* fp = fopen(file_name, "r");
    if(fp == NULL){
        printf("we can not open file!\n");
        return 0;
    }
    int sucess = Initialize_distance_matrix(fp);
    if (!sucess)
    {
        return 0;
    }
    
    printf("Number of cities is %d, label is 0-%d\n",N,N-1);
    Initialize_pheromone();
    printf("Let's we do!\n");
    for(int xunhuan=1;xunhuan<N_MAX;xunhuan++)
    {
        for(int ant=1;ant<=Ant;ant++)
        {
            MOVE();
        }
        Update_pheromone();
        for(int i=0;i<N;i++)/*������ÿ�ε������dertaPheromone����Ϊ0*/
        {
            for(int j=0;j<N;j++)
            {
                dertaPheromone[i][j]=0.0;
            }
        }
        
    }
    printf("over best way:");
    for(int i=0;i<N;i++)
    {
        if (i != N -1)
        {
            printf("%d-",Waybest[i]);
        }
        else
            printf("%d",Waybest[i]);
        
       
    }
    printf("\nbest distance:%f",Sbest);
    OutputToFile();

    return 0;
}

int Initialize_distance_matrix(FILE* fp)
{
    int j = 0;
    while(!feof(fp))   //���ļ���ֱ���ļ�ĩβ
    {
        fscanf(fp, "%lf %lf", &citycoordinate[j][0],&citycoordinate[j][1]);  //���ļ����ݸ���flag
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
        int jinjibiaoshi=0;/*����֮���Ƿ����ͬ�У�0������ԣ�1������*/
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
        for(int i=0;i<N;i++)/*�������i��j�ľ���*/
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

void MOVE(void)/*���ϵ��ƶ�����*/
{
        srand((unsigned)time(NULL)+(unsigned)rand());
        int suijishucity=rand()%N;
        int city[N]={0};/*���ɱ���ʶ������û���߹���0����û�߹���1�����߹�*/
        int Now=suijishucity;/*��ʼ����ǰ�����б�־*/
        int antway[N]={antway[0]=Now};/*����·����¼��*/
        int di=1;/*�Ѿ����˼�������*/
        antdistance=0.0;/*��ʼ��ȫ�ֱ�������Ϊ0*/
        Antbest=DBL_MAX;/*��ʼ��ȫ�ֱ���Ϊ���ֵ���Ա�֤�洢����ÿ�ֵ����ڼ������·������*/
        do/*������·��*/
        {   if(di==(N))
            {
                city[suijishucity]=1;
                antdistance=antdistance+Distance_Matrix[antway[N-1]][suijishucity];
            }
            else
            {
            int Next=0;/*���б�־,��0��ʼ����*/
            double total=0;/*���̶���Ϣ�ص��ܺͣ��������ڵ�ѡ��ʽ�ķ�ĸ*/
            double lunpan[N];/*���̶Ĺ���*/
            int panbiaoshi=0;/*���̱�ʶ*/
            int PCSBS[N];/*���̶ĳ��б�ʶ*/
            while (Next<N)/*�������г���,Ѱ�ҿ��ܵĳ��У����ܵĳ�������Ϊpanbiaoshi*/
            {
                if((!city[Next]&&Distance_Matrix[Now][Next])&&(Next!=suijishucity))/*�����next�����п��Ա�ѡ��û�����ҿ��Ե�����û���߱����г���ʱ������ص���ʼ����б�־�����벻Ϊ0��*/
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
            for(int kk=0,bb=1;kk<panbiaoshi&&bb;kk++)/*Ѱ����һ��Ҫ�ߵĳ���pcsbs[kk]*/
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
        for(int antwayPheromone=0;antwayPheromone<N-1;antwayPheromone++)/*��¼ÿֻ�����߹�·������Ϣ�أ��ӵ�ȫ�ֱ���derta�У�����ȫ�������������Ժ������Ϣ��*/
        {
            dertaPheromone[antway[antwayPheromone]][antway[antwayPheromone+1]]=dertaPheromone[antway[antwayPheromone]][antway[antwayPheromone+1]]+rou*Q/antdistance;
        }
        dertaPheromone[antway[N-1]][antway[0]]=dertaPheromone[antway[N-1]][antway[0]]+rou*Q/antdistance;
        if(antdistance<Antbest)/*���µ�ǰ�����ڼ������·��*/
        {
            Antbest=antdistance;
        }
        if(antdistance<Sbest)/*�������Ž�*/
        {
            for(int ss=0;ss<N;ss++)
            {
                Waybest[ss]=antway[ss];/*��������·���Ǿֲ����������Ҫ�ڴ�ʱ��������·��*/
            }
        }
        for(int i=0;i<N-1;i++)/*�ֲ���̬������Ϣ��*/
        {
            Pheromone[antway[i]][antway[i+1]]=(1-rou)*Pheromone[antway[i]][antway[i+1]]+rou*dertaPheromone[antway[i]][antway[i+1]];
        }
            Pheromone[antway[N-1]][antway[0]]=(1-rou)*Pheromone[antway[N-1]][antway[0]]+rou*dertaPheromone[antway[N-1]][antway[0]];
        
}

void Update_pheromone(void)/*���������ж����������ȫ�ֶ�̬������Ϣ�أ���֪����·��Waybest[N]�Լ���ǰ��Ϣ�ؾ���*/
{
    for(int zuiyouPheromone=0;zuiyouPheromone<N-1;zuiyouPheromone++)
    {
        Pheromone[Waybest[zuiyouPheromone]][Waybest[zuiyouPheromone+1]]=(1-rou)*Pheromone[Waybest[zuiyouPheromone]][Waybest[zuiyouPheromone+1]]+rou*(Antbest-Sbest)/Sbest;
    }
    Pheromone[Waybest[N-1]][Waybest[0]]=(1-rou)*Pheromone[Waybest[N-1]][Waybest[0]]+rou*(Antbest-Sbest)/Sbest;
    if(Antbest<Sbest)/*ÿ�ֵ������������ȫ������·������*/
    {
        Sbest=Antbest;
    }
}

void OutputToFile(void)/*��ӡ�������������ӡ*/
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
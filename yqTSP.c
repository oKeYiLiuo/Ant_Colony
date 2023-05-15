#include"function.c"

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
        for(int i=0;i<N;i++)/*更新完每次迭代后的dertaPheromone矩阵为0*/
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
    printf("\nbest distance:%f\n",Sbest);
    OutputToFile();

    return 0;
}

/*************************************************************************
    > File Name: solt.c
    > Author: Yiwen Xue
    > Mail: 15225434259xue@gmail.com 
    > Created Time: Thu 11 Jul 2019 06:52:12 AM CST
*************************************************************************/

#include<stdio.h>
#include<time.h>
#include<stdlib.h>
#define random(x) (rand()%x)

double get_mid(double *data, int num){
    double *out;
    out = (double *)malloc(sizeof(double)*num);
    for(int i=0;i<num;i++){
        out[i] = data[i];
    }
    double temp =0;
    for(int i=0;i<num;i++){
        for(int j=0;j<i;j++){
            if(out[j] > out[i]){
                temp = out[i]; 
                out[i] = out[j];
                out[j] = temp;
            }
        }
    }
    double mid;
    if(num%2==0){
        mid = out[num/2] + out[num/2-1];
        mid /= 2.0;
    }else{
        mid = out[(num-1)/2];
    }
    free(out);
    return mid;
}




int main(int argc,char **argv){
    srand((int)time(0));
    int num = atoi(argv[1]);
    double *data;
    data = (double*)malloc(sizeof(double)*num);
    for(int i=0;i<num;i++){
        data[i] =( (double)random(10000) )/100.0; 
    }
    double mid = get_mid(data,num);
    for (int i=0;i<num;i++){
        printf("%d-> %.3f\n",i,data[i]);
    }
    printf("mid = %.3f\n",mid);
    free(data);
    return 0;
}

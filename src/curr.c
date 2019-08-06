/*************************************************************************
    > File Name: curr.c
    > Author: Yiwen Xue
    > Mail: 15225434259xue@gmail.com 
    > Created Time: Mon 22 Jul 2019 01:19:12 PM CST
*************************************************************************/

#include<stdio.h>
#include<stdlib.h>


int main(){
    FILE *in;
    in = fopen("temp1","rw");
    double * store;
    store = (double*)malloc(sizeof(double)*1000);
    for(int i=0;i<1000;i++){
        fscanf(in,"%lf,",&store[i]);
    }
    fclose(in);
    in = fopen("temp1","w");
    for(int i=0;i<1000;i++){
        fprintf(in,"%lf\n",store[i]);
    }
    fclose(in);
    free(store);

    return 0;}

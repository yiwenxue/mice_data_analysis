/*************************************************************************
    > File Name: ststic.c
    > Author: Yiwen Xue
    > Mail: 15225434259xue@gmail.com 
    > Created Time: Wed 31 Jul 2019 08:59:15 AM CST
*************************************************************************/

#include<stdio.h>
#include<malloc.h>

int main(){
    FILE *stream = fopen("/tmp/mice_sync","r");
    //Count the totla lines;
    char chr;
    int line =0;
    chr = fgetc(stream);
    while(chr != EOF){
        if(chr == '\n')
            line ++;
        chr = fgetc(stream);
    }
    fclose(stream);

    stream = fopen("/tmp/mice_sync","r");

    double *phas = (double *)malloc(sizeof(double)*line);
    double temp;
    for(int i=0;i<line;i++){
        fscanf(stream,"%*lf\t%*lf\t%*lf\t%*lf\t%*lf\t%*lf\t%*lf\t%*lf\t%lf\n",&phas[i]);
    }
    fclose(stream);

    for(int i=0;i<line;i++){
        printf("%d\t%f\n",i,phas[i]);
    }




    return 0;
}

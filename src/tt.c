/*************************************************************************
    > File Name: tt.c
    > Author: Yiwen Xue
    > Mail: 15225434259xue@gmail.com 
    > Created Time: Tue 30 Jul 2019 03:56:54 PM CST
*************************************************************************/

#include<stdio.h>

int main(int argc, char **argv){
    FILE *stream;
    double d1[1000],d2[1000];
    stream = fopen(argv[1],"r");
    if(stream == NULL)
        fprintf(stderr,"can't open");
    for(int i=0;i<1000;i++){
        fscanf(stream,"%lf ",&d1[i]);
        printf("%lf\n",d1[i]);
    }
    /* fgetc(stream); */
    /* for(int i=0;i<1000;i++) */
    /*     fscanf(stream,"%lf,",&d2[i]); */
    printf("hello");
    fclose(stream);
    stream = fopen(argv[1],"w");
    for(int i=0;i<1000;i++){
        fprintf(stream,"%d\t%lf\n",i,d1[i]);
    }
    fclose(stream);
    return 0;
}

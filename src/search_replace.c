/*************************************************************************
    > File Name: search_replace.c
    > Author: Yiwen Xue
    > Mail: 15225434259xue@gmail.com 
    > Created Time: Thu 18 Jul 2019 05:37:25 PM CST
*************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

FILE *fstream;
char stream[1024];
char replace[255] = ;
char *inistal = "tag";

int one_replace(char *str){
    fstream = fopen("./temp.tex","rw");
    if(fstream == NULL){
        fprintf(stderr,"Error! File doesn't exist!\n");
        return -1;
    }
    fgets();
}

int main(int argc, char **argv){

    fclose(fstream);
    return 0;
}

/*************************************************************************
    > File Name: file.c
    > Author: Yiwen Xue
    > Mail: 15225434259xue@gmail.com 
    > Created Time: Thu 11 Apr 2019 11:35:48 AM IDT
*************************************************************************/

#include "file.h"

int setmem_double(double *data,int size, double def){
    for (int i = 0; i < size; i ++){
        data[i] = def;
    }
    return 1;
}

int file_lines(char *ifname){
    FILE *input = fopen(ifname,"r");
    if(input == NULL){
        fprintf(stderr,"Failed to read %s\n",ifname);
        return 0;
    }

    //Count the totla lines;
    char chr;
    int line =0;
    chr = fgetc(input);
    while(chr != EOF){
        if(chr == '\n')
            line ++;
        chr = fgetc(input);
    }
    fclose(input);

    return line;
}

double* read_mice_file(char* ifname, int *line){

    //Count the totla lines;
    *line = file_lines(ifname);

    FILE* temp = fopen(ifname,"r");
    //Check if one can read this file;
    if(temp == NULL){
        fprintf(stderr,"Failed to read %s.",ifname);
        return NULL;
    }

    double *in_data;
    in_data = (double*)malloc(*line * sizeof(double));
    double tempx = 0, tempy = 0, tempxx = 0;
    for (int i=0;i<*line;i++){
        if (fscanf (temp,"%lf\t%lf\n",&tempx, &tempy) == 2 ){
            if((tempx-tempxx) > 100.0){
                *line -= i;
                i=0;
            }
            in_data[i] = tempy;
            tempxx = tempx;
        }
        else {
            fprintf(stderr,"File format error.\n");
            fclose(temp);
            return NULL;
        }
    }

    fclose(temp);
    return in_data;
}

double* single_colum_data(char *ifname,int *lines){
    //Count the total lines;
    *lines = file_lines (ifname);

    FILE *input = fopen(ifname,"r");
    if (input == NULL){
        fprintf (stderr,"Failed to read %s\n",ifname);
        return NULL;
    }

    double *data;
    data = (double*)malloc(sizeof(double)*(*lines));
    setmem_double(data,*lines,0.0);
    for (int i = 0; i < *lines; i++){
        fscanf(input,"%lf\n",&data[i]);
    }

    return data;
}


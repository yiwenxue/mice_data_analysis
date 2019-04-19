/*************************************************************************
    > File Name: file.c
    > Author: Yiwen Xue
    > Mail: 15225434259xue@gmail.com 
    > Created Time: Thu 11 Apr 2019 11:35:48 AM IDT
*************************************************************************/

#include "file.h"

///* Not Finished */
//bool wrist_csv_split(char* filename)
//{
//    FILE fstream;
//    input = fopen(filename,"r");
//    /* check if the file can be read */
//    if(input == NULL)
//        return false;
//    char buffer_line[BUFFER_SIZE];
//    /* This command will set the buffer line to a blank line; */
//    memset(buffer_line,'\0',BUFFER_SIZE);
//    while(getline(buffer_line,BUFFER_SIZE,fstream) != -1){
//        for();
//    }
//    fclose(fstream);
//
//    fstream = fopen();
//
//    /* get the important useful information from initial file */
//    
//    /* if every steps are normal, return 1 */ 
//    return true;
//}

double* read_mice_file(char* ifname, int *line){
    FILE* temp = fopen(ifname,"r");
    if(temp == NULL){
        fprintf(stderr,"Failed to read %s.",ifname);
        return NULL;
    }
    /* Used to find the number of lines in the data file, */ 
    /* which determines the memory size of array to allocate */
    char chr;
    *line =0;
    double *in_data;
    chr = fgetc(temp);
    while(chr != EOF){
        if(chr == '\n')
            *line = *line + 1;
        chr = fgetc(temp);
    }
    fclose(temp);
    
    temp = fopen(ifname,"r");
    in_data = (double*)malloc(*line * sizeof(double));
    for(int i=0;i<*line;i++){
        fscanf(temp,"%*lf\t%lf\n",&in_data[i]);
    }
    fclose(temp);
    return in_data;
}

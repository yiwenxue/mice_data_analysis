/*************************************************************************
    > File Name: main.c
    > Author: Yiwen Xue
    > Mail: 15225434259xue@gmail.com 
    > Created Time: Wed 17 Apr 2019 07:35:07 PM IDT
 *************************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <getopt.h>
#include "mathematic.h"
#include "plot.h"
#include "file.h"


/* This is a structure that stores the options and the following */ 
/* array of strings are the description to these options */
/* {option-name,required_argument,args,short} */
static struct option long_options[]={
    {"file",    required_argument,  0,  'f'},
    {"graph",   no_argument,        0,  'g'},
    {"dfa",     required_argument,  0,  'd'},
    {"dfas",    required_argument,  0,  'D'},
    {"mean-std",no_argument,        0,  'm'},
    {"size",    required_argument,  0,  's'},
    {"output",  required_argument,  0,  'o'},
    {0,         0,                  0,  0  }
};
char *desc[]={
    "Name of the file to be analysised, followed by a file",
    "Show a graph at the end or not",
    "Exec dfa, followed by dfa order.",
    "Exec dfa for each segment, followed by dfa order.",
    "Exec std/mean analysis",
    "Determint the dimensions of the graph",
    "Generate a pdf file for the graph, followed by a file",
    ""
};

void print_usage(void){
    printf("Usage: ./main [OPTION]... [ARGS]... \n");
    int i=0;
    while(1){
        if(long_options[i].name != NULL)
            printf("\t-%c, --%s\n\t\t%s\n",long_options[i].val,long_options[i].name,desc[i]);
        else 
            break;
        i++;
    }
}

void mice_dfa(int order, 
        double *data,
        int line
        )
{
    FILE* output;
    output = fopen("data","w");

    int step = 1000;
    double fit[2];
    double *fn,*n;
    int size = (int)(log10((double)step/5.0)*100);
    fn = (double*)malloc(sizeof(double)*size);
    n = (double*)malloc(sizeof(double)*size);
    double hurst,mean;
    line -= step;

    for(int i=0;i<=line;i+=step){
        mean = gsl_stats_mean(data+i,1,step);
        data[i] -= mean;
        for(int j=i+1;j<i+step;j++){
            data[j] -= mean;
            data[j] += data[j-1];
        }
        hurst = detrend_flucuation(order,data+i,step,fn,n,&size,fit);
        fprintf(output,"%d\t%lf\n",i,hurst);
    }
    free(fn);
    free(n);
    fclose(output);
}
void mice_dfas(char *filename, 
        double *fit, 
        int order, 
        double *data,
        int line
        )
{
    FILE* output;
    output = fopen("data","w");

    int step = 1000;
    double fit[2];
    double *fn,*n;
    int size = (int)(log10((double)step/5.0)*100);
    fn = (double*)malloc(sizeof(double)*size);
    n = (double*)malloc(sizeof(double)*size);
    double hurst,mean;
    line -= step;

    for(int i=0;i<=line;i+=step){
        mean = gsl_stats_mean(data+i,1,step);
        data[i] -= mean;
        for(int j=i+1;j<i+step;j++){
            data[j] -= mean;
            data[j] += data[j-1];
        }
        hurst = detrend_flucuation(order,data+i,step,fn,n,&size,fit);
        fprintf(output,"%d\t%lf\n",i,hurst);
    }
    free(fn);
    free(n);
    fclose(output);
}

void mice_mean_std(double *data,
        int line
        )
{
    int step = 600;
    FILE* output;
    output = fopen("data","w");
    double mean =0;
    double stddev =0;
    for(int i=0;i<=line;i+=step){
        if(i+step>line){
            mean = gsl_stats_mean(data+i,1,line - i);
            stddev= gsl_stats_sd(data+i,1,line - i);
        }else{
            mean = gsl_stats_mean(data+i,1,step);
            stddev= gsl_stats_sd(data+i,1,step);
        }
        fprintf(output,"%d\t%10lf\t%10lf\n",i,mean,stddev);
    }
    fclose(output);
}

int main(int argc,char** argv){
    int width, height; 
    int ifflag =0 , sizef=0 , graphf=0, dfaf=0, mean_stdf=0 ,outputf =0,dfasf =0; /*flags hahah*/
    char ifname[255], outname[255];
    int long_index = 0, opt = 0, order = 1;

    /* Read parameters from command line; */
    while((opt = getopt_long(argc,argv,"f:gd:D:ms:o:",long_options,&long_index)) != -1){
        switch(opt){
            case 'f': ifflag = 1;
                      strcpy(ifname,optarg);
                      break;
            case 'g': graphf = 1;
                      outputf= 0;
                      break;
            case 'd': dfaf = 1;
                      dfasf =0;
                      order = atoi(optarg);
                      break; 
            case 'D': dfasf = 1;
                      dfaf =0;
                      order = atoi(optarg);
                      break; 
            case 'm': mean_stdf = 1;
                      break;
            case 's': sizef = 1;
                      graphf = 1;
                      sscanf(optarg,"%dx%d",&width,&height);
                      break;
            case 'o': outputf =1;
                      graphf =0;
                      strcpy(outname,optarg);
            default: print_usage();
                     return -1;
                     break;
        }
    }

    if(argc == 1){
        print_usage();
        return -1;
    }
    /* Check if there is a data file, and readin data if true*/ 
    double* in_data ;
    int line;
    if(ifflag){
        in_data = read_mice_file(ifname,&line);
        if(line <= 0 )
            return -1;
    }else{
        fprintf(stderr,"Input file excepted.\n");
        return -1;
    }

    //for(int i=0;i<line;i++){
    //    printf("%d\t%3.4lf\n",i,in_data[i]);
    //}

    /* Default size of the plot; */
    if(!sizef){
        width = 600;
        height = 450;
    }

    /* Start to plot diagrams */ 
    if(graphf){
        if(dfaf){
            mice_dfa(order,in_data,line);
            plot_dfa(order,width,height);
        }else if(dfasf){
            double fit[2];
            mice_dfas(order,in_data,line);
            plot_dfas("data",fit,order,width,height);
        }
        if(mean_stdf){
            mice_mean_std(in_data,line);
            plot_std(width,height);
        }
    }else{
        if(dfaf)
            mice_dfa(order,in_data,line);
        else if(dfasf)
            mice_dfas();
        if(mean_stdf)
            mice_mean_std(in_data,line);
    }


    /* Free all alloced memories. */ 
    free(in_data);
    return 0;
}


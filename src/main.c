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
    {"mean",    no_argument,        0,  'm'},
    {"standard",no_argument,        0,  's'},
    {"size",    required_argument,  0,  'S'},
    {"output",  required_argument,  0,  'o'},
    {0,         0,                  0,  0  }
};
char *desc[]={
    "Name of the file to be analysised, followed by a file",
    "Show a graph at the end or not",
    "Exec dfa, followed by dfa order.",
    "Exec dfa for each segment, followed by dfa order.",
    "Exec mean analysis",
    "Exec standard analysis",
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
        double *fit, 
        int head
        )
{
    FILE* output;
    output = fopen("data","w");

    int step = 1200;
    double *fn,*n;
    int size = (int)(log10((double)step/5.0)*100);
    fn = (double*)malloc(sizeof(double)*size);
    n = (double*)malloc(sizeof(double)*size);
    double hurst,mean;

    mean = gsl_stats_mean(data+head,1,step);
    data[head] -= mean;
    for(int j=head+1;j<head+step;j++){
        data[j] -= mean;
        data[j] += data[j-1];
    }
    hurst = detrend_flucuation(order,data+head,step,fn,n,&size,fit);
    for(int i=0;i<size;i++){
        fprintf(output,"%.10f\t%.10f\n",n[i],log10(fn[i]));
    }

    free(fn);
    free(n);
    fclose(output);
}

void mice_dfas(int order, 
        double *data,
        int line
        )
{
    FILE* output;
    output = fopen("data","w");

    int step = 1200;
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

void mice_std(double *func,
        double *data,
        int line
        )
{
    int step = 1200;
    FILE* output;
    output = fopen("data","w");
    double stddev =0;

    int size = line /step +1;
    double *std;
    double *x; 
    std = (double *)malloc(sizeof(double)*size);
    x = (double *)malloc(sizeof(double)*size);

    int j=0;
    for(int i=0;i<=line;i+=step){
        if(i+step>line){
            stddev= stats_std(data+i,line - i);
        }else{
            stddev= stats_std(data+i,step);
        }
        fprintf(output,"%f\t%10lf\n",i*6.0/3600.0,stddev);
        x[j] = i/600.0*4*M_PI/30;
        std[j] = stddev;
        j++;
    }
    fit_sin4(func,x,std,size);
    free(x);
    free(std);
    fclose(output);
}

void mice_mean(double *func,
        double *data,
        int line
        )
{
    int step = 1200;
    FILE* output;
    output = fopen("data","w");
    double mean =0;

    int size = line /step +1;
    double *means;
    double *x; 
    means = (double *)malloc(sizeof(double)*size);
    x = (double *)malloc(sizeof(double)*size);

    int j=0;
    for(int i=0;i<=line;i+=step){
        if(i+step>line){
            mean = stats_mean(data+i,line - i);
        }else{
            mean = stats_mean(data+i,step);
        }
        fprintf(output,"%f\t%10lf\n",i*6.0/3600.0,mean);
        x[j] = i/600.0*2*M_PI/15;
        means[j] = mean;
        j++;
    }
    fit_sin4(func,x,means,size);
    free(x);
    free(means);
    fclose(output);
}

int main(int argc,char** argv){
    int width, height; 
    int ifflag =0 , sizef=0 , graphf=0, dfaf=0, meanf=0 ,stdf=0 ,outputf =0,dfasf =0; /*flags hahah*/
    char ifname[255], outname[255];
    int long_index = 0, opt = 0, order = 1;

    /* Read parameters from command line; */
    while((opt = getopt_long(argc,argv,"f:gd:D:msS:o:",long_options,&long_index)) != -1){
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
            case 'm': meanf = 1;
                      stdf = 0;
                      break;
            case 's': stdf = 1;
                      meanf = 0;
                      break;
            case 'S': sizef = 1;
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

    double fit[2];
    double func[4];
    /* Start to plot diagrams */ 
    if(graphf){
        if(dfaf){
            mice_dfa(order,in_data,fit,1000);
            plot_dfa("data",fit,order,width,height);
        }else if(dfasf){
            mice_dfas(order,in_data,line);
            plot_dfas("data",order,width,height);
        }
        if(stdf){
            mice_std(func,in_data,line);
            plot_std(func,"data",width,height);
        }else if(meanf){
            mice_mean(func,in_data,line);
            plot_mean(func,"data",width,height);
        }
    }else{
        if(dfaf){
            mice_dfa(order,in_data,fit,line);
        }else if(dfasf)
            mice_dfas(order,in_data,line);
        if(stdf){
            mice_std(func,in_data,line);
        }else if(meanf){
            mice_mean(func,in_data,line);
        }
    }


    /* Free all alloced memories. */ 
    free(in_data);
    return 0;
}


/*************************************************************************
    > File Name: test.c
    > Author: Yiwen Xue
    > Mail: 15225434259xue@gmail.com 
    > Created Time: Wed 17 Apr 2019 07:35:07 PM IDT
 *************************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <getopt.h>
#include "file.h"
#include "utility.c"

int divide_name(char *inname,char *path, char *name, char *type){
    char *tag;
    char *backup;
    backup = inname;
    //GET THE PATH TO THE FOLDER
    tag = strchr(backup,'/');
    while(tag != NULL){
        backup = tag+1;
        tag = strchr(backup,'/');
    }
    int i=0;
    while(inname+i < backup){
        path[i] = inname[i];
        i++;
    }

    //get the animal name
    tag = strchr(backup,'.');
    i=0;
    while(backup+i < tag){
        name[i] = backup[i];
        i++;
    }
    backup = tag+1;

    //GET THE TYPE;
    tag = strchr(backup,'.');
    if(tag == NULL){
        printf("END\n");
        return -1;
    }
    i=0;
    while(backup+i < tag){
        type[i] = backup[i];
        i++;
    }
    return 0;
}

/* This is a structure that stores the options and the following */ 
/* array of strings are the description to these options */
/* {option-name,required_argument,args,short} */
static struct option long_options[]={
    {"file",    required_argument,  0,  'f'}, //iffile;
    {"filetype",required_argument,  0,  'T'}, //iffile;
    {"dfa",     required_argument,  0,  'd'}, //dfa
    {"mean",    no_argument,        0,  'm'}, //mean
    {"std",     no_argument,        0,  's'}, //std
    {"graph-type",required_argument,0,  't'}, //TYPE of diagram
    {"checkp",  no_argument,        0,  'c'}, //check period
    {"duration",required_argument,  0,  'D'}, //Time duration
    {"dlength", required_argument,  0,  'L'}, //single dfa length
    {"startp",  required_argument,  0,  'P'}, //start point
    {"graph-size",required_argument,0,  'S'}, //size of the graph
    {"output",  required_argument,  0,  'o'},
    {0,         0,                  0,  0  }
};
char *desc[]={
    "Name of the file to be analysised, followed by a file name",
    "The type of graph. [ MICE_DATA | SINGLE_COLUM ]",
    "Exec dfa, followed by dfa order.",
    "Exec mean analysis",
    "Exec standard analysis",
    "The type of graph. [ GINDIVIDUAL | GAVERAGE ]",
    "Check if the period equals to 24h",
    "Set the duration of a single segment, 1 = 10sec (1080 = 3h)",
    "Set the whole length of individual dfa, 1 = 10sec",
    "Set the start point of individual dfa, 1 = 10sec",
    "Determint the dimensions of the graph",
    "Generate a pdf file for the graph, followed by a file name. If \n\tnot seted, show a gnuplot window",
    ""
};

void print_usage(void){
    printf("Usage: \n./main [OPTION]... [ARGS]... \n");
    int i=0;
    while(1){
        if(long_options[i].name != NULL)
            printf("\t-%c, --%s\n\t    %s\n",long_options[i].val,long_options[i].name,desc[i]);
        else 
            break;
        i++;
    }
}


int main(int argc,char** argv){
     
    int ifflag =0, iftypef =0, dfaf=0, meanf=0, stdf =0, gtypef =0, checkf =0,
        durationf =0, dlengthf =0, startpf =0, gsizef =0, outputf =0; /*flags hahah*/
    Gtype graphtype;
    Ftype filetype;

    //default values
    int order = 1, width = 600, height = 450, duration = 1080, dlength = 1080, startp = 0;

    char ifname[255], outname[255], gtype[255], iftype[255];

    /* Read parameters from command line; */
    int long_index = 0, opt = 0;
    while((opt = getopt_long(argc,argv,"f:T:d:mst:cD:L:P:S:o:",long_options,&long_index)) != -1){
        switch(opt){
            case 'f': ifflag = 1;
                      strcpy(ifname,optarg);
                      /* printf("ifflag = %d\tifname = %s\n",ifflag,ifname); */
                      break;
            case 'T': iftypef = 1;
                      strcpy(iftype,optarg);
                      /* printf("iftypef = %d\tiftype = %s\n",iftypef,iftype); */
                      break;
            case 'd': dfaf = 1;
                      meanf = 0;
                      stdf = 0;
                      checkf =0;
                      order = atoi(optarg);
                      /* printf("meanf = %d\tstdf = %d\tcheckf = %d\tdfaf = %d\tdfa_order = %d\n",meanf,stdf,checkf,dfaf,order); */
                      break; 
            case 'm': meanf = 1;
                      dfaf = 0;
                      stdf = 0;
                      checkf =0;
                      /* printf("meanf = %d\tstdf = %d\tcheckf = %d\tdfaf = %d\tdfa_order = %d\n",meanf,stdf,checkf,dfaf,order); */
                      break;
            case 's': stdf = 1;
                      dfaf = 0;
                      meanf = 0;
                      checkf =0;
                      /* printf("meanf = %d\tstdf = %d\tcheckf = %d\tdfaf = %d\tdfa_order = %d\n",meanf,stdf,checkf,dfaf,order); */
                      break;
            case 't': gtypef = 1;
                      strcpy(gtype,optarg);
                      /* printf("gtypef = %d\tgtype = %s\n",gtypef,gtype); */
                      break;
            case 'c': checkf = 1;
                      meanf = 0;
                      stdf = 0;
                      dfaf = 0;
                      /* printf("meanf = %d\tstdf = %d\tcheckf = %d\tdfaf = %d\tdfa_order = %d\n",meanf,stdf,checkf,dfaf,order); */
                      break;
            case 'D': durationf = 1;
                      duration = atoi(optarg);
                      /* printf("durationf = %d\tduration = %d\n",durationf,duration); */
                      break; 
            case 'L': dlengthf = 1;
                      dlength = atoi(optarg);
                      /* printf("dlengthf = %d\tdlength = %d\n",dlengthf,dlength); */
                      break; 
            case 'P': startpf = 1;
                      startp = atoi(optarg);
                      /* printf("startpf = %d\tstartp = %d\n",startpf,startp); */
                      break; 
            case 'S': gsizef = 1;
                      sscanf(optarg,"%dx%d",&width,&height);
                      /* printf("gsizef = %d\tsize = %dx%d\n",gsizef,width,height); */
                      break;
            case 'o': outputf =1;
                      strcpy(outname,optarg);
                      /* printf("outputf = %d\toutname = %s\n",outputf,outname); */
                      break;
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
   double *data; 
   int lines;


   //readin data with check process
   if(ifflag){
       if(iftypef){
           if(!strcmp(iftype,"MICE_DATA")){
               filetype = MICE_DATA;
               /* data = read_mice_file(ifname,&lines); */
           }else if(!strcmp(iftype,"SINGLE_COLUM")){
               filetype = SINGLE_COLUM;
               /* data = read_single_colum_data(ifname,&lines); */
           }else{
               fprintf(stderr,"File types: MICE_DATA | SINGLE_COLUM\n");
               return 0;
           }
       }else
           filetype = MICE_DATA;
   }else{
       fprintf(stderr,"An input data file is needed.\n");
       return 0;
   }

   //set graph type;
   if(gtypef){
       if(!strcmp(gtype,"GAVERAGE")){
           graphtype = GAVERAGE;
       }else if(!strcmp(gtype,"GINDIVIDUAL")){
           graphtype = GINDIVIDUAL;
       }else if(!strcmp(gtype,"GDEVIATION")){
           graphtype = GDEVIATION;
       }else{
           fprintf(stderr,"Graph type: GAVERAGE | GINDIVIDUAL | GDEVIATION\n");
           return 0;
       }
   }else
       graphtype = GAVERAGE ;

   if(duration % 360 != 0){
       fprintf(stderr,"Duration is not complete hours. You'd better set it to be N*360 (N is an integer)\n");
       return 0;
   }

   char name[255],type[255],path[255];
   memset(name,'\0',255);
   memset(path,'\0',255);
   memset(type,'\0',255);
   printf("name:%s\ntype:%s\n",name,type);
   divide_name(ifname,path,name,type);
   printf("name:%s\ntype:%s\npath:%s\n",name,type,path);

   if(dfaf){
       printf("DFA MODE\nOrder = %d", order);
   }else if(meanf){
       ;
   }else if(stdf){
       ;
   }else if(checkf){
       ;
   }

    return 0;
}


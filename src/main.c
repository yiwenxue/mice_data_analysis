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
#include "file.h"
#include "utility.c"


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
    {"duration",required_argument,  0,  'D'}, //Time duration
    {"dlength", required_argument,  0,  'L'}, //single dfa length
    {"startp",  required_argument,  0,  'P'}, //start point
    {"graph-size",required_argument,0,  'S'}, //size of the graph
    {"output",  required_argument,  0,  'o'},
    {0,         0,                  0,  0  }
};
char *desc[]={
    "Name of the file to be analysised, followed by a file name",
    "The type of graph. [ MICE_DATA | MICE_DATA_38 | SINGLE_COLUM ]",
    "Exec dfa, followed by dfa order.",
    "Exec mean analysis",
    "Exec standard analysis",
    "The type of graph. [ GINDIVIDUAL | GAVERAGE | GDEVIATION | GMID | GMIN | GMAX | GPOWERSPEC | GCHECK ]",
    "Set the duration of a single segment, 1 = 10sec (1080 = 3h)",
    "Set the whole length of individual dfa, 1 = 10sec",
    "Set the start point of individual dfa, 1 = 10sec",
    "Determint the dimensions of the graph",
    "Generate a pdf file for the graph, followed by a file name. If \n\tnot seted, show a gnuplot window",
    ""
};

extern char name[255],type[255],path[255];

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
     
    int ifflag =0, iftypef =0, dfaf=0, meanf=0, stdf =0, gtypef =0,
        durationf =0, dlengthf =0, startpf =0, gsizef =0, outputf =0; /*flags hahah*/
    Gtype graphtype;
    Ftype filetype;

    //default values
    int order = 1, width = 600, height = 450, duration = 1080, dlength = 1080, startp = 0;

    char ifname[255], outname[255], gtype[255], iftype[255];

    /* Read parameters from command line; */
    int long_index = 0, opt = 0;
    while((opt = getopt_long(argc,argv,"f:T:d:mst:D:L:P:S:o:",long_options,&long_index)) != -1){
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
                      order = atoi(optarg);
                      /* printf("meanf = %d\tstdf = %d\tcheckf = %d\tdfaf = %d\tdfa_order = %d\n",meanf,stdf,checkf,dfaf,order); */
                      break; 
            case 'm': meanf = 1;
                      dfaf = 0;
                      stdf = 0;
                      /* printf("meanf = %d\tstdf = %d\tcheckf = %d\tdfaf = %d\tdfa_order = %d\n",meanf,stdf,checkf,dfaf,order); */
                      break;
            case 's': stdf = 1;
                      dfaf = 0;
                      meanf = 0;
                      /* printf("meanf = %d\tstdf = %d\tcheckf = %d\tdfaf = %d\tdfa_order = %d\n",meanf,stdf,checkf,dfaf,order); */
                      break;
            case 't': gtypef = 1;
                      strcpy(gtype,optarg);
                      /* printf("gtypef = %d\tgtype = %s\n",gtypef,gtype); */
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
   int lines = 0;

   memset(name,'\0',255);
   memset(path,'\0',255);
   memset(type,'\0',255);

   //readin data with check process
   if(ifflag){
       if(iftypef){
           if(!strcmp(iftype,"MICE_DATA_38")){
               filetype = MICE_DATA_38;
               data = read_mice_file(ifname,&lines);
           }else if(!strcmp(iftype,"SINGLE_COLUM")){
               filetype = SINGLE_COLUM;
               data = read_single_colum_data(ifname,&lines);
           }else if(!strcmp(iftype,"MICE_DATA")){
               filetype = MICE_DATA;
               data = read_mice_file(ifname,&lines);
               //
           }else{
               fprintf(stderr,"File types: MICE_DATA | MICE_DATA_38 | SINGLE_COLUM\n");
               return -1;
           }
       }else{
           filetype = MICE_DATA;
           data = read_mice_file(ifname,&lines);
       }
   }else{
       fprintf(stderr,"An input data file is needed.\n");
       return -1;
   }

   // Check file name according to the file type
   if(filetype == MICE_DATA || filetype == MICE_DATA_38){
       //CHEKC THE MICE_DATA* FILE THYE
       mice_name(ifname,path,name,type);
       /* printf("name: %s\ntype: %s\npath: %s\n",name,type,path); */
   }

   //set graph type;
   if(gtypef){
       if(!strcmp(gtype,"GAVERAGE")){
           graphtype = GAVERAGE;
       }else if(!strcmp(gtype,"GINDIVIDUAL")){
           graphtype = GINDIVIDUAL;
       }else if(!strcmp(gtype,"GMID")){
           graphtype = GMID;
       }else if(!strcmp(gtype,"GMAX")){
           graphtype = GMAX;
       }else if(!strcmp(gtype,"GMIN")){
           graphtype = GMIN;
       }else if(!strcmp(gtype,"GDEVIATION")){
           graphtype = GDEVIATION;
       }else if(!strcmp(gtype,"GCHECK")){
           if(filetype == MICE_DATA_38)
               graphtype = GCHECK; // Set the graph type onlyif the file contains 38 days data.
           else {
               fprintf(stderr,"Filetype error. MICE_DATA_38 nedded.\nExit...\n");
               return -1;
           }
       }else if(!strcmp(gtype,"GPOWERSPEC")){
           graphtype = GPOWERSPEC;
       }else{
           fprintf(stderr,"Graph type: GINDIVIDUAL|GAVERAGE|GDEVIATION|GMID|GMIN|GMAX|GPOWERSPEC|GCHECK\n");
           return -1;
       }
   }else
       graphtype = GAVERAGE; // set a default graph type 

   // Set an time duration of every windows(segment)
   if(duration % 360 != 0){
       fprintf(stderr,"Duration is not complete hours. You'd better set it to be N*360 (N is an integer)\n");
       /* return -1; */
   }

   // Set the analysis type  And then process
   if(dfaf){
       if(filetype == MICE_DATA_38)
           mice38_dfa(data,lines,order,duration,graphtype,outputf);
       if(filetype == MICE_DATA)
           mice_dfap(name,path,type,data,lines,15120,order,duration);
           /* mice_dfa(name,path,type,data,lines,order,duration); */
   }else if(meanf){
       if(filetype == MICE_DATA_38)
           mice38_mean(data,lines,duration,graphtype,outputf);
       if(filetype == MICE_DATA)
           mice_mean(name,path,type,data,lines,duration);
   }else if(stdf){
       if(filetype == MICE_DATA_38)
           mice38_std(data,lines,duration,graphtype,outputf);
       if(filetype == MICE_DATA)
           mice_std(name,path,type,data,lines,duration);
   }else 
       return -1; 
   
   free(data);

   return 0;
}


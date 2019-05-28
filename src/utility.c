/*************************************************************************
    > File Name: utility.c
    > Author: Yiwen Xue
    > Mail: 15225434259xue@gmail.com 
    > Created Time: Sun 19 May 2019 07:23:16 PM IDT
*************************************************************************/

#include "utility.h"

int mice_name(char *inname,char *path, char *name, char *type){
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


int dfa_fnsize(int num){
    return (int)(log10((double)num/5.0)*100);
}

/*
 * hahahahaahha
 * author: Yiwen Xue
 * date: 20/05/2019
 * */
int mice_dfa(char *name ,char *type, double *data, int size, int order, int duration, Gtype graphtype, int outputf, char *outname){

    int ssize = dfa_fnsize(duration);

    //num of colume is sagments, num of raws is different days;
    int colum = 48*360 / duration;
    int  raw = 18;

    /* printf("colum: %d\nraw: %d\n",colum,raw); */
    double *xx,*yy;
    xx = (double*)malloc(sizeof(double)*colum*raw);
    yy = (double*)malloc(sizeof(double)*colum*raw);

    //Prepare for dfa 
    double *fn,*n; double fit[2];
    fn = (double*)malloc(sizeof(double)*ssize);
    n = (double*)malloc(sizeof(double)*ssize);
    for(int i=0;i<ssize;i++){
        fn[i] = 0.0;
        n[i] = 0.0;
    }

    if( graphtype == GAVERAGE ){
        for(int j=0;j<raw;j++){
            for(int i=0;i<colum;i++){
                xx[j*colum+i] = (i+0.5)*((double)duration/360.0); // hours
                yy[j*colum+i] = detrend_flucuation(order,data+(j*17280/*2 days*/+i*duration),duration,fn,n,&ssize,fit); //mean value for every 3 hours 
            }
        }
    }else if(graphtype == GINDIVIDUAL){
        printf("Not usable.\n");
    }else 
        return -1;

    double *meany,*devy,*std;
    meany = (double*)malloc(sizeof(double)*colum);
    devy = (double*)malloc(sizeof(double)*colum);
    std = (double*)malloc(sizeof(double)*colum);
    for(int i=0;i<colum;i++){
        meany[i] = 0.0;
        devy[i] = 0.0;
        std[i] = 0.0;
    }

    //calculation of mean value;
    for(int i=0;i<colum;i++){
        for(int j=0;j<raw;j++){
            meany[i] += yy[j*colum+i];
        }
        meany[i] /= raw;
    }
    double meant = 0.0;
    for(int i=0;i<colum;i++){
        meant += meany[i];
    }
    meant /= (double)colum; 
    /* printf("meant = %f\n",meant); */
    //calculation of deviation;
    for(int i=0;i<colum;i++){
        devy[i] = meany[i] - meant;
        for(int j=0;j<raw;j++){
            std[i] += pow(yy[j*colum+i] - meany[i],2.0);
        }
        std[i] = sqrt(std[i]/raw);
        devy[i] /= meany[i] /100.0;
        /* std[i] = std[i] / meany[i] * 100.0; */
    }
    //finished !!! 

    //Wants to print the results into a file;
    FILE *output; 
    output = fopen("/tmp/mice","w");
    for(int i=0;i<colum;i++){
        fprintf(output,"%.2f\t%.3f\t%.3f\n",xx[i],devy[i],std[i]);
    }
    fclose(output);

    double func[5];
    cosinor(xx,devy,colum,func,5);

    //
    // The progress to plot a graph;
    //
    FILE *plo; 
    plo = gnuplot_create_t();
    if(plo == NULL){
        fprintf(stdout,"Error when call gnuplot.\n");
        return 0;
    }
    double t = M_PI/12.0;
    char cmd[255];
    plot_cmd(plo,"set terminal qt size 500,400");
    plot_cmd(plo,"set grid");
    plot_cmd(plo,"set ylabel \"\%Deviation from mean α\"");
    /* plot_cmd(plo,"set ylabel \"\%Deviation\""); */
    /* plot_cmd(plo,"set ylabel \"α\""); */
    plot_cmd(plo,"set xlabel \"time(h)\"");
    plot_cmd(plo,"set xtics 3");
    sprintf(cmd,"set title \"DFA%d of %s - day average(%s)\"",order,type,name);
    plot_cmd(plo,cmd);
    plot_cmd(plo,"set xrange [0:48]");
    sprintf(cmd,"plot \'/tmp/mice\' u 1:2 with points pt 4 title \"Data\",%lf+%lf*cos(%lf*x+%lf)+%lf*cos(%lf*x+%lf) title \"Fit\"",
            func[0],func[1],t,func[2],func[3],2*t,func[4]);
    plot_cmd(plo,cmd);

    free(yy);free(xx);free(devy);free(meany);free(fn);free(n);
    free(std);
    return 0;
}
/*
 * hahahahaahha
 * author: Yiwen Xue
 * date: 20/05/2019
 * */
int mice_mean(char *name ,char *type, double *data, int size, int duration, Gtype graphtype, int outputf, char *outname){

    //num of colume is sagments, num of raws is different days;
    int colum = 48*360 / duration;
    int  raw = 18;

    /* printf("colum: %d\nraw: %d\n",colum,raw); */
    double *xx,*yy;
    xx = (double*)malloc(sizeof(double)*colum*raw);
    yy = (double*)malloc(sizeof(double)*colum*raw);

    for(int j=0;j<raw;j++){
        for(int i=0;i<colum;i++){
            xx[j*colum+i] = (i+0.5)*((double)duration/360.0); // hours
            yy[j*colum+i] = gsl_stats_mean(data+(j*17280/*length of 2 days*/+i*duration),1,duration); //mean value for every 3 hours 
        }
    }

    double *meany,*devy,*std,*devstd;
    meany = (double*)malloc(sizeof(double)*colum);
    devy = (double*)malloc(sizeof(double)*colum);
    std = (double*)malloc(sizeof(double)*colum);
    devstd = (double*)malloc(sizeof(double)*colum);
    for(int i=0;i<colum;i++){
        meany[i] = 0.0;
        devy[i] = 0.0;
        std[i] = 0.0;
        devstd[i] = 0.0;
    }

    //calculation of mean value;
    for(int i=0;i<colum;i++){
        for(int j=0;j<raw;j++){
            meany[i] += yy[j*colum+i];
            /* printf("[%d,%d]: %lf\n",i,j,yy[j*colum+i]); */
        }
        meany[i] /= raw;
    }
    double meant = 0.0;
    for(int i=0;i<colum;i++){
        meant += meany[i];
    }
    meant /= (double)colum; 
    /* printf("meant = %f\n",meant); */
    //calculation of deviation;
    for(int i=0;i<colum;i++){
        devy[i] = meany[i] - meant;
        for(int j=0;j<raw;j++){
            std[i] += pow(yy[j*colum+i] - meany[i],2.0);
        }
        std[i] = sqrt(std[i]/raw);
        devy[i] /= meany[i] /100.0;
        devstd[i] = std[i] / meany[i] * 100.0;
    }
    //finished !!! 

    double func[5];

    //Wants to print the results into a file;
    FILE *output; 
    output = fopen("/tmp/mice","w");

    if( graphtype == GAVERAGE ){
        cosinor(xx,meany,colum,func,5);
        for(int i=0;i<colum;i++){
            fprintf(output,"%.2f\t%.3f\t%.3f\n",xx[i],meany[i],std[i]);
        }
    }else if(graphtype == GDEVIATION){
        cosinor(xx,devy,colum,func,5);
        for(int i=0;i<colum;i++){
            fprintf(output,"%.2f\t%.3f\t%.3f\n",xx[i],devy[i],devstd[i]);
        }
    }else 
        return -1;

    fclose(output);


    //
    // The progress to plot a graph;
    //
    FILE *plo; 
    plo = gnuplot_create_t();
    if(plo == NULL){
        fprintf(stdout,"Error when call gnuplot.\n");
        return 0;
    }
    double t = M_PI/12.0;
    char cmd[255];
    plot_cmd(plo,"set terminal qt size 500,400");
    plot_cmd(plo,"set grid");
    if(graphtype == GAVERAGE ){
        sprintf(cmd,"set title \" Mean %s - day average(%s)\"",type,name);
        plot_cmd(plo,"set ylabel \"Average\"");
    }else {
        sprintf(cmd,"set title \" Mean of %s - day average(%s)\"",type,name);
        plot_cmd(plo,"set ylabel \"\%Deviation\"");
    }
    plot_cmd(plo,cmd);
    plot_cmd(plo,"set xlabel \"time(h)\"");
    plot_cmd(plo,"set xtics 3");
    plot_cmd(plo,cmd);
    plot_cmd(plo,"set xrange [0:48]");
    sprintf(cmd,"plot \'/tmp/mice\' u 1:2:3 with yerr pt 4 notitle,%f + %f * cos(%f * x + %f )+%f * cos(%f * x+ %f ) title \"fit\"",
            func[0],func[1],t,func[2],func[3],2*t,func[4]);
    /* printf(" y = %.3f + %.3f * cos(%.3f * x + %.3f )+%.3f * cos(%.3f * x+ %.3f )\n", */
    /*         func[0],func[1],t,func[2],func[3],2*t,func[4]); */

    plot_cmd(plo,cmd);

    free(yy);free(xx);free(devy);free(meany);
    free(std);
    free(devstd);
    return 0;
}

/*
 * hahahahaahha
 * author: Yiwen Xue
 * date: 20/05/2019
 * */
int mice_std(char *name ,char *type, double *data, int size, int duration, Gtype graphtype, int outputf, char *outname){

    //num of colume is sagments, num of raws is different days;
    int colum = 48*360 / duration;
    int  raw = 18;

    /* printf("colum: %d\nraw: %d\n",colum,raw); */
    double *xx,*yy;
    xx = (double*)malloc(sizeof(double)*colum*raw);
    yy = (double*)malloc(sizeof(double)*colum*raw);

    for(int j=0;j<raw;j++){
        for(int i=0;i<colum;i++){
            xx[j*colum+i] = (i+0.5)*((double)duration/360.0); // hours
            yy[j*colum+i] = gsl_stats_sd(data+(j*17280/*2 days*/+i*duration),1,duration); //mean value for every 3 hours 
        }
    }

    double *meany,*devy,*std;
    meany = (double*)malloc(sizeof(double)*colum);
    devy = (double*)malloc(sizeof(double)*colum);
    std = (double*)malloc(sizeof(double)*colum);
    for(int i=0;i<colum;i++){
        meany[i] = 0.0;
        devy[i] = 0.0;
        std[i] = 0.0;
    }

    //calculation of mean value;
    for(int i=0;i<colum;i++){
        for(int j=0;j<raw;j++){
            meany[i] += yy[j*colum+i];
            /* printf("[%d,%d]: %lf\n",i,j,yy[j*colum+i]); */
        }
        meany[i] /= raw;
    }
    double meant = 0.0;
    for(int i=0;i<colum;i++){
        meant += meany[i];
    }
    meant /= (double)colum; 
    /* printf("meant = %f\n",meant); */
    //calculation of deviation;
    for(int i=0;i<colum;i++){
        devy[i] = meany[i] - meant;
        for(int j=0;j<raw;j++){
            std[i] += pow(yy[j*colum+i] - meany[i],2.0);
        }
        std[i] = sqrt(std[i]/raw);
        devy[i] /= meany[i] /100.0;
        std[i] = std[i] / meany[i] * 100.0;
    }
    //finished !!! 

    double func[5];

    //Wants to print the results into a file;
    FILE *output; 
    output = fopen("/tmp/mice","w");

    if( graphtype == GAVERAGE ){
        cosinor(xx,meany,colum,func,5);
        /* fit_sin4(func,xx,devy,colum); */
        for(int i=0;i<colum;i++){
            fprintf(output,"%.2f\t%.3f\t%.3f\n",xx[i],meany[i],std[i]);
        }
    }else if(graphtype == GDEVIATION){
        cosinor(xx,devy,colum,func,5);
        /* fit_sin4(func,xx,devy,colum); */
        for(int i=0;i<colum;i++){
            fprintf(output,"%.2f\t%.3f\t%.3f\n",xx[i],devy[i],std[i]);
        }
    }else 
        return -1;

    fclose(output);


    //
    // The progress to plot a graph;
    //
    FILE *plo; 
    plo = gnuplot_create_t();
    if(plo == NULL){
        fprintf(stdout,"Error when call gnuplot.\n");
        return 0;
    }
    double t = M_PI/12.0;
    char cmd[255];
    plot_cmd(plo,"set terminal qt size 500,400");
    plot_cmd(plo,"set grid");
    if(graphtype == GAVERAGE ){
        sprintf(cmd,"set title \" STD %s - day average(%s)\"",type,name);
        plot_cmd(plo,"set ylabel \"Average\"");
    }else {
        sprintf(cmd,"set title \" STD of %s - day average(%s)\"",type,name);
        plot_cmd(plo,"set ylabel \"\%Deviation\"");
    }
    plot_cmd(plo,cmd);
    plot_cmd(plo,"set xlabel \"time(h)\"");
    plot_cmd(plo,"set xtics 3");
    plot_cmd(plo,cmd);
    plot_cmd(plo,"set xrange [0:48]");
    sprintf(cmd,"plot \'/tmp/mice\' u 1:2:3 with yerr pt 4 notitle,%f + %f * cos(%f * x + %f )+%f * cos(%f * x+ %f ) title \"fit\"",
            func[0],func[1],t,func[2],func[3],2*t,func[4]);
    /* printf(" y = %.3f + %.3f * cos(%.3f * x + %.3f )+%.3f * cos(%.3f * x+ %.3f )\n", */
    /*         func[0],func[1],t,func[2],func[3],2*t,func[4]); */

    plot_cmd(plo,cmd);

    free(yy);free(xx);free(devy);free(meany);
    free(std);
    return 0;
}


int check_circadian(char *filename,int breakpoint){
    FILE *input;
    input = fopen(filename,"r");
    int lines = 0;
    char temp;
    temp = fgetc(input);
    while(temp != EOF){
        if(temp == '\n')
            lines ++;
        temp = fgetc(input);
    }
    fclose(input);
    double *x = (double*)malloc(lines*sizeof(double));
    double *y = (double*)malloc(lines*sizeof(double));
    
    for(int i=0;i<lines;i++){
        x[i] = 0.0;y[i] = 0.0;
    }
    int tpx;double tpy;

    input = fopen(filename,"r");
    while(breakpoint){
        fscanf(input,"%d\t%lf\n",&tpx,&tpy);
        breakpoint--;
    }
    for(int i=0;i<lines;i++){
        if(fscanf(input,"%d\t%lf\n",&tpx,&tpy) != 2){
            printf("End: x = %d\n",tpx);
            lines = i;
            break;
        }
        x[i] = (double)tpx;
        if(!isnan(tpy))
        y[i] = tpy;
        else y[i] = y[i-1];
    }
    fclose(input);
    fprintf(stdout,"Finished! \nBreakpoint : %d\nTotal :%d\n",breakpoint,lines);
    for(int i=0;i<lines;i++){
        //printf("%d : %.3lf\t%.3lf\n",i,x[i],y[i]);
    }
    char ofname[20];
    int tmp = lines - 34560;
    FILE *output;
    int segments = 0;
    for(int j=0;j<tmp;j+=34560){
        segments += 1;
        sprintf(ofname,"segment%d",segments);
        output = fopen(ofname,"w");
        for(int i=0;i<34560;i+=720){
            fprintf(output,"%f\t%f\n",(i)/360.0+1,gsl_stats_mean(y+i+j,1,720));
        }
        fclose(output);
    }
    printf("segments = %d \n",segments);

    FILE *plo; 
    plo = gnuplot_create_t();
    if(plo == NULL){
        fprintf(stdout,"Error when call gnuplot.\n");
        return 0;
    }
    char cmd[255];
    plot_cmd(plo,"set terminal qt size 900,900");
    plot_cmd(plo,"set multiplot");
    plot_cmd(plo,"set grid");
    plot_cmd(plo,"set origin 0,0");
    plot_cmd(plo,"set size 1,0.15");
    plot_cmd(plo,"set yrange [0:15]");
    plot_cmd(plo,"set xrange [1:95]");
    plot_cmd(plo,"set ytics 5");
    plot_cmd(plo,"set xlabel \"time(h)\"");
    plot_cmd(plo,"plot \'segment1\' with lines title \"seg1\" ");
    plot_cmd(plo,"set format x \"\"");
    plot_cmd(plo,"set xlabel \"\"");
    plot_cmd(plo,"set size 1,0.1");
    for(int i=2;i<=9;i++){
        if(i == 9){
            plot_cmd(plo,"set title \"Activity\"");
            plot_cmd(plo,"set size 1,0.15");
        }
        sprintf(cmd,"set origin 0,%f",-0.05+i*0.1);
        plot_cmd(plo,cmd);
        plot_cmd(plo,"set xrange [1:95]");
        sprintf(cmd,"plot \'segment%d\' with lines title \"seg%d\"",i,i);
        plot_cmd(plo,cmd);
    }
    return 0;
}

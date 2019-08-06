/*************************************************************************
    > File Name: utility.c
    > Author: Yiwen Xue
    > Mail: 15225434259xue@gmail.com 
    > Created Time: Sun 19 May 2019 07:23:16 PM IDT
*************************************************************************/

#include "utility.h"

int mice_name(char *inname,
              char *path, 
              char *name, 
              char *type)
{
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
int mice38_dfa(double *data, 
             int size, 
             int order, 
             int duration, 
             Gtype graphtype, 
             int outputf) 
{

    int ssize = dfa_fnsize(duration);
    char outname[255];
    memset(outname,'\0',255);

    //num of colume is sagments, num of raws is different days;
    int colum = 48*360 / duration;
    int  raw = 18;

    /* printf("colum: %d\nraw: %d\n",colum,raw); */
    double *xx,*yy,*err;
    xx = (double*)malloc(sizeof(double)*colum*raw);
    yy = (double*)malloc(sizeof(double)*colum*raw);
    err = (double*)malloc(sizeof(double)*colum*raw);

    //Prepare for dfa 
    double *fn,*n; double fit[2];

    fn = (double*)malloc(sizeof(double)*ssize);
    n = (double*)malloc(sizeof(double)*ssize);
    for(int i=0;i<ssize;i++){
        fn[i] = 0.0;
        n[i] = 0.0;
    }

    double mean =0.0; int cont =0;
    double *meany,*devy,*std,*mid;
    double meant = 0.0;

    if( graphtype != GINDIVIDUAL ){
        for(int j=0;j<raw;j++){
            for(int i=0;i<colum;i++){
                xx[j*colum+i] = (i+0.5)*((double)duration/360.0); // hours
                err[j*colum+i] = detrend_flucuation(order,data+(j*17280/*2 days*/+i*duration),duration,fn,n,&ssize,fit); //mean value for every 3 hours 
                yy[j*colum+i] = fit[1];
            }
        }

        meany = (double*)malloc(sizeof(double)*colum);
        devy = (double*)malloc(sizeof(double)*colum);
        std = (double*)malloc(sizeof(double)*colum);
        mid = (double*)malloc(sizeof(double)*colum);
        for(int i=0;i<colum;i++){
            meany[i] = 0.0;
            devy[i] = 0.0;
            std[i] = 0.0;
            mid[i] = 0.0;
        }

        //calculation of mean value;
        for(int i=0;i<colum;i++){
            for(int j=0;j<raw;j++){
                meany[i] += yy[j*colum+i];
            }
            meany[i] /= raw;
            mid[i] = get_mid(yy+i,colum,colum*raw);
        }
        for(int i=0;i<colum;i++){
            meant += meany[i];
        }
        meant /= (double)colum; 

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
    }

    //Wants to print the results into a file;
    FILE *output; 
    output = fopen("/tmp/mice","w");
    for(int i=0;i<colum;i++){
        fprintf(output,"%.2f\t%.3f\t%.3f\t%.3f\t%.3f\n",xx[i],meany[i],mid[i],devy[i],std[i]);
    }
    fclose(output);

    double func[5];
    double rsq =0.0;
    
    if(graphtype == GMID){
        sprintf(outname,"%s/%s_%s_DFA%d_%d_GMID.pdf",path,name,type,order,duration);
        rsq = cosinor(xx,mid,colum,func,5);
    }else if(graphtype == GMIN ){
        /* sprintf(outname,"%s/%s_%s_DFA%d_%d_GMIN.pdf",path,name,type,order,duration); */
        /* rsq = cosinor(xx,min,colum,func,5); */
    /* }else if(graphtype == GMAX ){ */
        /* sprintf(outname,"%s/%s_%s_DFA%d_%d_GMAX.pdf",path,name,type,order,duration); */
        /* rsq = cosinor(xx,max,colum,func,5); */
    }else if(graphtype == GAVERAGE ){
        sprintf(outname,"%s/%s_%s_DFA%d_%d_GAVERAGE.pdf",path,name,type,order,duration);
        rsq = cosinor(xx,meany,colum,func,5);
    }else if(graphtype == GDEVIATION ){
        sprintf(outname,"%s/%s_%s_DFA%d_%d_GDEVIATION.pdf",path,name,type,order,duration);
        rsq = cosinor(xx,std,colum,func,5);
    }else if(graphtype == GPOWERSPEC ){
        /* sprintf(outname,"%s/%s_%s_DFA%d_%d_GPOWERSPEC.pdf",path,name,type,order,duration); */
        ;// Define a method to solve this problem
    }
    

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

    if(!outputf)
        plot_cmd(plo,"set terminal qt size 500,400");
    else{
        plot_cmd(plo,"set terminal pdf size 6.0,4.0");
        sprintf(cmd,"set output \"%s\"",outname);
        plot_cmd(plo,cmd);
    }

    plot_cmd(plo,"set grid");
    plot_cmd(plo,"set xlabel \"time(h)\"");
    plot_cmd(plo,"set xtics 3");
    /* plot_cmd(plo,"set xrange [0:48]"); */

    if(graphtype == GMID){
        plot_cmd(plo,"set ylabel \"α min\"");
        sprintf(cmd,"set title \"DFA%d of %s - day mid(%s)\"",order,type,name);
        plot_cmd(plo,cmd);
        sprintf(cmd,"plot \'/tmp/mice\' u 1:3 with points pt 4 title \"Data\",%lf+%lf*cos(%lf*x+%lf)+%lf*cos(%lf*x+%lf) title \"Fit(R^2=%.3f)\"",
                func[0],func[1],t,func[2],func[3],2*t,func[4],rsq);
    }else if(graphtype == GMIN ){
        plot_cmd(plo,"set ylabel \"α mid\"");
        sprintf(cmd,"set title \"DFA%d of %s - day min(%s)\"",order,type,name);
        plot_cmd(plo,cmd);
        sprintf(cmd,"plot \'/tmp/mice\' u 1:3 with points pt 4 title \"Data\",%lf+%lf*cos(%lf*x+%lf)+%lf*cos(%lf*x+%lf) title \"Fit(R^2=%.3f)\"",
                func[0],func[1],t,func[2],func[3],2*t,func[4],rsq);
        return -1;
    /* }else if(graphtype == GMAX ){ */
    /*     plot_cmd(plo,"set ylabel \"α max\""); */
    /*     sprintf(cmd,"set title \"DFA%d of %s - day max(%s)\"",order,type,name); */
    /*     plot_cmd(plo,cmd); */
    /*     sprintf(cmd,"plot \'/tmp/mice\' u 1:3 with points pt 4 title \"Data\",%lf+%lf*cos(%lf*x+%lf)+%lf*cos(%lf*x+%lf) title \"Fit(R^2=%.3f)\"", */
    /*             func[0],func[1],t,func[2],func[3],2*t,func[4],rsq); */
    /*     return -1; */
    }else if(graphtype == GAVERAGE ){
        plot_cmd(plo,"set ylabel \"α(day average)\"");
        sprintf(cmd,"set title \"DFA%d of %s - day average(%s)\"",order,type,name);
        plot_cmd(plo,cmd);
        sprintf(cmd,"plot \'/tmp/mice\' u 1:2 with points pt 4 title \"Data\",%lf+%lf*cos(%lf*x+%lf)+%lf*cos(%lf*x+%lf) title \"Fit(R^2=%.3f)\"",
                func[0],func[1],t,func[2],func[3],2*t,func[4],rsq);
    }else if(graphtype == GDEVIATION ){
        plot_cmd(plo,"set ylabel \"\%Deviation from mean α\"");
        sprintf(cmd,"set title \"DFA%d of %s - day average(%s)\"",order,type,name);
        plot_cmd(plo,cmd);
        sprintf(cmd,"plot \'/tmp/mice\' u 1:5 with points pt 4 title \"Data\",%lf+%lf*cos(%lf*x+%lf)+%lf*cos(%lf*x+%lf) title \"Fit(R^2=%.3f)\"",
                func[0],func[1],t,func[2],func[3],2*t,func[4],rsq);
    }else if(graphtype == GPOWERSPEC ){
        plot_cmd(plo,"set ylabel \"powerspectrum\"");
        sprintf(cmd,"set title \"DFA%d powerspectrum of %s (%s)\"",order,type,name);
        plot_cmd(plo,cmd);
        sprintf(cmd,"plot \'/tmp/mice\' u 1:3 with points pt 4 title \"Data\",%lf+%lf*cos(%lf*x+%lf)+%lf*cos(%lf*x+%lf) title \"Fit(R^2=%.3f)\"",
                func[0],func[1],t,func[2],func[3],2*t,func[4],rsq);
        return -1;
    }
    plot_cmd(plo,cmd);
    plot_cmd(plo,"set out");

    free(yy);free(xx);free(devy);free(meany);free(fn);free(mid);free(n);free(std);free(err);
    return 0;
}
/*
 * hahahahaahha
 * author: Yiwen Xue
 * date: 20/05/2019
 * */
int mice38_mean(
        double *data, 
        int size, 
        int duration, 
        Gtype graphtype, 
        int outputf)
{

    //num of colume is sagments, num of raws is different days;
    int colum = 48*360 / duration;
    int  raw = 18;

    char outname[255];
    memset(outname,'\0',255);

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

    double *meany,*devy,*std,*devstd,*mid;

    meany = (double*)malloc(sizeof(double)*colum);
    devy = (double*)malloc(sizeof(double)*colum);
    std = (double*)malloc(sizeof(double)*colum);
    mid = (double*)malloc(sizeof(double)*colum);
    devstd = (double*)malloc(sizeof(double)*colum);

    for(int i=0;i<colum;i++){
        meany[i] = 0.0;
        devy[i] = 0.0;
        std[i] = 0.0;
        mid[i] = 0.0;
        devstd[i] = 0.0;
    }

    //calculation of mean value;
    for(int i=0;i<colum;i++){
        for(int j=0;j<raw;j++){
            meany[i] += yy[j*colum+i];
            /* printf("[%d,%d]: %lf\n",i,j,yy[j*colum+i]); */
        }
        mid[i] = get_mid(yy+i,colum,colum*raw);
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
    double rsq = 0.0;

    for(int i=0;i<colum;i++){
        fprintf(output,"%.2f\t%.3f\t%.3f\t%.3f\t%.3f\n",xx[i],meany[i],mid[i],std[i],devstd[i]);
    }
    fclose(output);

    if( graphtype == GAVERAGE ){
        sprintf(outname,"%s/%s_%s_MEAN_%d_GAVERAGE.pdf",path,name,type,duration);
        rsq = cosinor(xx,meany,colum,func,5);
    }else if(graphtype == GDEVIATION){
        sprintf(outname,"%s/%s_%s_MEAN_%d_GDEVIATION.pdf",path,name,type,duration);
        rsq = cosinor(xx,devy,colum,func,5);
    }else if(graphtype == GMID){
        sprintf(outname,"%s/%s_%s_MEAN_%d_GMIN.pdf",path,name,type,duration);
        rsq = cosinor(xx,mid,colum,func,5);
    }else 
        return -1;


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

    //Check the output file name...
    if(!outputf)
        plot_cmd(plo,"set terminal qt size 500,400");
    else{
        plot_cmd(plo,"set terminal pdf size 6.0,4.0");
        sprintf(cmd,"set output \"%s\"",outname);
        printf("%s\n",cmd);
        plot_cmd(plo,cmd);
    }

    plot_cmd(plo,"set grid");
    plot_cmd(plo,"set xlabel \"time(h)\"");
    plot_cmd(plo,"set xtics 3");
    plot_cmd(plo,cmd);
    /* plot_cmd(plo,"set xrange [0:48]"); */

    if(graphtype == GAVERAGE ){
        sprintf(cmd,"set title \" Mean %s - day average(%s)\"",type,name);
        plot_cmd(plo,cmd);
        plot_cmd(plo,"set ylabel \"Average\"");
        sprintf(cmd,"plot \'/tmp/mice\' u 1:2 with points pt 4 notitle,%f + %f * cos(%f * x + %f )+%f * cos(%f * x+ %f ) title \"fit(R^2=%.3f)\"",
                func[0],func[1],t,func[2],func[3],2*t,func[4],rsq);
    }else if(graphtype == GDEVIATION ){
        sprintf(cmd,"set title \" Mean of %s - day average(%s)\"",type,name);
        plot_cmd(plo,cmd);
        plot_cmd(plo,"set ylabel \"\%Deviation\"");
        sprintf(cmd,"plot \'/tmp/mice\' u 1:4 with points pt 4 notitle,%f + %f * cos(%f * x + %f )+%f * cos(%f * x+ %f ) title \"fit(R^2=%.3f)\"",
                func[0],func[1],t,func[2],func[3],2*t,func[4],rsq);
    }else if(graphtype == GMID ){
        sprintf(cmd,"set title \" Mean of %s - day mid(%s)\"",type,name);
        plot_cmd(plo,cmd);
        plot_cmd(plo,"set ylabel \"Mid of Average\"");
        sprintf(cmd,"plot \'/tmp/mice\' u 1:3 with points pt 4 notitle,%f + %f * cos(%f * x + %f )+%f * cos(%f * x+ %f ) title \"fit(R^2=%.3f)\"",
                func[0],func[1],t,func[2],func[3],2*t,func[4],rsq);
    }
    plot_cmd(plo,cmd);
    plot_cmd(plo,"set out");

    free(yy);free(xx);free(devy);free(meany);free(std);free(devstd);free(mid);
    return 0;
}

/*
 * hahahahaahha
 * author: Yiwen Xue
 * date: 20/05/2019
 * */
int mice38_std(
             double *data, 
             int size, 
             int duration, 
             Gtype graphtype, 
             int outputf)
{

    //num of colume is sagments, num of raws is different days;
    int colum = 48*360 / duration;
    int  raw = 18;

    char outname[255];
    memset(outname,'\0',255);

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

    double *meany,*devy,*std,*mid;
    meany = (double*)malloc(sizeof(double)*colum);
    devy = (double*)malloc(sizeof(double)*colum);
    std = (double*)malloc(sizeof(double)*colum);
    mid = (double*)malloc(sizeof(double)*colum);
    
    for(int i=0;i<colum;i++){
        meany[i] = 0.0;
        devy[i] = 0.0;
        std[i] = 0.0;
        mid[i] = 0.0;
    }

    //calculation of mean value;
    for(int i=0;i<colum;i++){
        for(int j=0;j<raw;j++){
            meany[i] += yy[j*colum+i];
            /* printf("[%d,%d]: %lf\n",i,j,yy[j*colum+i]); */
        }
        mid[i] = get_mid(yy+i,colum,colum*raw);
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
    }
    //finished !!! 

    double func[5];

    //Wants to print the results into a file;
    FILE *output; 
    output = fopen("/tmp/mice","w");
    double rsq =  0.0;

    for(int i=0;i<colum;i++){
        fprintf(output,"%.2f\t%.3f\t%.3f\t%.3f\n",xx[i],meany[i],mid[i],std[i]);
    }
    fclose(output);

    if( graphtype == GAVERAGE ){
        sprintf(outname,"%s/%s_%s_STD_%d_GAVERAGE.pdf",path,name,type,duration);
        rsq = cosinor(xx,meany,colum,func,5);
    }else if(graphtype == GDEVIATION){
        sprintf(outname,"%s/%s_%s_STD_%d_GDEVIATION.pdf",path,name,type,duration);
        rsq = cosinor(xx,devy,colum,func,5);
    }else if(graphtype == GMID){
        sprintf(outname,"%s/%s_%s_STD_%d_GMID.pdf",path,name,type,duration);
        rsq = cosinor(xx,mid,colum,func,5);
    }else 
        return -1;

    // The progress to plot a graph;
    //
    FILE *plo; 
    double t = M_PI/12.0;
    char cmd[255];
    plo = gnuplot_create_t();
    if(plo == NULL){
        fprintf(stdout,"Error when call gnuplot.\n");
        return 0;
    }

    //Check the output file name...
    if(!outputf)
        plot_cmd(plo,"set terminal qt size 500,400");
    else{
        plot_cmd(plo,"set terminal pdf size 6.0,4.0");
        sprintf(cmd,"set output \"%s\"",outname);
        plot_cmd(plo,cmd);
    }
    
    plot_cmd(plo,"set grid");
    plot_cmd(plo,"set xlabel \"time(h)\"");
    plot_cmd(plo,"set xtics 3");
    plot_cmd(plo,cmd);
    /* plot_cmd(plo,"set xrange [0:48]"); */


    if(graphtype == GAVERAGE ){
        sprintf(cmd,"set title \" STD of %s - day average(%s)\"",type,name);
        plot_cmd(plo,cmd);
        plot_cmd(plo,"set ylabel \"Average\"");
        sprintf(cmd,"plot \'/tmp/mice\' u 1:2:4 with yerr pt 4 notitle,%f + %f * cos(%f * x + %f )+%f * cos(%f * x+ %f ) title \"fit(R^2=%.3f)\"",
                func[0],func[1],t,func[2],func[3],2*t,func[4],rsq);
    }else if(graphtype == GMID){
        sprintf(cmd,"set title \"MID STD of %s - day average(%s)\"",type,name);
        plot_cmd(plo,cmd);
        plot_cmd(plo,"set ylabel \"\%Deviation\"");
        sprintf(cmd,"plot \'/tmp/mice\' u 1:3 with points pt 4 notitle,%f + %f * cos(%f * x + %f )+%f * cos(%f * x+ %f ) title \"fit(R^2=%.3f)\"",
                func[0],func[1],t,func[2],func[3],2*t,func[4],rsq);
    }

    plot_cmd(plo,cmd);
    plot_cmd(plo,"set out");

    free(yy);free(xx);free(devy);free(meany);free(std);free(mid);
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

int mice_dfa(char *name,    //file name of data
        char *path,         //full path to data
        char *type,         //activity or temperature
        double *data,       //array of data stream read
        int size,           //size of the array 
        int order,          //order of dfa
        int duration       //size of every single segment
)
{

    FILE *output;
    output = fopen("/tmp/mice", "w");
    double fit[2];
    double *fn, *n;
    int logsize = dfa_fnsize(duration);
    fn = (double*)malloc(sizeof(double)*logsize);
    n = (double*)malloc(sizeof(double)*logsize);
    for(int i=0;i<logsize;i++){
        fn[i] = 0.0; n[i] = 0.0;
    }
    double hurst = 0.0, mean = 0.0;
    size -= duration;
    double *x, *h, *err;
    x = (double*)malloc(sizeof(double)*size/duration);
    h = (double*)malloc(sizeof(double)*size/duration);
    err = (double*)malloc(sizeof(double)*size/duration);
    for(int i=0;i<size;i+=duration){
        err[i/duration] = detrend_flucuation(order,data+i,duration,fn,n,&logsize,fit);
        h[i/duration] = fit[1];
        /* printf("fit[0] = %f\tfit[1] = %f\terr = %f\n",fit[0],fit[1],err[i/duration]); */
        x[i/duration] = (i/1080.0)*3.0+1.5;
        fprintf(output,"%f\t%f\t%f\n",x[i/duration],h[i/duration],err[i/duration]);
    }
    fclose(output);

    double func[5] = {
        0.0,
        0.0,
        0.0,
        0.0,
        0.0};

    double t = M_PI/12.0;
    double rsq = cosinor(x,h,size/duration,func,5);
    FILE *plo; 
    plo = gnuplot_create_t();
    if(plo == NULL){
        fprintf(stdout,"Error when call gnuplot.\n");
        return 0;
    }
    char cmd[255];
    plot_cmd(plo,"set terminal pdf size 6.0,4.0");
    sprintf(cmd,"set out \"%s%s_%s_dfa%d.pdf\"",path,name,type,order);
    fprintf(stdout,"%s\n",cmd);
    plot_cmd(plo,cmd);
    sprintf(cmd,"set title \"%s %s DFA%d \"",name,type,order);
    plot_cmd(plo,cmd);
    plot_cmd(plo,"set grid");
    plot_cmd(plo,"set xlabel \"time(h)\"");
    plot_cmd(plo,"set xtics 3");
    plot_cmd(plo,"set ylabel \"α\"");
    plot_cmd(plo,"set xrange [0:48]");

    sprintf(cmd,"plot \"/tmp/mice\" u 1:2:(1-$3) with yerr pt 4 title \"Data\", %f + %f * cos(%f * x + %f )+%f * cos(%f * x+ %f ) title \"fit(R^2=%.3f)\"",
            func[0],func[1],t,func[2],func[3],2*t,func[4],rsq);
    plot_cmd(plo,cmd);

    plot_cmd(plo,"set out");
    free(fn);free(n);
    free(x);free(h);free(err);
    return 1;
}

// process dfa at any point with certen duration 
int mice_dfap(char *name,    //file name of data
        char *path,         //full path to data
        char *type,         //activity or temperature
        double *data,       //array of data stream read
        int size,           //size of the array 
        int pointss,          //a start point to check
        int order,          //order of dfa
        int duration       //size of every single segment
)
{

    FILE *output;
    output = fopen("/tmp/mice", "w");
    double fit[2];
    double *fn, *n;
    int logsize = dfa_fnsize(duration);
    fn = (double*)malloc(sizeof(double)*logsize);
    n = (double*)malloc(sizeof(double)*logsize);
    for(int i=0;i<logsize;i++){
        fn[i] = 0.0; n[i] = 0.0;
    }

    double err, mean;
    size -= duration;

    err = detrend_flucuation(order,data+pointss,duration,fn,n,&logsize,fit);
    for(int i=0;i<logsize;i++){
        fprintf(output,"%f\t%f\n",(n[i]),(fn[i]));
    }
    fclose(output);

    FILE *plo; 
    plo = gnuplot_create_t();
    if(plo == NULL){
        fprintf(stdout,"Error when call gnuplot.\n");
        return 0;
    }
    char cmd[255];
    plot_cmd(plo,"set terminal pdf size 6.0,4.0");
    sprintf(cmd,"set out \"%s%s_%s_dfa%d_%d.pdf\"",path,name,type,order,pointss);
    fprintf(stdout,"%s\n",cmd);
    plot_cmd(plo,cmd);
    sprintf(cmd,"set title \"%s %s DFA%d At %.1fh\"",name,type,order,pointss/360+1.5);
    plot_cmd(plo,cmd);
    plot_cmd(plo,"set encoding utf8");
    plot_cmd(plo,"set grid");
    plot_cmd(plo,"set logscale x 10");
    plot_cmd(plo,"set logscale y 10");
    plot_cmd(plo,"set xlabel \"n\"");
    plot_cmd(plo,"set ylabel \"fn\"");

    sprintf(cmd,"set label \"α = %.3f\" right at graph 0.3,graph 0.6",fit[1]);
    plot_cmd(plo,cmd);
    plot_cmd(plo,"show label");
    
    sprintf(cmd,"plot \"/tmp/mice\" with points notitle, %f*(x**%f) title \"fit(R^2=%.2f)\"",pow(10,fit[0]),fit[1],err);
    plot_cmd(plo,cmd);

    plot_cmd(plo,"set out");
    free(fn);free(n);
    fclose(plo);
    return 1;
}

int mice_mean(char *name,    //file name of data
        char *path,         //full path to data
        char *type,         //activity or temperature
        double *data,       //array of data stream read
        int size,           //size of the array 
        int duration       //size of every single segment
)
{

    FILE *output;
    output = fopen("/tmp/mice", "w");

    double mean = 0.0;
    size -= duration;

    double *x, *h;
    x = (double*)malloc(sizeof(double)*size/duration);
    h = (double*)malloc(sizeof(double)*size/duration);
    for(int i=0;i<size;i+=duration){
        h[i/duration] = gsl_stats_mean(data+i,1,duration);
        x[i/duration] = (i/1080.0)*3.0+1.5;
        fprintf(output,"%f\t%f\n",x[i/duration],h[i/duration]);
    }
    fclose(output);

    double func[5] = {0.0,0.0,0.0,0.0,0.0};
    double t = M_PI/12.0;
    double rsq = cosinor(x,h,size/duration,func,5);
    FILE *plo; 
    plo = gnuplot_create_t();
    if(plo == NULL){
        fprintf(stdout,"Error when call gnuplot.\n");
        return 0;
    }
    char cmd[255];
    plot_cmd(plo,"set terminal pdf size 6.0,4.0");

    sprintf(cmd,"set out \"%s%s_%s_Mean.pdf\"",path,name,type);
    fprintf(stdout,"%s\n",cmd);
    plot_cmd(plo,cmd);

    sprintf(cmd,"set title \"%s %s Mean\"",name,type);
    plot_cmd(plo,cmd);

    plot_cmd(plo,"set grid");
    plot_cmd(plo,"set xlabel \"time(h)\"");

    sprintf(cmd,"set ylabel \"Mean %s\"",type);
    plot_cmd(plo,cmd);

    plot_cmd(plo,"set xtics 3");
    plot_cmd(plo,"set xrange [0:48]");

    sprintf(cmd,"plot \"/tmp/mice\" with points title \"Data\", %f + %f * cos(%f * x + %f )+%f * cos(%f * x+ %f ) title \"fit(R^2=%.3f)\"",
            func[0],func[1],t,func[2],func[3],2*t,func[4],rsq);
    plot_cmd(plo,cmd);

    plot_cmd(plo,"set out");
    fclose(plo);
    free(x);free(h);
    return 1;
}
int mice_std(char *name,    //file name of data
        char *path,         //full path to data
        char *type,         //activity or temperature
        double *data,       //array of data stream read
        int size,           //size of the array 
        int duration       //size of every single segment
)
{

    FILE *output;
    output = fopen("/tmp/mice", "w");

    double std = 0.0;
    size -= duration;

    double *x, *h;
    x = (double*)malloc(sizeof(double)*size/duration);
    h = (double*)malloc(sizeof(double)*size/duration);
    for(int i=0;i<size;i+=duration){
        h[i/duration] = gsl_stats_sd(data+i,1,duration);
        x[i/duration] = (i/1080.0)*3.0+1.5;
        fprintf(output,"%f\t%f\n",x[i/duration],h[i/duration]);
    }
    fclose(output);

    double func[5] = {0.0,0.0,0.0,0.0,0.0};
    double t = M_PI/12.0;
    double rsq = cosinor(x,h,size/duration,func,5);

    FILE *plo; 
    plo = gnuplot_create_t();
    if(plo == NULL){
        fprintf(stdout,"Error when call gnuplot.\n");
        return 0;
    }
    char cmd[255];
    plot_cmd(plo,"set terminal pdf size 6.0,4.0");

    sprintf(cmd,"set out \"%s%s_%s_STD.pdf\"",path,name,type);
    fprintf(stdout,"%s\n",cmd);
    plot_cmd(plo,cmd);

    sprintf(cmd,"set title \"%s %s STD\"",name,type);
    plot_cmd(plo,cmd);

    plot_cmd(plo,"set grid");
    plot_cmd(plo,"set xlabel \"time(h)\"");
    plot_cmd(plo,"set xtics 3");

    sprintf(cmd,"set ylabel \"STD %s\"",type);
    plot_cmd(plo,cmd);

    plot_cmd(plo,"set xrange [0:48]");

    sprintf(cmd,"plot \"/tmp/mice\" with points title \"Data\", %f + %f * cos(%f * x + %f )+%f * cos(%f * x+ %f ) title \"fit(R^2=%.3f)\"",
            func[0],func[1],t,func[2],func[3],2*t,func[4],rsq);
    plot_cmd(plo,cmd);

    plot_cmd(plo,"set out");
    fclose(plo);
    free(x);free(h);
    return 1;
}

int mice_sync(
        double *data1, 
        double *data2, 
        int size, 
        int duration, 
        Gtype graphtype, 
        int outputf)
{
    //num of colume is sagments, num of raws is different days;
    int colum = 48*360 / duration;
    int  raw = 18;
    int xtotal = colum*raw;

    char outname[255];
    memset(outname,'\0',255);

    /* printf("colum: %d\nraw: %d\n",colum,raw); */
    /* printf("name %s/%s.%s\n",path,name,type); */
    double *xx,*y1,*y2,*r,*amp1,*amp2,*pha1,*pha2;
    fftw_complex *y1p,*y2p;
    xx = (double*)malloc(sizeof(double)*colum*raw);
    y1 = (double*)malloc(sizeof(double)*colum*raw);
    y2 = (double*)malloc(sizeof(double)*colum*raw);
    y1p = fftw_alloc_complex(colum*raw);
    y2p = fftw_alloc_complex(colum*raw);
    amp1 = (double*)malloc(sizeof(double)*colum*raw);
    amp2 = (double*)malloc(sizeof(double)*colum*raw);
    pha1 = (double*)malloc(sizeof(double)*colum*raw);
    pha2 = (double*)malloc(sizeof(double)*colum*raw);
    r = (double*)malloc(sizeof(double)*2*colum*raw);

    for(int j=0;j<raw;j++){
        for(int i=0;i<colum;i++){
            xx[j*colum+i] = (i+0.5)*((double)duration/360.0); // hours
            y1[j*colum+i] = gsl_stats_mean(data1+(j*17280/*length of 2 days*/+i*duration),1,duration); //mean value for every 3 hours 
            y2[j*colum+i] = gsl_stats_mean(data2+(j*17280/*length of 2 days*/+i*duration),1,duration); //mean value for every 3 hours 
        }
    }

    /* Because we should calculate on the amplitude, so we should remove the offset */ 
    double mean1 = gsl_stats_mean(y1,1,xtotal);
    double mean2 = gsl_stats_mean(y2,1,xtotal);
    for(int i=0;i<xtotal;i++){
        y1[i] -= mean1;
        y2[i] -= mean2;
    }

    hilbert_trans(y1,y1p,xtotal);
    hilbert_trans(y2,y2p,xtotal);

    for(int i=0;i<xtotal;i++){
        amp1[i] = sqrt(y1p[i][IM]*y1p[i][IM] + y1p[i][RE]*y1p[i][RE] );
        pha1[i] = atan(y1p[i][IM]/y1p[i][RE]);
        amp2[i] = sqrt(y2p[i][IM]*y2p[i][IM] + y2p[i][RE]*y2p[i][RE] );
        pha2[i] = atan(y2p[i][IM]/y2p[i][RE]);
    }

    //Wants to print the results into a file;
    FILE *output; 
    output = fopen("/tmp/mice_sync","w");

    for(int i=0;i<xtotal;i++){
        fprintf(output,"%.2f\t%.3f\t%.3f\t%.3f\t%.3f\t%.3f\t%.3f\t%.3f\t%.3f\n",(i+0.5)*((double)duration/360.0),y1[i],y1p[i][RE],amp1[i],pha1[i],y2[i],y2p[i][RE],amp2[i],pha2[i]);
    }
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
    char cmd[255];
    if(!outputf)
        plot_cmd(plo,"set terminal qt size 500,400");
    else{
        sprintf(outname,"%s/%s_%s_SYNC_Phase_%d.pdf",path,name,type,duration);
        plot_cmd(plo,"set terminal pdf size 6.0,4.0");
        sprintf(cmd,"set output \"%s\"",outname);
        printf("%s\n",cmd);
        plot_cmd(plo,cmd);
    }
    /* plot_cmd(plo,"set title \"phase difference distribute\""); */
    plot_cmd(plo,"set autoscale");
    plot_cmd(plo,"set grid");
    plot_cmd(plo,"bin(x,s) = s*int(x/s)");
    plot_cmd(plo,"set boxwidth 0.05");
    plot_cmd(plo,"set xrange [-pi:pi]");
    plot_cmd(plo,"set style fill solid 0.5");
    plot_cmd(plo,"set xlabel \"phase difference\"");
    plot_cmd(plo,"plot '/tmp/mice_sync' u (bin($5-$9,0.05)):(20/300.) smooth frequency w boxes notitle");
    if(outputf)
        plot_cmd(plo,"set out");
    fclose(plo);


    FILE *plo2; 
    plo2 = gnuplot_create_t();
    if(plo2 == NULL){
        fprintf(stdout,"Error when call gnuplot.\n");
        return 0;
    }
    if(!outputf)
        plot_cmd(plo2,"set terminal qt size 500,400");
    else{
        sprintf(outname,"%s/%s_%s_SYNC_Amp_%d.pdf",path,name,type,duration);
        plot_cmd(plo2,"set terminal pdf size 6.0,4.0");
        sprintf(cmd,"set output \"%s\"",outname);
        printf("%s\n",cmd);
        plot_cmd(plo2,cmd);
    }
    printf("\n\n\n\n\n");
    /* plot_cmd(plo,"set title \"phase difference distribute\""); */
    plot_cmd(plo2,"set ylabel \"Amp2\"");
    plot_cmd(plo2,"set xlabel \"Amp1\"");
    plot_cmd(plo2,"plot '/tmp/mice_sync' u 4:8 w lines lw 0.5 notitle");
    if(outputf)
        plot_cmd(plo2,"set out");
    fclose(plo2);

    free(y1);free(y2);free(xx);free(r);
    fftw_free(y1p);fftw_free(y2p);
    free(amp1);free(amp2);free(pha1);free(pha2);
    return 0;
}

int mice_ccor(
        double *data1, 
        double *data2, 
        int size, 
        int duration, 
        Gtype graphtype, 
        int outputf)
{
    //num of colume is sagments, num of raws is different days;
    int colum = 48*360 / duration;
    int  raw = 18;

    char outname[255];
    memset(outname,'\0',255);

    /* printf("colum: %d\nraw: %d\n",colum,raw); */
    /* printf("name %s/%s.%s\n",path,name,type); */
    double *xx,*yy, *y2,*r;
    xx = (double*)malloc(sizeof(double)*colum*raw);
    yy = (double*)malloc(sizeof(double)*colum*raw);
    y2 = (double*)malloc(sizeof(double)*colum*raw);
    r = (double*)malloc(sizeof(double)*2*colum*raw);

    for(int j=0;j<raw;j++){
        for(int i=0;i<colum;i++){
            xx[j*colum+i] = (i+0.5)*((double)duration/360.0); // hours
            yy[j*colum+i] = gsl_stats_mean(data1+(j*17280/*length of 2 days*/+i*duration),1,duration); //mean value for every 3 hours 
            y2[j*colum+i] = gsl_stats_mean(data2+(j*17280/*length of 2 days*/+i*duration),1,duration); //mean value for every 3 hours 
        }
    }

    //Wants to print the results into a file;
    FILE *output; 
    output = fopen("/tmp/mice_mean","w");

    int xtemp = colum*raw;
    for(int i=0;i<xtemp;i++){
        fprintf(output,"%.2f\t%.3f\t%.3f\n",xx[i],yy[i],y2[i]);
    }
    fclose(output);

    cross_corr(r,yy,y2,colum*raw);
    output = fopen("/tmp/mice","w");
    double xxtemp = xtemp*2.0;
    for(int i=0;i<xxtemp;i++){
        fprintf(output,"%.3f\t%.3f\n",(i-xtemp)*((double)duration/360.0),r[i]);
    }
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
    char cmd[255];

    //Check the output file name...
    if(!outputf)
        plot_cmd(plo,"set terminal qt size 500,400");
    else{
        sprintf(outname,"%s/%s_%s_CCOR_%d.pdf",path,name,type,duration);
        plot_cmd(plo,"set terminal pdf size 6.0,4.0");
        sprintf(cmd,"set output \"%s\"",outname);
        printf("%s\n",cmd);
        plot_cmd(plo,cmd);
    }

    plot_cmd(plo,"set grid");
    plot_cmd(plo,"set xlabel \"delay(h)\"");

    sprintf(cmd,"set title \" Cross-correlation of Temp and Act(%s)\"",name);
    plot_cmd(plo,cmd);
    plot_cmd(plo,"set ylabel \"CCOR\"");
    sprintf(cmd,"plot \'/tmp/mice\' with lines notitle");
    plot_cmd(plo,cmd);
    if(outputf)
        plot_cmd(plo,"set out");


    fclose(plo);

    free(y2);free(yy);free(xx);free(r);
    return 0;
}

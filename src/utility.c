/*************************************************************************
    > File Name: utility.c
    > Author: Yiwen Xue
    > Mail: 15225434259xue@gmail.com 
    > Created Time: Sun 19 May 2019 07:23:16 PM IDT
*************************************************************************/

#include "utility.h"

typedef enum{
    MEAN;
    STD;
    DFA;
}MDtype;

int dfa_fnsize(int num){
    return (int)(log10((double)num/5.0)*100);
}

int mice_dfa(double *data, int size, int order, int duration, MDtype mdtype, int ifgraph){
    int ssize = dfa_fnsize(1080);

    double *xx,*yy;
    xx = (double*)malloc(sizeof(double)*16*18);
    yy = (double*)malloc(sizeof(double)*16*18);

    //Prepare for dfa 
    double *fn,*n; double fit[2];
    fn = (double*)malloc(sizeof(double)*ssize);
    n = (double*)malloc(sizeof(double)*ssize);
    for(int i=0;i<ssize;i++){
        fn[i] = 0.0;
        n[i] = 0.0;
    }
    //Check parameter and do the analysis 
    if( mdtype == MEAN ){
        //
        for(int j=0;j<18;j++){
            for(int i=0;i<16;i++){
                xx[j*16+i] = i*3+1.5; // hours
                yy[j*16+i] = gsl_stats_mean(data+(j*17280+i*1080),1,1080); //mean value for every 3 hours 
            }
        }
    }else if( mdtype == STD ){
        //
        for(int j=0;j<18;j++){
            for(int i=0;i<16;i++){
                xx[j*16+i] = i*3+1.5; // hours
                yy[j*16+i] = gsl_stats_sd(data+(j*17280+i*1080),1,1080); //mean value for every 3 hours 
            }
        }
    }else if( mdtype == DFA ){
        //
        for(int j=0;j<18;j++){
            for(int i=0;i<16;i++){
                xx[j*16+i] = i*3+1.5; // hours
                yy[j*16+i] = detrend_flucuation(order,data+(j*17280+i*1080),1080,fn,n,&ssize,fit); //mean value for every 3 hours 
            }
        }
    }else 
        fprintf(stderr,"wrong type.\n");

    double meany[16],devy[16];
    for(int i=0;i<16;i++){
        meany[i] = 0.0;
        devy[i] = 0.0;
    }
    //calculation of mean value;
    for(int i=0;i<16;i++){
        for(int j=0;j<18;j++){
            meany[i] += yy[j*16+i];
        }
        meany[i] /= 18.0;
    }
    double meant = 0.0;
    for(int i=0;i<16;i++){
        meant += meany[i];
    }
    meant /= 16.0; 
    printf("meant = %f\n",meant);
    //calculation of deviation;
    for(int i=0;i<16;i++){
        /* for(int j=0;j<18;j++){ */
            devy[i] = meany[i] - meant;
        /* } */
        /* devy[i] /= 18.0; */
        /* devy[i] = sqrt(devy[i]); */
        devy[i] /= meany[i] /100.0;
    }
    //finished !!! 

    //Wants to print the results into a file;
    FILE *output; 
    output = fopen("data","w");
    for(int i=0;i<16;i++){
        fprintf(output,"%.2f\t%.3f\n",i*3+0.5,devy[i]);
        /* fprintf(output,"%.2f\t%.3f\n",i*3+0.5,meany[i]); */
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
    plot_cmd(plo,"set terminal qt size 500,400");
    plot_cmd(plo,"set grid");
    plot_cmd(plo,"set ylabel \"\%Deviation from mean α\"");
    /* plot_cmd(plo,"set ylabel \"\%Deviation\""); */
    /* plot_cmd(plo,"set ylabel \"α\""); */
    plot_cmd(plo,"set xlabel \"time(h)\"");
    plot_cmd(plo,"set xtics 3");
    /* plot_cmd(plo,"set title \"dfa of temperature - group average\""); */
    /* plot_cmd(plo,"set title \"STD of Temperature - group average\""); */
    plot_cmd(plo,"set title \"STD of Activity - group average\"");
    plot_cmd(plo,"set xrange [0:48]");
    plot_cmd(plo,"plot \'data\' with lines lw 1 lt 5 notitle");

    free(yy);free(xx);free(x);free(y);free(fn);free(n);
    return 0;
}

int mean_dev(char *filename, int breakpoint, int order){
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

    char ofname[20];
    int tmp = lines - 17280;

    double *xx,*yy;
    xx = (double*)malloc(sizeof(double)*16*18);
    yy = (double*)malloc(sizeof(double)*16*18);
    for(int j=0;j<18;j++){
        for(int i=0;i<16;i++){
            xx[j*16+i] = i*3+0.5; // hours
            yy[j*16+i] = gsl_stats_mean(y+(j*17280+i*1080),1,1080); //mean value for every 3 hours 
        printf("%d = %f\n",i,yy[j*16+i]);
        }
    }
    double meany[16],devy[16];
    for(int i=0;i<16;i++){
        meany[i] = 0.0;
        devy[i] = 0.0;
    }
    //calculation of mean value;
    for(int i=0;i<16;i++){
        for(int j=0;j<18;j++){
            meany[i] += yy[j*16+i];
        }
        meany[i] /= 18.0;
    }
    //calculation of deviation;
    for(int i=0;i<16;i++){
        for(int j=0;j<18;j++){
            devy[i] += pow(yy[j*16+i]-meany[i],2.0);
        }
        devy[i] /= 18.0;
        devy[i] = sqrt(devy[i]);
        devy[i] /= meany[i];
    }
    //finished !!! 

    //Wants to print the results into a file;
    FILE *output; 
    output = fopen("data","w");
    for(int i=0;i<16;i++){
        fprintf(output,"%.2f\t%.3f\n",i*3+0.5,devy[i]);
        //fprintf(output,"%.2f\t%.3f\n",i*3+0.5,meany[i]);
    }
    fclose(output);

    FILE *plo; 
    plo = gnuplot_create_t();
    if(plo == NULL){
        fprintf(stdout,"Error when call gnuplot.\n");
        return 0;
    }
    char cmd[255];
    plot_cmd(plo,"set terminal qt size 500,400");
    plot_cmd(plo,"set grid");
    plot_cmd(plo,"set ylabel \"Deviation\%\"");
    plot_cmd(plo,"set xlabel \"time(h)\"");
    plot_cmd(plo,"set xtics 3");
    plot_cmd(plo,"set title \"What should the title be\"");
    plot_cmd(plo,"set xrange [0:48]");
    plot_cmd(plo,"plot \'data\' with lines lw 1 lt 5");

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


#include "usage.h"

void mice_dfa(int order, 
        double *data,
        double *fit, 
        int head
        )
{
    FILE* output;
    output = fopen("data","w");

    int step = 1080;
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

    int step = 1080;
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
    /* fit_sin4(func,x,std,size); */
    free(x);
    free(std);
    fclose(output);
}

void mice_mean(double *func,
        double *data,
        int line
        )
{
    int step = 1080;
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
    /* fit_sin4(func,x,means,size); */
    free(x);
    free(means);
    fclose(output);
}

int dfa_mean_dev(char *filename, int breakpoint, int order){
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
    int ssize = (int)(log10((double)1080/5.0)*100);
    double *xx,*yy;
    xx = (double*)malloc(sizeof(double)*16*18);
    yy = (double*)malloc(sizeof(double)*16*18);
    double *fn,*n; double fit[2];
    fn = (double*)malloc(sizeof(double)*ssize);
    n = (double*)malloc(sizeof(double)*ssize);
    for(int i=0;i<ssize;i++){
        fn[i] = 0.0;
        n[i] = 0.0;
    }

    order = 1;
    for(int j=0;j<18;j++){
        for(int i=0;i<16;i++){
            xx[j*16+i] = i*3+1.5; // hours
            yy[j*16+i] = detrend_flucuation(order,y+(j*17280+i*1080),1080,fn,n,&ssize,fit); //mean value for every 3 hours 
            /* yy[j*16+i] = gsl_stats_mean(y+(j*17280+i*1080),1,1080); //mean value for every 3 hours */ 
            /* yy[j*16+i] = gsl_stats_sd(y+(j*17280+i*1080),1,1080); //mean value for every 3 hours */ 
            /* printf("%d = %f\n",i,yy[j*16+i]); */
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
    //
    double func[5];
    cosinor(xx,meany,16,func,5);

    //Wants to print the results into a file;
    FILE *output; 
    output = fopen("data","w");
    for(int i=0;i<16;i++){
        /* fprintf(output,"%.2f\t%.3f\n",(i+0.5)*3.0,devy[i]); */
        fprintf(output,"%.2f\t%.3f\n",i*3+0.5,meany[i]);
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
    plot_cmd(plo,"set ylabel \"\%Deviation from mean α\"");
    /* plot_cmd(plo,"set ylabel \"\%Deviation\""); */
    /* plot_cmd(plo,"set ylabel \"α\""); */
    plot_cmd(plo,"set xlabel \"time(h)\"");
    plot_cmd(plo,"set xtics 3");
    /* plot_cmd(plo,"set title \"dfa of temperature - group average\""); */
    /* plot_cmd(plo,"set title \"STD of Temperature - group average\""); */
    plot_cmd(plo,"set title \"STD of Activity - group average\"");
    plot_cmd(plo,"set xrange [0:48]");
    /* plot_cmd(plo,"plot \'data\' with lines lw 1 lt 5 notitle"); */
    double t = (M_PI/12);
    sprintf(cmd,"plot \'data\' with points ps 1.5 notitle, %lf+%lf*cos( %lf*x + %lf ) + %lf*cos(%lf*x + %lf) title \'Fit\'",
            func[0],func[1],t,func[2],func[3],2*t,func[4]);
    plot_cmd(plo,cmd);

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

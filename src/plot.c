/*************************************************************************
    > File Name: plot.c
    > Author: Yiwen Xue
    > Mail: 15225434259xue@gmail.com 
    > Created Time: Thu 18 Apr 2019 12:41:54 PM IDT
*************************************************************************/

#include "plot.h"

/* A wrap for gnuplot */
FILE* gnuplot_create_t(){
    FILE* gnuplotPipe = popen("gnuplot -persistent","w");
    return gnuplotPipe;
}

void plot_cmd(FILE* gnuplotPipe, char* command){
    fprintf(gnuplotPipe,"%s \n",command);
}

/* Used to plot a mean standard deviation plot for each segment; */
bool plot_mean(double *func,char *filename ,int width,int height){
    FILE* plo; 
    plo = gnuplot_create_t();
    if(plo == NULL){
        fprintf(stderr,"Unable to access gnuplot");
        return false;
    }
    char cmd[255];
    sprintf(cmd,"set terminal qt size %d,%d",width,height);
    plot_cmd(plo,cmd);
    plot_cmd(plo,"set xlabel \'Phase(hour)\'");
    plot_cmd(plo,"set ylabel \'mean\'");
    plot_cmd(plo,"set grid");
    plot_cmd(plo,"set title \'Mean\'");
    sprintf(cmd,"plot \'%s\' using 1:2 with  points pointsize 1 pointtype 5 title \'Mean\'",filename);//,%lf+%lf*sin(x/15*2*pi)+%lf*cos(x/15*2*pi)+%lf*sin(2*x/15*2*pi)+%lf*cos(2*x/15*2*pi) title \'Fit\'",
            //filename,func[0],func[1],func[2],func[3],func[4]);
    plot_cmd(plo,cmd);

    fclose(plo);
    return true;
}
/* Used to plot a mean standard deviation plot for each segment; */
bool plot_std(double *func,char *filename ,int width,int height){
    FILE* plo; 
    plo = gnuplot_create_t();
    if(plo == NULL){
        fprintf(stderr,"Unable to access gnuplot");
        return false;
    }
    char cmd[255];
    sprintf(cmd,"set terminal qt size %d,%d",width,height);
    plot_cmd(plo,cmd);
    plot_cmd(plo,"set xlabel \'Phase(hour)\'");
    plot_cmd(plo,"set ylabel \'Standard deviation\'");
    plot_cmd(plo,"set grid");
    plot_cmd(plo,"set title \'Standard deviation\'");
    sprintf(cmd,"plot \'%s\' using 1:2 with  points pointsize 1 pointtype 5 title \'STD\',%lf+%lf*sin(x/15*2*pi)+%lf*cos(x/15*2*pi)+%lf*sin(2*x/15*2*pi)+%lf*cos(2*x/15*2*pi) title \'Fit\'",
            filename,func[0],func[1],func[2],func[3],func[4]);
    plot_cmd(plo,cmd);

    fclose(plo);
    return true;
}

/* Used to plot a diagram of dfa exponent for each segment */
bool plot_dfas(char *filename,int order,int width,int height){
    FILE* plo; 
    plo = gnuplot_create_t();
    if(plo == NULL){
        fprintf(stderr,"Unable to access gnuplot");
        return false;
    }
    char cmd[255];
    sprintf(cmd,"set terminal qt size %d,%d",width,height);
    plot_cmd(plo,cmd);
    plot_cmd(plo,"set xlabel \'Phase(hour)\'");
    plot_cmd(plo,"set ylabel \'α\'");
    plot_cmd(plo,"set grid");
    sprintf(cmd,"set title \'DFA%d\'",order);
    plot_cmd(plo,cmd);
    sprintf(cmd,"plot \'%s\' with points ps 2 pt 5 title \'data\' ",filename);
    plot_cmd(plo,cmd);

    fclose(plo);
    return true;
}

/* Used to plot a diagram of dfa fit function for a single segment */
bool plot_dfa(char *filename,double *fit,int order,int width,int height){
    FILE* plo; 
    plo = gnuplot_create_t();
    if(plo == NULL){
        fprintf(stderr,"Unable to access gnuplot");
        return false;
    }
    char cmd[255];
    sprintf(cmd,"set terminal qt size %d,%d",width,height);
    plot_cmd(plo,cmd);
    plot_cmd(plo,"set logscale x 10");
    plot_cmd(plo,"set ylabel \'log_{10}(fn)\'");
    plot_cmd(plo,"set xlabel \'n\'");
    plot_cmd(plo,"set grid");
    sprintf(cmd,"set title \'DFA%d\'",order);
    plot_cmd(plo,cmd);
    sprintf(cmd,"plot \'%s\' with points ps 2 pt 5 title \'α=%lf\' , %.10f*log10(x)+%.10f with lines lw 2 notitle",filename,fit[1],fit[1],fit[0]);
    plot_cmd(plo,cmd);

    fclose(plo);
    return true;
}


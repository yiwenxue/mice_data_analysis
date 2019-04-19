/*************************************************************************
    > File Name: bar.h
    > Author: Yiwen Xue
    > Mail: 15225434259xue@gmail.com 
    > Created Time: Sat 16 Mar 2019 12:55:37 PM IST
*************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <string.h>

int                             //return the max width of terminal in the current frame 
progressbar(char *head,         //head title of this bar 
        int percentage,         //percentage of the process 
        int barlen              //max width of the terminal in the last frame 
)
{
    struct winsize w;
    char tail[20]={};
    sprintf(tail,"%d",(int)percentage);
    strcat(tail,"\%");

    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);

    if(barlen != w.ws_col ){
        printf("\n");
        barlen = w.ws_col;
    }

    barlen -= strlen(head);
    barlen --;//the character '['
    barlen -= strlen(tail);
    barlen --;//the character ']' 
    barlen --; //the last character '\%'
    barlen -= 1;
    //printf ("\nstrlen(tail)= %d\t|\tbarlen= %d\n",strlen(tail),barlen);
    printf("%s[",head);

    int tmp = barlen * percentage / 100;
    for(int j=0;j<tmp;j++){
        printf("#");
    }
    if((int)percentage<100){
        printf(">");
    }
    for(int j=tmp+1;j<barlen;j++){
        printf(" ");
    }
    printf("]%s",tail);
    printf("\r");
    fflush(stdout);
    return w.ws_col;
}

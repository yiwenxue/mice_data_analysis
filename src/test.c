/*************************************************************************
    > File Name: test.c
    > Author: Yiwen Xue
    > Mail: 15225434259xue@gmail.com 
    > Created Time: Wed 17 Jul 2019 10:47:53 AM CST
*************************************************************************/

#include<stdio.h>
#include<string.h>
#include<stdlib.h>

int main(int argc,char **argv){
    char *file;
    file = argv[1];
    char cmd[1000];
    sprintf(cmd,"../build//main -f %s -T MICE_DATA_38 -d 1 -t GAVERAGE -D 1080 -o null ",file);
    system(cmd);
    sprintf(cmd,"../build//main -f %s -T MICE_DATA_38 -d 1 -t GMID -D 1080 -o null ",file);
    system(cmd);
    sprintf(cmd,"../build//main -f %s -T MICE_DATA_38 -d 1 -t GAVERAGE -D 360 -o null ",file);
    system(cmd);
    sprintf(cmd,"../build//main -f %s -T MICE_DATA_38 -d 1 -t GMID -D 360 -o null ",file);
    system(cmd);
    sprintf(cmd,"../build//main -f %s -T MICE_DATA_38 -d 1 -t GAVERAGE -D 180 -o null ",file);
    system(cmd);
    sprintf(cmd,"../build//main -f %s -T MICE_DATA_38 -d 1 -t GMID -D 180 -o null ",file);
    system(cmd);

    sprintf(cmd,"../build//main -f %s -T MICE_DATA_38 -m -t GAVERAGE -D 1080 -o null ",file);
    system(cmd);
    sprintf(cmd,"../build//main -f %s -T MICE_DATA_38 -m -t GMID -D 1080 -o null ",file);
    system(cmd);
    sprintf(cmd,"../build//main -f %s -T MICE_DATA_38 -m -t GAVERAGE -D 360 -o null ",file);
    system(cmd);
    sprintf(cmd,"../build//main -f %s -T MICE_DATA_38 -m -t GMID -D 360 -o null ",file);
    system(cmd);
    sprintf(cmd,"../build//main -f %s -T MICE_DATA_38 -m -t GAVERAGE -D 180 -o null ",file);
    system(cmd);
    sprintf(cmd,"../build//main -f %s -T MICE_DATA_38 -m -t GMID -D 180 -o null ",file);
    system(cmd);

    sprintf(cmd,"../build//main -f %s -T MICE_DATA_38 -s -t GAVERAGE -D 1080 -o null ",file);
    system(cmd);
    sprintf(cmd,"../build//main -f %s -T MICE_DATA_38 -s -t GMID -D 1080 -o null ",file);
    system(cmd);
    sprintf(cmd,"../build//main -f %s -T MICE_DATA_38 -s -t GAVERAGE -D 360 -o null ",file);
    system(cmd);
    sprintf(cmd,"../build//main -f %s -T MICE_DATA_38 -s -t GMID -D 360 -o null ",file);
    system(cmd);
    sprintf(cmd,"../build//main -f %s -T MICE_DATA_38 -s -t GAVERAGE -D 180 -o null ",file);
    system(cmd);
    sprintf(cmd,"../build//main -f %s -T MICE_DATA_38 -s -t GMID -D 180 -o null ",file);
    system(cmd);

    sprintf(cmd,"../build//main -f %s -T MICE_DATA_38 -m -t GSYNC -D 180 -o null ",file);
    system(cmd);
    sprintf(cmd,"../build//main -f %s -T MICE_DATA_38 -m -t GSYNC -D 360 -o null ",file);
    system(cmd);
    sprintf(cmd,"../build//main -f %s -T MICE_DATA_38 -m -t GSYNC -D 1080 -o null ",file);
    system(cmd);

    sprintf(cmd,"../build//main -f %s -T MICE_DATA_38 -m -t GCCOR -D 180 -o null ",file);
    system(cmd);
    sprintf(cmd,"../build//main -f %s -T MICE_DATA_38 -m -t GCCOR -D 360 -o null ",file);
    system(cmd);
    sprintf(cmd,"../build//main -f %s -T MICE_DATA_38 -m -t GCCOR -D 1080 -o null ",file);
    system(cmd);

    return 0;
}

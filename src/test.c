/*************************************************************************
    > File Name: run.c
    > Author: Yiwen Xue
    > Mail: 15225434259xue@gmail.com 
    > Created Time: Mon 24 Jun 2019 05:09:20 PM IDT
*************************************************************************/

#include<stdio.h>
#include<dirent.h>
#include<string.h>
#include "utility.h"
struct{
    char name[255];
    char type[255];
    char path[255];
}mice_info;
typedef struct{
    char name[255];
}NAME;

char *doc_head = "\\documentclass{article} \n\\usepackage{indentfirst} \n\\usepackage{graphicx} \n\\usepackage{amsmath} \n\\usepackage{listings} \n\\usepackage{url} \n\\usepackage{tikz} \n\\usepackage[left=2cm, right=2cm, top=2cm]{geometry} \n\\title{Report of Mice 12Otx} \n\\date{} \n\\begin{document} \n \n";
char *doc_pic_1 = "\\begin{figure}[htp]\n    \\begin{minipage}{0.5\\linewidth}\n        \\centering \n        \\includegraphics[width = 0.8\\linewidth]{";
char *doc_pic_2 = "}\n    \\end{minipage}\n    \\begin{minipage}{0.5\\linewidth}\n        \\centering \n        \\includegraphics[width = 0.8\\linewidth]{";
char *doc_pic_3 = "}\n    \\end{minipage}\n\\end{figure}\n";

int doc_init(FILE *ifstream){
    fprintf(ifstream,"%s",doc_head);
    return 0;
}

int doc_pic(FILE *ifstream, char *name){
    char temp1[255],temp2[255];
    strcpy(temp1,"./");strcpy(temp2,"./");
    strcat(temp1,name);strcat(temp2,name);
    strcat(temp1,"_Temperature_Mean.pdf");strcat(temp2,"_Temperature_dfa1.pdf");
    fprintf(ifstream,"%s%s%s%s%s\n\n",doc_pic_1,temp1,doc_pic_2,temp2,doc_pic_3);

    strcpy(temp1,"./");strcpy(temp2,"./");
    strcat(temp1,name);strcat(temp2,name);
    strcat(temp1,"_Activity_Mean.pdf");strcat(temp2,"_Activity_STD.pdf");
    fprintf(ifstream,"%s%s%s%s%s\n\n",doc_pic_1,temp1,doc_pic_2,temp2,doc_pic_3);

    fprintf(ifstream,"%s./%s_Activity_dfa1.pdf%s\n\n",doc_pic_1,name,doc_pic_3);
    return 0;
}


int main(int argc, char **argv){
    struct dirent *de;
    DIR *dr = opendir(argv[1]);
    if(dr == NULL){
        printf("couldn't open current dir\n");
        return 0;
    }

    int count = 0;
    char *name;
    name = (char *)malloc(sizeof(char)*1024);
    memset(name,'\0',sizeof(name));
    char *path = argv[1];

    char *point;char check[255];int i=0;
    while((de = readdir(dr)) != NULL){
        memset(check,'\0',sizeof(check));
        if(strstr(de->d_name,"txt") != NULL){
            mice_name(de->d_name,mice_info.path,mice_info.name,mice_info.type);
            if(strcmp(mice_info.type,"Temperature") == 0 || strcmp(mice_info.type,"Activity") == 0 ){
                printf("%s.%s\n",mice_info.name,mice_info.type);
                i=0;
                while(1){
                    point = strstr(name+i,mice_info.name);
                    if(point == NULL ){
                        strcat(name,mice_info.name);
                        strcat(name," ");
                        break;
                    }else {
                        i = point - name;
                        sscanf(name+i,"%s",check);
                        if(strcmp(check,mice_info.name)==0 ){
                            break;
                        }
                        i = strlen(check) + 1;
                    }
                    printf("i = %d\n",i);
                }
            }
        }
        memset(mice_info.name,'\0',sizeof(mice_info.name));
        memset(mice_info.type,'\0',sizeof(mice_info.type));
        memset(mice_info.path,'\0',sizeof(mice_info.path));
    }
    /* strcat(name," "); */
    closedir(dr);
    printf("%s\n",name);
    char temp[255];
    char *ptr;
    ptr = name;
    char sdoc[255];
    strcpy(sdoc,path);
    strcat(sdoc,"/");
    strcat(sdoc,"main.tex");

    fprintf(stdout,"%s\n",sdoc);
    FILE *doc;
    doc = fopen(sdoc,"w");
    if(doc == NULL){
        fprintf(stderr,"Cant operate this file.\n");
        return -1;
    }
    doc_init(doc);

    char cmd[512];
    char stemp[512];
    strcpy(stemp,"./main -f ");
    strcat(stemp,path);
    strcat(stemp,"/");
    while(sscanf(name,"%s",temp) != 0 && strlen(temp) != 0){
        /* printf("%s/%s \tstrlen = %d\n",path,temp,strlen(temp)); */

        strcpy(cmd,stemp);
        strcat(cmd,temp);
        strcat(cmd,".Temperature.txt -m");
        printf("%s\n",cmd);
        system(cmd);
        system("sleep 0.1");

        strcpy(cmd,stemp);
        strcat(cmd,temp);
        strcat(cmd,".Temperature.txt -d 1");
        printf("%s\n",cmd);
        system(cmd);
        system("sleep 0.1");

        strcpy(cmd,stemp);
        strcat(cmd,temp);
        strcat(cmd,".Activity.txt -m");
        printf("%s\n",cmd);
        system(cmd);
        system("sleep 0.1");

        strcpy(cmd,stemp);
        strcat(cmd,temp);
        strcat(cmd,".Activity.txt -s");
        printf("%s\n",cmd);
        system(cmd);
        system("sleep 0.1");

        strcpy(cmd,stemp);
        strcat(cmd,temp);
        strcat(cmd,".Activity.txt -d 1");
        printf("%s\n",cmd);
        system(cmd);
        system("sleep 0.1");

        doc_pic(doc,temp);
        fprintf(doc,"\\newpage\n");
        /* move pointer */
        name += (strlen(temp) +1);
        /* printf("%s\n",name); */
        memset(temp,'\0',sizeof(temp));
    }

    fprintf(doc,"\\end{document}");
    fclose(doc);
    free(ptr);
    return 0;
}

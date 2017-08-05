
#ifndef  __LOG__H
#define  __LOG__H



#include <stdio.h>
#include <time.h>
static void  logOut(char flag[],char name[]){
    //生成日志
    //从原函数获得文件名和事件信息
    //获取服务器名称，系统时间
    //每天以日期命名log，追加写数据
    FILE *fp;
    char file_path[80];
    time_t t;
    struct tm *lt;
    time(&t);
    lt=localtime(&t);
    int year=lt->tm_year+1900;
    
    //输出到相应路径，以时间命名
    sprintf(file_path,"/home/jackwu/Desktop/test/%d-%d-%d.log",lt->tm_year+1900, lt->tm_mon, lt->tm_mday);
    
    fp=fopen(file_path,"a");
    fprintf(fp,"servername=%s&ip=127.0.0.1&time=%d-%d-%d %d:%d:%d&info=%s&fileortask=%s\n","01",year, lt->tm_mon, lt->tm_mday, lt->tm_hour, lt->tm_min, lt->tm_sec,flag,name);
    fclose(fp);

}

extern void log_err(char *flag,char *name){
       
       if(strcasecmp(flag,"010")==0){
              logOut("010",name);
       }else if(strcasecmp(flag,"011")==0){
              logOut("011",name);
       }
      
}


#endif
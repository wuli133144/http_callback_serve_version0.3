
#ifndef HTTP_TYPE_H

#define HTTP_TYPE_H

#undef BUFSIZE
#define BUFSIZE 4096
#define READ_COUNT        100

#define  READ_COUNT_BUF   1000
#define LINE_NUM    10

#include <errno.h>
#include <stdlib.h>
#include<string.h>


/*http contains*/
typedef struct __head__{
     char host[BUFSIZE];/*serve name*/
    char Conn_status[BUFSIZE];/*serve con_status*/
    char  Upgrade_Insecure_Requests;/*requ*/
    char user_agent[BUFSIZE];/*user agent*/
    char accept[BUFSIZE];/*accept*/
    char accept_encoding[BUFSIZE];/*encoding*/
    char accept_lauguage[BUFSIZE];/*language*/
              
}http_head_contain;

typedef struct head {
    char header[BUFSIZE];
    char value[BUFSIZE];
}newhead;


/*http header*/
typedef struct __http_header{
    char method[BUFSIZE];
    char url[BUFSIZE];
    char version[BUFSIZE];
    newhead nhead[BUFSIZE];
    http_head_contain contain;/*http contain head*/
}http_request;


typedef struct __http_context{
           char *buf;
           http_request * (*parse_req)(char *);
}http_context;



int readline_buf(char **buf,char *usr,size_t maxlen){
           int i=0,len1,len2;

           if(*buf==NULL||(usr)==NULL){
               return (-1);
            }
            len1=strlen(*buf);
            len2=maxlen;
            (*buf)+=strspn(*buf," ");
           
            maxlen=len2>len1?len2:len1;
           
             usr[0]=*(*buf);  
            for(i=1;i<maxlen;i++){
               if(*(*buf+i)=='\n'&&( *(*buf+i-1)=='\r')){
                    usr[i-1]=0;
                    break;
               }else if(*(*buf+i)=='\n'&&( *(*buf+i+1)=='\r')){
                    
                       usr[i]=0;
                       return 0;
               }
               else if(*(*buf+i)=='\0'){
                   break;
               }
               usr[i]=*(*buf+i);        
            }
            usr[i]=0;
            *buf+=(i+1);
            return (i);
            
}


//分析报头。if转换
//分析几个报头，其他忽略
/*
void get_header(char *p,struct __head__ *header){
    
    if(strcasecmp(p, "Host")==0){
        strcpy(header->host, p);
        printf("host is %s",header->host);
    }
    if(strcasecmp(p, "Connection")==0)
        strcpy(header->Conn_status, p);
    if(strcasecmp(p, "Upgrade-Insecure-Requests")==0)
        strcpy(&(header->Upgrade_Insecure_Requests), p);
    if(strcasecmp(p, "User-Agent")==0)
        strcpy(header->user_agent, p);
    if(strcasecmp(p, "Accept")==0)
        strcpy(header->accept, p);
    if(strcasecmp(p, "Accept-Encoding")==0)
        strcpy(header->accept_encoding, p);
    if(strcasecmp(p, "Accept-Language")==0)
        strcpy(header->accept_lauguage, p);
    //不在报头的值将会被忽略
    //return 0;
}
 */

#include <stdlib.h>

http_request* analasy(char *buf){
    //解析行，解析头
    //处理报头结束
    //返回结构体
    
    printf("120>>[%s]\n",buf);
    //struct __head__ *header=(struct __head__*)malloc(sizeof(struct __head__));
    struct __http_header *headline=(struct __http_header*)malloc(sizeof(struct __http_header));
    //struct head nhead[BUFSIZE];
    
    char *p=NULL;
    int i=0;
    //strcat(buf," \r\n\r\n");
    //解析头
    
    __info();
    if(strstr(buf,"\r\n\r\n")==NULL){
        return NULL;
    }
   
    p=strtok(buf," ");
    //if(!strcmp(p,"")) return 0;
    strcpy(headline->method, p);
    
    p=strtok(NULL," ");
    //if(!strcmp(p,"")) return 0;
    //printf("url is :%s\n",p);
    
    strcpy(headline->url, p);
    
    
    p=strtok(NULL,"\r\n");
    //printf("method is %s\n",p);
    //if(!strcmp(p,"")) return 0;
    strcpy(headline->version, p);
    //解析头
    
    //printf("method2 is %s\n",headline->url);
    
    __info();
      while(1)
    {
        p=strtok(NULL,":");
        if(strncmp(p+1,"\r\n", 2)==0) break;
        
        strcpy(headline->nhead[i++].header,p+1);
        p=strtok(NULL,"\r\n");
        //if(p==NULL) break;
        
        strcpy(headline->nhead[i++].value, p+1);
       
        //printf("%s\n",nhead[i].value);
        
    }
     __info();

        if(strncmp(p+1,"\r\n", 1)==0) return headline;//成功
        return NULL;//失败
    
    return  NULL;
}





/*create http_context info*/
http_context *alloc_http_context(){
        
        http_context *item=NULL;
        if((item=(http_context *)malloc(sizeof(http_context)))==NULL){
             //perror("alloc_http_context error");
             goto err;
        }
        item->buf=(char *)malloc(sizeof(char)*BUFSIZE);
        if(item->buf==NULL){
               goto err;
        }
        item->parse_req=analasy;//append http parse
        return item;
        err:
          errno=(-1);
          return NULL; 
}

/*
GET /demLog.png HTTP/1.1
Host: localhost:8888
Connection: keep-alive
Upgrade-Insecure-Requests: 1
User-Agent: Mozilla/5.0 (Macintosh; Intel Mac OS X 10_12_6) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/59.0.3071.115 Safari/537.36
Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,image/apng
q=0.8
Accept-Encoding: gzip, deflate, br
Accept-Language: zh-CN,zh;q=0.8
*/

typedef    size_t (*func_t)(int ,char *,size_t);

size_t onRecv(int fd,char *buf,size_t len);
/*core calllback*/
http_request * callback(http_context *http_ctx,func_t f_recv,int fd,char *buf,size_t len){
             
                   //f_recv(fd,buf,len);//callback

                   onRecv(fd,buf,len);//callback
                       printf("1>>>>[%s]\n",buf);
                   snprintf(http_ctx->buf,BUFSIZE,"%s",buf);

                   printf("2>>>>[%s]\n",buf);
                   return http_ctx->parse_req(http_ctx->buf);
}                          


/*recv new data*/
size_t onRecv(int fd,char *buf,size_t len){
        size_t nleft=len;
        char cur_ptr[READ_COUNT_BUF];
        bzero(cur_ptr,READ_COUNT_BUF);
        char *ptr=cur_ptr;
        size_t nread=0;
         
        while(nleft>0){
                    memset(ptr,0,READ_COUNT_BUF);
                    printf("ccccccccccccccccccccccccccc\n");
                    if((nread=read(fd,ptr,nleft))<0){
                            if(errno==EAGAIN||errno==EWOULDBLOCK){
                                break;
                            }else if(errno==EINTR){
                                nread=0;
                                continue;
                            }
                            goto err;
                    }else if(nread==0){
                        break;//eof
                    }
                     nleft-=nread;
                     strncat(buf,ptr,strlen(ptr));
                     ptr+=nread;
                     
        }
        printf(" %d - xxxxxxxxxxxxxxxxxxx  %d[%s]\n", getpid(), len-nleft,buf);
         
         return len-nleft;
         err:
             errno=(-1);
             return errno;
}

typedef struct http__tcp__level{
        int fd;/*connected fd*/
        http_context *http_contxt;/*http_context object*/
        http_request * (*callback)(http_context *,func_t,int ,char *,size_t  );/*call back*/
        char  *buf;/*void buf*/
}http_tcp;


http_tcp * http_tcp_alloc(int fd){

       
        http_tcp *item=NULL;
       
        item=(http_tcp *)malloc(sizeof(http_tcp));
        if(item==NULL){
            goto err;
        }
        item->fd=fd;
        item->http_contxt=alloc_http_context();
        item->buf=malloc(sizeof(char)*BUFSIZE);
        item->callback=callback;
        
        
        if( item->http_contxt==NULL){
            goto err;
        }
        __info();
        return item;
        err:
           __info();
           perror("error");
           errno=(-1);
           return NULL;
}



/*****http parse arg****************/





/**************************************/



#endif 
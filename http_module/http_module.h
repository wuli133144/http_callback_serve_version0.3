
#ifndef ___HTTP__MODULE__H
#define ___HTTP__MODULE__H

#include "../inc/http_error.h"
#include "../inc/http_config.h"
#include "../utils/utils.h"
#include "../inc/log.h"
#include "../inc/http_types.h"

///////////////////////

#define  HTTP_ERROR_NOT_FIND      404
#define  HTTP_ERROR_METHOD        500
#define  HTTP_ERROR_REQUEST       400
#define  HTTP_SUCCESS             200
///////////////////////


typedef struct _context{
  int errtype;
  char filename[BUFSIZE];
}info_t;



/*@http handler start@*/
extern info_t * http_module_handler_request(int epollfd,void *arg);
extern void http_module_handler_response(http_request *req,int clientfd);
extern void http_module_error(FILE *arg);

 static ssize_t readline(int fd,void *buf,size_t maxlen);
 static ssize_t http_rcv(int fd,void *buf,size_t len);
 static ssize_t http_send(int fd,void *buf,size_t len);

/*@http handler end@*/
/*@param_accept@*/




#define SERVER_TYPE "httpd of file get and sent system"
#define BUFFSIZE 1024
#define PARTSIZE 512


static int first_gate=0;
int  accept_request(char *localPath){
    char buf[BUFFSIZE];
    char buffer[BUFFSIZE];

    char method[PARTSIZE];
    char url[PARTSIZE];
    struct stat myStat;
    int i=0,j=0;
    int nread=0;
    //测试数据解析，读取数据函数协调加
   strcpy(buf,localPath);
    
    while (!isspace(buf[j])&&i<PARTSIZE) {
        method[i++]=buf[j++];
    }
    method[i]='\0';//读取出get 或 post 方法
   
    i=0;
    while (isspace(buf[j])&&j<BUFFSIZE) {
        j++;
    }
    while (!isspace(buf[j])&&i<PARTSIZE) {
        url[i++]=buf[j++];
    }
    url[i]='\0';//读取出网络路径
   
  

    if (strcasecmp(method, "GET") && strcasecmp(method, "POST"))
    {
       
        localPath=NULL;
        return HTTP_ERROR_METHOD;
    }
   
    //获取本地路径，加上本地前缀
     sprintf(localPath, "/home/jackwu/Desktop/test%s", url);
    if(localPath[strlen(localPath)-1]=='/'){
        strcat(localPath, "index.html");
    }//获得本地文件路径，开始寻找文件
    if(stat(localPath, &myStat)==-1){
       
        //丢弃header信息
        localPath=NULL;
        return HTTP_ERROR_NOT_FIND;
        // error : 404
    }
   else{
        if((myStat.st_mode & S_IFMT)==S_IFDIR){//是个文件夹
            strcat(localPath, "/index.html");
            sprintf(localPath, "/home/jackwu/Desktop/test%s", url);

        }
        return HTTP_SUCCESS;
    }
}

#define BUFFER_SIZE 1024
#define PARTSIZE 512

void response_ok(int sockFd,char pathName[]){
    
    char buf[BUFFER_SIZE];
    long iDataNum;
    struct stat st;
    FILE *localFp;
    unsigned long num;
    char *name;
    
    printf("url %s\n",pathName);

    localFp=fopen(pathName, "rb");
    stat(pathName, &st);
    iDataNum=st.st_size;
    unsigned char byte[BUFFER_SIZE];
    name=strrchr(pathName,'/');
    name+=1;
    //while(1)
        sprintf(buf,"HTTP/1.0 200 OK \r\n");
        send(sockFd,buf,strlen(buf),0);
        sprintf(buf,"Content-Length: %ld\r\n",iDataNum);
        printf("num is %ld\n",iDataNum);
        send(sockFd,buf,strlen(buf),0);
        sprintf(buf,"Content-Type: application/octet-stream\r\n");
        send(sockFd,buf,strlen(buf),0);
        sprintf(buf,"Content-Disposition: filename=%s\r\n",name);
        send(sockFd,buf,strlen(buf),0);
        sprintf(buf, "Connection: close\r\n\r\n");
        send(sockFd, buf, strlen(buf), 0);
        
        while(!feof(localFp))
        {   
            num=fread(byte, 1, BUFFER_SIZE, localFp);
            http_send(sockFd, byte, num);
        }
        //send(conn, buffer, len, 0);
        if(ferror(localFp)){
            log_err("011",name);
        }
        log_err("010",name);
    
        fclose(localFp);       
}


//各种错误消息返回：
void unknow(int sockfd){
    //404
    char buf[BUFFSIZE];
    sprintf(buf,"HTTP/1.0 404 NOT FOUND\r\n");
    http_send(sockfd,buf,strlen(buf));
    sprintf(buf,SERVER_TYPE);
    http_send(sockfd,buf,strlen(buf));
    sprintf(buf,"Content-Type: text/html\r\n");
    http_send(sockfd,buf,strlen(buf));
    sprintf(buf, "Connection: close\r\n");
    send(sockfd, buf, strlen(buf), 0);
    sprintf(buf,"\r\n");
    http_send(sockfd,buf,strlen(buf));
    sprintf(buf, "<HTML><TITLE>Not Found</TITLE>\r\n");
    http_send(sockfd, buf, strlen(buf));
    sprintf(buf, "<BODY><P>The server could not fulfill\r\n");
    http_send(sockfd, buf, strlen(buf));
    sprintf(buf, "your request because the resource specified\r\n");
    http_send(sockfd, buf, strlen(buf));
    sprintf(buf, "is unavailable or nonexistent.\r\n");
    http_send(sockfd, buf, strlen(buf));
    sprintf(buf, "</BODY></HTML>\r\n");
    http_send(sockfd, buf, strlen(buf));
   
}


void error_method(int sockfd)
{
    char buf[BUFFSIZE];
    // 501  not post and get
    __info();
    sprintf(buf, "HTTP/1.0 501 Method Not Implemented\r\n");
    send(sockfd, buf, strlen(buf),0);
    __info();
    sprintf(buf, SERVER_TYPE);
    send(sockfd, buf, strlen(buf),0);
    sprintf(buf, "Content-Type: text/html\r\n");
    send(sockfd, buf, strlen(buf),0);
    sprintf(buf, "\r\n");
    send(sockfd, buf, strlen(buf),0);
    __info();
    sprintf(buf, "<HTML><HEAD><TITLE>Method Not Implemented\r\n");
    send(sockfd, buf, strlen(buf),0);
    sprintf(buf, "</TITLE></HEAD>\r\n");
    send(sockfd, buf, strlen(buf),0);
    sprintf(buf, "<BODY><P>HTTP request method not supported.\r\n");
    send(sockfd, buf, strlen(buf),0);
    sprintf(buf, "</BODY></HTML>\r\n");
    send(sockfd, buf, strlen(buf),0);
    
}

void bad_request(int sockfd)
{
    char buf[BUFFSIZE];
    // 400  请求有问题
    sprintf(buf, "HTTP/1.0 400 BAD REQUEST\r\n");
    http_send(sockfd, buf, sizeof(buf));
    sprintf(buf, "Content-type: text/html\r\n");
    http_send(sockfd, buf, sizeof(buf));
    sprintf(buf, "\r\n");
    http_send(sockfd, buf, sizeof(buf));
    sprintf(buf, "<P>Your browser sent a bad request, ");
    http_send(sockfd, buf, sizeof(buf));
    sprintf(buf, "such as a POST without a Content-Length.\r\n");
    http_send(sockfd, buf, sizeof(buf));
}

void Conti_request(int sockfd)
{
    char buf[BUFFSIZE];
    // 100 继续传递
    sprintf(buf, "HTTP/1.0 100 REQUEST not full\r\n");
    http_send(sockfd, buf, sizeof(buf));
    sprintf(buf, "Content-type: text/html\r\n");
    http_send(sockfd, buf, sizeof(buf));
    sprintf(buf, "\r\n");
    http_send(sockfd, buf, sizeof(buf));
    sprintf(buf, "<P>Your browser sent a missing data, ");
    http_send(sockfd, buf, sizeof(buf));
    sprintf(buf, "such as a POST without a Content-Length.\r\n");
    http_send(sockfd, buf, sizeof(buf));
}

void wrong_path(int sockfd)
{
    char buf[BUFFSIZE];
    // 303  换个位置
    sprintf(buf, "HTTP/1.0 303 BAD PATH\r\n");
    http_send(sockfd, buf, sizeof(buf));
    sprintf(buf, "Content-type: text/html\r\n");
    http_send(sockfd, buf, sizeof(buf));
    sprintf(buf, "\r\n");
    http_send(sockfd, buf, sizeof(buf));
    sprintf(buf, "<P>Your browser sent a wrong localpath, ");
    http_send(sockfd, buf, sizeof(buf));
    sprintf(buf, "such as a POST without a Content-Length.\r\n");
    http_send(sockfd, buf, sizeof(buf));
}


void toolongUrl(int sockfd)
{
    char buf[BUFFSIZE];
    // 411  请求有问题
    sprintf(buf, "HTTP/1.0 411 BAD REQUEST\r\n");
    http_send(sockfd, buf, sizeof(buf));
    sprintf(buf, "Content-type: text/html\r\n");
    http_send(sockfd, buf, sizeof(buf));
    sprintf(buf, "\r\n");
    http_send(sockfd, buf, sizeof(buf));
    sprintf(buf, "<P>Your browser sent a so long request that cant read full, ");
    http_send(sockfd, buf, sizeof(buf));
    sprintf(buf, "such as a POST without a Content-Length.\r\n");
    http_send(sockfd, buf, sizeof(buf));
}


void bad_http(int sockfd)
{
    char buf[BUFFSIZE];
    // 505  http协议不支持
    sprintf(buf, "HTTP/1.0 505 BAD REQUEST\r\n");
    http_send(sockfd, buf, sizeof(buf));
    sprintf(buf, "Content-type: text/html\r\n");
    http_send(sockfd, buf, sizeof(buf));
    sprintf(buf, "\r\n");
    http_send(sockfd, buf, sizeof(buf));
    sprintf(buf, "<P>Your http is not fit, ");
    http_send(sockfd, buf, sizeof(buf));
    sprintf(buf, "such as a POST without a Content-Length.\r\n");
    http_send(sockfd, buf, sizeof(buf));
}

/*@param_accept end@*/






static ssize_t  http_rcv(int fd,void *buf,size_t len){
       
       int nleft=len;
       void *cur_ptr=buf;
       ssize_t nread=0;
       while(nleft>0){
           
            if((nread=recv(fd,cur_ptr,nleft,0)<0)){
                  if(errno==EINTR){//intrupt
                    
                     nread=0;
                     continue;
                  }

                  //break;
                  return (-1);//error
            }else if(nread==0){//eof  file end
              break;
            }

           nleft-=nread;// socket buffer
           cur_ptr+=nread;               
       }
       return (cur_ptr-buf);
}


//signal(SIGPIPE,SIG_DFL);
static ssize_t http_send(int fd,void *buf,size_t len){
     int nleft=len;
     void *cur_ptr=buf;
     int nsend=0;

     while(nleft>0){
           if((nsend=send(fd,cur_ptr,nleft,0))<0){
                 if(errno==EINTR){
                    nsend=0;
                    continue;
                 }else if(errno==EAGAIN||errno==EWOULDBLOCK){
                     nsend=0;
                     continue;
                 }
                 return (-1);

           }else if(nsend==0){
                 break;
           }
           cur_ptr+=nsend;
           nleft-=nsend;
     }
       return (cur_ptr-buf);
}

static ssize_t readline(int fd,void *buf,size_t maxlen){
      size_t read_cnt=0;
      size_t i=0;
      char c,*cur_ptr=buf;
      
      for(;i<maxlen;i++){
         
           if((read_cnt=read(fd,&c,1))==1){//0k
                
                *cur_ptr++=c;
                if(c=='\n'){//line
                     break;
                }

           }else if(read_cnt==0){//eof
                
                 *cur_ptr='\0';
                  break;

           }else {
              if(errno==EINTR){
                  read_cnt=0;
                  continue;
              }else if(errno==EAGAIN){
                  read_cnt=0;
                  continue;
              }else if(errno==EWOULDBLOCK){
                   read_cnt=0;
                   continue;
              }
              return (-1);//errno
           }
      }
      *cur_ptr='\0';
      return (i-1);
}


/*************************************************************************/
/*************************************************************************/
/*
      GET / HTTP/1.1
      Host: www.baidu.com
      User-Agent: Mozilla/5.0 (Windows; U; Windows NT 5.1; en-US; rv:1.7.6)
      Gecko/20050225 Firefox/1.0.1
      Connection: Keep-Alive
*/

/*http handler*/
 info_t * http_module_handler_request(int epollfd,void *arg)
{

      int clientfd = *((int *)(arg));
      int ret_errno=0;
      int fd;
      int read_cnt;
      int write_cnt;
      char buf[BUFSIZE];
      char filename[BUFSIZE];
      char method[BUFSIZE];
      char content_type[BUFSIZE];

      zero_to_buffer(buf);
      zero_to_buffer(filename);
      zero_to_buffer(method);
      zero_to_buffer(content_type);
      ret_errno=accept_request(filename);
      
      info_t *pinfo=(info_t *)malloc(sizeof(info_t));
      strcpy(pinfo->filename,filename);
      pinfo->errtype=ret_errno;
     
       return pinfo;

}

/*http handler*/
/*
HTTP/1.1 200 OK
Date: Sat, 31 Dec 2005 23:59:59 GMT
Content-Type: text/html;charset=ISO-8859-1
Content-Length: 12
*/
/*geterror_type find error type example 404 200*/
int geterror_type(http_request *req){
         return accept_request(req->url);
}

void http_module_handler_response( http_request *req,int clientfd)
{    
        
           char *method=req->method;
           char buff_filename[BUFFSIZE];
           sprintf(buff_filename,"%s ",method);
           strcat(buff_filename,req->url);
            
            printf("###[%s]\n",buff_filename);
           int flag=accept_request(buff_filename);
          

           if(flag==HTTP_SUCCESS){
                 response_ok(clientfd,buff_filename);
           }else if(flag==HTTP_ERROR_NOT_FIND){
                   unknow(clientfd);
                   
           }else if(flag==HTTP_ERROR_METHOD){
                  error_method(clientfd);
           }else if(flag==HTTP_ERROR_REQUEST){
                 //
                 wrong_path(clientfd);
           }
          
}


/*@getfiletype end@*/
/*
 HTTP/1.1 400 Bad Request
Server: openresty
Date: Fri, 06 Jan 2017 03:06:53 GMT
Content-Type: text/html
Content-Length: 166
Connection: close
*/

/*@http error page return@*/
void http_module_error(FILE *fp)
{
      //int clientfd=*((int *)arg);
      char date_tmp[50];
      char date[50];
      int file_len;
      int len;
      int read_cnt;
      char buffer[BUFSIZE];
      char cont_len[50];
      char Connection[50];

      char protocol[] = " HTTP/1.1 400 bad Request \r\n";
      char serve[] = "openresty\r\n";

      zero_to_buffer(buffer);
      zero_to_buffer(date);
      zero_to_buffer(date_tmp);
      zero_to_buffer(cont_len);
      zero_to_buffer(Connection);

      get_current_time(date_tmp, sizeof(date));
      snprintf(date, strlen(date), "Date:%s\r\n", date);
      char Content_Type[] = "Content-Type:txt/html\r\n";
      // SCPY(cont_len,"Content-Length:")

      get_file_length(log_error_file, &len);
      snprintf(cont_len, strlen(cont_len), "Content-Length:%d\r\n", len);
      snprintf(Connection, strlen(Connection), "Connection: close\r\n");

      fputs(protocol, fp);
      fputs(serve, fp);
      fputs(date, fp);
      fputs(Content_Type, fp);
      fputs(cont_len, fp);
      fputs(Connection, fp);

      FILE *fp_error = fopen(log_error_file, "r");
      if (fp_error == NULL)
      {
            unix_error("fopen error!");
      }

      //    while((!feof(fp_error))&&(Fgets(fp_error,buffer,BUFSIZE))!=NULL){
      //            int len=strlen(buffer);
      //            fputs(buffer,fp);
      //            fseek(fp_error,0,SEEK_CUR);
      //            //fflush(fp);
      //    }
      while (!feof(fp_error))
      {
            if ((read_cnt = fread(buffer, BUFSIZE, 1, fp_error)) > 1)
            {
                  unix_error("fread error!");
            }
            else if (read_cnt == 1)
            { //eof
                  fwrite(buffer, strlen(buffer), 1, fp);
            }
      }
      fflush(fp);
      fclose(fp_error);
}
/*@http error page return end@*/
/*************************************************************************/



#endif

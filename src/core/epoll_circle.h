


#include"../../inc/http_task.h"
#include "../../http_module/http_module.h"
#include "../../utils/utils.h"


void sig_quit(int sig){
    exit(0);
    return ;
    
}

void sig_int_kill(int sig){
        exit(0);
        return ;
}



int g_count=0;

void sig_handler(int sig){
      g_count++;
      printf("gcount=%d\n",g_count);
      return ;

}  

int start(int argc, char **argv)
{

    /*@socket description@*/
    int listenfd;
    int clientfd;
    socklen_t client_len;
    struct sockaddr_in clientsock;
    /*@socket description@*/

    /*@epoll var@*/

    int epfd;
    int i,flag=1,len;
    struct epoll_event *pevent;
    struct epoll_event event;
    char buf[BUFSIZE];
    int qid;
    int unix_fd[2];
    int ret;
    
    /*@signal handler@*/
    //signal(SIGPIPE,SIG_IGN);
    //signal(SIGCHLD,SIG_DFL);
    signal(SIGINT,sig_int_kill);
    signal(SIGQUIT,sig_quit);
    /*@signal hander end@*/
   // Init_sockpair();
    /*@get socket filefd@*/

      
 
    // create pool
    ret=S_pipe(unix_fd);

     while(ret<0){
     ret=S_pipe(unix_fd);//create unix_do
    }

    init_manager_proc(unix_fd); //create processro pool   

    //printf("%s %s",sockpair.ip_addr,sockpair.port);
    listenfd = open_listenfd("127.0.0.1", "8080");
    Setnoblock(listenfd, O_NONBLOCK);
    if (setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &flag, len) ==-1){ 
        perror("setsockopt"); 
        exit(1); } 
    /*@get socket filefd end@*/
    /*@epoll @*/
    bzero(buf, BUFSIZE);
    event.data.fd = listenfd;
    event.events = EPOLLIN;
    pevent = (struct epoll_event *)Malloc(BUFSIZE * sizeof(struct epoll_event));
    epfd = Epoll_create(BUFSIZE);
    Epoll_ctl(epfd, EPOLL_CTL_ADD, listenfd, &event);
    /*@epoll end@*/

   
    while (1)
    {
          
         int event_cnt = Epoll_wait(epfd, pevent, BUFSIZE, -1);
         printf("%d",event_cnt);
        for (i = 0; i < event_cnt; i++)
        {
            if (pevent[i].events & EPOLLERR || pevent[i].events & EPOLLHUP)
            {
                free(pevent);
                close(listenfd);
                unix_error("there is something wrong with socketfd");
            }
            else if (pevent[i].data.fd == listenfd)
            {        
                   int write_cnt=0;
                   char buf[2];
                   while(1){
                      
                      clientfd = Accept(listenfd, (struct sockaddr *)&clientsock, (socklen_t *)&client_len);
                      if(clientfd==0){
                                printf("none connectfd\n");
                                break;
                      }else if(clientfd==-1){
                          printf("errno =%d %s\n",errno,strerror(errno));
                          unix_error("accept error");
                      }
                     
                        while((write_cnt=Sock_fd_write(unix_fd[1],buf,2,clientfd))<=0){
                           printf("error ocurred! in sock_fd_write\n");
                           Sock_fd_write(unix_fd[1],buf,2,clientfd);
                         }
                         printf("new connectfd count=%d\n",g_count++);

                   }    
                   
                   printf("up\n");
            }
            else if (pevent[i].events & EPOLLIN)
            {  
                event.events = EPOLLOUT | EPOLLET; //
                event.data.fd = clientfd;
                Epoll_ctl(epfd, EPOLL_CTL_ADD, clientfd, &event);
            }
            else if (pevent[i].events & EPOLLOUT)
            { 
            } 
            else
            {  
                free(pevent);
                close(clientfd);
                
            }
            printf("end\n");
        }

   }
          close(listenfd);

        return 0;
    
}
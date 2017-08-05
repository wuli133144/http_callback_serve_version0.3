#ifndef __HTTP__IPC__H
#define __HTTP__IPC__H

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <err.h>
#include <fcntl.h>
#include "http_error.h"
#include "list.h"

#define FILEPATH "."

#define MODE 1

#ifndef BUFSIZE
#define BUFSIZE 1024
#else
#undef BUFSIZE
#endif

#ifndef CHECK 
  #define  check(msg) do{ \
    fprintf(stderr, "time:%s:error::%s::%s::line number:%d:function:%s::filename:%s\n",__TIME__,msg, \
    strerror(errno),__LINE__,__func__,__FILE__);\
    exit(0);\
  }while(0)

 #define __info() do{\
  fprintf(stdout, "time:%s:line number:%d:function:%s::filename:%s\n",__TIME__\
   ,__LINE__,__func__,__FILE__);\
      }while(0)
#endif 

typedef struct _msg {
  long mtype;
  char mcontext[BUFSIZE];
  int pid;
} msg_t;




//arr[0]：main=======>master
//arr[1]:master=======>child
//arr[2]:child==error==>master|main===>master tell master exit

int arr[3];

// ftok;
key_t Ftok(int i) {
  key_t rval;
  if ((rval = ftok(FILEPATH, i)) == -1) {
      check("ftok");
  }
  return rval;
}

// int    msgget ( key_t  key , int  msgflg );
int open_queue() {
  int qid;
  if ((qid=msgget(Ftok(1), IPC_CREAT | 0666)) == -1) {
         check("msgget");
  }
 
  return qid;
}

int open_queue2(){
  int qid;
   if ((qid=msgget(Ftok(2), IPC_CREAT | 0666)) == -1) {
         check("msget2");
  }

  return qid;
  
}
int open_queue3(){
   int qid;
   if ((qid=msgget(Ftok(3), IPC_CREAT| 0666)) == -1) {
             check("msgget3");
             
  }
  return qid;
}

// int  msgsnd ( int msgid ,  struct msgbuf*msgp , int msgsz, int msgflg );

int send_queue(int msgid, msg_t *p) {
  int length, result;

  length = sizeof(msg_t) - sizeof(long);

  if ((result = msgsnd(msgid, p, length, 0) == -1)) {
  check("msgsnd");
  }
  return result;
}

// int  msgrcv( int  msgid , struct   msgbuf*  msgp ,  int msgsz ,  long msgtyp,
// int msgflg);

int rcv_queue(int msgid, msg_t *p, long type) {
  int length, result;
  length = sizeof(msg_t) - sizeof(long);
  if ((result = msgrcv(msgid, p, length, type, 0)) == -1) {
       printf("msgid=%d",msgid);
        check("msgrcv");
  }
  return result;
}

int peek_message( int qid, long type )
{
  int result, length;
  if((result = msgrcv( qid, NULL, 0, type, IPC_NOWAIT)) == -1)
  {
  if(errno == E2BIG)
  return(-1);
  }
  return(0);
}

/*@remove_message@*/
int remove_message( int msgqid)
{      
       int rval;
       if((rval=msgctl(msgqid,IPC_RMID,NULL)<0)){
            check("msgqid");
       }
       return rval;

}
/*@remove_message@*/


void create_queue(){
  int qid;
  qid=open_queue();
  arr[0]=qid;
  qid=open_queue2();
  arr[1]=qid;
  qid=open_queue3();
  arr[2]=qid;
}


void destroy_queue(){
    int i=0;
    for(;i<3;i++){
        printf("queue_id=%d\n",arr[i]);
        remove_message(arr[i]);
    }
}


/*sempore_t*/



#endif
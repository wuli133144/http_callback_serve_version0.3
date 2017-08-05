
#ifndef  IO_BASE__H
#define  IO_BASE__H

#include "sock.h"
#include "epoll_inc.h"
#include "../http_module/http_module.h"
// extern void http_module_handler_request(int fd,void *f);

#define FDSIZE 1000
#define EPOLLEVENTS 100
#define MAXSIZE 1024

static void do_epoll(int listenfd);

static void handle_events(int epollfd, struct epoll_event *events, int num,
                          int listenfd, char *buf);

static void handle_accpet(int epollfd, int listenfd);

static void do_read(int epollfd, int fd, char *buf);

static void do_write(int epollfd, int fd, char *buf);

static void add_event(int epollfd, int fd, int state);

static void modify_event(int epollfd, int fd, int state);

/*@http_delete_event start@*/
static void delete_event(int epollfd, int fd, int state);


int epollfd;
int flag_x=0;

static void do_epoll(int listenfd) {

 
  struct epoll_event events[EPOLLEVENTS];
  int ret;

  char buf[MAXSIZE];
  memset(buf, 0, MAXSIZE);
  if(flag_x==0)
      epollfd= Epoll_create(FDSIZE);
     flag_x++;

  printf("epoll flag_x=%d",flag_x);
  add_event(epollfd, listenfd, EPOLLIN|EPOLLET);
  for (;;) {

     ret = Epoll_wait(epollfd, events, EPOLLEVENTS, -1);
     handle_events(epollfd, events, ret, listenfd, buf);
  }
  close(epollfd);
}

static void handle_events(int epollfd, struct epoll_event *events, int num,
                          int listenfd, char *buf) {
  int i;
  int fd;

  for (i = 0; i < num; i++) {
    fd = events[i].data.fd;
      
    // if ((fd == listenfd) && (events[i].events & EPOLLIN))
    //  handle_accpet(epollfd, listenfd);
     if (events[i].events & EPOLLIN)
              do_read(epollfd, fd, buf);
      else if (events[i].events & EPOLLOUT)
              do_write(epollfd,fd,buf);
  }
}
static void handle_accpet(int epollfd, int listenfd) {
  int clifd;
  struct sockaddr_in cliaddr;
  socklen_t cliaddrlen;
  clifd = accept(listenfd, (struct sockaddr *)&cliaddr, &cliaddrlen);
  if (clifd == -1)
    perror("accpet error:");
  else {
    printf("accept a new client: %s:%d\n", inet_ntoa(cliaddr.sin_addr),
           cliaddr.sin_port);
    add_event(epollfd, clifd, EPOLLIN);
  }
}






static void do_read(int epollfd, int fd, char *buf) {
      http_module_handler_request(epollfd,(void *)&fd);     
}

static void do_write(int epollfd, int fd, char *buf) {
  int nwrite;
  
  nwrite = write(fd, buf, strlen(buf));

  if (nwrite == -1) {
    perror("write error:");
    close(fd);
    delete_event(epollfd, fd, EPOLLOUT);
  } else
    modify_event(epollfd, fd, EPOLLIN);
    memset(buf, 0, MAXSIZE);
}

static void add_event(int epollfd, int fd, int state) {
  struct epoll_event ev;
  ev.events = state;
  ev.data.fd = fd;
  epoll_ctl(epollfd, EPOLL_CTL_ADD, fd, &ev);
}

static void delete_event(int epollfd, int fd, int state) {
  struct epoll_event ev;
  ev.events = state;
  ev.data.fd = fd;
  Epoll_ctl(epollfd, EPOLL_CTL_DEL, fd, &ev);
}

static void modify_event(int epollfd, int fd, int state) {
  struct epoll_event ev;
  ev.events = state;
  ev.data.fd = fd;
  Epoll_ctl(epollfd, EPOLL_CTL_MOD, fd, &ev);
}

void http_do_epoll(int listenfd) { return do_epoll(listenfd); }

void http_handle_events(int epollfd, struct epoll_event *events, int num,
                        int listenfd, char *buf) {
  return handle_events(epollfd, events, num, listenfd, buf);
}

void http_handle_accpet(int epollfd, int listenfd) {
  return handle_accpet(epollfd, listenfd);
}

void http_do_read(int epollfd, int fd, char *buf) {
  return do_read(epollfd, fd, buf);
}

void http_do_write(int epollfd, int fd, char *buf) {
  return do_write(epollfd, fd, buf);
}

void http_add_event(int epollfd, int fd, int state) {
  return add_event(epollfd, fd, state);
}

void http_modify_event(int epollfd, int fd, int state) {
  return modify_event(epollfd, fd, state);
}

/*@http_delete_event start@*/
void http_delete_event(int epollfd, int fd, int state) {
  return delete_event(epollfd, fd, state);
}
/*@http_delete_event end @*/
#endif 
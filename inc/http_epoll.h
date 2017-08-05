#ifndef  HTTP__EPOLL_H
#define  HTTP__EPOLL_H
#include "http_io_base.h"
#include "../utils/utils.h"
#include "../http_module/http_module.h"

/**
 启动监听的功能
**/

/*@open listen start@*/
int Open_listen() {

  int listenfd;
  Init_sockpair();
  // printf("%s,%s",sockpair.ip_addr,sockpair.port);
  listenfd = open_listenfd((char *)sockpair.ip_addr, (char *)sockpair.port);

  return listenfd;
}
/*@open listen end@*/

/*@start epoll circle@*/

void Open_epoll(int connfd) { return http_do_epoll(connfd); }
/*@end@*/

void Add_event(int epollfd, int fd, int state) {
  return http_add_event(epollfd, fd, state);
}

void Modify_event(int epollfd, int fd, int state) {
    return http_modify_event(epollfd, fd, state);
}

/*@http_delete_event start@*/
void Delete_event(int epollfd, int fd, int state) {
  return http_delete_event(epollfd, fd, state);
}
/*@http_delete_event end @*/

// int main() {
//   struct sockaddr addr;
//   socklen_t len = sizeof(struct sockaddr);
//   int listen = Open_listen();
//   accept(listen, (struct sockaddr *)&addr, &len);
//   return 0;
// }

#endif
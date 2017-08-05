

#include "../inc/http_config.h"

#define BUFSIZE 1024
#define NR_PIPE 2

#define nullptr 0

#define LISTEN_NUM 10

typedef struct sockaddr_un UN_SOCK;
typedef struct sockaddr_in IN_SOCK;
typedef struct sockaddr SOCK;

#define OUT(MSG) fprintf(stdout, "%s\n", MSG)

#define PARA (int argc, char **argv)

#define err_exit(e)                                                            \
  \
do {                                                                           \
    perror(e);                                                                 \
    exit(0);                                                                   \
  \
}                                                                         \
  while (0)

#define err_dump(err)                                                          \
  do {                                                                         \
    fprintf(stderr, "%s %s", strerror(errno), err);                            \
    exit(0);                                                                   \
  \
} while (0)

#ifdef __SCP__N
#define SCPYN(a, b) strncpy(a, b, sizeof(a))

#else
#define SCPY(a, b) strcpy(a, b)
#define MMCP(a, b) memcpy(a, b, siseof(b))
#endif

#define Init_Buffer(buf)                                                       \
  char buf[BUFSIZE];                                                           \
  memset(buf, 0, sizeof(a))

#define EVER                                                                   \
  ;                                                                            \
  ;

/*@getpwuid get user info@*/
struct passwd *Getpwuid(int uid) {
  struct passwd *pw_st = NULL;
  if ((pw_st = getpwuid(uid)) == NULL) {
    err_exit("getpwuid");
  }
  return pw_st;
}
/*@getpwuid end@*/

/*@skip start@*/
// find position
static char *skip(char *start, char *op) {
  while (*start && strcpy(start, op) != 0) {
    start++;
  }
  if (*start != 0)
    start++;
  return start;
}
/*@skip end@*/

/*@getpwname get user info by username@*/
struct passwd *Getpwnam(const char *name) {
  struct passwd *pw_st = NULL;
  if ((pw_st = getpwnam(name)) == NULL) {
    err_exit("getpwuid");
  }
  return pw_st;
}

/*@getpwname end@*/

/*@getpwent start @*/
struct passwd *Getpwent() {
  struct passwd *pw_st = NULL;
  if ((pw_st = getpwent()) == NULL) {
    err_exit("getpwent");
  }
  return pw_st;
}

/*@getpwent end@*/

/*@getprent start*/

struct group *Getgrent() {
  struct group *pw_st = NULL;
  if ((pw_st = getgrent()) == NULL) {
    err_exit("getprent");
  }
  return pw_st;
}

/*@getprent end@*/

/*@Fork ()start@*/

pid_t Fork() {
  int pid;
  if ((pid = fork()) < 0) {
    err_dump("fork error!");
  }
  return pid;
}

/**@fork end@*/

/*@close start@*/
int Close(int fd) {
  int ret;
  if ((ret = close(fd)) != 0) {
    err_dump("close");
  }
  return ret;
}
/*@close end@*/

/*@dup2_stdin start@*/

int dup2_stdin(int fd) {
  if (fd != STDIN_FILENO) {

    if (dup2(fd, STDIN_FILENO) != STDIN_FILENO) {
      err_dump("dup2 stdin_fileno");
    }
    close(fd);
  }
  return 0;
}

/*@dup2_stdin end@*/

/*@dup2_stdout@*/
int dup2_stdout(int fd) {
  if (fd == STDOUT_FILENO) {
    if (dup2(fd, STDOUT_FILENO) != STDOUT_FILENO) {
      err_dump("dup2 stdout ");
    }
    close(fd);
  }
  return 0;
}

/*@dup2_stdout@*/

/*@pipe() start@*/
int Pipe(int fd[2]) {
  int ret;
  if ((ret = pipe(fd)) == -1) {
    err_dump("pipe error");
  }
  return ret;
}

/*@pipe()end@*/

/*@Socket start@*/

int Socket(int domain, int type, int prot) {
  int ret;
  if ((ret = socket(domain, type, prot)) < 0) {
    err_dump("socket error!");
  }
  return ret;
}

/*@socket end@*/
/*@connectg start@*/
int Connect(int socketfd, SOCK *addr, socklen_t len) {
  int ret;
  if ((ret = connect(socketfd, addr, len)) < 0) {
    err_dump("connect error");
  }
  return ret;
}
/*@connectg end@*/

/*@bind start@*/

int Bind(int socketfd, const SOCK *addr, socklen_t len) {
  int rval;
  if ((rval = bind(socketfd, addr, len)) == -1) {
    err_dump("bind error");
  }
  return rval;
}

/*@bing end@*/

/*@Listen start@*/
int Listen(int fd, int backlog) {
  int rval;
  if ((rval = listen(fd, backlog)) < 0) {
    err_dump("listen error");
  }
  return rval;
}
/*@Listen end@*/

/*@Ser_accept Start@*/

int Ser_accept(int listfd, struct sockaddr *addr, socklen_t *len) {
  int rval;
  if ((rval = accept(listfd, addr, len)) < 0) {
    err_dump("accept");
  }
  return rval;
}
/*@ser_accept end@*/

/*@Accept start@*/
int Accept(int listenfd, struct sockaddr *addr, socklen_t *len) {
  int rval;
  if ((rval = accept(listenfd, addr, len)) < 0) {
    err_dump("accept error!");
  }
  return rval;
}

/*@Accept end@*/

/*@memory operation@*/

void *Malloc(size_t sz) {
  void *prval = nullptr;
  if ((prval = malloc(sz)) == nullptr) {
    err_dump("malloc error!");
  }
  return prval;
}

/*@memory operation@*/

/*@open  error@*/
int Open(const char *fd, int type, int mode) {
  int rval;
  if ((rval = open(fd, type, mode)) < 0) {
    err_dump("open error");
  }
  return rval;
}
/*@open error@*/

/*@file operation start@*/

int Stat(const char *filename, struct stat *st) {
  int rval;
  if ((rval = stat(filename, st)) < 0) {
    err_dump("stat file failed");
  }
  return rval;
}

/*@read_s start@*/
size_t Read_s(int fd, void *buf, ssize_t sz) {
  size_t nleft;
  ssize_t nread;
  nleft = sz;

  while (nleft > 0) {

    if ((nread = read(fd, buf, nleft)) < 0) {
      if (nleft == 0) {
        return -1;
      } else {
        break;
      }
    } else if (nread == 0) {
      break; // eof
    } else {
      nleft -= nread;
      buf += nread;
    }
  }
  return sz - nleft;
}

/*@read end@*/

/*@write start@*/

size_t Write_s(int fd, void *buf, ssize_t sz) {
  size_t nwrite;
  ssize_t nleft;
  nleft = sz;
  while (nleft > 0) {

    if ((nwrite = write(fd, buf, nleft)) < 0) {
      if (nleft == 0) {
        return -1;
      } else
        break;
    } else if (nwrite == 0) {
      break; // eof
    } else {
      nleft -= nwrite;
      buf += nwrite;
    }
  }
  return sz - nleft;
}

/*@write end@*/

/*@file operation end@*/

int Fcntl(int fd, int cmd, struct flock *fl) {
  int rval;
  if ((rval = fcntl(fd, cmd, fl)) == -1) {
    err_dump("fcntl error");
  }
  return rval;
}
/*@flock file unlock@*/
#include <sys/mman.h>

/********************************memory*******************************/

/*@mmap start@*/
void *Mmap(void *addr, size_t len, int prot, int flag, int fd, off_t offset) {
  void *pval;
  if ((pval = mmap(addr, len, prot, flag, fd, offset)) == MAP_FAILED) {
    err_dump("mmap error!");
  }
  return pval;
}
/*@mamp end@*/

/*@munmap start@*/
int Munmap(void *ptr, size_t sz) {
  int ret;
  if ((ret = munmap(ptr, sz)) != 0) {
    err_dump("munmap error");
  }
  return ret;
}
/*@munmap end@*/

/*@msync start@*/
int Msync(void *addr, size_t len, int flag) {
  int ret;
  if ((ret = msync(addr, len, flag)) != 0) {
    err_dump("msync error");
  }
  return ret;
}

/*@msync end@*/
/*@mprotect start@*/
int Mprotect(void *addr, size_t len, int flag) {
  int ret;
  if ((ret = mprotect(addr, len, flag)) != 0) {
    err_dump("mprotect error");
  }
  return ret;
}
/*@mprotect end@*/

/********************************memory*******************************/

/*@@IPC@@*/

/*@Shm_open start@*/

int Shm_open(const char *name, int oflag, mode_t mode) {
  int rval;
  if ((rval = shm_open(name, oflag, mode)) < 0) {
    err_dump("shm_open error!");
  }
  return rval;
}
/*@Shm_open end@*/

/*@shm_unlink@*/

int Shm_unlink(const char *name) {
  int rval;
  if ((rval = shm_unlink(name)) < 0) {
    err_dump("shm_unlink error!");
  }
  return rval;
}

/*@shm_unlink@*/

/*@Ftruncate Start@*/
int Ftruncate(int fd, int length) {
  int rval;
  if ((rval = ftruncate(fd, length)) < 0) {
    err_dump("Ftruncate error!");
  }
  return rval;
}
/*@ftruncate end@*/

/*@setsid @*/
int Setsid(void) {
  int rval;
  if ((rval = setsid()) == -1) {
    err_dump("setsid");
  }
  return rval;
}
/*@end setsid@*/

/*@getrlimit get sys resource@*/

int Getrlimit(int res, struct rlimit *limit) {
  int rval;
  if ((rval = getrlimit(res, limit)) == -1) {
    err_dump("getrlimit");
  }
  return rval;
}
/*@getrlimit get sys resource@*/

/*@setrlimit start@*/
int Setrlimit(int res, const struct rlimit *limit) {
  int rval;
  if ((rval = setrlimit(res, limit)) == -1) {
    err_dump("setlimit");
  }
  return rval;
}
/*@setrlimit end@*/
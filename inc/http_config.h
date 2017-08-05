

#ifndef HTTP_CONFIG__H
#define HTTP_CONFIG__H

#include <time.h>
#include <stdarg.h>

#include <unistd.h>

#include <sys/types.h>

#include <signal.h>
#include <pthread.h>
#include <pwd.h>
#include <grp.h>

#include <sys/socket.h>
#include <sys/un.h>
#include <stddef.h>

#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/resource.h>



#define BUFSIZE 1024
#define SIZE 100

const char log_error_file[] = "../html/error.html";
const char conf[] = "../conf/config";
#endif 
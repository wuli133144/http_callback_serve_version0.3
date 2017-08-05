

#ifndef __UNIX_ERROR__H
#define __UNIX_ERROR__H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include<stdarg.h>
#include "http_ipc.h"



static void inline_unix_error(const char *msg) {
 
  fprintf(stderr, "time:%s:error::%s::%s::line number:%d:function:%s\n",__TIME__,msg, strerror(errno),__LINE__,__func__);
  exit(0);
}



void unix_error(const char *msg){
      inline_unix_error(msg);
}

#endif

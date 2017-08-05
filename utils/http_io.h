#ifndef __HTTP_IO_H
#define __HTTP_IO_H

  

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h>
#include<sys/types.h>
#include<errno.h>
#include<string.h>





ssize_t http_Readn(int fd,void *buffer,size_t len);
ssize_t http_Writen(int fd,void*buffer,size_t len);
ssize_t http_Fgets_s(int fd,void *buffer,size_t sz);
char *http_Fgets(char *buffer,size_t n,FILE *fp);

#endif
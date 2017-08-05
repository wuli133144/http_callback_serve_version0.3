

/*@
  standard io/rio operation
  if you offen work with net/io ,please use rio_t struct object
  it's more safer.
@*/

#define RIO_BUFSIZE 4096
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <string.h>

#define ngx_align(p) (char *)((NGX_ALIGN_CAST p + NGX_ALIGN) & ~NGX_ALIGN)

#define ngx_test_null(p, alloc, rc) \
    if ((p = alloc) == NULL)        \
    {                               \
        return rc;                  \
    }

// #define zero_test_null(p, func, rc) \
//     if ((p = func) < 0)             \
//     {                               \
//         perror(#func##failed);      \
//         exit(0);                    \
//     }

pid_t Fork()
{
    pid_t ret;
    if ((ret = fork()) < 0)
    {
        unix_error("unix error!");
    }
    return ret;
}

typedef struct
{

    int fd;
    int nread;
    char *ptr;
    char rio_buffer[RIO_BUFSIZE];
} rio_t;

/*@rio read@*/
void rio_readinit(rio_t *rp, int fd)
{
    rp->fd = fd;
    rp->nread = 0;
    rp->ptr = rp->rio_buffer;
}

static int rio_read(rio_t *rp, char *usrbuf, size_t sz)
{
    int cnt;

    while (rp->nread <= 0)
    { //rio lack data
        rp->nread = read(rp->fd, rp->rio_buffer, sizeof(rp->rio_buffer));
        if (rp->nread < 0) //intr
        {
            if (errno == EINTR)
            {
                break;
            }
            else
                return -1;
        }
        else if (rp->nread == 0)
        {
            return 0;
        }
        rp->ptr = rp->rio_buffer;
    }
    cnt = sz;
    if (rp->nread < cnt)
    {
        cnt = rp->nread;
    }
    memcpy(usrbuf, rp->rio_buffer, cnt);
    rp->ptr += cnt;
    rp->nread -= cnt;
    return cnt;
}

static int rio_write(int fd, char *usrbuf, size_t sz)
{

    int nleft = sz;
    char *ptr = usrbuf;
    int nwrite = 0;
    while (nleft > 0)
    {
        if ((nwrite = write(fd, ptr, nleft)) < 0)
        {
            if (errno == EINTR)
            {
                nwrite = 0;
            }
            else
            {
                return (-1);
            }
        }
        else if (nwrite == 0)
        { //eof
            break;
        }
        nleft -= nwrite;
        ptr += nwrite;
    }
    return sz - nleft;
}
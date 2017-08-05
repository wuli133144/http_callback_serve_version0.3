## About
*this project about http server can provide download files many times

## Features
* callback
* download files
* high concurrence


## Installation
Run:
```
git clone https://github.com/wuli133144/http_callback_serve_version0.3.git
```
and copy all files to you source code tree.
then go to ./src/core/ 
Run ```gcc serve_http.c -o start -lpthread``` to compile and run tests.

## Examples
### config
'''
go to conf/config modify it to yourself ipaddress
'''

```
Calling ```callback function() is core driver``` :  
```

...
some phony code:
   http_request *req=tcp->callback(tcp->http_contxt,                                 \
                                                  onRecv,tcp->fd,                    \
                                                  (char *)tcp->buf,READ_COUNT);
    ---------------------------------------------------------------------------------
    http_request * callback(http_context *http_ctx,func_t f_recv,int fd,char *buf,size_t len){
             
                   //f_recv(fd,buf,len);//callback

                   onRecv(fd,buf,len);//callback
                       printf("1>>>>[%s]\n",buf);
                   snprintf(http_ctx->buf,BUFSIZE,"%s",buf);

                   printf("2>>>>[%s]\n",buf);
                   return http_ctx->parse_req(http_ctx->buf);
}                          
                     
```

### config
install config file
'''
   vim conf/config like:
   @127.0.0.1:8080@
```

Output:
```
 
```

## Contributing

I will always merge *working* bug fixes. However, if you want to add something new to the API, please create an "issue" on github for this first so i can modify it if it should end up in the library before you start implementing it.
Remember to follow jackwu's code style and write appropriate tests.

## License
[The MIT License (MIT)](http://opensource.org/licenses/mit-license.php)


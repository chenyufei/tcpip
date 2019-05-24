#ifndef LST_TIMER
#define LST_TIMER

#include <time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_SIZE 64
class util_timer;
struct client_data
{
    struct sockaddr_in address;
    int sockfd;
    char buf[ BUFFER_SIZE ];
    util_timer* timer;
};

class util_timer
{
public:
    util_timer(); 

public:
   time_t expire; 
   void (*cb_func)( client_data* );
   client_data* user_data;
   util_timer* prev;
   util_timer* next;
};

class sort_timer_lst
{
public:
    sort_timer_lst();
    ~sort_timer_lst();
    void add_timer( util_timer* timer );
    void adjust_timer( util_timer* timer );
    void del_timer( util_timer* timer );
    void tick();

private:
    void add_timer( util_timer* timer, util_timer* lst_head );

private:
    util_timer* head;
    util_timer* tail;
};

#endif


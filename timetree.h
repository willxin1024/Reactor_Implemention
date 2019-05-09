#ifndef intIME_TREE
#define intIME_TREE

#include <iostream>
#include <time.h>
#include <map>

using namespace std;

typedef union {
    void *ptr;
    int fd;
    uint32_t u32;
    uint64_t u64;
} cb_data;

class heap_timer

{
public:
    heap_timer(int delay)
    {
        expire = time(NULL) + delay;
    }

public:
    time_t expire;
    void (*cb_func)(cb_data);
    cb_data user_data;
};

class time_heap
{
public:
    time_heap(int cap = 0)
    {
    }
    ~time_heap()
    {
    }

public:
    void add_timer(heap_timer *timer)
    {
        if (!timer)
        {
            return;
        }

        timer_manager.emplace(timer->expire, timer);
    }
    void del_timer(heap_timer *timer)
    {
        if (!timer)
        {
            return;
        }
        // lazy delelte
        timer->cb_func = NULL;
    }
    inline heap_timer *top() const
    {
        if (empty())
        {
            return NULL;
        }
        auto it = timer_manager.begin();
        return it->second;
    }
    void pop_timer()
    {
        if (empty())
        {
            return;
        }
        auto it = timer_manager.begin();
        timer_manager.erase(it);
    }
    void tick()
    {
        heap_timer *head_timer = top();
        time_t cur = time(NULL);
        while (!empty())
        {
            if (head_timer->expire > cur)
            {
                // 当前时间还未到定时器的时间，pass
                break;
            }
            if (head_timer->cb_func)
            {
                head_timer->cb_func(head_timer->user_data);
            }
            pop_timer();
            head_timer = top();
        }
    }
    bool empty() const { return timer_manager.empty(); }

private:
    map<time_t, heap_timer *> timer_manager;
    
};

#endif

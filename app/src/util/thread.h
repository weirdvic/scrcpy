#ifndef SC_THREAD_H
#define SC_THREAD_H

#include "common.h"

#include <stdbool.h>
#include <stdint.h>

/* Forward declarations */
typedef struct SDL_Thread SDL_Thread;
typedef struct SDL_mutex SDL_mutex;
typedef struct SDL_cond SDL_cond;

typedef int sc_thread_fn(void *);
typedef unsigned int sc_thread_id;

struct sc_thread {
    SDL_Thread *thread;
};

struct sc_mutex {
    SDL_mutex *mutex;
#ifndef NDEBUG
    sc_thread_id locker;
#endif
};

struct sc_cond {
    SDL_cond *cond;
};

bool
sc_thread_create(struct sc_thread *thread, sc_thread_fn fn, const char *name,
                 void *userdata);

void
sc_thread_join(struct sc_thread *thread, int *status);

bool
sc_mutex_init(struct sc_mutex *mutex);

void
sc_mutex_destroy(struct sc_mutex *mutex);

void
sc_mutex_lock(struct sc_mutex *mutex);

void
sc_mutex_unlock(struct sc_mutex *mutex);

sc_thread_id
sc_thread_get_id(void);

#ifndef NDEBUG
bool
sc_mutex_held(struct sc_mutex *mutex);
# define sc_mutex_assert(mutex) assert(sc_mutex_held(mutex))
#else
# define sc_mutex_assert(mutex)
#endif

bool
sc_cond_init(struct sc_cond *cond);

void
sc_cond_destroy(struct sc_cond *cond);

void
sc_cond_wait(struct sc_cond *cond, struct sc_mutex *mutex);

// return true on signaled, false on timeout
bool
sc_cond_timedwait(struct sc_cond *cond, struct sc_mutex *mutex, uint32_t ms);

void
sc_cond_signal(struct sc_cond *cond);

void
sc_cond_broadcast(struct sc_cond *cond);

#endif

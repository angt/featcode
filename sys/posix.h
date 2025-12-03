#pragma once

#include <signal.h>
#include <setjmp.h>

static sigjmp_buf sys_jmp;

static void
sys_signal_handler(int sig) {
    (void)sig;
    siglongjmp(sys_jmp, 1);
}

static void
sys_init(void) {
    struct sigaction sa = {
        .sa_handler = sys_signal_handler
    };
    sigaction(SIGILL, &sa, 0);
}

static void
sys_exit(void) {
    struct sigaction sa = {
        .sa_handler = SIG_DFL
    };
    sigaction(SIGILL, &sa, 0);
}

#define sys_setjmp() sigsetjmp(sys_jmp, 1)

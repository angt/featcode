#pragma once

#include <setjmp.h>
#include <signal.h>
#include <stdio.h>
#include <string.h>

struct feature {
    const char *name;
    void (*detect)(void);
    int set, dep;
};

struct features {
    struct feature *list;
    size_t count;
    void (*reduce)(struct feature *);
};

static sigjmp_buf jmp;

static void
handler(int sig)
{
    (void)sig;
    siglongjmp(jmp, 1);
}

static void
show(struct features features)
{
    features.reduce(features.list);

    for (size_t i = 0; i < features.count; i++) {
        if (features.list[i].set && !features.list[i].dep)
            printf("%s\n", features.list[i].name);
    }
}

static int
set(struct features features, const char *name)
{
    for (size_t i = 0; i < features.count; i++) {
        if (!strcmp(features.list[i].name, name)) {
            features.list[i].set = 1;
            return 0;
        }
    }
    return 1;
}

static void
detect(struct features features)
{
    struct sigaction sa = {
        .sa_handler = handler
    };
    sigaction(SIGILL, &sa, NULL);

    for (size_t i = 0; i < features.count; i++) {
        if (features.list[i].detect && !sigsetjmp(jmp, 1)) {
            features.list[i].detect();
            features.list[i].set = 1;
        }
    }
}

static void
decode(struct features features, const char *code)
{
    for (size_t i = 0; i < features.count; i++) {
        char c = code[i / 4];

        if (!c)
            break;

        if (((c - 'k') >> (i & 3)) & 1)
            features.list[i].set = 1;
    }
}

static int
encode(struct features features)
{
    int h = 0;

    features.reduce(features.list);

    for (size_t i = 0; i < features.count; i++) {
        if (features.list[i].set && !features.list[i].dep)
            h |= (1 << (i & 3));

        if ((i & 3) == 3) {
            printf("%c", h + 'k');
            h = 0;
        }
    }
    if (features.count & 3)
        printf("%c", h + 'k');

    printf("\n");
    return 0;
}

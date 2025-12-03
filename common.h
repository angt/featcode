#pragma once

#include <setjmp.h>
#include <signal.h>
#include <stdio.h>
#include <string.h>

#ifdef _WIN32
#include "sys/win32.h"
#else
#include "sys/posix.h"
#endif

#define COUNT(x) (sizeof(x) / sizeof((x)[0]))

struct feature {
    const char *name;
    void (*detect)(void);
    int set, dep;
};

struct dependency {
    int set;
    int dep;
};

struct features {
    struct feature *list;
    size_t count;
    struct {
        struct dependency *list;
        size_t count;
    } deps;
};

static void
reduce(struct features features)
{
    for (size_t i = 0; i < features.deps.count; i++) {
        struct dependency d = features.deps.list[i];
        if (features.list[d.set].set)
            features.list[d.dep].dep = 1;
    }
}

static void
show(struct features features)
{
    reduce(features);

    for (size_t i = 0; i < features.count; i++) {
        if (features.list[i].set && !features.list[i].dep)
            printf("%s\n", features.list[i].name);
    }
}

static int
set(struct features features, const char *name, int value)
{
    for (size_t i = 0; i < features.count; i++) {
        if (!strcmp(features.list[i].name, name)) {
            features.list[i].set = value;
            return 0;
        }
    }
    return 1;
}

static void
detect(struct features features)
{
    sys_init();

    for (size_t i = 0; i < features.count; i++) {
        if (features.list[i].detect && !sys_setjmp()) {
            features.list[i].detect();
            features.list[i].set = 1;
        }
    }
    sys_exit();
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

    reduce(features);

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

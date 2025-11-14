#include "../common.h"

#ifdef __aarch64__
#define FEATURE(feat, name) [feat]={name,check_##feat}
#define host_features aarch64_features

static void __attribute__((target("+fp16")))
check_fp16(void) {
    asm volatile("fadd v0.4h, v0.4h, v0.4h" ::: "v0");
}

static void __attribute__((target("+dotprod")))
check_dotprod(void) {
    asm volatile("sdot v0.4s, v0.16b, v0.16b" ::: "v0");
}

static void __attribute__((target("+i8mm")))
check_i8mm(void) {
    asm volatile("smmla v0.4s, v0.16b, v0.16b" ::: "v0");
}

static void __attribute__((target("+sve")))
check_sve(void) {
    asm volatile("incw x0" ::: "x0");
}

static void __attribute__((target("+sve2")))
check_sve2(void) {
    asm volatile("smlalb z0.s, z0.h, z0.h" ::: "z0");
}

static void __attribute__((target("+sme")))
check_sme(void) {
    asm volatile("smstart sm\n\tsmstop sm");
}

#else
#define FEATURE(feat, name) [feat]={name,NULL}
#endif

enum {
    fp16,
    dotprod, i8mm,
    sve, sve2, sme
};

static struct feature
aarch64_list[] = {
    FEATURE(fp16,    "fp16"   ),
    FEATURE(dotprod, "dotprod"),
    FEATURE(i8mm,    "i8mm"   ),
    FEATURE(sve,     "sve"    ),
    FEATURE(sve2,    "sve2"   ),
    FEATURE(sme,     "sme"    ),
};

#undef FEATURE

static void
aarch64_reduce(struct feature *feature)
{
    if (feature[sve].set ||
        feature[sme].set)
        feature[fp16].dep = 1;

    if (feature[sve2].set)
        feature[sve].dep = 1;
}

static struct features
aarch64_features = {
    .list   = aarch64_list,
    .count  = sizeof(aarch64_list) / sizeof(aarch64_list[0]),
    .reduce = aarch64_reduce
};

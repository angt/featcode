#include "../common.h"

#ifdef __x86_64__
#define FEATURE(feat, name) [feat]={name,check_##feat}
#define host_features x86_64_features

static void __attribute__((target("avx")))
check_avx(void) {
    asm volatile("vaddps %%ymm0, %%ymm1, %%ymm2" ::: "ymm0", "ymm1", "ymm2");
}

static void __attribute__((target("f16c")))
check_f16c(void) {
    asm volatile("vcvtph2ps %%xmm0, %%ymm1" ::: "xmm0", "ymm1");
}

static void __attribute__((target("fma")))
check_fma(void) {
    asm volatile("vfmadd132ps %%ymm0, %%ymm1, %%ymm2" ::: "ymm0", "ymm1", "ymm2");
}

static void __attribute__((target("avx2")))
check_avx2(void) {
    asm volatile("vpaddd %%ymm0, %%ymm1, %%ymm2" ::: "ymm0", "ymm1", "ymm2");
}

static void __attribute__((target("bmi2")))
check_bmi2(void) {
    asm volatile("pext %%rax, %%rbx, %%rcx" ::: "rax", "rbx", "rcx");
}

static void __attribute__((target("avxvnniint8")))
check_avxvnniint8(void) {
    asm volatile("vpdpbssd %%ymm0, %%ymm1, %%ymm2" ::: "ymm0", "ymm1", "ymm2");
}

static void __attribute__((target("avxvnni")))
check_avxvnni(void) {
    asm volatile("vpdpwssd %%ymm0, %%ymm1, %%ymm2" ::: "ymm0", "ymm1", "ymm2");
}

static void __attribute__((target("evex512,avx512f")))
check_avx512f(void) {
    asm volatile("vaddps %%zmm0, %%zmm1, %%zmm2" ::: "zmm0", "zmm1", "zmm2");
}

static void __attribute__((target("evex512,avx512vl")))
check_avx512vl(void) {
    asm volatile("vaddps %%ymm0, %%ymm1, %%ymm2" ::: "ymm0", "ymm1", "ymm2");
}

static void __attribute__((target("evex512,avx512bw")))
check_avx512bw(void) {
    asm volatile("vpaddw %%zmm0, %%zmm1, %%zmm2" ::: "zmm0", "zmm1", "zmm2");
}

static void __attribute__((target("evex512,avx512dq")))
check_avx512dq(void) {
    asm volatile("vpmulld %%zmm0, %%zmm1, %%zmm2" ::: "zmm0", "zmm1", "zmm2");
}

static void __attribute__((target("evex512,avx512cd")))
check_avx512cd(void) {
    asm volatile("vpconflictd %%zmm0, %%zmm1" ::: "zmm0", "zmm1");
}

static void __attribute__((target("evex512,avx512vnni")))
check_avx512vnni(void) {
    asm volatile("vpdpbusd %%zmm0, %%zmm1, %%zmm2" ::: "zmm0", "zmm1", "zmm2");
}

static void __attribute__((target("evex512,avx512vbmi")))
check_avx512vbmi(void) {
    asm volatile("vpermb %%zmm0, %%zmm1, %%zmm2" ::: "zmm0", "zmm1", "zmm2");
}

static void __attribute__((target("evex512,avx512bf16")))
check_avx512bf16(void) {
    asm volatile("vdpbf16ps %%zmm0, %%zmm1, %%zmm2" ::: "zmm0", "zmm1", "zmm2");
}

static const unsigned char
__attribute__((aligned(64)))
amx_cfg[64] = {
    [ 0] =  1,
    [16] = 64,
    [18] = 64,
    [20] = 64,
    [48] = 16,
    [49] = 16,
    [50] = 16,
};

#ifdef __linux__
#include <unistd.h>
#include <sys/syscall.h>
#include <asm/prctl.h>
#endif

static void
amx_init(void) {
#ifdef __linux__
    syscall(SYS_arch_prctl, ARCH_REQ_XCOMP_PERM, 18);
#endif
}

static void __attribute__((target("amx-tile")))
check_amxtile(void) {
    amx_init();
    asm volatile(
        "ldtilecfg %0\n\t"
        "tilerelease\n\t"
        :: "m"(amx_cfg) : "memory", "tmm2"
    );
}

static void __attribute__((target("amx-int8")))
check_amxint8(void) {
    amx_init();
    asm volatile(
        "ldtilecfg %0\n\t"
        "tdpbssd %%tmm0, %%tmm1, %%tmm2\n\t"
        "tilerelease\n\t"
        :: "m"(amx_cfg) : "memory", "tmm0", "tmm1", "tmm2"
    );
}

static void __attribute__((target("amx-bf16")))
check_amxbf16(void) {
    amx_init();
    asm volatile(
        "ldtilecfg %0\n\t"
        "tdpbf16ps %%tmm0, %%tmm1, %%tmm2\n\t"
        "tilerelease\n\t"
        :: "m"(amx_cfg) : "memory", "tmm0", "tmm1", "tmm2"
    );
}

#else
#define FEATURE(feat, name) [feat]={name,NULL}
#endif

enum {
    avx, f16c, fma, avx2, bmi2,
    avx512f, avx512vl, avx512bw,
    avx512cd, avx512dq,
    avx512vnni, avx512vbmi, avx512bf16,
    avxvnniint8, avxvnni,
    amxtile, amxint8, amxbf16
};

static struct feature
x86_64_list[] = {
    FEATURE(avx,         "avx"        ),
    FEATURE(f16c,        "f16c"       ),
    FEATURE(fma,         "fma"        ),
    FEATURE(avx2,        "avx2"       ),
    FEATURE(bmi2,        "bmi2"       ),
    FEATURE(avxvnni,     "avxvnni"    ),
    FEATURE(avxvnniint8, "avxvnniint8"),
    FEATURE(avx512f,     "avx512f"    ),
    FEATURE(avx512vl,    "avx512vl"   ),
    FEATURE(avx512bw,    "avx512bw"   ),
    FEATURE(avx512dq,    "avx512dq"   ),
    FEATURE(avx512cd,    "avx512cd"   ),
    FEATURE(avx512vnni,  "avx512vnni" ),
    FEATURE(avx512vbmi,  "avx512vbmi" ),
    FEATURE(avx512bf16,  "avx512bf16" ),
    FEATURE(amxtile,     "amx-tile"   ),
    FEATURE(amxint8,     "amx-int8"   ),
    FEATURE(amxbf16,     "amx-bf16"   ),
};

#undef FEATURE

static void
x86_64_reduce(struct feature *feature)
{
    if (feature[f16c].set ||
        feature[fma].set  ||
        feature[avx2].set)
        feature[avx].dep = 1;

    if (feature[avx512f].set) {
        feature[avx2].dep = 1;
        feature[f16c].dep = 1;
        feature[fma].dep = 1;
    }
    if (feature[avxvnni].set ||
        feature[avxvnniint8].set)
        feature[avx2].dep = 1;

    if (feature[avx512vl].set ||
        feature[avx512bw].set ||
        feature[avx512dq].set ||
        feature[avx512cd].set ||
        feature[avx512vnni].set)
        feature[avx512f].dep = 1;

    if (feature[avx512vbmi].set ||
        feature[avx512bf16].set)
        feature[avx512bw].dep = 1;

    if (feature[amxint8].set ||
        feature[amxbf16].set)
        feature[amxtile].dep = 1;
}

static struct features
x86_64_features = {
    .list   = x86_64_list,
    .count  = sizeof(x86_64_list) / sizeof(x86_64_list[0]),
    .reduce = x86_64_reduce,
};

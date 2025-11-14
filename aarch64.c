#include "common.h"

void __attribute__((target("+fp16")))
check_fp16(void) {
    asm volatile("fadd v0.4h, v0.4h, v0.4h" ::: "v0");
}

void __attribute__((target("+dotprod")))
check_dotprod(void) {
    asm volatile("sdot v0.4s, v0.16b, v0.16b" ::: "v0");
}

void __attribute__((target("+i8mm")))
check_i8mm(void) {
    asm volatile("smmla v0.4s, v0.16b, v0.16b" ::: "v0");
}

void __attribute__((target("+sve")))
check_sve(void) {
    asm volatile("incw x0" ::: "x0");
}

void __attribute__((target("+sve2")))
check_sve2(void) {
    asm volatile("smlalb z0.s, z0.h, z0.h" ::: "z0");
}

void __attribute__((target("+sme")))
check_sme(void) {
    asm volatile("smstart sm\n\tsmstop sm");
}

enum {
    fp16,
    dotprod, i8mm,
    sve, sve2, sme
};

static void
reduce(struct feature *feature)
{
    if (feature[sve].set || feature[sme].set)
        feature[fp16].dep = 1;

    if (feature[sve2].set)
        feature[sve].dep = 1;
}

int
main(int argc, char *argv[])
{
    struct feature list[] = {
        [fp16   ] = {"fp16",    check_fp16   },
        [dotprod] = {"dotprod", check_dotprod},
        [i8mm   ] = {"i8mm",    check_i8mm   },
        [sve    ] = {"sve",     check_sve    },
        [sve2   ] = {"sve2",    check_sve2   },
        [sme    ] = {"sme",     check_sme    },
    };
    struct features features = {
        .list = list,
        .count = sizeof(list) / sizeof(list[0]),
        .reduce = reduce
    };
    if (argc <= 1) {
        detect(features);
        return encode(features);
    }
    for (int i = 1; i < argc; i++)
        helper(features, argv[i]);

    show(features);
    return 0;
}

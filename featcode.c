#include <stdlib.h>
#include "common.h"

#include "arch/aarch64.h"
#include "arch/x86_64.h"

static int modified = 0;

static void
helper(struct features features, const char *s)
{
    if (s[0] == '+' || s[0] == '-') {
        modified = 1;
        set(features, s + 1, s[0] == '+');
    } else if (!strcmp(s, ".")) {
        detect(features);
    } else {
        decode(features, s);
    }
}

int
main(int argc, char *argv[])
{
    struct features features = host_features;
    const char *arch = getenv("FEATCODE_ARCH");

    if (arch) {
        if (!strcmp(arch, "aarch64")) {
            features = aarch64_features;
        } else if (!strcmp(arch, "x86_64")) {
            features = x86_64_features;
        } else {
            return 1;
        }
    }
    if (argc <= 1) {
        modified = 1;
        helper(features, ".");
    }
    for (int i = 1; i < argc; i++)
        helper(features, argv[i]);

    if (modified) {
        encode(features);
    } else {
        show(features);
    }
    return 0;
}

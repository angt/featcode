#include <stdlib.h>
#include "common.h"

#include "arch/aarch64.h"
#include "arch/x86_64.h"

static void
helper(struct features features, const char *s)
{
    if (s[0] == '+') {
        set(features, s + 1);
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

    if (argc <= 1) {
        detect(host_features);
        return encode(host_features);
    }
    const char *arch = getenv("ARCH");

    if (arch) {
        if (!strcmp(arch, "aarch64")) {
            features = aarch64_features;
        } else if (!strcmp(arch, "x86_64")) {
            features = x86_64_features;
        } else {
            return 1;
        }
    }
    for (int i = 1; i < argc; i++)
        helper(features, argv[i]);

    show(features);
    return 0;
}

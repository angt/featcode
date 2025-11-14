NAME := featcode

CFLAGS  = -Os -ffunction-sections -fdata-sections -fno-unwind-tables
LDFLAGS = -Wl,--gc-sections -Wl,--strip-all

ALL :=

ALL += aarch64-macos-$(NAME)
ALL += aarch64-linux-$(NAME)

ALL += x86_64-macos-$(NAME)
ALL += x86_64-linux-$(NAME)


.PHONY: all
all: $(ALL)

%-linux-$(NAME): LDFLAGS += -static
%-$(NAME): TARGET_ARCH = $(patsubst %linux,%linux-musl,$(patsubst %macos,%macos-none,$*))

%-$(NAME): featcode.c common.h arch/x86_64.h
	zig cc -target $(TARGET_ARCH) $(CFLAGS) $< -o $@ $(LDFLAGS)

.PHONY: clean
clean:
	rm -f $(ALL)

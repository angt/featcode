NAME := featcode
CFLAGS = -Os -ffunction-sections -fdata-sections -fno-unwind-tables
LDFLAGS = -Wl,--gc-sections -Wl,--strip-all

.PHONY: all
all: aarch64-macos-$(NAME) x86_64-macos-$(NAME) \
     aarch64-linux-$(NAME) x86_64-linux-$(NAME)

%-linux-$(NAME): LDFLAGS += -static

%-linux-$(NAME): %.c common.h
	zig cc -target $*-linux-musl $(CFLAGS) $< -o $@ $(LDFLAGS)

%-macos-$(NAME): %.c common.h
	zig cc -target $*-macos-none $(CFLAGS) $< -o $@ $(LDFLAGS)

.PHONY: clean
clean:
	rm -f *-$(NAME)

# featcode

Show target architecture features as a compact code

## Install

### From precompiled binaries

Binaries are provided for Linux and macOS (`x86_64` and `aarch64`).

    curl cdn.statically.io/gh/angt/featcode/master/install.sh | sh

Then run:

    ./featcode

### From source

    git clone https://github.com/angt/featcode
    cd featicode
    zig build

Then run:

    zig-out/bin/featcode

## Usage

Detect architecture features and print the featcode

    $ ./featcode
    rk

Display the features corresponding to a given featcode

    $ ./featcode rk
    fp16
    dotprod
    i8mm

Use `.` to detect and display the current CPU features

    $ ./featcode .
    fp16
    dotprod
    i8mm

Use `+` to add a specific feature

    $ ./featcode +sme .
    qm


## Note

This tool useful for [installama.sh](https://github.com/angt/installama.sh) at least :)

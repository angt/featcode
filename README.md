# featcode

Show target's features for real, no joking

## Usage

### Detect architecture features and print the featcode

    $ ./aarch64-macos-featcode
    rk

### Display features corresponding to a given featcode

    $ ./aarch64-macos-featcode rk
    fp16
    dotprod
    i8mm

### Use '.' to detect the current CPU

    $ ./aarch64-macos-featcode .
    fp16
    dotprod
    i8mm

###  Use '+' to add a specific feature

    $ ./aarch64-macos-featcode +sme .
    dotprod
    i8mm
    sme

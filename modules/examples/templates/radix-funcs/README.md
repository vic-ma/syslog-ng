# Radix Template Functions

This set of example template functions covert numbers between different radixes (bases).

It supports conversions between decimal $(dec), hexadecimal $(hex), and octal $(oct).

Numbers must be unsigned longs.

### Usage

```
source s_local {
    stdin();
};

destination d_local {
    file("/dev/stdout" template("$(hex ${PID}): ${MESSAGE}\n"));
};

log {
    source(s_local);
    destination(d_local);
};
```

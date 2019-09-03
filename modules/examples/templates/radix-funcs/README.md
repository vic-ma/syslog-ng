# Radix Template Functions

This set of example template functions covert numbers between different radixes (bases).

It supports conversions between decimal $(dec), hexadecimal $(hex), and octal $(oct).

Numbers must be unsigned longs.

### Usage

```
source s_local {
    file("/tmp/input.log");
};

template t_hexpid_msg {
    template("$(hex ${PID}): ${MESSAGE}\n");
};

destination d_local {
    file("/tmp/output.log" template(t_hexpid_msg));
};

log {
    source(s_local);
    destination(d_local);
};
```

# Ordered parser

This example parser plugin parses ordered lists.

The parser supports one option, `suffix`, which sets the key-value seperator for the ordered lists.

e.g. `A) B) C)` vs. `A: B: C:`

The parser also supports two flags, `letters` and `numbers`, which sets the type of symbol for
the keys of the ordered lists.

e.g. `A) B) C)` vs. `1) 2) 3)`

The name of the macros filled is the same as the name of the keys.

For example, parsing the message, "A) Apple B) Banana C) Cherry" would result in
the following macros being filled:
```
${A}=="Apple"
${B}=="Banana"
${C}=="Cherry"
```

### Usage

```
source s_file {
  file("/tmp/input.log");
};

parser ordered {
  example_ordered_parser(flags(numbers));
};

template t_sqr {
  template("$(* $1 $1), $(* $2 $2), $(* $3 $3)\n");
};

destination d_file {
  file("/tmp/output.log" template(t_sqr));
};

log {
  source(s_file);
  parser(ordered);
  destination(d_file);
};
```

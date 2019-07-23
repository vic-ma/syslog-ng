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
parser p_ordered_parser {
  example-ordered-parser(
    suffix(")")
    flags(letters)
  );
};

log {
  source {
    stdin();
  };

  parser(p_ordered_parser);

  destination {
    file("/dev/stdout" template("A=$A B=$B C=$C\n"));
  };
};

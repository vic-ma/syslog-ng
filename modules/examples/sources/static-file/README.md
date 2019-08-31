# Static File Source

This example source driver reads existing log messages from a text file.

### Usage

```
log {
  source {
    example-static-file("/tmp/input.log");
  };
  destination {
    file("/tmp/output.log");
  };
};
```

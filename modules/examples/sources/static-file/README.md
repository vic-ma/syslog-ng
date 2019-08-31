# Static File Source

This example source driver which reads existing log messages from a text file.

Unlike the `file` source driver, `static-file`:
1. Does not read new lines added to the file (it stops reading after reaching EOF)
2. Re-reads all lines from the start after a reload
3. Re-reads all lines from the start after a restart

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

# GBK ↔ UTF-8 conversion library

Converts between GBK and UTF-8 using the system API on Windows and `iconv` on Linux/macOS.

## Build

```bash
make
```

This produces `utf8.o`, which you can link into your program. On Linux/macOS, link with `-liconv`:

```bash
gcc -o myapp myapp.c utf8.o -liconv
```

## Tests

```bash
make test
```

This builds and runs `test_utf8`, covering UTF-8↔GBK conversion, round-trip, ASCII, null/length checks, and buffer-too-small cases. On Windows without iconv, run: `make test LDFLAGS=`.

## API

- **`int utf8_to_gb(const char *src, char *dst, int len)`**  
  UTF-8 → GBK. `src` is a null-terminated UTF-8 string, `dst` is the output buffer, `len` is the size of `dst` in bytes. Returns `0` on success, `-1` on error.

- **`int gb_to_utf8(const char *src, char *dst, int len)`**  
  GBK → UTF-8. Same parameters and return value as above.

The caller must ensure `dst` has at least `len` bytes. On failure, the contents of `dst` are undefined; check the return value to retry or report an error.

## License

Apache License 2.0

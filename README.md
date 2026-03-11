# GBK ↔ UTF-8 转换库

在 Windows 上使用系统 API，在 Linux/macOS 上使用 `iconv`，实现 GBK 与 UTF-8 之间的转换。

## 构建

```bash
make
```

生成 `utf8.o`，可链接到你的程序中。Linux/macOS 下链接时需加 `-liconv`：

```bash
gcc -o myapp myapp.c utf8.o -liconv
```

## 测试

```bash
make test
```

会编译并运行 `test_utf8`，覆盖 UTF-8↔GBK 转换、往返、ASCII、空指针/长度校验及缓冲区过小等用例。Windows 下若未装 iconv，可执行：`make test LDFLAGS=`。

## API

- **`int utf8_to_gb(const char *src, char *dst, int len)`**  
  UTF-8 → GBK。`src` 为以 `\0` 结尾的 UTF-8 串，`dst` 为输出缓冲区，`len` 为 `dst` 的字节数。成功返回 `0`，失败返回 `-1`。

- **`int gb_to_utf8(const char *src, char *dst, int len)`**  
  GBK → UTF-8。参数与返回值同上。

调用方需保证 `dst` 至少为 `len` 字节；失败时 `dst` 内容未定义，可根据返回值判断并重试或报错。

## 许可证

Apache License 2.0

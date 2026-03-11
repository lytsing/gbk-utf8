/**
 * Unit tests for utf8_to_gb / gb_to_utf8.
 * Build: make test  (or make && ./test_utf8)
 * Run:   ./test_utf8
 */

#include <stdio.h>
#include <string.h>
#include "utf8.h"

static int failed;

#define ASSERT(c) do { \
    if (!(c)) { \
        fprintf(stderr, "FAIL: %s:%d: %s\n", __FILE__, __LINE__, #c); \
        failed = 1; \
    } \
} while (0)

/* UTF-8 "中文" (without null) */
static const unsigned char utf8_zh[] = { 0xE4, 0xB8, 0xAD, 0xE6, 0x96, 0x87 };
/* GBK "中文" (without null) */
static const unsigned char gbk_zh[] = { 0xD6, 0xD0, 0xCE, 0xC4 };

static void test_utf8_to_gb(void)
{
    char src[16], dst[16];
    int r;

    memcpy(src, utf8_zh, sizeof(utf8_zh));
    src[sizeof(utf8_zh)] = '\0';

    memset(dst, 0xAA, sizeof(dst));
    r = utf8_to_gb(src, dst, sizeof(dst));
    ASSERT(r == 0);
    ASSERT(memcmp(dst, gbk_zh, sizeof(gbk_zh)) == 0);
    ASSERT(dst[sizeof(gbk_zh)] == '\0');
}

static void test_gb_to_utf8(void)
{
    char src[16], dst[16];
    int r;

    memcpy(src, gbk_zh, sizeof(gbk_zh));
    src[sizeof(gbk_zh)] = '\0';

    memset(dst, 0xAA, sizeof(dst));
    r = gb_to_utf8(src, dst, sizeof(dst));
    ASSERT(r == 0);
    ASSERT(memcmp(dst, utf8_zh, sizeof(utf8_zh)) == 0);
    ASSERT(dst[sizeof(utf8_zh)] == '\0');
}

static void test_roundtrip(void)
{
    char buf[64];
    int r;

    memcpy(buf, utf8_zh, sizeof(utf8_zh));
    buf[sizeof(utf8_zh)] = '\0';

    r = utf8_to_gb(buf, buf, sizeof(buf));
    ASSERT(r == 0);
    ASSERT(memcmp(buf, gbk_zh, sizeof(gbk_zh)) == 0);

    r = gb_to_utf8(buf, buf, sizeof(buf));
    ASSERT(r == 0);
    ASSERT(memcmp(buf, utf8_zh, sizeof(utf8_zh)) == 0);
}

static void test_ascii(void)
{
    char dst[32];
    int r;

    r = utf8_to_gb("Hello", dst, sizeof(dst));
    ASSERT(r == 0);
    ASSERT(strcmp(dst, "Hello") == 0);

    r = gb_to_utf8("Hello", dst, sizeof(dst));
    ASSERT(r == 0);
    ASSERT(strcmp(dst, "Hello") == 0);
}

static void test_null_src(void)
{
    char dst[32];
    ASSERT(utf8_to_gb(NULL, dst, sizeof(dst)) == -1);
    ASSERT(gb_to_utf8(NULL, dst, sizeof(dst)) == -1);
}

static void test_null_dst(void)
{
    char src[] = "x";
    ASSERT(utf8_to_gb(src, NULL, 32) == -1);
    ASSERT(gb_to_utf8(src, NULL, 32) == -1);
}

static void test_invalid_len(void)
{
    char src[32], dst[32];
    memcpy(src, utf8_zh, sizeof(utf8_zh));
    src[sizeof(utf8_zh)] = '\0';

    ASSERT(utf8_to_gb(src, dst, 0) == -1);
    ASSERT(utf8_to_gb(src, dst, -1) == -1);
    ASSERT(gb_to_utf8(src, dst, 0) == -1);
    ASSERT(gb_to_utf8(src, dst, -1) == -1);
}

static void test_buffer_too_small(void)
{
    char src[32], dst[4];
    memcpy(src, utf8_zh, sizeof(utf8_zh));
    src[sizeof(utf8_zh)] = '\0';

    /* 1 byte buffer cannot hold even the null terminator for "中文" */
    ASSERT(utf8_to_gb(src, dst, 1) == -1);
    memcpy(src, gbk_zh, sizeof(gbk_zh));
    src[sizeof(gbk_zh)] = '\0';
    ASSERT(gb_to_utf8(src, dst, 1) == -1);
}

int main(void)
{
    failed = 0;

    test_utf8_to_gb();
    test_gb_to_utf8();
    test_roundtrip();
    test_ascii();
    test_null_src();
    test_null_dst();
    test_invalid_len();
    test_buffer_too_small();

    if (failed) {
        fprintf(stderr, "Some tests failed.\n");
        return 1;
    }
    printf("All tests passed.\n");
    return 0;
}

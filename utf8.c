/**
 *  Copyright (C) 2008  Huang Guan
 *  Copyright (C) 2011  iBoxpay.com inc.
 *
 *  Description: This file mainly includes the functions about utf8
 *
 *  History:
 *  2008-7-10 13:31:57 Created.
 *  2011-12-28 Format the code style, and add comments by Lytsing
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */

#ifdef __WIN32__
#include <windows.h>
#else
#include <iconv.h>
#endif

#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

#include "utf8.h"


#ifdef __WIN32__
int utf8_to_gb(const char* src, char* dst, int len)
{
    int ret = 0;
    WCHAR* strA;

    if (!src || !dst || len <= 0)
        return -1;

    int i = MultiByteToWideChar(CP_UTF8, 0, src, -1, NULL, 0);
    if (i <= 0)
        return -1;
    strA = (WCHAR*)malloc((size_t)i * sizeof(WCHAR));
    if (!strA)
        return -1;
    MultiByteToWideChar(CP_UTF8, 0, src, -1, strA, i);
    i = WideCharToMultiByte(CP_ACP, 0, strA, -1, NULL, 0, NULL, NULL);
    if (len < i) {
        free(strA);
        return -1;
    }
    ret = WideCharToMultiByte(CP_ACP, 0, strA, -1, dst, i, NULL, NULL);
    free(strA);
    if (ret <= 0)
        return -1;
    return 0;
}

int gb_to_utf8(const char* src, char* dst, int len)
{
    int ret = 0;
    WCHAR* strA;

    if (!src || !dst || len <= 0)
        return -1;

    int i = MultiByteToWideChar(CP_ACP, 0, src, -1, NULL, 0);
    if (i <= 0)
        return -1;
    strA = (WCHAR*)malloc((size_t)i * sizeof(WCHAR));
    if (!strA)
        return -1;
    MultiByteToWideChar(CP_ACP, 0, src, -1, strA, i);
    i = WideCharToMultiByte(CP_UTF8, 0, strA, -1, NULL, 0, NULL, NULL);
    if (len < i) {
        free(strA);
        return -1;
    }
    ret = WideCharToMultiByte(CP_UTF8, 0, strA, -1, dst, i, NULL, NULL);
    free(strA);
    if (ret <= 0)
        return -1;
    return 0;
}
#else   /* Linux / macOS */
// starkwong: In iconv implementations, inlen and outlen should be type of size_t not uint, which is different in length on Mac
int utf8_to_gb(const char* src, char* dst, int len)
{
    int ret = -1;
    size_t inlen;
    size_t outlen;

    if (!src || !dst || len <= 0)
        return -1;

    inlen = strlen(src) + 1;
    outlen = (size_t)len;

    char* inbuf = (char *)malloc(inlen);
    if (!inbuf)
        return -1;
    char* inbuf_hold = inbuf;
    memcpy(inbuf, src, inlen);

    char* outbuf = dst;
    iconv_t cd = iconv_open("GBK", "UTF-8");
    if (cd == (iconv_t)-1) {
        free(inbuf_hold);
        return -1;
    }
    ret = iconv(cd, &inbuf, &inlen, &outbuf, &outlen);
    iconv_close(cd);
    free(inbuf_hold);
    if (ret != 0)
        return -1;
    *outbuf = '\0';
    return 0;
}

int gb_to_utf8(const char* src, char* dst, int len)
{
    int ret = -1;
    size_t inlen;
    size_t outlen;

    if (!src || !dst || len <= 0)
        return -1;

    inlen = strlen(src) + 1;
    outlen = (size_t)len;

    char* inbuf = (char *)malloc(inlen);
    if (!inbuf)
        return -1;
    char* inbuf_hold = inbuf;
    memcpy(inbuf, src, inlen);

    char* outbuf2 = NULL;
    char* outbuf = dst;
    if (src == dst) {
        outbuf2 = (char*)malloc((size_t)len);
        if (!outbuf2) {
            free(inbuf_hold);
            return -1;
        }
        outbuf = outbuf2;
    }

    iconv_t cd = iconv_open("UTF-8", "GBK");
    if (cd == (iconv_t)-1) {
        free(outbuf2);
        free(inbuf_hold);
        return -1;
    }
    ret = iconv(cd, &inbuf, &inlen, &outbuf, &outlen);
    if (ret != 0) {
        iconv_close(cd);
        free(outbuf2);
        free(inbuf_hold);
        return -1;
    }
    if (outbuf2 != NULL) {
        size_t written = (size_t)len - outlen;
        if (written > (size_t)len - 1)
            written = (size_t)len - 1;
        memcpy(dst, outbuf2, written);
        dst[written] = '\0';
        free(outbuf2);
    } else {
        *outbuf = '\0';
    }
    iconv_close(cd);
    free(inbuf_hold);
    return 0;
}
#endif


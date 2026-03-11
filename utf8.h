/**
 * Copyright (C) 2008  Huang Guan
 * Copyright (C) 2011  iBoxpay.com inc.
 *
 * Description: GBK UTF-8 iconv functions header file
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
 */

#ifndef UTF8_H
#define UTF8_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * UTF-8 to GBK
 * @param src [in]  Null-terminated UTF-8 string (must not be NULL)
 * @param dst [out] Output buffer for GBK string (must not be NULL, null-terminated on success)
 * @param len [in]  Size of dst in bytes, must be > 0 (max bytes to write including '\0')
 * @return 0 on success, -1 on error (NULL src/dst, len<=0, buffer too small, conversion error, or OOM)
 */
int utf8_to_gb(const char* src, char* dst, int len);

/**
 * GBK to UTF-8
 * @param src [in]  Null-terminated GBK string (must not be NULL)
 * @param dst [out] Output buffer for UTF-8 string (must not be NULL, null-terminated on success)
 * @param len [in]  Size of dst in bytes, must be > 0 (max bytes to write including '\0')
 * @return 0 on success, -1 on error (NULL src/dst, len<=0, buffer too small, conversion error, or OOM)
 */
int gb_to_utf8(const char* src, char* dst, int len);

#ifdef __cplusplus
}
#endif

#endif /* UTF8_H */


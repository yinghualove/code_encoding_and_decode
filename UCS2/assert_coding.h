#ifndef __ASSERT_CODING_H
#define __ASSERT_CODING_H

bool isUtf8(unsigned char* data, int len);
bool isGBK(unsigned char* data, int len);
bool is_str_utf8(const char* str);
bool is_str_gbk(const char* str);

#endif
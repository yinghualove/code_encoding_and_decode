#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <iconv.h>
#include <stdbool.h>
#include <locale.h>
#include <stdint.h>
#include "assert_coding.h"
#include "log_level.h"

#define INPUT_MAX_LEN 200
#define CODING_LEN 20

int ret[2] = {0, 1};

int detect_encoding_format(const char *str)
{
    DEBUG_LOG("start detect encoding format\n");
    DEBUG_LOG("ret = %d\n", is_str_utf8(str));
    DEBUG_LOG("ret = %d\n", is_str_gbk(str));
    if (is_str_utf8(str) == true && is_str_gbk(str) == false)
        return ret[0];
    if (is_str_utf8(str) == false && is_str_gbk(str) == true)
        return ret[1];
    if (is_str_utf8(str) == true && is_str_gbk(str) == true)
    {
// 双中文字符时，会出现utf8和gbk都为true情况
#ifdef WIN32
        return ret[1];
#else
        return ret[0];
#endif
    }
}

int encoding()
{

    char *input;          // 存放输入的汉字
    char *output;         // 存放转换后的结果
    char coding[10];      // 存放编码格式
    size_t inlen, outlen; // 输入和输出字符的长度

    input = (char *)malloc(INPUT_MAX_LEN);
    memset(input, 0, strlen(input));

    // 获取终端输入
    INFO_LOG("input chinese:\n");
    scanf("%s", input);
    DEBUG_LOG("input = %s\n", input);

    // 检测输入的字符串的编码格式是GBK还是UTF-8
    int det_ret = 0;
    det_ret = detect_encoding_format(input);
    DEBUG_LOG("det_ret = %d\n", det_ret);
    if (det_ret == 0)
        sprintf(coding, "UTF-8");
    else if (det_ret == 1)
        sprintf(coding, "GBK");

    inlen = strlen(input); // 输入字符的最大长度
    DEBUG_LOG("input size =%d\n", inlen);
    if (strncmp(coding, "UTF-8", 6) == 0)
        outlen = inlen * 2; // 输出字符的最大长度
    else
        outlen = inlen * 3; // 输出字符的最大长度
    output = (char *)malloc(outlen);
    memset(output, 0, strlen(output));

    // 创建转换描述符
    // iconv_t cd = iconv_open("UCS-2LE", "GBK");  // UCS-2LE表示UCS-2 Little Endian
    iconv_t cd = iconv_open("UCS-2LE", coding); // UCS-2LE表示UCS-2 Little Endian
    if (cd == (iconv_t)-1)
    {
        perror("iconv_open");
        exit(1);
    }

    // 进行转换
    char *inbuf = input;
    char *outbuf = output;
    if (iconv(cd, &inbuf, &inlen, &outbuf, &outlen) == (size_t)-1)
    {
        exit(1);
    }

    // 关闭转换描述符
    iconv_close(cd);

    // 输出转换结果
    INFO_LOG("encoding result:0x");
    for (int i = 0; i < outlen / 2; i += 2)
    {
        printf("%02X%02X", (unsigned char)output[i + 1], (unsigned char)output[i]); // 以16进制形式输出UCS-2编码
    }
    INFO_LOG("\n");

    free(input);
    free(output);

    return 0;
}

// 将十六进制字符转换为对应的数值
int hex_char_to_int(char c)
{
    if (c >= '0' && c <= '9')
    {
        return c - '0';
    }
    else if (c >= 'A' && c <= 'F')
    {
        return c - 'A' + 10;
    }
    else if (c >= 'a' && c <= 'f')
    {
        return c - 'a' + 10;
    }
    else
    {
        // 错误的输入
        return -1;
    }
}

// 将UCS2编码的字符解码为Unicode码点
uint16_t ucs2_to_unicode(uint16_t *ucs2)
{
    uint16_t high_byte = ucs2[0] << 8;
    uint16_t low_byte = ucs2[1];
    return high_byte | low_byte;
}

int decode()
{
    setlocale(LC_CTYPE, "");
    char input_string[100]; // 假设输入的字符串不超过10个字符
    INFO_LOG("input 16 bit char:\n");
    scanf("%s", input_string);
    DEBUG_LOG("input string size = %d\n", strlen(input_string));

    uint16_t code[2];
    memset(code, 0, sizeof(code));
    int i = 0;
    // for (i = 0; i < strlen(input_string); i++){
    //  printf("input_string[%d]=%c\n",i,input_string[i]);
    //   }
    for (i = 0; i < strlen(input_string); i += 4)
    {
        code[0] = (hex_char_to_int(input_string[i]) << 4) |
                  hex_char_to_int(input_string[i + 1]);
        code[1] = (hex_char_to_int(input_string[i + 2]) << 4) | hex_char_to_int(input_string[i + 3]);
        uint16_t unicode_code = ucs2_to_unicode((code));

        // printf("Unicode Code Point: U+%04X\n", unicode_code);

        INFO_LOG("%lc", unicode_code);
        // printf("The corresponding character is: %lc", unicode_code);
    }
    INFO_LOG("\n");

    return 0;
}


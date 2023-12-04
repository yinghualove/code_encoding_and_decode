#include <stdio.h>
#include <stdlib.h>
#include <iconv.h>
#include <string.h>
#include <errno.h>
//#include "iconv.h"
//#pragma comment(lib, "libiconv1.lib")

#define INPUT_MAX_LEN 200

int main() {
    char *input; // 存放输入的汉字
    char *output;  // 存放转换后的结果
    size_t inlen, outlen;  // 输入和输出字符的长度

    // 获取终端输入
    printf("请输入汉字：\n"); 
    input = (char *)malloc(INPUT_MAX_LEN);
    memset(input,0,strlen(input));
    scanf("%s",input);  

    inlen = strlen(input) ;  // 输入字符的最大长度
    outlen = inlen * 2;  // 输出字符的最大长度
    output = (char *)malloc(outlen);
    memset(output,0,strlen(output));

    // 创建转换描述符
    printf("iconv open\n");
    iconv_t cd = iconv_open("UCS-2LE", "UTF-8");  // UCS-2LE表示UCS-2 Little Endian
    if (cd == (iconv_t)-1) {
        perror("iconv_open");
        exit(1);
    }

    // 进行转换
    char *inbuf = input;
    char *outbuf = output;
    printf("iconv start convert\n");
    if (iconv(cd, &inbuf, &inlen, &outbuf, &outlen) == (size_t)-1) {
        printf("输出失败\n");
        exit(1);
    }

    // 关闭转换描述符
    iconv_close(cd);

    // 输出转换结果
    printf("编码结果:0x");
    for (int i = 0; i < outlen/2; i += 2) {
        printf("%02X%02X", (unsigned char)output[i+1], (unsigned char)output[i]);  // 以16进制形式输出UCS-2编码
    }
    printf("\n");
    free(input);
    free(output);
    
    return 0;
}

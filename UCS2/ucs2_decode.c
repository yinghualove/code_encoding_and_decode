#include <locale.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "log_level.h"

// 将十六进制字符转换为对应的数值
int hex_char_to_int(char c) {
  if (c >= '0' && c <= '9') {
    return c - '0';
  } else if (c >= 'A' && c <= 'F') {
    return c - 'A' + 10;
  } else if (c >= 'a' && c <= 'f') {
    return c - 'a' + 10;
  } else {
    // 错误的输入
    return -1;
  }
}

// 将UCS2编码的字符解码为Unicode码点
uint16_t ucs2_to_unicode(uint16_t *ucs2) {
  uint16_t high_byte = ucs2[0] << 8;
  uint16_t low_byte = ucs2[1];
  return high_byte | low_byte;
}

int main(int argc,char **argv) {
  setlocale(LC_CTYPE, "");
  char input_string[100]; // 假设输入的字符串不超过10个字符
  printf("input 16 bit char:\n");
  scanf("%s", input_string);
  printf("input string size = %d\n",strlen(input_string));

  uint16_t code[2];
  memset(code,0,sizeof(code));
  int i = 0;
  //for (i = 0; i < strlen(input_string); i++){
    // printf("input_string[%d]=%c\n",i,input_string[i]);
//   }
  for (i = 0; i < strlen(input_string); i+=4) {   
    code[0] = (hex_char_to_int(input_string[i]) << 4) |
              hex_char_to_int(input_string[i + 1]);
    code[1] = (hex_char_to_int(input_string[i + 2]) << 4) | hex_char_to_int(input_string[i + 3]);
    uint16_t unicode_code = ucs2_to_unicode((code));

    // printf("Unicode Code Point: U+%04X\n", unicode_code);

    printf("%lc", unicode_code);
    //printf("The corresponding character is: %lc", unicode_code);

  }
  printf("\n");
  return 0;
}

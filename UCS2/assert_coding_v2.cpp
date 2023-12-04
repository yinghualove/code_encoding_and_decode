#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#ifdef WIN32
#include <windows.h>
#else
#include <iconv.h> 
#endif
#include "assert_coding.h"


int preNUm(unsigned char byte) {
    unsigned char mask = 0x80;
    int num = 0;
    for (int i = 0; i < 8; i++) {
        if ((byte & mask) == mask) {
            mask = mask >> 1;
            num++;
        } else {
            break;
        }
    }
	return num;
}


bool isUtf8(unsigned char* data, int len) {
    int num = 0;
    int i = 0;
    while (i < len) {
        if ((data[i] & 0x80) == 0x00) {
            // 0XXX_XXXX
            i++;
			continue;
        }
        else if ((num = preNUm(data[i])) > 2) {
        // 110X_XXXX 10XX_XXXX
        // 1110_XXXX 10XX_XXXX 10XX_XXXX
        // 1111_0XXX 10XX_XXXX 10XX_XXXX 10XX_XXXX
        // 1111_10XX 10XX_XXXX 10XX_XXXX 10XX_XXXX 10XX_XXXX
        // 1111_110X 10XX_XXXX 10XX_XXXX 10XX_XXXX 10XX_XXXX 10XX_XXXX
        // preNUm() 返回首个字节8个bits中首�?0bit前面1bit的个数，该数量也是该字符所使用的字节数        
        i++;
        for(int j = 0; j < num - 1; j++) {
            //判断后面num - 1 个字节是不是都是10开
            if ((data[i] & 0xc0) != 0x80) {
					return false;
				}
				i++;
        }
    } else {
        //其他情况说明不是utf-8
        return false;
    }
    } 
    return true;
}

bool isGBK(unsigned char* data, int len)  {
	int i  = 0;
	while (i < len)  {
		if (data[i] <= 0x7f) {
			//编码小于等于127,只有一个字节的编码，兼容ASCII
			i++;
			continue;
		} else {
			//大于127的使用双字节编码
			if 	(data[i] >= 0x81 &&
				data[i] <= 0xfe &&
				data[i + 1] >= 0x40 &&
				data[i + 1] <= 0xfe &&
				data[i + 1] != 0xf7) {
				i += 2;
				continue;
			} else {
				return false;
			}
		}
	}
	return true;
}
typedef enum {
    GBK,
    UTF8,
    UNKOWN
} CODING;
//需要说明的是，isGBK()是通过双字节是否落在gbk的编码范围内实现的，
//而utf-8编码格式的每个字节都是落在gbk的编码范围内�?
//所以只有先调用isUtf8()先判断不是utf-8编码，再调用isGBK()才有意义
CODING GetCoding(unsigned char* data, int len) {
    CODING coding;
    if (isUtf8(data, len) == true) {
        coding = UTF8;
	} else if (isGBK(data, len) == true) {
		coding = GBK;
	} else {
		coding = UNKOWN;
	}
    return coding;
}
int main() {
    // char src[512] = "你好";
    char src[512];
    memset(src,0,strlen(src));
    scanf("%d",&src);
    int len = strlen(src);
    //printf("%s, len:%d\n",src, len);
    char dstgbk[512] = {0};
    char dstutf8[512] = {0};
    printf("coding:%d\n", GetCoding((unsigned char*)src, len));   //判断是否是utf-8
    #ifndef WIN32
    iconv_t  cd;
    char* pSrc = src;
    char* pUTFOUT = dstutf8;
    char* pGBKOUT = dstgbk;
    size_t srcLen = (size_t)len;
    size_t outLenUTF = sizeof(dstutf8);
    size_t outLenGBK = sizeof(dstgbk);
    size_t ret;
    #endif
    #ifdef WIN32
    wchar_t * pUnicodeBuff = NULL;
    int rlen = 0;
    rlen = MultiByteToWideChar(CP_UTF8, 0, src, -1, NULL ,NULL);
    pUnicodeBuff =  new WCHAR[rlen + 1];  //为Unicode字符串空间
    rlen = MultiByteToWideChar(CP_UTF8, 0, src, -1, pUnicodeBuff, rlen); 
    rlen = WideCharToMultiByte(936, 0, pUnicodeBuff, -1, NULL, NULL, NULL, NULL); //936 为windows gb2312代码页码
    WideCharToMultiByte(936, 0, pUnicodeBuff ,-1, dstgbk, rlen, NULL ,NULL);
    delete[] pUnicodeBuff;
    #else
    cd = iconv_open("GBK", "UTF-8");
    if (cd == (iconv_t)-1) {
        printf("iconv_open error\n");
    }
    ret = iconv(cd, &pSrc, &srcLen, &pGBKOUT, &outLenGBK);
    iconv_close(cd);
    #endif

    //printf("%s, len:%d\n",dstgbk, strlen(dstgbk));
    printf("coding:%d\n", GetCoding((unsigned char*)dstgbk, strlen(dstgbk)));   //判断是否是gbk

    #ifdef WIN32
    rlen = MultiByteToWideChar(936, 0, dstgbk, -1, NULL, NULL);
    pUnicodeBuff =  new WCHAR[rlen + 1];  //为Unicode字符串空间
    rlen = MultiByteToWideChar(936, 0, dstgbk, -1, pUnicodeBuff, rlen); 
    rlen = WideCharToMultiByte(CP_UTF8, 0, pUnicodeBuff, -1, NULL, NULL, NULL, NULL);
    WideCharToMultiByte(CP_UTF8, 0, pUnicodeBuff, -1, dstutf8, rlen, NULL, NULL);
    delete[] pUnicodeBuff;
    #else
    cd = iconv_open("UTF-8", "GBK");
    if (cd == (iconv_t)-1) {
        printf("iconv_open error\n");
    }
    //pSrc = pGBKOUT; 错误，上面调用过一次此时iconv()后，pGBKOUT指向的是dstgbk[512]可用的位置，
    //即dstgbk[512]保存gbk字符串的后一位，也就是结束符的位置
    pSrc = dstgbk;
    srcLen = strlen(dstgbk);
    ret = iconv(cd, &pSrc, &srcLen, &pUTFOUT, &outLenUTF);
    iconv_close(cd);
    #endif

    //printf("%s, len:%d\n",dstutf8, strlen(dstutf8));
    printf("coding:%d\n", GetCoding((unsigned char*)dstutf8, strlen(dstutf8)));   //判断是否是utf-8
    getchar();
}

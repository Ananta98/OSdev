#include "../include/string.h"

size_t strlen(char *str) {
    char *tmp = str;
    while (*tmp != '\0')
        tmp++;
    return tmp-str;
}

char *strcpy(char * src,char *dst,size_t size) {
    for (int i=0;i<size;i++)
        dst[i] = src[i];
    return dst; 
}


void *memcpy(void *src, void *dst,size_t size) {
    unsigned char *tmp_dst = (unsigned char *) dst;
    unsigned char *tmp_src = (unsigned char *) src;
    for (int i=0;i<size;i++)
        tmp_dst[i] = tmp_src[i];
    return dst;
}

void *memset(void *buff,size_t value,size_t size) {
    unsigned char *tmp_buff = buff;
    for (int j = 0;j < size; j++)
        tmp_buff[j] = value;
    return buff;
}

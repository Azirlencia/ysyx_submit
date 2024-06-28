#include <klib.h>
#include <klib-macros.h>
#include <stdint.h>

#if !defined(__ISA_NATIVE__) || defined(__NATIVE_USE_KLIB__)

size_t strlen(const char *s) {
  size_t length = 0;
  while (s[length] != '\0') {
    length++;
  }
  return length;
  //panic("Not implemented");
}

char *strcpy(char *dst, const char *src) {
  char *tmp = dst;
  if(src == NULL || dst == NULL) return tmp;
  while ((*tmp = *src) != '\0') {
    tmp++;
    src++;
  }
  return dst;
  //panic("Not implemented");
}

char *strncpy(char *dst, const char *src, size_t n) {
  size_t i;
  for(i = 0; src[i] != '\0'; i++){
    dst[i] = src[i];
  }
  //add '\0'
  dst[i] = '\0';
  return dst;
  //panic("Not implemented");
}

//str connect
char *strcat(char *dst, const char *src) {
  /*
  char *tmp = dst;
  while (*tmp) tmp++;
  while((*tmp = *src) != '\0'){
    tmp++;
    src++;
  }
  return dst;
  */
  //use strcpy
  size_t i = 0;
  while(dst[i] != '\0') i++; //Find where to insert
  strcpy(dst + i , src);
  return dst;
  //panic("Not implemented");
}

int strcmp(const char *s1, const char *s2) {
  while (*s1 && (*s1 == *s2)) {
    s1++;
    s2++;
  }
  return ((unsigned char)*s1) - ((unsigned char)*s2);
  //panic("Not implemented");
}

int strncmp(const char *s1, const char *s2, size_t n) {
  /*
  while(n--){
    if(*s1 > *s2) return 1;
    if(*s1 < *s2) return -1;
    s1++;
    s2++;
  }
  return 0;
  */
   while (n-- > 0 && *s1 && (*s1 == *s2)) {
      s1++;
      s2++;
    }
    return (n == 0) ? 0 : ((unsigned char)*s1) - ((unsigned char)*s2);
  //panic("Not implemented");
}

void *memset(void *s, int c, size_t n) {
  char *tmp = (char*) s;
  do{
    *tmp++ = c;
  }while(n -- > 0);
  return s;
  //panic("Not implemented");
}

void *memmove(void *dst, const void *src, size_t n) {
  if(dst < src)
  {
	  char *d = (char *) dst;
	  char *s = (char *) src;
	  while(n--)
	  {
		  *d = *s;
		  d++;
		  s++;
	  }
  }
  else
  {
	  char *d = (char *) dst + n - 1;
	  char *s = (char *) src + n - 1;
	  while(n--)
	  {
		  *d = *s;
		  d--;
		  s--;
	  }
  }
  return dst;
  //panic("Not implemented");
}

void *memcpy(void *out, const void *in, size_t n) {
  char *char_out = (char *)out;
  const char *char_in = (const char *)in;
  for (int i = 0; i < n; ++i){
    char_out[i] = char_in[i];
  }

  return out;
  //panic("Not implemented");
}

int memcmp(const void *s1, const void *s2, size_t n) {
  char *char_s1 = (char *)s1;
  char *char_s2 = (char *)s2;

  int i;
  for (i = 0; i < n; ++i){
    if (char_s1[i] != char_s2[i])
      return (int)(char_s1[i]) - (int)(char_s2[i]);
  }
  if (i == n)
    return 0;
  return (int)(char_s1[i]) - (int)(char_s2[i]);
  //panic("Not implemented");
}

#endif

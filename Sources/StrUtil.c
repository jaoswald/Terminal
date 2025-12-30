/* Retro68 version of CtoPstr/PtoCStr */

#include "StrUtil.h"

#include <assert.h>
#include <string.h>

void CtoPstr(char *c_str) {
  size_t c_len = strlen(c_str);
  assert(c_len < 256);
  memcpy(/*dest =*/ c_str+1, c_str, c_len);
  c_str[0] = (unsigned char) c_len;
}

void PtoCstr(char *p_str) {
  unsigned char p_len = p_str[0];
  memcpy(/*dest =*/ p_str, p_str+1, p_len);
  p_str[p_len] = '\0';
}

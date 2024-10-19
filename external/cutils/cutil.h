#ifndef __CUTIL_H__
#define __CUTIL_H__

char * copyCString(const char * s);

#endif // __CUTIL_H__

#ifdef __CUTIL_IMPLEMENTATION__
#include <stdlib.h>
#include <string.h>

char * copyCString(const char * s) {
    char * r = calloc(strlen(s) + 1, sizeof(char));    
    return strcpy(r, s);
}

#endif // __CUTIL_IMPLEMENTATION__

#ifndef ERROR_H
#define ERROR_H

#define glCheckError() glCheckError_(__FILE__, __LINE__)

int glCheckError_(const char *file, int line);

#endif /* error.h */

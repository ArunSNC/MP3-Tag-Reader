#ifndef TYPES_H
#define TYPES_H

/* User defined types */
typedef unsigned int uint;
typedef unsigned char uchar;
typedef unsigned long int ulint;

/* Status will be used in fn. return type */
typedef enum
{
    mp3_success,
    mp3_failure
} Status;

typedef enum
{
    mp3_edit,
    mp3_display,
    mp3_help,
    mp3_unsupported
} OperationType;

#endif

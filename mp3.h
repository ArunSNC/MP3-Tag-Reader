#ifndef MP3_H
#define MP3_H

#include"types.h"
typedef struct _Mp3Info
{
    /* Source Image info */
    char *fname;
    char *flag;
    char *data;
    FILE *fptr;
    uint file_size;
    uint buffer;
    char ver_id[4];
    char artist[31];
    char song_title[31];
    char year[5];
    char comment[31];
    char album[31];

} Mp3Info;

#endif

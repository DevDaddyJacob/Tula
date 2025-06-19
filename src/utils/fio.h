#ifndef tula_utils_fio
#define tula_utils_fio

#include <stdio.h>
#include <limits.h>

#include "../common.h"


#define INVALID_CHAR -2


typedef struct tulaFio_ReaderPeekQueue {
    char* data;
    UInt8 count;
    UInt8 current;
} ReaderPeekQueue;


typedef struct tulaFio_FileReader {
    /**
     * \brief           The next character in the file to be read
     */
    char next;

    /**
     * \brief           Stack of characters which have been peeked
     */
    ReaderPeekQueue* peek;

    /**
     * \brief           The length of the file, including the line terminator
     */
    Int32 size;

    /**
     * \brief           Counter for how many characters have been read
     */
    Int32 charsRead;

    /**
     * \brief           Pointer to the file being read
     * \note            Internal/Private value
     */
    FILE* file;
} FileReader;


#define tulaFio_hasNext(reader) \
    (reader->next != EOF)


#define tulaFio_peek(reader) \
    (reader->next)


TULA_FUNC FileReader* tulaFio_openReader(const char* filePath);


TULA_FUNC void tulaFio_closeReader(FileReader* reader);


TULA_FUNC void tulaFio_consume(FileReader* reader);


TULA_FUNC char tulaFio_read(FileReader* reader);


TULA_FUNC char tulaFio_peekN(FileReader* reader, UInt8 n);


#endif /* tula_utils_fio */
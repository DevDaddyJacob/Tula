#ifndef tula_utils_io
#define tula_utils_io

#include "../common.h"

typedef struct {
    const char* start;      /** The start of the buffer data */
    char* next;             /** The next character in the buffer */
    UInt32 size;            /** Lenth of the data including the line term */
} BufferReader;


#define tulaIo_hasNext(reader) \
    (reader->next < reader->start + reader->size)


#define tulaIo_currentIndex(reader) \
    (reader->next - reader->start)
    

#define tulaIo_read(reader) \
    (*(reader->next++))


#define tulaIo_comsume(reader) \
    (reader->next++)
    

#define tulaIo_peek(reader) \
    ((char)(*(reader->next)))


#define tulaIo_peekN(reader, n) \
    ((char*)((reader->next + n > reader->start + reader->size) \
        ? NULL : reader->next + n))


TULA_FUNC BufferReader* tulaIo_openBuffer(const char* data);


TULA_FUNC void tulaIo_closeBuffer(BufferReader* reader);


#endif /* tula_utils_io */
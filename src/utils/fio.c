#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

#include "fio.h"
#include "uint64.h"
#include "../common.h"
#include "../platform.h"

/*
 * ==================================================
 * Macros
 * ==================================================
 */

#define MAX_PEEK UCHAR_MAX

#define MAX_PEEK_INDEX (MAX_PEEK - 1)

#define peekQueue_isEmpty(queue) \
    (queue->count == 0)

#define peekQueue_isFull(queue) \
    (queue->count == MAX_PEEK)

#define peekQueue_nextIndex(queue) \
    (peekQueue_isFull(queue) \
        ? -1 \
        : (queue->current + queue->count >= MAX_PEEK \
            ? (queue->current + queue->count) - MAX_PEEK \
            : queue->current + queue->count))


/*
 * ==================================================
 * Typedefs & Prototypes
 * ==================================================
 */

static ReaderPeekQueue* peekQueue_new();


static void peekQueue_destroy(ReaderPeekQueue* queue);


static void peekQueue_enqueue(ReaderPeekQueue* queue, char value);


static char peekQueue_dequeue(ReaderPeekQueue* queue);


static char peekQueue_getAt(ReaderPeekQueue* queue, UInt8 i);


/*
 * ==================================================
 * Module Level Variables & Constants
 * ==================================================
 */

/* int magicNumber = 420; */


/*
 * ==================================================
 * Function Definitions
 * ==================================================
 */

static ReaderPeekQueue* peekQueue_new() {
    ReaderPeekQueue* queue;

    /* Allocate the queue to the heap */
    queue = (ReaderPeekQueue*)malloc(sizeof(ReaderPeekQueue));
    if (queue == NULL) return NULL;


    /* Allocate the data */
    queue->data = (char*)malloc(sizeof(char) * MAX_PEEK);
    if (queue->data == NULL) {
        peekQueue_destroy(queue);
        return NULL;
    }


    /* Default the fields */
    queue->count = 0;
    queue->current = 0;
    
    return queue;
}


static void peekQueue_destroy(ReaderPeekQueue* queue) {
    if (queue == NULL) return;
    if (queue->data != NULL) free(queue->data);
    free(queue);
}


static void peekQueue_enqueue(ReaderPeekQueue* queue, char value) {
    Int16 nextI;

    /* Null check of sanity */
    if (queue == NULL) return;
    nextI = peekQueue_nextIndex(queue);


    if (nextI < 0) {
        tula_errPrint("Cannot enqueue on a already full queue!");
        return;
    }


    queue->data[nextI] = value;
    queue->count++;
}


static char peekQueue_dequeue(ReaderPeekQueue* queue) {
    char data;

    /* Null check of sanity */
    if (queue == NULL) return;
    
    if (peekQueue_isEmpty(queue)) return INVALID_CHAR;

    data = queue->data[queue->current];
    queue->current = queue->current == MAX_PEEK_INDEX ? 0 : queue->current + 1;
    queue->count--;

    return data;
}


static char peekQueue_getAt(ReaderPeekQueue* queue, UInt8 i) {
    /* Null check of sanity */
    if (queue == NULL) return;

    
    if (peekQueue_isEmpty(queue)) return INVALID_CHAR;
    if (i + 1 > queue->count) return INVALID_CHAR;

    return queue->data[
        (queue->current + i) >= MAX_PEEK_INDEX 
            ? (queue->current + i) - MAX_PEEK
            : (queue->current + i)
    ];
}


TULA_FUNC FileReader* tulaFio_openReader(const char* filePath) {
    FileReader* reader;

    /* Ensure the path exists and is a file */
    if (!tulaPlat_isFile(filePath)) return NULL;


    /* Allocate the reader onto the heap */
    reader = (FileReader*)malloc(sizeof(FileReader));
    if (reader == NULL) return NULL;


    /* Allocate the peek queue onto the heap */
    reader->peek = peekQueue_new();
    if (reader->peek == NULL) {
        tulaFio_closeReader(reader);
        return NULL;
    }


    /* Open the file in a reading mode */
    reader->file = fopen(filePath, "r");
    if (reader->file == NULL) {
        tulaFio_closeReader(reader);
        return NULL;
    }


    /* Get the size of the file */
    fseek(reader->file, 0L, SEEK_END);
    reader->size = ftell(reader->file);
    rewind(reader->file);


    /* Prime the first character */
    reader->next = fgetc(reader->file);

    return reader;
}


TULA_FUNC void tulaFio_closeReader(FileReader* reader) {
    if (reader == NULL) return;

    if (reader->peek != NULL) {
        peekQueue_destroy(reader->peek);
    }

    if (reader->file != NULL) fclose(reader->file);
}


TULA_FUNC void tulaFio_consume(FileReader* reader) {
    /* Null check of sanity */
    if (reader == NULL) return;


    /* If the next value if EOF, don't advance */
    if (reader->next == EOF) {
        reader->charsRead = reader->size;
        return;
    }


    /* Store the char to return and replace it */
    reader->next = !peekQueue_isEmpty(reader->peek)
        ? peekQueue_dequeue(reader->peek)
        : fgetc(reader->file);

    reader->charsRead++;

    return;
}


TULA_FUNC char tulaFio_read(FileReader* reader) {
    char value;

    /* Null check of sanity */
    if (reader == NULL) return INVALID_CHAR;


    /* If the next value if EOF, don't advance */
    if (reader->next == EOF) {
        reader->charsRead = reader->size;
        return EOF;
    }


    /* Store the char to return and replace it */
    value = reader->next;
    reader->next = !peekQueue_isEmpty(reader->peek)
        ? peekQueue_dequeue(reader->peek)
        : fgetc(reader->file);

    reader->charsRead++;

    return value;
}


TULA_FUNC char tulaFio_peekN(FileReader* reader, UInt8 n) {
    /* Null check of sanity */
    if (reader == NULL) return INVALID_CHAR;


    /* If we have at least n chars peeked, return the stored value */
    if (reader->peek->count >= n) return peekQueue_getAt(reader->peek, n - 1);


    /* Enqueue peeked chars until we have enough */
    printf("* queueing from file\n");
    while (reader->peek->count < n) {
        peekQueue_enqueue(reader->peek, fgetc(reader->file));
    }

    return peekQueue_getAt(reader->peek, n - 1);
}

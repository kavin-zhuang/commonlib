#ifndef _RINGBUF_H_
#define _RINGBUF_H_

#define RINGBUF_LENGTH  256

typedef struct _ringbuf
{
    unsigned char buffer[RINGBUF_LENGTH];
    unsigned int length;

	unsigned int datalen;

    unsigned int head;
    unsigned int tail;
} ringbuf_t;

void ringbuf_init(ringbuf_t *ringbuf);
unsigned int ringbuf_put(ringbuf_t *ringbuf, unsigned char *buffer, unsigned int length);
unsigned int ringbuf_get(ringbuf_t *ringbuf, unsigned char *buffer, unsigned int length);
unsigned int ringbuf_get_datalen(ringbuf_t *ringbuf);

#endif


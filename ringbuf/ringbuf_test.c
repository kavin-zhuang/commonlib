#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "ringbuf.h"

#define assert(test) {if(!(test)) \
    {printf("%s: line %d\n", __FUNCTION__, __LINE__); return __LINE__;}}

static ringbuf_t ringbuf;

unsigned char data[RINGBUF_LENGTH] = { '\0' };
unsigned char data_in[RINGBUF_LENGTH] = { '1' };

int ringbuf_test()
{
    int ret;

    int i;

    int rand_value;

    srand((unsigned int)time(NULL));

    for (i = 0; i < 1000000; i++) {
        rand_value = rand() % RINGBUF_LENGTH;

        if (rand_value % 2) {
            ret = ringbuf_put(&ringbuf, data_in, rand_value);
            assert((0 == ret) || (rand_value == ret));
        }
        else {
            // get all data out of ring
            do {
                ret = ringbuf_get_datalen(&ringbuf);
                rand_value = rand() % RINGBUF_LENGTH;
                if (rand_value <= ret) {
                    ret = ringbuf_get(&ringbuf, data, rand_value);
                    assert(rand_value == ret);
                }
            } while (0 != ringbuf_get_datalen(&ringbuf));
        }
    }

    printf("PASS\n");

    return 0;
}

void main(void)
{
    ringbuf_test();
}

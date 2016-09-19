#include <stdio.h>

/*

unsigned int regvalues[] = {
    0x00000000, 0x00000000, 0x00000000, 0x00000000,
    ....
};

*/

void reg32_clone(volatile unsigned int *start, int num)
{
    int i;

    int line = num / 4;
    int last = num % 4;

    printf("unsigned int regvalues[] = {\n");

    for (i = 0; i< line; i++, start += 4) {
        printf("    0x%08X, 0x%08X, 0x%08X, 0x%08X,\n",
            start[0], start[1], start[2], start[3]);
    }

    if (last) {
        printf("    ");
        for (i = 0; i < last; i++) {
            printf("0x%08X ", start[i]);
        }
        printf("\n");
    }

    printf("};\n");
}

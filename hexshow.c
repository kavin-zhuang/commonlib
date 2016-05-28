#include <stdio.h>

void hex_show(unsigned char *buffer, unsigned int length)
{
    unsigned int row, col, row_bytes;

    for (row = 0; row < length / 16; row++)
    {
        row_bytes = (row == length / 16) ? (row % 16) : 16;

        printf("\n%08X : ", row * 16);

        for (col = 0; col < row_bytes; col++)
        {
            printf("%02X ", buffer[row * 16 + col]);
        }

        printf("; ");

        for (col = 0; col < row_bytes; col++)
        {
            if ((buffer[row * 16 + col] >= 32) && (buffer[row * 16 + col] <= 126)) {
                printf("%c", buffer[row * 16 + col]);
            }
            else {
                printf(".");
            }
        }
    }
    printf("\n");
}

/*
 * Copyright (C) 2016, Jinfeng Zhuang
 *
 * BUG FIX:
 *   1. show line-1
 * */

#include <stdio.h>

#define SHOW(m, arg...) printf(m, ##arg)

void hex_show(unsigned char *buffer, unsigned int length)
{
    unsigned int row, col, row_bytes, row_total;
    
    if(NULL == buffer) {
    	return;
    }

    // fix bug, the last line should be added
    row_total = (length % 16) ? (length / 16 + 1) : (length / 16);
    
    printf("row_total = %d, length = %d\n", row_total, length);

    for (row = 0; row < row_total; row++)
    {
    	if(row != row_total - 1) {
    		row_bytes = 16;
    	}
    	else{
    		row_bytes = (length % 16) ? (length % 16) : 16;
    	}

        SHOW("\n%08X : ", row * 16);

        // fix with blank
        for (col = 0; col < 16; col++)
        {
            if (col < row_bytes)
            {
                SHOW("%02X ", buffer[row * 16 + col]);
            }
            else
            {
                SHOW("   ");
            }
        }

        SHOW("; ");

        // fix with blank
        for (col = 0; col < 16; col++)
        {
            if (col < row_bytes)
            {
                if ((buffer[row * 16 + col] >= 32) && (buffer[row * 16 + col] <= 126))
                {
                    SHOW("%c", buffer[row * 16 + col]);
                }
                else
                {
                    SHOW(".");
                }
            }
            else
            {
                SHOW("   ");
            }
        }
    }

    printf("\n");
}

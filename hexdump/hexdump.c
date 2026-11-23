#include <stddef.h>
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>

int open_file(const char* filename);

int main(int argc, char* argv[])
{
    assert(argv != NULL);

    if(argc != 2)
    {
        fprintf(stderr, "Usage: %s <file>\n", argv[0]);
        return 1;
    }

    return open_file(argv[1]);

}

int open_file(const char* filename)
{
    unsigned char arr[16];
    assert(filename != NULL); 
    FILE* fptr = fopen(filename, "rb");


    if (fptr == NULL)
    {
        fprintf(stderr, "Failed to open file.\n");
        return 2;
    }

    
    assert(fptr != NULL && arr != NULL);

    size_t bytes_read;
    size_t offset = 0;
    
     while ((bytes_read = fread(arr, sizeof(unsigned char),16, fptr)) > 0)
    {
        assert(bytes_read <= 16);
        printf("%08zX ", offset);

        for (size_t i = 0; i < bytes_read; i++)
        {
            assert(i < bytes_read);
            printf("%02X ", arr[i]);

        }
        
        offset += bytes_read;

        for (size_t i = bytes_read; i < 16; i++)
        {
            printf("   "); // same width as "XX "
            
        }
        printf((" |"));

        for (size_t i = 0; i < bytes_read; i++)
        {
            if(isprint(arr[i]))
            {
                printf("%c", arr[i]);
            }
            else
            {
                printf(".");
            }

            
        }
        
        printf("|\n");
    }

    fclose(fptr);
    return 0;

    
}

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

int search_file(const char* keyword, const char *filename);

int main(int argc, char* argv[])
{
    assert(argv != NULL);
    if (argc != 3)
    {
        fprintf(stderr, "Usage: %s <keyword> <file>\n", argv[0]);
        return 1;
    }
    return search_file(argv[1], argv[2]);

}

int search_file(const char* keyword, const char *filename)
{
    assert(keyword != NULL && filename != NULL);

    char line[1024];
    size_t matches = 0;

    FILE* fptr = fopen(filename, "r");

    if (fptr == NULL)
    {
        fprintf(stderr, "Failed to open file\n");
        return 2;
    }

    size_t line_number = 1;

    while(fgets(line, sizeof(line), fptr))
    {
        char* match = strstr(line, keyword);
        if (match != NULL)
        {
            printf("%zu: ", line_number);

            const char* cursor = line;
            size_t key_len = strlen(keyword);

            while (match != NULL)
            {
                printf("%.*s", (int)(match - cursor), cursor);

                printf("\033[1;31m%s\033[0m", keyword);

                cursor = match + key_len;
                match = strstr(cursor, keyword);

            }

            printf("%s", cursor);

            

            matches++;
        }
        line_number++;
        
    }
    
    printf("Total found: %zu \n", matches);

    

    fclose(fptr);
    return 0;

}

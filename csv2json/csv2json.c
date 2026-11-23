#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>

#define MAX_COLS 32
#define MAX_LEN 128
#define MAX_LINE 1024

int convert_csv_to_json(const char* csv_path, const char* json_path);
static size_t split_line(char* line, char fields[MAX_COLS][MAX_LEN]);
static void write_json_string(FILE* out, const char* s);

int main(int argc, char* argv[])
{
    if (argc != 3)
    {
        fprintf(stderr, "Usage: %s <input.csv> <output.json>\n", argv[0]);
        return 1;
    }
    return convert_csv_to_json(argv[1], argv[2]);
}

int convert_csv_to_json(const char* csv_path, const char* json_path)
{
    assert(csv_path != NULL && json_path != NULL);

    char headers[MAX_COLS][MAX_LEN];
    char values[MAX_COLS][MAX_LEN];
    char line[MAX_LINE];

    FILE* csvopen = fopen(csv_path, "r");
    if (csvopen == NULL)
    {
        fprintf(stderr, "Failed to open CSV file: %s\n", csv_path);
        return 1;
    }

    if (fgets(line, sizeof(line), csvopen) == NULL)
    {
        fprintf(stderr, "CSV file is empty: %s\n", csv_path);
        fclose(csvopen);
        return 1;
    }
    size_t col_count = split_line(line, headers);

    FILE* jsonopen = fopen(json_path, "w");
    if (jsonopen == NULL)
    {
        fprintf(stderr, "Failed to open JSON file: %s\n", json_path);
        fclose(csvopen);
        return 2;
    }

    size_t rows = 0;
    fprintf(jsonopen, "[");
    while (fgets(line, sizeof(line), csvopen) != NULL)
    {
        if (line[0] == '\n' || line[0] == '\r')
        {
            continue; // skip empty lines
        }
        size_t value_count = split_line(line, values);

        fprintf(jsonopen, "%s\n  {", rows == 0 ? "" : ",");
        for (size_t i = 0; i < col_count; i++)
        {
            fprintf(jsonopen, "%s", i == 0 ? "" : ", ");
            write_json_string(jsonopen, headers[i]);
            fprintf(jsonopen, ": ");
            // missing values at the end of a row become empty strings
            write_json_string(jsonopen, i < value_count ? values[i] : "");
        }
        fprintf(jsonopen, "}");
        rows++;
    }
    fprintf(jsonopen, "\n]\n");

    fclose(jsonopen);
    fclose(csvopen);
    printf("Converted %zu rows with %zu columns\n", rows, col_count);
    return 0;
}

// Splits one CSV line on commas into fields. Returns the number of fields.
// shortcut: no support for quoted fields with commas inside ("a,b"), add a small state machine if needed.
static size_t split_line(char* line, char fields[MAX_COLS][MAX_LEN])
{
    assert(line != NULL);

    line[strcspn(line, "\r\n")] = '\0';

    size_t count = 0;
    char* start = line;
    while (count < MAX_COLS)
    {
        char* comma = strchr(start, ',');
        size_t len = comma ? (size_t)(comma - start) : strlen(start);
        if (len >= MAX_LEN)
        {
            len = MAX_LEN - 1;
        }
        memcpy(fields[count], start, len);
        fields[count][len] = '\0';
        count++;

        if (comma == NULL)
        {
            break;
        }
        start = comma + 1;
    }
    return count;
}

// Writes s as a JSON string, escaping quotes, backslashes and control characters.
static void write_json_string(FILE* out, const char* s)
{
    assert(out != NULL && s != NULL);

    fputc('"', out);
    for (; *s != '\0'; s++)
    {
        unsigned char c = (unsigned char)*s;
        if (c == '"' || c == '\\')
        {
            fprintf(out, "\\%c", c);
        }
        else if (c < 0x20)
        {
            fprintf(out, "\\u%04x", c);
        }
        else
        {
            fputc(c, out);
        }
    }
    fputc('"', out);
}

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "function.h"

void view_mp3(char *filename)
{
    FILE *fp = fopen(filename, "rb");

    if (fp == NULL)
    {
        printf("ERROR: Unable to open file\n");
        return;
    }

    unsigned char header[10];

    if (fread(header, 1, 10, fp) != 10)
    {
        printf("ERROR: Unable to read MP3 file\n");
        fclose(fp);
        return;
    }

    /* Check ID3 */
    if (header[0] != 'I' ||
        header[1] != 'D' ||
        header[2] != '3')
    {
        printf("ID3 tag not found\n");
        fclose(fp);
        return;
    }

    /* Check ID3 version */
    if (header[3] != 3)
    {
        printf("Only ID3v2.3 supported\n");
        fclose(fp);
        return;
    }

    /* Move after ID3 header */
    fseek(fp, 10, SEEK_SET);

    int found = 0;

    while (found < 6)
    {
        char frame_id[5];

        /* Read frame ID */
        if (fread(frame_id, 1, 4, fp) != 4)
        {
            break;
        }

        frame_id[4] = '\0';

        /* Check valid frame */
        if (frame_id[0] < 'A' ||
            frame_id[0] > 'Z')
        {
            break;
        }

        /* Read frame size */
        unsigned char size[4];

        if (fread(size, 1, 4, fp) != 4)
        {
            break;
        }

        /* Big endian to little endian */
        char *ptr = (char *)size;

        for (int i = 0; i < 2; i++)
        {
            char temp = ptr[i];

            ptr[i] = ptr[3 - i];

            ptr[3 - i] = temp;
        }

        unsigned int frame_size;

        memcpy(&frame_size, size, 4);

        /*
           COMM FRAME
        */
        if (strcmp(frame_id, "COMM") == 0)
        {
            /* Skip flags */
            fseek(fp, 2, SEEK_CUR);

            if (frame_size < 5)
            {
                printf("Invalid COMM frame\n");
                fclose(fp);
                return;
            }

            /*
               Skip:
               encoding = 1 byte
               language = 3 bytes
               description = 1 byte
            */
            fseek(fp, 5, SEEK_CUR);

            unsigned int comment_size = frame_size - 5;

            char *comment_data =
                malloc(comment_size + 1);

            if (comment_data == NULL)
            {
                printf("Memory allocation failed\n");
                fclose(fp);
                return;
            }

            if (fread(comment_data,
                      1,
                      comment_size,
                      fp) != comment_size)
            {
                free(comment_data);
                break;
            }

            comment_data[comment_size] = '\0';

            printf("Comment: %s\n", comment_data);

            free(comment_data);

            found++;
        }

        /*
           TEXT FRAMES
        */
        else if (strcmp(frame_id, "TIT2") == 0 ||
                 strcmp(frame_id, "TPE1") == 0 ||
                 strcmp(frame_id, "TALB") == 0 ||
                 strcmp(frame_id, "TYER") == 0 ||
                 strcmp(frame_id, "TCON") == 0)
        {
            /*
               Skip:
               flags = 2 bytes
               encoding = 1 byte
            */
            fseek(fp, 3, SEEK_CUR);

            if (frame_size < 1)
            {
                printf("Invalid frame size\n");
                break;
            }

            frame_size = frame_size - 1;

            char *data =
                malloc(frame_size + 1);

            if (data == NULL)
            {
                printf("Memory allocation failed\n");
                break;
            }

            if (fread(data,
                      1,
                      frame_size,
                      fp) != frame_size)
            {
                free(data);
                break;
            }

            data[frame_size] = '\0';

            if (strcmp(frame_id, "TIT2") == 0)
            {
                printf("Title: %s\n", data);
                found++;
            }

            else if (strcmp(frame_id, "TPE1") == 0)
            {
                printf("Artist: %s\n", data);
                found++;
            }

            else if (strcmp(frame_id, "TALB") == 0)
            {
                printf("Album: %s\n", data);
                found++;
            }

            else if (strcmp(frame_id, "TYER") == 0)
            {
                printf("Year: %s\n", data);
                found++;
            }

            else if (strcmp(frame_id, "TCON") == 0)
            {
                printf("Genre: %s\n", data);
                found++;
            }

            free(data);
        }

        /*
           Unknown frame
        */
        else
        {
            fseek(fp, 2 + frame_size, SEEK_CUR);
        }
    }

    fclose(fp);

    if (found < 6)
    {
        printf("Required tag(s) not found\n");
    }
}
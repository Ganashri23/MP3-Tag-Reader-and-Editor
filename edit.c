#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "function.h"

void edit_mp3(EditInfo *edit)
{
    FILE *src = fopen(edit->filename, "rb");

    FILE *temp = fopen("temp.mp3", "wb");

    if (src == NULL || temp == NULL)
    {
        printf("Error opening file\n");

        if (src != NULL)
            fclose(src);

        if (temp != NULL)
            fclose(temp);

        return;
    }

    /* Read ID3 header */
    unsigned char header[10];

    if (fread(header, 1, 10, src) != 10)
    {
        printf("Error reading file\n");

        fclose(src);
        fclose(temp);

        remove("temp.mp3");

        return;
    }

    /* Check ID3 */
    if (header[0] != 'I' ||
        header[1] != 'D' ||
        header[2] != '3')
    {
        printf("ID3 tag not found\n");

        fclose(src);
        fclose(temp);

        remove("temp.mp3");

        return;
    }

    /* Check version */
    if (header[3] != 3)
    {
        printf("Only ID3v2.3 supported\n");

        fclose(src);
        fclose(temp);

        remove("temp.mp3");

        return;
    }

    /* Write header */
    fwrite(header, 1, 10, temp);

    int edited = 0;

    while (1)
    {
        char frame_id[5];

        /* Read frame ID */
        if (fread(frame_id, 1, 4, src) != 4)
        {
            break;
        }

        frame_id[4] = '\0';

        /* Invalid frame */
        if (frame_id[0] < 'A' ||
            frame_id[0] > 'Z')
        {
            break;
        }

        /* Read frame size */
        unsigned char size[4];

        if (fread(size, 1, 4, src) != 4)
        {
            break;
        }

        unsigned char original_size[4];

        memcpy(original_size, size, 4);

        /* Big endian to little endian */
        char *ptr = (char *)size;

        for (int i = 0; i < 2; i++)
        {
            char temp_byte = ptr[i];

            ptr[i] = ptr[3 - i];

            ptr[3 - i] = temp_byte;
        }

        unsigned int frame_size;

        memcpy(&frame_size, size, 4);

        /* Read flags */
        unsigned char flags[2];

        if (fread(flags, 1, 2, src) != 2)
        {
            break;
        }

        char *data = malloc(frame_size);

        if (data == NULL)
        {
            printf("Memory allocation failed\n");

            fclose(src);
            fclose(temp);

            remove("temp.mp3");

            return;
        }

        if (fread(data, 1, frame_size, src) != frame_size)
        {
            free(data);

            fclose(src);
            fclose(temp);

            remove("temp.mp3");

            return;
        }

        /*
           EDIT TEXT FRAMES
        */
        if ((strcmp(edit->option, "-t") == 0 &&
             strcmp(frame_id, "TIT2") == 0) ||

            (strcmp(edit->option, "-a") == 0 &&
             strcmp(frame_id, "TPE1") == 0) ||

            (strcmp(edit->option, "-A") == 0 &&
             strcmp(frame_id, "TALB") == 0) ||

            (strcmp(edit->option, "-y") == 0 &&
             strcmp(frame_id, "TYER") == 0) ||

            (strcmp(edit->option, "-g") == 0 &&
             strcmp(frame_id, "TCON") == 0))
        {
            edited = 1;

            /*
               Encoding byte + new text
            */
            unsigned int new_size =
                strlen(edit->new_info) + 1;

            unsigned char new_size_bytes[4];

            memcpy(new_size_bytes,
                   &new_size,
                   4);

            /* Little endian to big endian */
            char *ptr2 =
                (char *)new_size_bytes;

            for (int i = 0; i < 2; i++)
            {
                char temp_byte = ptr2[i];

                ptr2[i] = ptr2[3 - i];

                ptr2[3 - i] = temp_byte;
            }

            /* Frame ID */
            fwrite(frame_id, 1, 4, temp);

            /* New frame size */
            fwrite(new_size_bytes, 1, 4, temp);

            /* Flags */
            fwrite(flags, 1, 2, temp);

            /* Keep original encoding */
            unsigned char encoding = data[0];

            fwrite(&encoding, 1, 1, temp);

            /* New value */
            fwrite(edit->new_info,
                   1,
                   strlen(edit->new_info),
                   temp);
        }

        /*
           EDIT COMMENT
        */
        else if (strcmp(edit->option, "-c") == 0 &&
                 strcmp(frame_id, "COMM") == 0)
        {
            edited = 1;

            /*
               encoding = 1
               language = 3
               description = 1
               new comment = strlen
            */
            unsigned int new_size =
                strlen(edit->new_info) + 5;

            unsigned char new_size_bytes[4];

            memcpy(new_size_bytes,
                   &new_size,
                   4);

            /* Little endian to big endian */
            char *ptr2 =
                (char *)new_size_bytes;

            for (int i = 0; i < 2; i++)
            {
                char temp_byte = ptr2[i];

                ptr2[i] = ptr2[3 - i];

                ptr2[3 - i] = temp_byte;
            }

            /* Frame ID */
            fwrite(frame_id, 1, 4, temp);

            /* New size */
            fwrite(new_size_bytes, 1, 4, temp);

            /* Flags */
            fwrite(flags, 1, 2, temp);

            /* Encoding */
            fwrite(data, 1, 1, temp);

            /* Language */
            fwrite(data + 1, 1, 3, temp);

            /* Empty description */
            fwrite("\0", 1, 1, temp);

            /* New comment */
            fwrite(edit->new_info,
                   1,
                   strlen(edit->new_info),
                   temp);
        }

        /*
           COPY UNCHANGED FRAME
        */
        else
        {
            fwrite(frame_id, 1, 4, temp);

            fwrite(original_size,
                   1,
                   4,
                   temp);

            fwrite(flags, 1, 2, temp);

            fwrite(data,
                   1,
                   frame_size,
                   temp);
        }

        free(data);
    }

    /*
       No required tag was found
    */
    if (edited == 0)
    {
        fclose(src);
        fclose(temp);

        remove("temp.mp3");

        printf("Required tag not found\n");

        return;
    }

    /*
       Copy remaining MP3 audio data
    */
    char buffer[1024];

    size_t bytes;

    while ((bytes = fread(buffer,
                          1,
                          sizeof(buffer),
                          src)) > 0)
    {
        fwrite(buffer,
               1,
               bytes,
               temp);
    }

    fclose(src);
    fclose(temp);

    /*
       Delete original file
    */
    if (remove(edit->filename) != 0)
    {
        printf("Error deleting original file\n");

        remove("temp.mp3");

        return;
    }

    /*
       Rename temporary file
    */
    if (rename("temp.mp3",
               edit->filename) != 0)
    {
        printf("Error renaming temp file\n");

        return;
    }

    printf("MP3 file updated successfully\n");
}
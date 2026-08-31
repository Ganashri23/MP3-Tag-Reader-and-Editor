#include <stdio.h>
#include<string.h>
#include<stdlib.h>
#include "function.h"
void edit_mp3(EditInfo *edit)
{
    FILE *src = fopen(edit->filename, "rb");
    FILE *temp = fopen("temp.mp3", "wb");
    if (src == NULL || temp == NULL)
    {
        printf("Error opening file\n");
        return;
    }
    unsigned char header[10];

    fread(header, 1, 10, src); // read from src write in header

        if (header[0] != 'I' ||
            header[1] != 'D' ||
            header[2] != '3')
        {
            printf("ID3 tag not found\n");
            fclose(src);
            fclose(temp);
            return;
        }

        if (header[3] != 3)
        {
            printf("Only ID3v2.3 supported\n");
            fclose(src);
            fclose(temp);
            return;
        }
    fwrite(header, 1, 10, temp); // read from header write in temp
       int edited =0;
while(1)
{
    char frame_id[5];
  
    if (fread(frame_id, 1, 4, src) != 4)
    {
        break;
    }
    frame_id[4] = '\0';
    if (frame_id[0] < 'A' || frame_id[0] > 'Z')
    {
       break;
    }
    //printf("Frame ID: %s\n", frame_id);

    unsigned char size[4];
    if (fread(size, 1, 4, src) != 4)
    {
        break;
    }
    unsigned char original_size[4];
    memcpy(original_size, size, 4);

    // convertion of big to little
    char *ptr = (char *)size;

    for (int i = 0; i < 2; i++)
    {
        char temp_byte = ptr[i];
        ptr[i] = ptr[4 - i - 1];
        ptr[4 - i - 1] = temp_byte;
    }

    unsigned int frame_size;
    memcpy(&frame_size, size, 4);
    unsigned char flags[2];

    if (fread(flags, 1, 2, src) != 2)
    {
       break ;
    }
    char *data = malloc(frame_size);
    if (data == NULL)
    {
        printf("Memory allocation failed\n");
        fclose(src);
        fclose(temp);
        return;
    }
    if (fread(data, 1, frame_size, src) != frame_size)
    {
        free(data);
        fclose(src);
        fclose(temp);
        return;
    }
    
    if ((strcmp(edit->option, "-t") == 0 && strcmp(frame_id, "TIT2") == 0) ||
    (strcmp(edit->option, "-a") == 0 && strcmp(frame_id, "TPE1") == 0) ||
    (strcmp(edit->option, "-A") == 0 && strcmp(frame_id, "TALB") == 0) ||
    (strcmp(edit->option, "-y") == 0 && strcmp(frame_id, "TYER") == 0) ||
    (strcmp(edit->option, "-g") == 0 && strcmp(frame_id, "TCON") == 0) 
     )
    {
        edited = 1;

        
         unsigned int new_size = strlen(edit->new_info) + 1;

       // printf("Old size: %u\n", frame_size);
       // printf("New size: %u\n", new_size);
            unsigned char new_size_bytes[4];

            memcpy(new_size_bytes, &new_size, 4);
            char *ptr = (char *)new_size_bytes;
        //little to big
            for (int i = 0; i < 2; i++)
            {
                char temp_byte = ptr[i];
                ptr[i] = ptr[4 - i - 1];
                ptr[4 - i - 1] = temp_byte;
            }

            fwrite(frame_id, 1, 4, temp);
            fwrite(new_size_bytes, 1, 4, temp);
            fwrite(flags, 1, 2, temp);
            unsigned char encoding = data[0];

            fwrite(&encoding, 1, 1, temp);
            fwrite(edit->new_info, 1, strlen(edit->new_info), temp);
        }

    // for comment 
    else if (strcmp(edit->option, "-c") == 0 && strcmp(frame_id, "COMM") == 0)
    {
           edited = 1;
        //printf("Comment frame found\n");

        unsigned int new_size = strlen(edit->new_info) + 5;

        unsigned char new_size_bytes[4];

        memcpy(new_size_bytes, &new_size, 4);

        char *ptr = (char *)new_size_bytes;

        for (int i = 0; i < 2; i++)
        {
            char temp_byte = ptr[i];
            ptr[i] = ptr[4 - i - 1];
            ptr[4 - i - 1] = temp_byte;
        }

        fwrite(frame_id, 1, 4, temp);
        fwrite(new_size_bytes, 1, 4, temp);
        fwrite(flags, 1, 2, temp);

        // Copy encoding
        fwrite(data, 1, 1, temp);

        // Copy language
        fwrite(data + 1, 1, 3, temp);

        // Empty description
        fwrite("\0", 1, 1, temp);

        // New comment
        fwrite(edit->new_info, 1, strlen(edit->new_info), temp);
    }
        // read remaining data
    else
    {
    fwrite(frame_id, 1, 4, temp);
    fwrite(original_size, 1, 4, temp);
    fwrite(flags, 1, 2, temp);
    fwrite(data, 1, frame_size, temp);
    }
    free(data);
    }

    // shouldn't replace the original sample.mp3 if edited == 0, because nothing was changed.
    if (edited == 0)
    {
        fclose(src);
        fclose(temp);
        remove("temp.mp3");

        printf("Required tag not found\n"); // if comm was not there
        return;
    }
// copying audio data segment 3
    char buffer[1024];
    size_t bytes;

    while ((bytes = fread(buffer, 1, sizeof(buffer), src)) > 0)
    {
        fwrite(buffer, 1, bytes, temp);
    }

    fclose(src);
    fclose(temp);
// renaming and removing
    if (remove(edit->filename) != 0)
    {
        printf("Error deleting original file\n");
        return;
    }

    if (rename("temp.mp3", edit->filename) != 0)
    {
        printf("Error renaming temp file\n");
        return;
    }

    printf("MP3 file updated successfully\n");
    }
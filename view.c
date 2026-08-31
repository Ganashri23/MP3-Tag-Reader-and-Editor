#include <stdio.h>
#include<string.h>
#include<stdlib.h>
#include "function.h"

void view_mp3(char *filename) // collect array of string
{
     FILE *fp = fopen(filename, "rb");

    if (fp == NULL)
    {
        printf("ERROR: Unable to open file\n");
        return;
    }
    fseek(fp, 10, SEEK_SET);
    int count = 0;
    int found = 0;
  //  printf("Position: %ld\n", ftell(fp)); // skips 10 bytes
  while(count < 80 && found < 6) 
  { 
  char frame_id[5];

    if (fread(frame_id, 1, 4, fp) != 4)
    {
        break;
    }
    if (frame_id[0] < 'A' || frame_id[0] > 'Z')
    {
        break;
    }

    frame_id[4] = '\0';

    //printf("Frame ID: %s\n", frame_id);
    // reading size 
    unsigned char size[4];

    if (fread(size, 1, 4, fp) != 4)
    {
        break;
    }
    // big to little endiann
    char *ptr = (char *)size;

    for (int i = 0; i < 2; i++)
    {
        char temp = ptr[i];
        ptr[i] = ptr[4 - i - 1];
        ptr[4 - i - 1] = temp;
    }
    // printf("%d %d %d %d\n",
    //     size[0], size[1], size[2], size[3]);
    // store size in integer format
    unsigned int frame_size;
    memcpy(&frame_size, size, 4);
     // ================= COMM =================
    if (strcmp(frame_id, "COMM") == 0)
    {
        // Skip flags
        fseek(fp, 2, SEEK_CUR);

        //skip encoding
        // unsigned char encoding;
        // fread(&encoding, 1, 1, fp);

        // Skip language 3 +encoding 1 + description 1
        fseek(fp, 5, SEEK_CUR);

        // Remaining = description + comment
        if (frame_size < 5)
        {
            printf("Invalid COMM frame\n");
            fclose(fp);
            return;
        }
        unsigned int comment_size = frame_size - 5;

        char *comment_data = malloc(comment_size + 1);// +1 for \0
        if (comment_data == NULL)
        {
            printf("Memory allocation failed\n");
            fclose(fp);
            return;
        }

        fread(comment_data, 1, comment_size, fp);

        comment_data[comment_size] = '\0';

        // Skip empty description
        printf("Comment: %s\n", comment_data );

        free(comment_data);

         found++;
    }

    else if(strcmp(frame_id, "TIT2") == 0 ||
             strcmp(frame_id, "TPE1") == 0 ||
             strcmp(frame_id, "TALB") == 0 ||
             strcmp(frame_id, "TYER") == 0 ||
             strcmp(frame_id, "TCON") == 0)
    {
    //printf("%u\n", frame_size);
     fseek(fp, 3, SEEK_CUR);
      if (frame_size < 1)
    {
        printf("Invalid frame size\n");
        break;
    }
     frame_size = frame_size - 1;
    

    char *data = malloc(frame_size + 1);

    if (data == NULL)
    {
        printf("Memory allocation failed\n");
        break;
    }

    fread(data, 1, frame_size, fp);

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
     else
    {
        // unknown frame → SKIP it
        fseek(fp, 2 + frame_size, SEEK_CUR);
    }
    count++;
    }
    fclose(fp);
    if (found < 6)
    {
        printf("Required tag(s) not found\n");
    } 
}
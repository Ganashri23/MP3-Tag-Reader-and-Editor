#include<stdio.h>
#include<string.h>
#include"function.h"
 
int main(int argc, char *argv[])
{
    
  if(argc==1)
   {
     
    choose();
     return 0;}
    /*
       CLA processing
       ./a.out -v sample.mp3
       ./a.out -e -t "new title" sample.mp3
       ./a.out --help
    */
   

      /* Help through CLA */
    if (strcmp(argv[1], "--help") == 0)
    {
        print_help();
        choose();
        return 0;
    }
    
        /* View through CLA */
    if (strcmp(argv[1], "-v") == 0)
    {
        if (argc != 3)
        {
            printf("\nERROR: INVALID ARGUMENTS\n");
            printf("Usage: ./a.out -v mp3filename\n");
            return 1;
        }

        // printf("View operation selected\n");
        // printf("MP3 file: %s\n", argv[2]);
        // check for extension 

        int len = strlen(argv[2]);

        if (len < 4 || strcmp(argv[2] + len - 4, ".mp3") != 0)
        {
          printf("ERROR: File should have .mp3 extension\n");
          return 1;
        }
        // open file
        FILE *fp; 
        fp = fopen(argv[2], "rb");

        if (fp == NULL)
        {
        printf("ERROR: Unable to open file\n");
        return 1;
        }
        char header[3];
        fread(header, 1, 3, fp);

        if (header[0] != 'I' ||
        header[1] != 'D' ||
        header[2] != '3')
        {
        printf("ID3 tag not found\n");
        fclose(fp);
        return 1;
        }
         printf("ID3 tag found\n");

         //check for version
        unsigned char version[2];
        fread(version, 1, 2, fp);
        printf("ID3 Version: %d.%d\n", version[0], version[1]);
        if (version[0] != 3)
        {
          printf("Only ID3v2.3 supported\n");
          fclose(fp);
          return 1;
        }
 
         //printf("%ld\n", ftell(fp));
         
        fclose(fp);
        view_mp3(argv[2]);
        choose( );
         
        return 0;
    }

 

        /* Edit */
    if (strcmp(argv[1], "-e") == 0)
    {
        if (argc != 5)
        {
            printf("\nERROR: INVALID ARGUMENTS\n");
            printf("Usage: ./a.out -e -t/-a/-A/-y/-g/-c(pass any one tag) \"new_value\" mp3filename\n");
            return 1;
        }

        if (strcmp(argv[2], "-t") != 0 &&
            strcmp(argv[2], "-a") != 0 &&
            strcmp(argv[2], "-A") != 0 &&
            strcmp(argv[2], "-y") != 0 &&
            strcmp(argv[2], "-g") != 0 &&
            strcmp(argv[2], "-c") != 0)
        {
            printf("\nERROR: INVALID EDIT OPTION\n");
            printf("Usage: ./a.out -e -t/-a/-A/-y/-g/-c (pass any one tag) \"new_value\" mp3filename\n");
            edit_help();
            return 1;
        }

        printf("Edit operation selected\n");
        printf("Option : %s\n", argv[2]);
        printf("New Info is  : %s\n", argv[3]);
        printf("File : %s\n", argv[4]);
        EditInfo edit;

        edit.option = argv[2];
        edit.new_info = argv[3];
        edit.filename = argv[4];

        edit_mp3(&edit);
        choose();
        return 0;
    }
 
 
/* Invalid first argument */
    printf("\n--------------------------------------------------\n");
    printf("ERROR: ./a.out : INVALID ARGUMENTS\n");

    printf("\nUSAGE:\n");
    printf("To view please pass like:\n");
    printf("./a.out -v mp3filename\n");

    printf("\nTo edit please pass like:\n");
    printf("./a.out -e -t/-a/-A/-y/-g/-c(any one tag) \"new_value\" mp3filename\n");

    printf("\nTo get help please pass like:\n");
    printf("./a.out --help\n");

    printf("--------------------------------------------------\n");

    return 1;
 }
 
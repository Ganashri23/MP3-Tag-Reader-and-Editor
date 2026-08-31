#include <stdio.h>
#include <string.h>
#include "function.h"

int main(int argc, char *argv[])
{
    /* Normal execution */
    if (argc == 1)
    {
        choose();
        return 0;
    }

    /* HELP */
    if (strcmp(argv[1], "--help") == 0 ||
        strcmp(argv[1], "-h") == 0)
    {
        print_help();
        return 0;
    }

    /* VIEW */
    if (strcmp(argv[1], "-v") == 0)
    {
        if (argc != 3)
        {
            printf("\nERROR: INVALID ARGUMENTS\n");
            printf("Usage: ./a.out -v mp3filename\n");
            return 1;
        }

        int len = strlen(argv[2]);

        if (len < 4 ||
            strcmp(argv[2] + len - 4, ".mp3") != 0)
        {
            printf("ERROR: File should have .mp3 extension\n");
            return 1;
        }

        view_mp3(argv[2]);

        return 0;
    }

    /* EDIT */
    if (strcmp(argv[1], "-e") == 0)
    {
        if (argc != 5)
        {
            printf("\nERROR: INVALID ARGUMENTS\n");
            printf("Usage: ./a.out -e -t/-a/-A/-y/-g/-c \"new_value\" mp3filename\n");
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
            printf("Usage: ./a.out -e -t/-a/-A/-y/-g/-c \"new_value\" mp3filename\n");
            return 1;
        }

        EditInfo edit;

        edit.option = argv[2];
        edit.new_info = argv[3];
        edit.filename = argv[4];

        edit_mp3(&edit);

        return 0;
    }

    /* INVALID ARGUMENT */
    printf("\n-----------------------------------------------\n");
    printf("ERROR: INVALID ARGUMENTS\n");
    printf("-----------------------------------------------\n");

    printf("\nUSAGE:\n");

    printf("\nTo view:\n");
    printf("./a.out -v mp3filename\n");

    printf("\nTo edit:\n");
    printf("./a.out -e -t/-a/-A/-y/-g/-c \"new_value\" mp3filename\n");

    printf("\nTo get help:\n");
    printf("./a.out --help\n");

    printf("-----------------------------------------------\n");

    return 1;
}
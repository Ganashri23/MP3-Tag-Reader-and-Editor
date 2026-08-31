#include <stdio.h>
#include <string.h>
#include "function.h"

void print_help(void)
{
    printf("\n");
    printf("-----------------------------------------------\n");
    printf("                    HELP MENU\n");
    printf("-----------------------------------------------\n");

    printf("1. -v  -> to view mp3 file contents\n");
    printf("2. -e  -> to edit mp3 file contents\n");
    printf("     2.1. -t -> to edit song title\n");
    printf("     2.2. -a -> to edit artist name\n");
    printf("     2.3. -A -> to edit album name\n");
    printf("     2.4. -y -> to edit year\n");
    printf("     2.5. -g -> to edit genre\n");
    printf("     2.6. -c -> to edit comment\n");

    printf("-----------------------------------------------\n");
}


void edit_help(void)
{
    printf("\n");
    printf("------------- EDIT OPTIONS -------------\n");

    printf("1. -t -> to edit song title\n");
    printf("2. -a -> to edit artist name\n");
    printf("3. -A -> to edit album name\n");
    printf("4. -y -> to edit year\n");
    printf("5. -g -> to edit genre\n");
    printf("6. -c -> to edit comment\n");

    printf("----------------------------------------\n");
}


void choose(void)
{
    int choice;
    char filename[100];

    /*
       Ask filename only once.
       Then show the main menu.
    */

    printf("Enter MP3 filename: ");
    scanf("%s", filename);

    while (1)
    {
        printf("\n========================================\n");
        printf("          MP3 TAG READER & EDITOR\n");
        printf("========================================\n");

        printf("1. View\n");
        printf("2. Edit\n");
        printf("3. Help\n");
        printf("4. Exit\n");

        printf("\nEnter your choice: ");
        scanf("%d", &choice);

        /* VIEW */
        if (choice == 1)
        {
            printf("\n---------- MP3 TAG DETAILS ----------\n");

            view_mp3(filename);
        }

        /* EDIT */
        else if (choice == 2)
        {
            int option;
            char new_value[200];

            printf("\n---------- EDIT MENU ----------\n");

            printf("1. Title\n");
            printf("2. Artist\n");
            printf("3. Album\n");
            printf("4. Year\n");
            printf("5. Genre\n");
            printf("6. Comment\n");

            printf("\nEnter your choice: ");
            scanf("%d", &option);

            printf("Enter new value: ");
            scanf(" %[^\n]", new_value);

            EditInfo edit;

            edit.filename = filename;
            edit.new_info = new_value;

            if (option == 1)
            {
                edit.option = "-t";
            }
            else if (option == 2)
            {
                edit.option = "-a";
            }
            else if (option == 3)
            {
                edit.option = "-A";
            }
            else if (option == 4)
            {
                edit.option = "-y";
            }
            else if (option == 5)
            {
                edit.option = "-g";
            }
            else if (option == 6)
            {
                edit.option = "-c";
            }
            else
            {
                printf("Invalid choice\n");
                continue;
            }

            edit_mp3(&edit);
        }

        /* HELP */
        else if (choice == 3)
        {
            print_help();
        }

        /* EXIT */
        else if (choice == 4)
        {
            printf("\nProgram Exited...\n");
            break;
        }

        else
        {
            printf("\nInvalid choice\n");
        }
    }
}
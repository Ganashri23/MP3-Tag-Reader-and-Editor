#include<stdio.h>
#include"function.h"
void print_help(void)
{
    printf("\n");
    printf("--------------------------------------------------\n");
    printf("                    HELP MENU\n");
    printf("--------------------------------------------------\n");

    printf("1. -v  -> to view mp3 file contents\n");
    printf("2. -e  -> to edit mp3 file contents\n");
    printf("     2.1. -t -> to edit song title\n");
    printf("     2.2. -a -> to edit artist name\n");
    printf("     2.3. -A -> to edit album name\n");
    printf("     2.4. -y -> to edit year\n");
    printf("     2.5. -g -> to edit content\n");
    printf("     2.6. -c -> to edit comment\n");

    printf("--------------------------------------------------\n");
}
void edit_help(void)
{
    
    printf("\n EDIT OPTIONS:\n");
    printf(" 1. -t -> to edit song title\n");
    printf(" 2. -a -> to edit artist name\n");
    printf(" 3. -A -> to edit album name\n");
    printf(" 4. -y -> to edit year\n");
    printf(" 5. -g -> to edit content\n");
    printf(" 6. -c -> to edit comment\n");

}
void choose(void)
{
    while(1)
    {

    
    int choice;
    printf("Enter your choice: \n");
    printf("1. View\n");
    printf("2. Edit\n");
    printf("3. Help\n");
    printf("4. Exit\n");
    scanf("%d", &choice);
    if(choice<1||choice>4)
    {
        printf("Invalid choice");
        return ;
    }
    
    /* Choice 1 - View */
    if (choice == 1)
    {
        printf("\nYou have chosen View option.\n");
        printf("Pass CLA as shown below:\n");
        printf("./a.out -v filename.mp3\n");

        return ;
    }

    /* Choice 2 - Edit */
    else if (choice == 2)
    {
        printf("\nYou have chosen Edit option.\n");
        printf("Pass CLA as shown below:\n");
        printf("./a.out -e -t/-a/-A/-y/-g/-c \"new_value\" mp3filename\n");

        return ;
    }

    /* Choice 3 - Help */
    else if (choice == 3)
    {
        print_help();
       // return ;
    }
    else if (choice == 4)
    {
        printf("Program Exited...");
        return ;
    }}}
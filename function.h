#ifndef FUNCTION_H
#define FUNCTION_H
typedef struct
{
    char *option;
    char *new_info;
    char *filename;
} EditInfo;
void print_help(void);
void view_mp3(char *filename);
void print_help(void);
void edit_help(void);
void edit_mp3(EditInfo *edit);
void choose(void);
#endif
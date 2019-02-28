//-----------------------------------------------------------------------------
// ass2.c
//
// Multiple-choice Game
//
// Group: 8 study assistant <Valentin Leutgeb>
//
// Author: Ilhan Skender <015531567>
//
// Latest Changes: 07.12.2017 (Ilhan Skender)
//-----------------------------------------------------------------------------
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef struct tree_elem
{
    char *title_;
    struct tree_elem *choice_a_;
    struct tree_elem *choice_b_;
    char *choice_a_title_;
    char *choice_b_title_;
    char *text_;
} tree_elem;

// ------------------------------------------------ -----------------------------
///
/// Parse file function
///
/// @(FILE *file) parameter
/// Takes the string from @(FILE *file),
/// and stores the string into the typedef struct Tree_Elem_
///
/// @return new_elem;
//

tree_elem* parseFile(FILE *file)
{
    tree_elem *new_elem = (tree_elem*)malloc(sizeof(tree_elem));
    new_elem->choice_a_ = NULL;
    new_elem->choice_b_ = NULL;
    new_elem->choice_a_title_ = NULL;
    new_elem->choice_b_title_ = NULL;
    new_elem->title_ = NULL;
    new_elem->text_ = NULL;

    int counter = 0, len;

    fseek(file, 0, SEEK_END);
    len = ftell(file);
    fseek(file, 0, SEEK_SET);

    char *file_stream = (char*)malloc(sizeof(char) * len);

    while((file_stream[counter] = (char)fgetc(file)) != EOF)
        counter++;

    file_stream[counter] = '\0';

    const char delimiter_data[2] = "\n", delimiter_text[2] = "\0";
    char *token;
    int i;

    token = strtok(file_stream, delimiter_data);
    new_elem->title_ = (char*)malloc(sizeof(char) * (strlen(token) + 1));
    strcpy(new_elem->title_, token);
    new_elem->title_[strlen(token)] = '\0';

    for(i = 0; i < (int)strlen(new_elem->title_); i++)
    {
	if(new_elem->title_[i] == '\r')
	    new_elem->title_[i] = '\0';
    }

    token = strtok(NULL, delimiter_data);
    new_elem->choice_a_title_ = (char*)malloc(sizeof(char) * (strlen(token) + 1));
    strcpy(new_elem->choice_a_title_, token);
    new_elem->choice_a_title_[strlen(token)] = '\0';

    for(i = 0; i < (int)strlen(new_elem->choice_a_title_); i++)
    {

	if(new_elem->choice_a_title_[i] == '\r')
	    new_elem->choice_a_title_[i] = '\0';
    }

    token = strtok(NULL, delimiter_data);
    new_elem->choice_b_title_ = (char*)malloc(sizeof(char) * (strlen(token) + 1));
    strcpy(new_elem->choice_b_title_, token);
    new_elem->choice_b_title_[strlen(token)] = '\0';

    for(i = 0; i < (int)strlen(new_elem->choice_b_title_); i++)
    {
	if(new_elem->choice_b_title_[i] == '\r')
	    new_elem->choice_b_title_[i] = '\0';
    }

    token = strtok(NULL, delimiter_text);
    new_elem->text_ = (char*)malloc(sizeof(char) * (strlen(token) + 1));
    strcpy(new_elem->text_, token);
    new_elem->text_[strlen(token)] = '\0';

    for(i = 0; i < (int)strlen(new_elem->text_); i++)
    {
	if(new_elem->text_[i] == '\r')
	    new_elem->text_[i] = '\0';
    }

    free(file_stream);

    return new_elem;
}

// ------------------------------------------------ -----------------------------
///
/// initFileStructure function
///
/// @(tree_elem **new_elem) assigns new elements to tree.
/// @(FILE * file) to call the function parseFile
/// Inititialises the data and creates a struct tree from the parsed files.
///
/// @return 0;
//


int initFileStructure(tree_elem **new_elem, FILE *file)
{
    *new_elem = parseFile(file);
    fclose(file);
    if(strcmp((*new_elem)->choice_a_title_, "-") == 0 &&
       strcmp((*new_elem)->choice_b_title_, "-") == 0)
        return 0;

    FILE *file_a, *file_b;

    file_a = fopen((*new_elem)->choice_a_title_, "r");
    file_b = fopen((*new_elem)->choice_b_title_, "r");



    if(file_a == NULL)
    {
        printf("[ERR] Could not read file %s.\n", (*new_elem)->choice_a_title_);
        return 3;
        fclose(file_a);
    }
    else if(file_b == NULL)
    {
        printf("[ERR] Could not read file %s.\n", (*new_elem)->choice_b_title_);
        return 3;
        fclose(file_b);
    }
    tree_elem *choice_a, *choice_b;
    int return_value = 0;

    return_value = initFileStructure(&choice_a, file_a);

    if(return_value != 0){    (*new_elem)->choice_a_ = choice_a;
        return return_value;}

(*new_elem)->choice_a_ = choice_a;

    return_value = initFileStructure(&choice_b, file_b);

    if(return_value != 0){    (*new_elem)->choice_b_ = choice_b;
        return return_value;}

(*new_elem)->choice_b_ = choice_b;

    return 0;
}

// ------------------------------------------------ -----------------------------
///
/// initProgram function
///
/// @(int argc) Program argument, argument count
/// @(char * argv[]) argument vector
/// The elements are stored in a double string
/// @(tree_elem ** tree_root) tree_elem = instance of tree_root
/// Initialises File structure, checks first element,
/// Error checks for program argumentns
/// @return initFileStructure(tree_root, file);


int initProgram(int argc, char *argv[], tree_elem **tree_root)
{
    if(argc > 2)
    {
        printf("Usage: ./ass2 %s\n", argv[1]);
        return 1;
    }
    else if(argc < 2)
    {
        printf("Usage: ./ass2 [file-name]\n");
        return 1;
    }

    FILE *file = fopen(argv[1], "r");

    if(file == NULL)
    {
        printf("[ERR] Could not read file %s.\n", argv[1]);
        return 3;
    }

    return initFileStructure(tree_root, file);
}

// ------------------------------------------------ -----------------------------
///
/// readInput function
///
/// Reads input and stores it into buffer.
///
///
///
/// @return 0;
//
int readInput()
{
    char buffer[1024];

    while(1)
    {
        printf("Deine Wahl (A/B)? ");
        fgets(buffer, 1024, stdin);

        if(strlen(buffer) > 2 || (buffer[0] != 'A' && buffer[0] != 'B'))
            printf("[ERR] Please enter A or B.\n");
        else
            break;
    }

    if(buffer[0] == 'A')
        return 0;
    else
        return 1;
}

// ------------------------------------------------ -----------------------------
///
/// playGame function
///
/// @(tree_elem * story_chapter) For the chapter that the player
/// is currently in prints out the chapter data,
/// like title and text
/// and based on the choice A or choice B prints out the required chapter.
/// @return void
///

void playGame(tree_elem *story_chapter)
{
    printf("------------------------------\n");
    printf("%s\n", story_chapter->title_);
    printf("\n");
    printf("%s\n", story_chapter->text_);
    printf("\n");

    if(strcmp(story_chapter->choice_a_title_, "-") == 0 &&
       strcmp(story_chapter->choice_b_title_, "-") == 0)
    {
        printf("ENDE\n");
        return;
    }
    else
    {
        int input_result = readInput();

        if(input_result == 0)
            story_chapter = story_chapter->choice_a_;
        else
            story_chapter = story_chapter->choice_b_;

        playGame(story_chapter);
    }
}

// ------------------------------------------------ -----------------------------
///
/// freeStructureMemory
///
/// @(tree_elem *story_chapter) tree element becomes a unique story chapter.
/// All memory is freed after the game is finished.
///
/// @return void!
///

void freeStructureMemory(tree_elem *story_chapter)
{
    if(story_chapter == NULL)
	return;

    tree_elem *choice_a = story_chapter->choice_a_;
    tree_elem *choice_b = story_chapter->choice_b_;

    free(story_chapter->title_);
    free(story_chapter->text_);
    free(story_chapter->choice_a_title_);
    free(story_chapter->choice_b_title_);
    free(story_chapter);


    freeStructureMemory(choice_a);
    freeStructureMemory(choice_b);
}

int main(int argc, char *argv[])
{
    tree_elem *tree_root = NULL;
    int return_value = 0;

    return_value = initProgram(argc, argv, &tree_root);

    if(return_value != 0)
    {
        freeStructureMemory(tree_root);
        return return_value;
    }

    playGame(tree_root);
    freeStructureMemory(tree_root);

    return return_value;
}

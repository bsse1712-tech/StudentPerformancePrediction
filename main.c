#include <stdio.h>
#include <string.h>


int main()
{
    char word[50];

    printf("=====================================\n");
    printf(" PDF DICTIONARY PROJECT\n");
    printf("=====================================\n\n");

    printf("Enter a word: ");
    scanf("%49s", word);

    if (strcmp(word, "computer") == 0)
    {
        printf("\nMeaning: An electronic device that processes data.\n");
    }
    else if (strcmp(word, "algorithm") == 0)
    {
        printf("\nMeaning: A step-by-step procedure for solving a problem.\n");
    }
    else if (strcmp(word, "program") == 0)
    {
        printf("\nMeaning: A set of instructions given to a computer.\n");
    }
    else
    {
        printf("\nMeaning not found.\n");
    }

    return 0;
}
	

#include "commands.h"

int main(int argc, char *argv[])
{
    // We should have atleast 3 arguments
    // argv[0] for the program itself
    // argv[1] for the info so we can just compare strings
    // argv[2] for the image

    if (argc != 3)
    {
        printf("ERROR: Missing arguments.\n");
        return 1;
    }

    if (!argv)
    {
        printf("ERROR: Missing arguments");
        return 1;
    }

    u_int16_t result;
    if (strcmp(argv[1], "--info") == 0)
    {
        result = infoCommand(argv);
    }
    else if (strcmp(argv[1], "--reveal") == 0)
    {
        result = revealCommand(argv);
    }
    else if (strcmp(argv[1], "--hide") == 0)
    {
        result = hideCommand(argv);
    }
    else 
    {
        printf("ERROR: Missing Arguments.\n");
        return 0;
    }

    if (result == 0)
    {
        printf("Command failed");
    }

    return 0;
}
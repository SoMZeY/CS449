#include "commands.h"

int main(int argc, char *argv[])
{
    // We should have at least 3 or 4 arguments
    // argv[0] for the program itself
    // argv[1] for the command (--info, --reveal, --hide)
    // argv[2] for FILENAME1 (image file)
    // argv[3] for FILENAME2 (hidden file, only for --hide)

    if (argc != 3 && argc != 4)
    {
        printf("ERROR: Missing arguments.\n");
        return 1;
    }

    // The following check is unnecessary because argv is always non-NULL
    // when argc > 0. Commenting it out to preserve comments without affecting functionality.
    /*
    if (!argv)
    {
        printf("ERROR: Missing arguments");
        return 1;
    }
    */

    uint16_t result;
    if (strcmp(argv[1], "--info") == 0)
    {
        // For --info, we expect exactly 3 arguments
        if (argc != 3)
        {
            printf("ERROR: Missing arguments.\n");
            return 1;
        }
        result = infoCommand(argv);
    }
    else if (strcmp(argv[1], "--reveal") == 0)
    {
        // For --reveal, we expect exactly 3 arguments
        if (argc != 3)
        {
            printf("ERROR: Missing arguments.\n");
            return 1;
        }
        result = revealCommand(argv);
    }
    else if (strcmp(argv[1], "--hide") == 0)
    {
        // For --hide, we expect exactly 4 arguments
        if (argc != 4)
        {
            printf("ERROR: Missing arguments.\n");
            return 1;
        }
        result = hideCommand(argv);
    }
    else 
    {
        // Changed error message to indicate an invalid command
        printf("ERROR: Invalid command.\n");
        return 1;
    }

    // Check the result of the command function
    if (result == 0)
    {
        // Command failed; return non-zero exit code without printing extra messages
        return 1;
    }

    // Command succeeded
    return 0;
}

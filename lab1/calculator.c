#include <stdio.h>

void calculator()
{
    int num1, num2;
    char oper;

    while (1)
    {
        // Get the input
        printf("Enter your calculation:\n");
        if (scanf("%d %c %d", &num1, &oper, &num2) != 3)
        {
            printf("Invalid input! Please enter a valid calculation.\n");
            while (getchar() != '\n'); // Clear the input buffer
            continue;
        }

        // Handle different operators
        if (oper == '+')
        {
            printf("> %d %c %d = %d\n", num1, oper, num2, num1 + num2);
        }
        else if (oper == '-')
        {
            printf("> %d %c %d = %d\n", num1, oper, num2, num1 - num2);
        }
        else if (oper == '*')
        {
            printf("> %d %c %d = %d\n", num1, oper, num2, num1 * num2);
        }
        else if (oper == '/')
        {
            if (num2 == 0)
            {
                printf("Error: Division by zero is not allowed.\n");
                continue;
            }
            printf("> %d %c %d = %d\n", num1, oper, num2, num1 / num2);
        }
        else if (oper == '%')
        {
            if (num2 == 0)
            {
                printf("Error: Modulus by zero is not allowed.\n");
                continue;
            }
            printf("> %d %c %d = %d\n", num1, oper, num2, num1 % num2);
        }
        else if (oper == '&')
        {
            printf("> %d %c %d = %d\n", num1, oper, num2, num1 & num2);
        }
        else
        {
            // If an invalid operator is used, print the error and quit
            printf("Invalid calculation! \"%d %c %d\"\n", num1, oper, num2);
            break;
        }
    }
}

int main()
{
    calculator();
    return 0;
}

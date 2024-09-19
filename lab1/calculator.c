#include <stdio.h>

void calculator()
{
    int num1, num2;
    char oper;

    while (1)
    {
        // Get the input
        printf("Enter your calculation:\n");
        scanf("%d %c %d", &num1, &oper, &num2);

        // Handle different operators
        if (oper == '+')
        {
            printf("%d %c %d = %d\n", num1, oper, num2, num1 + num2);
            break;
        }
        else if (oper == '-')
        {
            printf("%d %c %d = %d\n", num1, oper, num2, num1 - num2);
            break;
        }
        else if (oper == '*')
        {
            printf("%d %c %d = %d\n", num1, oper, num2, num1 * num2);
            break;
        }
        else if (oper == '/')
        {
            if (num2 == 0) // Check for division by zero
            {
                printf("Error: Division by zero is not allowed.\n");
                continue;
            }
            printf("%d %c %d = %d\n", num1, oper, num2, num1 / num2);
            break;
        }
        else if (oper == '%')
        {
            if (num2 == 0) // Check for modulus by zero
            {
                printf("Error: Modulus by zero is not allowed.\n");
                continue;
            }
            printf("%d %c %d = %d\n", num1, oper, num2, num1 % num2);
            break;
        }
        else if (oper == '&')
        {
            printf("%d %c %d = %d\n", num1, oper, num2, num1 & num2);
            break;
        }
        else
        {
            printf("Invalid calculation! \"%d %c %d\"\n", num1, oper, num2);
        }
    }
}

int main()
{
    calculator();
    return 0;
}

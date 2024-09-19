#include "calculator.h"

void calculator()
{
	int num1, num2;
	char oper;
	
	while(1)
	{
		// Get the input
		printf("Enter your calculation: \n");
		scanf("%d %c %d", &num1, &oper, &num2);
	
		// I'm too lazy to do this elegantly, so enjoy this crappy code
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

			printf("%d %c %d = %d\n", num1, oper, num2, num1 / num2);
			break;
		}
		else if (oper == '%')
		{

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

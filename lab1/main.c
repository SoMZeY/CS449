#include <stdio.h>
#include "ternary.h"
#include "calculator.h"

int main(void) 
{
	int cond, if_true, if_false, ternary_result;
	cond = 0;
	if_true = 15;
	if_false = 16;	

	// Write your tests 
	ternary_result = cond? if_true: if_false;
	if (ternary(cond, if_true, if_false) == ternary_result )
	{
		printf("Passed!\n");
	}
	else 
	{
		printf("Failed!\n");
	}

	calculator();

	printf("End!");	
	return 0;
}


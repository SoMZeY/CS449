#include "ternary.h"

int ternary(int cond, int if_true, int if_false)
{	
	// If TRUE
	if (cond != 0)
	{
		return if_true;
	}
	// If FALSE
        else if (cond == 0)
	{
		return if_false;
	}
	
	// This should never happen!
	return 0;
}


#include <stdio.h>
int print_test(void)
{
	printf("%s(): Executing library\n", __func__);
	return 0;
}

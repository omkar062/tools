#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

extern int print_test(void);
main()
{
printf("%s(): executing main with PID: %d\n", __func__, getpid());
getchar();
printf("%s(): calling lib routine\n", __func__);
print_test();
getchar();
printf("%s(): end of lib\n", __func__);

getchar();
while(1);
}

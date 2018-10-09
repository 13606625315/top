#include <string>
#include <stdio.h>
#include <string.h>
#include <vector>
#include <stack>

int main()
{
    typedef char type_must_be_complete[1];
    int a = (void) sizeof(type_must_be_complete);
	printf("  %d\n",a);
	return 1;
}



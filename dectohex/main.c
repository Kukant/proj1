#include <stdio.h> 

int main()
{
	int c = 0;
	
	while(c != 666)
	{
		scanf("%d", &c);
		printf("%08x", c);
	}
	
	return 0;
}

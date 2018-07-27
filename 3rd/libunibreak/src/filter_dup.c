#include <stdio.h>
#include <string.h>

int main()
{
	char s[80];
	char beg[16];
	char end[16];
	char prop[16];
	char lastbeg[16];
	char lastend[16];
	char lastprop[16];
	lastprop[0] = 0;
	for (;;)
	{
		if (fgets(s, sizeof s, stdin) == NULL)
			break;
		if (strstr(s, "LBP_") == NULL || strstr(s, "LBP_Undef") != NULL)
		{
			if (lastprop[0])
			{
				printf("\t{ %s %s %s },\n", lastbeg, lastend, lastprop);
				lastprop[0] = 0;
			}
			printf("%s", s);
			continue;
		}
		sscanf(s, "\t{ %s %s %s }", beg, end, prop);
		/*printf("==>\t{ \"%s\" \"%s\" \"%s\" },\n", beg, end, prop);*/
		if (lastprop[0] && strcmp(lastprop, prop) != 0)
		{
			printf("\t{ %s %s %s },\n", lastbeg, lastend, lastprop);
			lastprop[0] = 0;
		}
		if (lastprop[0] == 0)
		{
			strcpy(lastbeg, beg);
			strcpy(lastprop, prop);
		}
		strcpy(lastend, end);
	}
	if (lastprop[0])
	{
		printf("\t{ %s %s %s },\n", lastbeg, lastend, prop);
	}
	return 0;
}

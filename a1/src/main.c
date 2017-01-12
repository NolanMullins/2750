#include <stdio.h>

int main()
{
	FILE* f = fopen("data/f.dat", "r");
	char tmp = getc(f);
	int flag = 0;
	while (tmp != EOF)
	{
		if (tmp == '\t' || tmp == ' ' || tmp == '\n' || tmp == '\r')
		{
			if (flag!=1)
			{
				printf("\n");
				flag = 1;
			}
		}
		else if (tmp == ';' || tmp == '(' || tmp == ')' || tmp == '{' || tmp == '}')
		{
			if (flag == 0)
				printf("\n");
			printf("%c\n", tmp);
			flag = 1;
		}
		else if (tmp == '"')
		{
			char last = tmp;
			while (tmp != '"' && last != '\\')
			{
				last = tmp;
				printf("%c", tmp);
				tmp = getc(f);
			}
			printf("%c", tmp);
		}
		else
		{
			flag = 0;
			//printf("%d\n", tmp);
			printf("%c", tmp);
		}
		tmp = getc(f);
	}
	fclose(f);
}
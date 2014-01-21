struct AdvCmdArgs
{
	char *command, *arg1, *arg2, *arg3, *arg4, *arg5;
};

AdvCmdArgs ParsingStr(char * str)
	{
		/* Объявление переменных и очистка аргументов */
		AdvCmdArgs AdvCmdStr;
		int i = 0, a = 0, b = 0;
		AdvCmdStr.command = new char[1]; AdvCmdStr.command[0] = '\0';
		AdvCmdStr.arg1 = new char[1]; AdvCmdStr.arg1[0] = '\0';
		AdvCmdStr.arg2 = new char[1]; AdvCmdStr.arg2[0] = '\0';
		AdvCmdStr.arg3 = new char[1]; AdvCmdStr.arg3[0] = '\0';
		AdvCmdStr.arg4 = new char[1]; AdvCmdStr.arg4[0] = '\0';
		AdvCmdStr.arg5 = new char[1]; AdvCmdStr.arg5[0] = '\0';

		/* Чтение команды */

		while(str[i] != ' ') 
		{
			i++;
		}
		AdvCmdStr.command = new char[i+1];
		while(a<i)
		{
			AdvCmdStr.command[a] = str[a];
			a++;
		}
		AdvCmdStr.command[a] = '\0';

		/* Чтение 1-го аргумента */

		while(str[i] == ' ') 
		{ 
			i++; a++;
		}
		while(str[i] != ' ')
		{
			i++;
		}
		AdvCmdStr.arg1 = new char[(i-a)+1];
		while(a<i)
		{
			AdvCmdStr.arg1[b] = str[a];
			a++;
			b++;
		}
		AdvCmdStr.arg1[b] = '\0';
		b = 0;

		/* Чтение 2-го аргумента */

		while(str[i] == ' ') 
		{ 
			i++; a++;
		}
		while(str[i] != ' ')
		{
			i++;
		}
		AdvCmdStr.arg2 = new char[(i-a)+1];
		while(a<i)
		{
			AdvCmdStr.arg2[b] = str[a];
			a++;
			b++;
		}
		AdvCmdStr.arg2[b] = '\0';
		b = 0;

		/* Чтение 3-го аргумента */

		while(str[i] == ' ') 
		{ 
			i++; a++;
		}
		while(str[i] != ' ')
		{
			i++;
		}
		AdvCmdStr.arg3 = new char[(i-a)+1];
		while(a<i)
		{
			AdvCmdStr.arg3[b] = str[a];
			a++;
			b++;
		}
		AdvCmdStr.arg3[b] = '\0';
		b = 0;

		/* Чтение 4-го аргумента */

		while(str[i] == ' ') 
		{ 
			i++; a++;
		}
		while(str[i] != ' ')
		{
			i++;
		}
		AdvCmdStr.arg4 = new char[(i-a)+1];
		while(a<i)
		{
			AdvCmdStr.arg4[b] = str[a];
			a++;
			b++;
		}
		AdvCmdStr.arg4[b] = '\0';
		b = 0;

		/* Чтение 5-го аргумента */

		while(str[i] == ' ') 
		{ 
			i++; a++;
		}
		while(str[i] != ' ')
		{
			i++;
		}
		AdvCmdStr.arg5 = new char[(i-a)+1];
		while(a<i)
		{
			AdvCmdStr.arg5[b] = str[a];
			a++;
			b++;
		}
		AdvCmdStr.arg5[b] = '\0';
		b = 0;

		return AdvCmdStr;
	}

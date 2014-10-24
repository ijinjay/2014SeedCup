/****************************************************
 * Author: Jin Jay
 * Created On: 2014-10-23
 * Last-Modified: 2014-10-23
 */

#include "SeedCup.h"

int main(int argc, char const *argv[])
{
	// 命令行模式下使用：SeedCup.exe pattern str
	printf("To beautiful you~ %d\n", argc);

	// 变长参数与指针函数的使用
	// handleRange(3, "to ", "beautiful ", "you !\n");
	// int (*fpointer)(int, ...);
	// fpointer = handleRange;
	// (*fpointer)(2, "yes", "successed\n");

	if (argc == 3) 
	{
		char *result = (char *)malloc(1024 * sizeof(char));
		printf("search pattern %s in %s\n", argv[1], argv[2]);
		if(patternSearch(argv[1], argv[2], result))
		{
			printf("%s\n", result);
		}
		else
			printf("Failed! Cannot match! :(\n");
	}
	else 
	{
		FILE *patternFile = fopen("../RegexDemo/regex.txt", "r");
		for (int i = 1; i <= TESTNUM; ++i)
		{
			// 读取模式串文件的第i行
			char pattern[1024];
			fgets(pattern, 1024, patternFile);
			int len = strlen(pattern);
			pattern[len - 1] = '\0';

			// 读取待匹配串文件数据到str
			char *str = (char *)malloc(1024 * sizeof(char));
			if (!readFileData(i, str))
				printf("File not exists.\n");

			// 开始匹配
			char *result = (char *)malloc(1024 * sizeof(char));
			patternSearch(pattern, str, result);
			printf("%s", result);

			// 将结果写入文件
			writeFile(i, str);

			free(str);
			free(result);
		}
	}
	return 0;
}

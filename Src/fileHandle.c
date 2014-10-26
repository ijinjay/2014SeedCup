/****************************************************
 * Author: Jin Jay
 * Created On: 2014-10-23
 * Last-Modified: 2014-10-24
 */

#include "fileHandle.h"

int readFileData(int i, char *data)
{
	// 产生待读取的文件名
	char *filename = (char *)malloc(30 * sizeof(char));
	sprintf(filename, "../RegexDemo/text%d.txt", i);
	int fd = open(filename, O_RDONLY);
	// 释放内存
	free(filename);
	// 文件不能打开
	if (fd == -1)
		return 0;
	// 需求上已经写明不超过1024字节
	int len = read(fd, data, 1024);
	assert(len < 1024);
	// 结尾添加'\0'作为字符串使用
	data[len] = '\0';
	close(fd);
	return 1;
}

int writeFile(int i, const char *data)
{
	// 产生待写入的文件名
	char *filename = (char *)malloc(sizeof(char));
	sprintf(filename , "output%d.txt", i);
	FILE *output = fopen(filename, "w");
	// 释放内存
	free(filename);
	// 文件不能打开
	if (output == NULL)
		return 0;
	fprintf(output, "%s", data);
	fclose(output);
	return 1;
}

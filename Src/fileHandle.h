/****************************************************
 * Author: Jin Jay
 * Created On: 2014-10-23
 * Last-Modified: 2014-10-23
 */

#ifndef _FILEHANDLE_H
#define _FILEHANDLE_H 
#endif

#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <assert.h>

#ifndef MAXNUM
#define MAXNUM 1024
#endif

/****************************************************
 * 外部接口readFileData:
 * 		读取文件数据，并返回  
 * 输入参数：
 * 		int i:  				文件编号
 * 输出参数：
 * 		char *data:				文件数据
 * 返回值：
 * 		0：						读取文件数据成功
 *		1：						读取文件数据失败
 */
int readFileData(int i, char *data);

/****************************************************
 * 外部接口writeFile:
 * 		写入数据到文件
 * 输入参数：
 * 		int i:  				文件编号
 * 		const char *data:		待写入数据
 * 返回值：
 * 		0： 						成功
 * 		1： 						失败
 */
int writeFile(int i, const char *data);

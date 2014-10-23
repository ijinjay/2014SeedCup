/****************************************************
 * Author: Jin Jay
 * Created On: 2014-10-23
 * Last-Modified: 2014-10-23
 */

#ifndef _REGULAREXPRESSION_H
#define _REGULAREXPRESSION_H 
#endif

#include <string.h>
#include <ctype.h>
#include <stdio.h>

/****************************************************
 * 外部接口函数patternSearch，完成串的匹配工作
 * 输入参数：
 *		const char *pattern: 模式串
 *		const char *str:	 待匹配串
 * 输出参数：
 * 		char *result:		 匹配结果
 * 返回值：
 *      0：					 匹配失败
 *      1：					 匹配成功
 */
int patternSearch(const char *pattern, const char *str, char *result);

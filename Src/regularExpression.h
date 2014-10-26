/****************************************************
 * Author: Jin Jay
 * Created On: 2014-10-23
 * Last-Modified: 2014-10-25
 */

#ifndef _REGULAREXPRESSION_H
#define _REGULAREXPRESSION_H 
#endif

#include <string.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

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
extern int patternSearch(const char *pattern, const char *str, char *result);

// 重复次数的枚举
enum eMulTimes {
	one = 0,	// 不重复，默认
	zero2one, 	// 重复0-1次
	one2n,		// 重复1-n次
	zero2n,		// 重复0-n次
	n,			// 重复n次
	n2more,		// 重复n-更多次
	n2m ,		// 重复n-m次
	leftP = -1,
	rightP = -2,
	quote = -3,
};
// 词性的枚举
enum eWord {
	dot       = 256, 	// 点
	word      = 257, 	// 字母或数字
	space     = 258, 	// 空格
	digit     = 259, 	// 数字
	begin     = 260, 	// 一行开头
	stOrEnd   = 261, 	// 单词开始或结尾
	dollar    = 262, 	// 结尾
	range     = 263, 	// 范围选择
	nonWord   = 264, 	// 非字母或数字
	nonSpace  = 265, 	// 非空白
	nonDigit  = 266, 	// 非数字
	nonStOrEnd= 267, 	// 非开始或结束的位置
	nonRange  = 268, 	// 范围内不选择
	//leftP     = 269,	// 左小括号
	//rightP    = 270,  // 右小括号
	//quote     = 271		// 引用类型 quote + x 即为引用的编号。
};
//括号状态枚举
enum eParentheses
{
	none  = 0,
	left  = 1,
	right = 2,
};
/****************************************************
 * 记录每个字符节点的结构体
 */
typedef struct WordNode {
	char content[20];					// 节点储存的内容，供比较实用
	int  contentLen;					// 节点存储的字符个数
	int  type;							// 节点的类型
	int  (*pCompareFunc)(char, ...); 	// 节点比较时应使用的函数
	int  quoteIndex;					// 后项引用的编号
}WordNode;

/****************************************************
 * 内部数据结构State
 * 		记录当前的状态信息
 */
typedef struct State {
	WordNode 		word;	// 当前待匹配单词
	enum eMulTimes 	type;	// 当前状态重复信息
	struct State * 	next;	// 下一个状态节点
} State, *pStateNode;
/******************************************************
分组结构
*/
typedef struct Group{
	char str[20];
	int len;
	int id;
}Group;
/***************************************************
分组字符栈
*/
/****************************************************
 * 匹配头结点结构
 * 结构体参数：
 * 		State *head;	头结点数组不存储节点信息，只作为指针
 * 		int len;			头结点包含的基本元素个数，匹配时的最小可匹配串长度
 */
typedef struct Head {
	State *head;		// 头结点
	int    len;			// 字符串长度
} Head, *pHeadNode;

/****************************************************
 * 分支匹配头结点结构，在模式串中含有'|'时使用
 * 结构体参数：
 * 		Head h[10];			分支数组
 * 		int len;			分支个数
 */
typedef struct Branch{
	Head h[10];			// 分支数组
	int  num;			// 分支个数
} Branch;

typedef struct Patterns
{
	char pattern[100];
	int  p;
}Patterns;

typedef struct StackNode
{
	int id;
	char c;
}StackNode;
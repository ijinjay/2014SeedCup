/****************************************************
 * Author: Jin Jay
 * Created On: 2014-10-23
 * Last-Modified: 2014-10-23
 */

#include "regularExpression.h"

// 重复次数的枚举
enum eMulTimes {
	one,		// 不重复
	zero2one, 	// 重复0-1次
	one2n,		// 重复1-n次
	zero2n,		// 重复0-n次
	n,			// 重复n次
	n2more,		// 重复n-更多次
	n2m 		// 重复n-m次
};
// 词性的枚举
enum eWord {
	dot      = 256,
	word     = 257,
	space    = 258,
	digit    = 259,
	begin    = 260,
	start    = 261,
	dollar   = 262,
	range    = 263,
	alpha    = 264,
	a_word   = 265,
	a_space  = 266,
	a_digit  = 267,
	a_begin  = 268,
	a_range  = 269,
	repeat   = 270,
	a_alpha  = 271,
};
/****************************************************
 * 记录每个节点的结构体
 */
typedef struct WordNode {
	char content[20];
	int contentLen;
	int wordType;
	int (*pCompareFunc)(int, ...);
}WordNode, *pWordNode;

/****************************************************
 * 内部数据结构State
 * 		记录当前的状态信息
 */
typedef struct State {
	WordNode word;
	enum eMulTimes type;
	struct State *next;	// 下一个状态节点
} State, *pStateNode;

/****************************************************
 * 分支匹配头结点结构
 * 结构体参数：
 * 		State *head;     	头结点不存储节点信息，只作为指针
 * 		int len;			头结点包含的基本元素个数，匹配时的最小可匹配串长度
 */
typedef struct Head {
	State *head;		// 头结点
	int len;			// 字符串长度
} Head, *pHeadNode;

/****************************************************
 * 内部函数handleBrace
 * 		处理大括号，重复次数
 * 输入参数：
 *		const char *pattern;	模式串
 * 输入和输出参数：
 *		int i;					当前{所在位置
 *		pStateNode node;		当前单词节点
 */
void handleBrace(const char *pattern, int i, pStateNode node) {
	if (pattern[i+2] == '}')  		// {n}	
	{
		node->type = eMulTimes.n;
		node->word.content[node->word.contentLen] = pattern[i+1];
		i = i + 2;
	}
	else if (pattern[i+3] == '}') 	// {n,}
	{
		node->type = eMulTimes.n2more;
		node->word.content[node->word.contentLen] = pattern[i+1];
		i = i + 3;
	}
	else if (pattern[i+4] == '}')	// {n,m}
	{
		node->type = eMulTimes.n2more;
		node->word.content[node->word.contentLen] = pattern[i+1];
		node->word.content[node->word.contentLen + 1] = pattern[i+3];
		i = i + 4;
	}
	else
		printf("pattern is not in right format.\n");
}

/****************************************************
 * 处理一般比较选择语句
 * 输入参数：
 * 		int i;			变长参数个数
 * 		const char ch;  模式字符
 * 		const char sch;	待匹配的字符
 * 返回值：
 *		0				失败
 *		1 				成功
 */
int normalCompare(int i,...) {
	va_list v;
	va_start(v, i);
	char ch = va_arg(v, char);
	char sch = va_arg(v, char);
	va_end(v);
	return (ch == sch);
}
/****************************************************
 * 处理[]选择语句
 * 输入参数：
 * 		int i;			变长参数个数
 * 		char *content;	供选择的字符串
 *		int len;		供选择的字符串长度
 * 		const char ch;	待匹配的字符
 * 返回值：
 *		0				失败
 *		1 				成功
 */
int selectStc(int i,...) {
	va_list v;
	va_start(v, i);
	char *content = va_arg(v, char *);
	int len = va_arg(v, int);
	char ch = va_arg(v, char);
	va_end(v);
	
	for (int j = 0; j < len; ++j) {
		if (ch == content[j])
			return 1;
	}
	return 0;
}
/****************************************************
 * 内部函数handleBracket
 * 		处理中括号，选择匹配
 * 输入参数：
 *		const char *pattern;	模式串
 * 输入和输出参数：
 *		int i;					当前[所在位置
 *		pStateNode node;		当前单词节点
 */
void handleBracket(const char *pattern, int i, pStateNode node) {
	// 新节点
	pStateNode newNode = (pStateNode)malloc(sizeof(State));
	// 将供选择的元素加入节点的content中
	while(pattern[++i] != ']') {
		newNode->word.content[newNode->word.contentLen ++] = pattern[i];
	}
	newNode->word.pCompareFunc = selectStc;
	newNode->next = NULL;
	node->newNode = newNode;
	node = newNode;
}
/****************************************************
 * 内部函数pattern2NFA
 * 		将模式串转为NFA
 * 输入参数：
 *		const char *pattern
 * 返回值：
 *		Head h;
 * 注意： 返回的Head 需要在不用的时候释放内存使用freeHead()函数
 */
static Head pattern2NFA(const char *pattern) {
	Head h;
	h.head = (State *)malloc(sizeof(State));
	h.len = 0;
	int length = strlen(pattern);
	pStateNode currentNode = h.head;
	for (int i = 0; i < length; ++i) {
		switch(pattern[i]) {
			case '?': currentNode->type = eMulTimes.zero2one ; continue;
			case '+': currentNode->type = eMulTimes.one2n; continue;
			case '*': currentNode->type = eMulTimes.zero2n; continue;
			case '{': handleBrace(pattern, i, currentNode);	continue;
			case '[': handleBrace(pattern, i, currentNode); h.len ++; continue;
			default:  break;
		}
		pStateNode newNode = (pStateNode)malloc(sizeof(State));
		newNode->word.content[0] = pattern[i];
		newNode->word.contentLen = 1;
		newNode->word.pCompareFunc = normalCompare;
		newNode->type = 0;
		newNode->next = NULL;
		currentNode->next = newNode;
		currentNode = newNode;
		h.len ++;
	}
	return h;
}

/****************************************************
 * 内部函数freeHead
 *		释放Head占用的内存
 * 输入参数：
 *		Head h
 */
static void freeHead(Head h) {
	State *currentNode = h.head;
	while(currentNode != NULL) {
		h.head = currentNode->next;
		free(currentNode);
		currentNode = h.head;
	}
}

// use for test
// static void printHead(const Head h) {
// 	pStateNode p = h.head->next;
// 	while(p != NULL) {
// 		printf("%c \t %d\n", p->ch, p->type);
// 		p = p->next;
// 	}
// }

/******
 * 处理变长参数列表
 */
int handleRange(int len, ...) {
	va_list vLists;
	va_start(vLists, len);
	for (int i = 0; i < len; ++i)
	{
		char *s = va_arg(vLists, char *);
		printf("%s", s);
	}
	return 0;

}

int patternSearch(const char *pattern, const char *str, char *result) {
	Head h = pattern2NFA(pattern);
	pStateNode root = h.head;

	for (int i = 0; i <= strlen(str) - h.len; ++i)
	{
		pStateNode currentNode = root->next;
		int j = i;
		int k = 0; 	// 记录result
		int failFlag = 0;
		while(currentNode != NULL)
		{
			switch(currentNode->type)
			{
				// 当前节点重复0-1次
				case eMulTimes.zero2one: 
						if (currentNode->ch == str[j]) {
							result[k++] = str[j++];
						}
						break;
				// 重复1-n次
				case eMulTimes.one2n: 
						if (!(*(currentNode->word.fCompareFunc))()) {
							failFlag = 1;
							break;
						}
						result[k ++] = str[j ++];
				// 重复0-n次
				case 3: while(currentNode->ch == str[j]) {
					result[k++] = result[j++];
				}
				break;
				default:if (currentNode->ch == str[j])
							result[k++] = result[j++];
						else
							failFlag = 1;
						break;
			}
			if (failFlag)
				break;
			currentNode = currentNode->next;
		}
		if (!failFlag)
		{
			result[k] = '\0';
			printf("Success %d, %d\n", i, k);
			for (int l = 0; l < k; ++l)
			{
				result[l] = str[i+l];
			}
			freeHead(h);
			return 1;
		}
	}
	freeHead(h);
	return 0;
}

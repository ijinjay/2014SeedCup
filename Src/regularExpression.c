/****************************************************
 * Author: Jin Jay
 * Created On: 2014-10-23
 * Last-Modified: 2014-10-23
 */

#include "regularExpression.h"

/* 内部数据结构State
 * 		记录当前的状态信息
 */
typedef struct State {
	int ch; 			// 记录当前的值
	int type;			/* 记录当前的重复类型
						 * 0: 不重复
						 * 1: 重复0-1次
						 * 2: 重复1-n次
						 * 3: 重复0-n次
						 */
	struct State *next;	// 下一个状态节点
} State, *pStateNode;

typedef struct Head {
	State *head;		// 头结点
	int len;			// 字符串长度
} Head, *pHeadNode;

static Head pattern2NFA(const char *pattern)
{
	Head h;
	h.head = (State *)malloc(sizeof(State));
	h.len = 0;
	int length = strlen(pattern);
	pStateNode currentNode = h.head;
	for (int i = 0; i < length; ++i)
	{
		switch(pattern[i])
		{
			case '?': currentNode->type = 1; continue;
			case '+': currentNode->type = 2; continue;
			case '*': currentNode->type = 3; continue;
			default: currentNode->type = 0; break;
		}
		pStateNode newNode = (pStateNode)malloc(sizeof(State));
		newNode->ch = pattern[i];
		newNode->type = 0;
		newNode->next = NULL;
		currentNode->next = newNode;
		currentNode = newNode;
		h.len ++;
	}
	return h;
}

static void freeHead(Head h)
{
	State *currentNode = h.head;
	while(currentNode != NULL) {
		h.head = currentNode->next;
		free(currentNode);
		currentNode = h.head;
	}
}
int patternSearch(const char *pattern, const char *str, char *result)
{
	Head h = pattern2NFA(pattern);
	pStateNode root = h.head;

	for (int i = 0; i <= strlen(str) - h.len; ++i)
	{
		pStateNode currentNode = root;
		int j = i;
		int k = 0; 	// 记录result
		int failFlag = 0;
		while(currentNode != NULL)
		{
			switch(currentNode->type)
			{
				// 当前节点重复0-1次
				case 1: if (currentNode->ch == str[j]) {
							result[k++] = str[j++];
						}
						break;
				// 重复1-n次
				case 2: if (currentNode->ch != str[j]) {
							break;
						}
						else
							failFlag = 1;
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
			freeHead(h);
			return 1;
		}
	}
	freeHead(h);
	return 0;
}

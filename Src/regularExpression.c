/****************************************************
 * Author: Jin Jay
 * Created On: 2014-10-23
 * Last-Modified: 2014-10-23
 */

#include "regularExpression.h"

static void handleBrace(const char *pattern, int *i, const pStateNode currentNode);
static void handleBracket(const char *pattern, int *i, pStateNode *pNode);
static void handleSlash(const char *pattern, int *i, pStateNode *pNode);
static void handleDot(const char *pattern, int *i, pStateNode *pNode);
static int handleSingle(pStateNode node, char ch);
static int normalCompare(int i, ...);
static int dotCompare(int i, ...);
static int numCompare(int i, ...);
static int wordCompare(int i, ...);
static int alphaCompare(int i, ...);
static int selectCompare(int i, ...);
static int spaceCompare(int i, ...);


/****************************************************
 * 处理比较的语句
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
	char ch = (char)va_arg(v, int);
	char sch = (char)va_arg(v, int);
	va_end(v);
	return (ch == sch);
}
int dotCompare(int i,...) {
	va_list v;
	va_start(v, i);
	char ch = (char)va_arg(v, int);
	va_end(v);
	return (ch != '\n');
}
int wordCompare(int i,...) {
	va_list v;
	va_start(v, i);
	char ch = (char)va_arg(v, int);
	va_end(v);
	return isalnum(ch);
}
int alphaCompare(int i,...) {
	va_list v;
	va_start(v, i);
	char ch = (char)va_arg(v, int);
	va_end(v);
	return isalpha(ch);
}
int numCompare(int i, ...) {
	va_list v;
	va_start(v, i);
	char ch = (char)va_arg(v, int);
	va_end(v);
	return isdigit(ch);
}
int spaceCompare(int i, ...) {
	va_list v;
	va_start(v, i);
	char ch = (char)va_arg(v, int);
	va_end(v);
	return isspace(ch);
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
 * 用法selectCompare(i, content, len, ch)
 */
int selectCompare(int i,...) {
	va_list v;
	va_start(v, i);
	char *content = va_arg(v, char *);
	int len = va_arg(v, int);
	char ch = (char)va_arg(v, int);
	va_end(v);
	
	for (int j = 0; j < len; ++j) {
		if (ch == content[j])
			return 1;
	}
	return 0;
}

/****************************************************
 * 内部函数handleBrace
 * 		处理大括号，重复次数
 * 输入参数：
 *		const char *pattern;	模式串
 * 输入和输出参数：
 *		int i;					当前{所在位置
 *		pStateNode node;		当前单词节点
 */
void handleBrace(const char *pattern, int *i, pStateNode node) {
	if (pattern[(*i)+2] == '}')  		// {n}	
	{
		node->type = n;
		node->word.content[node->word.contentLen] = pattern[(*i)+1];
		(*i) = (*i) + 2;
	}
	else if (pattern[(*i)+3] == '}') 	// {n,}
	{
		node->type = n2more;
		node->word.content[node->word.contentLen] = pattern[(*i)+1];
		(*i) = (*i) + 3;
	}
	else if (pattern[(*i)+4] == '}')	// {n,m}
	{
		node->type = n2m;
		node->word.content[node->word.contentLen] = pattern[(*i)+1];
		node->word.content[node->word.contentLen + 1] = pattern[(*i)+3];
		(*i) = (*i) + 4;
	}
	else
		printf("pattern is not in right format.\n");
}

/****************************************************
 * 内部函数handleBracket
 * 		处理中括号，范围匹配
 * 输入参数：
 *		const char *pattern;	模式串
 * 输入和输出参数：
 *		int i;					当前[所在位置
 *		pStateNode node;		当前单词节点
 */
void handleBracket(const char *pattern, int *i, pStateNode *pNode) {
	// 新节点
	pStateNode newNode = (pStateNode)malloc(sizeof(State));
	newNode->type = 0;
	newNode->next = NULL;
	newNode->word.type = range;
	// 是否含有^
	if (pattern[(*i)+1] == '^') {
		(*i) ++;
		newNode->word.type = nonRange;
	}
	// 将供选择的元素加入节点的content中
	while(pattern[++(*i)] != ']') {
		newNode->word.content[newNode->word.contentLen ++] = pattern[(*i)];
	}
	newNode->word.pCompareFunc = selectCompare;
	(*pNode)->next = newNode;
	(*pNode) = newNode;
}

void handleSlash(const char *pattern, int *i, pStateNode *pNode) {
	// 新节点
	pStateNode newNode = (pStateNode)malloc(sizeof(State));
	newNode->type = 0;
	switch(pattern[++(*i)]) {
		case 's': newNode->word.type = space;newNode->word.pCompareFunc = spaceCompare;break;
		case 'S': newNode->word.type = nonSpace;newNode->word.pCompareFunc = spaceCompare;break;
		case 'w': newNode->word.type = word;newNode->word.pCompareFunc = wordCompare;break;
		case 'W': newNode->word.type = nonWord;newNode->word.pCompareFunc = wordCompare;break;
		case 'd': newNode->word.type = digit;newNode->word.pCompareFunc = numCompare;break;
		case 'D': newNode->word.type = nonDigit;newNode->word.pCompareFunc = numCompare;break;
		default:  newNode->word.type = pattern[(*i)];
				  newNode->word.pCompareFunc = normalCompare;
				  newNode->word.content[0] = pattern[(*i)];
				  newNode->word.contentLen = 1;
 				  break;
	}
	newNode->next = NULL;
	(*pNode)->next = newNode;
	(*pNode) = newNode;
}

void handleDot(const char *pattern, int *i, pStateNode *pNode) {
	pStateNode newNode = (pStateNode)malloc(sizeof(State));
	newNode->type = 0;
	newNode->next = NULL;
	newNode->word.type = dot;
	newNode->word.pCompareFunc = dotCompare;
	(*pNode)->next = newNode;
	(*pNode) = newNode;
	(*i) ++;
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
			case '?': currentNode->type = zero2one; h.len --; continue;
			case '+': currentNode->type = one2n;continue;
			case '*': currentNode->type = zero2n; h.len --; continue;
			case '{': handleBrace(pattern, &i, currentNode);	continue;
			case '[': handleBracket(pattern, &i, &currentNode); h.len ++; continue;
			case '\\':handleSlash(pattern, &i, &currentNode); h.len ++; continue;
			case '.': handleDot(pattern, &i, &currentNode); h.len ++; continue;
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

int handleSingle(pStateNode node, char ch) {
	printf("%d\n", node->word.type);
	switch(node->word.type) {
		case dot: 
		case space:
		case digit:
		case begin:
		case dollar:
			return ((*(node->word.pCompareFunc))(1, ch));
		case nonSpace:
		case nonDigit:
		case nonBegin:
			return !((*(node->word.pCompareFunc))(1, ch));
		case range:
			return ((*(node->word.pCompareFunc))(1, node->word.content, node->word.contentLen, ch));
		case nonRange:
			return !((*(node->word.pCompareFunc))(1, node->word.content, node->word.contentLen, ch));
		default:
			return !((*(node->word.pCompareFunc))(node->word.content[0], ch));
	}
}

// use for test
static void printHead(const Head h) {
	pStateNode p = h.head->next;
	while(p != NULL) {
		p = p->next;
	}
}
int patternSearch(const char *pattern, const char *str, char *result) {
	Head h = pattern2NFA(pattern);
	pStateNode root = h.head;
	// test
	// printHead(h);

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
				case zero2one: 
						if (handleSingle(currentNode, str[j])) {
							result[k++] = str[j++];
						}
						break;
				// 重复1-n次
				case one2n: 
						if (!handleSingle(currentNode, str[j])) {
							failFlag = 1;
							break;
						}
						result[k ++] = str[j ++];
				// 重复0-n次
				case zero2n: 
						while(handleSingle(currentNode, str[j])) {
							result[k++] = result[j++];
						}
						break;
				// 重复n次
				case n:
						
						break;
				default:if (handleSingle(currentNode, str[j]))
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

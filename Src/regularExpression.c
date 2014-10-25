/****************************************************
 * Author: Jin Jay
 * Created On: 2014-10-23
 * Last-Modified: 2014-10-23
 */

#include "regularExpression.h"
/* 内部函数列表 */
// 处理模式串的函数
static void handleBrace(const char *pattern, int *i, const pStateNode currentNode); 
static void handleBracket(const char *pattern, int *i, pStateNode *pNode);
static void handleSlash(const char *pattern, int *i, pStateNode *pNode);
static void handleDotStartEnd(const char ch, pStateNode *pNode);
// 处理匹配时的比较函数
static int normalCompare(char ch, ...);
static int dotCompare(char ch, ...);
static int numCompare(char ch, ...);
static int wordCompare(char ch, ...);
static int selectCompare(char ch, ...);
static int spaceCompare(char ch, ...);
static int wordStartCompare(char ch, ...);
static int lineStartCompare(char ch, ...);
static int endCompare(char ch, ...);
static int startEndCompare(char ch, ...);
// 单个字符匹配的入口函数
static int singleCompare(pStateNode node, const char *str, int *pos, int *resultPos);
// 将模式串转为NFA
static Head pattern2NFA(const char *pattern);
// 释放节点链表分支占用的内存
static void freeHeads(Branch branches);


/****************************************************
 * 处理比较的语句
 * 输入参数：
 * 		char ch;		待比较的字符
 *   	char *str;		待匹配的字符串地址
 *		int  pos;		待匹配的字符位置
 * 返回值：
 *		0				失败
 *		1 				成功
 */
int normalCompare(char ch,...) {
	va_list v;
	va_start(v, ch);
	char *p = va_arg(v, char *);
	va_end(v);
	return (ch == p[0]);
}
int dotCompare(char ch,...) {
	return (ch != '\n');
}
int wordCompare(char ch,...) {
	return isalnum(ch);
}
int numCompare(char ch, ...) {
	return isdigit(ch);
}
int spaceCompare(char ch, ...) {
	return isspace(ch);
}
// 匹配单词开始位置，当前位置为第一个字符时或前一个字符是空字符且当前字符不为空字符
int wordStartCompare(char ch, ...) {
	// 解析可变参数列表
	va_list v;
	va_start(v, ch);
	char *str = va_arg(v, char *);
	int pos = va_arg(v, int);
	va_end(v);

	if (isspace(str[pos])) 		
		return 0;
	if (pos == 0 || isspace(str[pos-1]))
		return 1;
	return 0;
}
// 匹配行开始，当前位置为第一个字符或前一个字符为'\n'
int lineStartCompare(char ch, ...) {
	va_list v;
	va_start(v, ch);
	char *str = va_arg(v, char *);
	int pos = va_arg(v, int);
	va_end(v);
	int first_line = pos==0 && str[pos];
	int other_line = str[pos] == '\n';
	if (first_line|| other_line)
		return 1;
	return 0;
}
// 匹配一行的结束，当前位置为最后一个字符或为'\n'
int endCompare(char ch, ...) {
	va_list v;
	va_start(v, ch);
	char *str = va_arg(v, char *);
	int pos = va_arg(v, int);
	va_end(v);

	printf("$ compare %c\n", ch);
	int end_str = str[pos] == '\0' && str[pos-1]!='\0';
	int end_line = str[pos] == '\n' && str[pos-1]!='\n';
	if (end_str||end_line) 		
		return 1;
	return 0;
}
// 匹配单词的开始或结尾，当前位置不为空字符，前一个或后一个字符为空字符
int startEndCompare(char ch, ...) {
	va_list v;
	va_start(v, ch);
	char *str = va_arg(v, char *);
	int pos = va_arg(v, int);
	va_end(v);
	int start = (pos!=0 && isspace(str[pos-1])&&isalnum(str[pos]));
	int end   = (pos<strlen(str) && isalnum(str[pos-1])&&isspace(str[pos]));
	int line_start = (pos==0 && isalnum(str[pos]));
	int line_end   = pos==strlen(str)&&isalnum(str[pos]);
	if (line_start || line_end || start || end)
		return 1;
	return 0;
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
 * 用法selectCompare(ch, content, len)
 */
int selectCompare(char ch,...) {
	va_list v;
	va_start(v, ch);
	char *content = va_arg(v, char *);
	int len = va_arg(v, int);
	va_end(v);
	
	// 待选择的字符数组
	char selectAr[20];
	// 当前字符位置
	int pos = 0;
	for (int j = 0; j < len; ++j) {
		// 处理a-z这种范围
		if (content[j] == '-') {
			if (content[j-1] < ch && ch < content[j+1])
				return 1;
			j ++;
		}
		selectAr[pos++] = content[j];
	}
	// 处理供选择的字符数组中的内容
	for (int i = 0; i < pos; ++i) {
		if (selectAr[i] == ch)
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
 *		int *i;					当前{所在位置
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
 *		int *i;					当前[所在位置
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

/****************************************************
 * 内部函数handleSlash
 * 		处理转义字符
 * 输入参数：
 *		const char *pattern;	模式串
 * 输入和输出参数：
 *		int *i;					当前[所在位置
 *		pStateNode node;		当前单词节点
 */
void handleSlash(const char *pattern, int *i, pStateNode *pNode) {
	// 新节点
	pStateNode newNode = (pStateNode)malloc(sizeof(State));
	newNode->type = 0;
	newNode->word.content[0] = pattern[++(*i)];
	// 根据转义字符类型，添加节点的类型和相应的比较处理函数
	switch(pattern[(*i)]) {
		case 's': newNode->word.type = space;  	 	newNode->word.pCompareFunc = spaceCompare;break;
		case 'S': newNode->word.type = nonSpace; 	newNode->word.pCompareFunc = spaceCompare;break;
		case 'w': newNode->word.type = word; 	 	newNode->word.pCompareFunc = wordCompare;break;
		case 'W': newNode->word.type = nonWord;  	newNode->word.pCompareFunc = wordCompare;break;
		case 'd': newNode->word.type = digit;	 	newNode->word.pCompareFunc = numCompare;break;
		case 'D': newNode->word.type = nonDigit; 	newNode->word.pCompareFunc = numCompare;break;
		case 'b': newNode->word.type = stOrEnd;  	newNode->word.pCompareFunc = startEndCompare;break;
		case 'B': newNode->word.type = nonStOrEnd;  newNode->word.pCompareFunc = startEndCompare;break;
		default:  newNode->word.type = pattern[(*i)];
				  newNode->word.pCompareFunc = normalCompare;
				  newNode->word.contentLen = 1;
 				  break;
	}
	// 将生成的新节点加入链表
	newNode->next = NULL;
	(*pNode)->next = newNode;
	(*pNode) = newNode;
}

void handleDotStartEnd(const char ch, pStateNode *pNode) {
	pStateNode newNode = (pStateNode)malloc(sizeof(State));
	newNode->type = 0;
	newNode->next = NULL;
	// 根据不同的单词类型添加不同的比较函数
	switch (ch) {
		case '.': newNode->word.type = dot; 	newNode->word.pCompareFunc = dotCompare; break;
		case '^': newNode->word.type = begin; 	newNode->word.pCompareFunc = lineStartCompare; break;
		case '$': newNode->word.type = dollar; 	newNode->word.pCompareFunc = endCompare; break;
		default:  printf("Error !\n"); break;
	}
	// 将生成的新节点加入链表
	(*pNode)->next = newNode;
	(*pNode) = newNode;
}

/****************************************************
 * 内部函数pattern2NFA
 * 		将模式串转为NFA
 * 输入参数：
 *		const char *pattern
 * 返回值：
 *		Head h;
 * 注意： 返回的Head 需要在不用的时候释放内存使用freeHeads()函数
 */
Head pattern2NFA(const char *pattern) {
	Head h;
	// 初始化
	h.head = (State *)malloc(sizeof(State));
	h.len = 0;
	int length = strlen(pattern);
	pStateNode currentNode = h.head;
	// 遍历模式串，生成NFA
	for (int i = 0; i < length; ++i) {
		switch(pattern[i]) {
			case '?': currentNode->type = zero2one; h.len --; continue;
			case '+': currentNode->type = one2n;continue;
			case '*': currentNode->type = zero2n; h.len --; continue;
			case '{': handleBrace(pattern, &i, currentNode);	continue;
			case '[': handleBracket(pattern, &i, &currentNode); h.len ++; continue;
			case '\\':handleSlash(pattern, &i, &currentNode); continue;
			case '.': h.len ++;
			case '^': 
			case '$': handleDotStartEnd(pattern[i], &currentNode);continue;
			case '(': continue;
			case ')': continue;
			default:  break;
		}
		// 为普通的字符时，直接新建一个节点，保存为普通类型，加入节点链表
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
 *		Branch branches;
 */
void freeHeads(Branch branches) {
	for (int i = 0; i < branches.num; ++i) {
		State *currentNode = branches.h[i].head;
		while(currentNode != NULL) {
			branches.h[i].head = currentNode->next;
			free(currentNode);
			currentNode = branches.h[i].head;
		}
	}
}

/****************************************************
 * 内部函数singleCompare
 *		比较一个待匹配字符
 * 输入参数：
 *		pStateNode node;		当前NFA的节点
 *		const char *str;		待匹配的串
 * 		int *pos;				当前匹配的位置，会被更改
 * 		int *k;					结果串的位置，会被更改
 * 返回值：
 *		0						匹配失败
 *		1 						匹配成功
 */
int singleCompare(pStateNode node, const char *str, int *pos, int *k) {
	int j = *pos;
	switch(node->word.type) {
		case dot: 
		case space:
		case digit:
			return ((*(node->word.pCompareFunc))(str[j]));
		case nonSpace:
		case nonDigit:
			return !((*(node->word.pCompareFunc))(str[j]));
		case begin:
		case dollar:
		case stOrEnd:
			(*k) --;
			(*pos) --;
			return ((*(node->word.pCompareFunc))(str[j], str, j));
		case nonStOrEnd:
			(*pos) --;
			return !((*(node->word.pCompareFunc))(str[j], str, j));
		case range:
			return ((*(node->word.pCompareFunc))(str[j], node->word.content, node->word.contentLen));
		case nonRange:
			return !((*(node->word.pCompareFunc))(str[j], node->word.content, node->word.contentLen));
		default:
			return ((*(node->word.pCompareFunc))(node->word.content[0], str+j));
	}
}

// use for test
static void printHead(const Head h) {
	pStateNode p = h.head->next;
	printf("length of head is %d\n", h.len);	
	while(p != NULL) {
		printf("%c === %d\n", p->word.content[0], p->type);
		p = p->next;
	}
}
int patternSearch(const char *pattern, const char *str, char *result) {
	// 处理分支
	Patterns ps[10];
	int id = 1;
	int pos = 0;
	for (int i = 0; i < strlen(pattern); ++i) {
		if (pattern[i] == '|') {
			ps[id-1].pattern[pos] = '\0';
			id ++;
			pos = 0;
			continue;
		}
		ps[id-1].pattern[pos ++] = pattern[i];
	}
	ps[id-1].pattern[pos] = '\0';

	Branch branches;
	branches.num = id;
	int minLen = branches.h[0].len;
	for(int i = 0; i <id ;i++) {
		printf("%s\n", ps[i].pattern);
		branches.h[i] = pattern2NFA(ps[i].pattern);
		minLen = minLen > branches.h[i].len ? branches.h[i].len : minLen;
		printHead(branches.h[i]);
	}

	// 遍历待匹配字符串，逐一匹配
	for (int i = 0; i <= strlen(str) - minLen; ++i)
	{
		for (int branchID = 0; branchID < id; ++branchID)
		{
			pStateNode currentNode = branches.h[branchID].head->next;
			int j = i;
			int k = 0; 	// 记录result
			int failFlag = 0;
			while(currentNode != NULL) {
				switch(currentNode->type) {
					// 当前节点重复0-1次
					case zero2one: 
							if (singleCompare(currentNode, str, &j, &k)) {
								result[k++] = str[j++];
							}
							break;
					// 重复1-n次
					case one2n: 
							if (!singleCompare(currentNode, str, &j, &k)) {
								failFlag = 1;
								break;
							}
							result[k ++] = str[j ++];
					// 重复0-n次
					case zero2n: 
							while(singleCompare(currentNode, str, &j, &k) && j < strlen(str)) {
								result[k++] = result[j++];
							}
							break;
					// 重复n次,n2m次,n2more次
					case n:
					case n2m:
					case n2more:
							for (int t = 0; t < currentNode->word.content[currentNode->word.contentLen] - '0'; ++t) {
								if (!singleCompare(currentNode, str, &j, &k)) {
									failFlag = 1;break;
								}
								result[k++] = str[j++];
							}
							if (currentNode->type == n2m) {
								for (int i = currentNode->word.content[currentNode->word.contentLen]; i < currentNode->word.content[currentNode->word.contentLen + 1] && j < strlen(str); ++i) {
									if (!singleCompare(currentNode, str, &j, &k))
										break;
									result[k++] = str[j++];
								}
							}
							if (currentNode->type == n2more) {
								while(singleCompare(currentNode, str, &j, &k) && j < strlen(str)) {
									result[k++] = str[j++];
								}
							}
							break;
					// 默认情况是直接匹配，不重复更多次
					default:if (singleCompare(currentNode, str, &j, &k))
								result[k++] = result[j++];
							else
								failFlag = 1;
							break;
				}
				if (failFlag)
					break;
				currentNode = currentNode->next;
			}
			// 匹配成功
			if (!failFlag) {
				result[k] = '\0';
				printf("Success %d, %d\n", i, k);
				// 存入结果
				for (int l = 0; l < k; ++l) {
					result[l] = str[i+l];
				}
				freeHeads(branches);
				return 1;
			}
		}
	}
	// 匹配失败，应返回NULL给result
	
	// 释放内存
	freeHeads(branches);
	return 0;
}

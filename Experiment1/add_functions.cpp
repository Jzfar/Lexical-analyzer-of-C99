#include"lexeme_class_and_func.h"

void add_lex(string lexeme) {
	// 添加新识别的lexeme到tokens
	int size = tokens.size();
	tokens[size - 1]->lexeme = lexeme;
}
void add_num() {
	// 更新计数
	int n = tokens.size() - 1;
	int ptn = tokens[n]->ptn;
	each_ptn_num[ptn]++;
}


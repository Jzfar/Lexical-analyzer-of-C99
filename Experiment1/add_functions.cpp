#include"lexeme_class_and_func.h"

void add_lex(string lexeme) {
	// �����ʶ���lexeme��tokens
	int size = tokens.size();
	tokens[size - 1]->lexeme = lexeme;
}
void add_num() {
	// ���¼���
	int n = tokens.size() - 1;
	int ptn = tokens[n]->ptn;
	each_ptn_num[ptn]++;
}


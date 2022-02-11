#include"lexeme_class_and_func.h"

void proc_lexeme() {

	int pattern;
	// �ж�lexeme�����Ͳ�����bs_p��tokens��
	if (is_key()) {
		;
	}
	else if ((pattern = judge_type()) != -1) {// �ж���lexeme�����ʹ�Ϊpattern
		Token* token = new Token();
		tokens.push_back(token);
		token->ptn = pattern;
		string lex = get_lexeme[pattern]();// ��ȡ��lexeme
		add_lexeme[token->ptn](lex);// ��Ӹ�lexeme
	}
	else if (is_punc()) {
		;
	}
	else {// û��ƥ������ͣ���ΪERR
		Token* token = new Token(ERR, "undefined token");
		tokens.push_back(token);
		errors.push_back(tokens.size() - 1);
		string lex = get_lexeme[ERR]();
		add_lexeme[token->ptn](lex);
	}

	add_num();// ���¼���
}
string pre_process() {

	string src = get_src_file();// ��ȡԴ�ı�
	sum_num = src.length();

	int i = 0;

	// ��ȥ��б��+�س�
	line_num.push_back(i);
	while (src[i] != '\0') {
		if (src[i] != '\n')
			i++;
		else if (i > 0 && src[i - 1] == '\\') {
			src = src.erase(i - 1, 2);
			i -= 1;
			line_num.push_back(i);
		}
		else {
			i++;
			line_num.push_back(i);
		}
	}

	return src;
}
void skip_space() {
	// ��������հ�
	int i = bs_p;
	const char* ptr = src_t->c_str();
	while (true) {
		char c = ptr[i];
		if (c == ' ' || c == '\n' || c == '\t') {
			i++;
		}
		else {
			break;
		}
	}
	bs_p = i;
}

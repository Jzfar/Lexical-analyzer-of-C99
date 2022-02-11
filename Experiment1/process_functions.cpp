#include"lexeme_class_and_func.h"

void proc_lexeme() {

	int pattern;
	// 判定lexeme的类型并更新bs_p，tokens等
	if (is_key()) {
		;
	}
	else if ((pattern = judge_type()) != -1) {// 判定出lexeme的类型存为pattern
		Token* token = new Token();
		tokens.push_back(token);
		token->ptn = pattern;
		string lex = get_lexeme[pattern]();// 获取该lexeme
		add_lexeme[token->ptn](lex);// 添加该lexeme
	}
	else if (is_punc()) {
		;
	}
	else {// 没有匹配的类型，记为ERR
		Token* token = new Token(ERR, "undefined token");
		tokens.push_back(token);
		errors.push_back(tokens.size() - 1);
		string lex = get_lexeme[ERR]();
		add_lexeme[token->ptn](lex);
	}

	add_num();// 更新计数
}
string pre_process() {

	string src = get_src_file();// 获取源文本
	sum_num = src.length();

	int i = 0;

	// 脱去反斜杠+回车
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
	// 跳过三类空白
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

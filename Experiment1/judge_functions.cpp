#include"lexeme_class_and_func.h"
int judge_cons_type() {
	// 判定是否是int，flo，char三种常量之一，如果是返回对应的编号，如果不是返回-1
	const char* ptr = src_t->c_str();
	int i = bs_p;
	char c = ptr[i];
	if (c == '\'' || c == 'L' && ptr[i + 1] == '\'')
		return CHAR;
	else if (c == '.')
		return FLO;
	else if (is_digit(c)) {
		if (contain_flo_sign())
			return FLO;
		else
			return INT;
	}
	else
		return -1;
}
int judge_type() {
	// 判定待解析的lexeme的类型
	int pattern;
	if (is_com())
		return COM;
	else if ((pattern = judge_cons_type()) != -1)
		return pattern;
	else if (is_str())
		return STR;
	else if (is_id())
		return ID;
	return -1;
}

#include"lexeme_class_and_func.h"

Token::Token(int ptn, string lex) {
	this->ptn = ptn;
	this->lexeme = lex;
}
Token::Token() {
	this->ptn = -1;
	this->lexeme = "";
}

void get_position(int p,int* p2) {
	// 获取指向第p个字符的单词在原来文本中的行数，列数
	int i;
	for (i = 0; i < line_num.size()  && p >= line_num[i]; i++)
		;
	i--;
	p2[0] = i + 1;
	p2[1] = p - line_num[i] + 1;

}
string get_id() {
	// 判定当前lexeme是否是标识符
	int state = 0;
	int errState = -1;
	int i;
	const char* ptr = src_t->c_str();
	char c;
	for (i = bs_p; state>=0;i++) {

		c = ptr[i];
		if (is_space_break(c) || is_punc(c)) {
			if (state == 6) {
				string sub_str = src_t->substr(bs_p, i-bs_p);
				int p2[2];
				get_position(bs_p, p2);
				char pst[25];
				sprintf(pst, "%5d : %-5d", p2[0], p2[1]);
				string position = pst;
				id_info id_i;
				id_i.id = sub_str;
				id_i.position = position;
				id_vec.push_back(id_i);
				bs_p = i;
				return to_string(id_vec.size());
			}
			else
				state = errState;
		}
		int temp = i;
		switch (state) {
		case 0:
			if (c == '\\')
				state = 1;
			else if(is_nondigit(c))
				state = 6;
			break;
		case 1:
			if (c == 'u')
				state = 2;
			else if (c == 'U')
				state = 7;
			break;
		case 2:
			for (int j = 0; j < 4; j++) {
				i = temp + j;
				c = ptr[i];
				if (is_hexdigit(c))
					;
				else {
					state = errState;
					break;
				}
			}
			if (state == 2)
				state = 6;
			break;
		case 7:
			for (int j = 0; j < 8; j++) {
				i = temp + j;
				c = ptr[i];
				if (is_hexdigit(c))
					;
				else {
					state = errState;
					break;
				}
			}
			if (state == 7)
				state = 6;
			break;
		case 6:
			if (is_nondigit(c) || is_digit(c))
				;
			else if (c == '\\')
				state = 1;
			else
				state = errState;
			break;
		}
	}
	// 错误处理
	tokens[tokens.size() - 1]->ptn = ERR;
	errors.push_back(tokens.size() - 1);
	int p2[2];
	get_position(bs_p, p2);
	string errMsg =to_string(p2[0]) + ":" + to_string(p2[1]) +" : undefined identifier";
	i--;
	while (true) {
		c = ptr[i];
		if (is_space_break(c) || is_punc(c)) {
			bs_p = i;
			break;
		}
		i++;
	}
	return errMsg;
}
string get_int() {
	// 判断当前lexeme是否是整型
	int state = 0;
	const char* ptr = src_t->c_str();
	int i;
	char c;
	int errState = -1;
	int end_state[] = { 1,2,4,5,6 };
	for (i = bs_p; state >= 0; i++) {
		c = ptr[i];
		if (is_space_break(c) || is_punc(c)) {
			int arr_len = sizeof(end_state) / sizeof(end_state[0]);
			int* index = find(end_state, end_state + arr_len, state);
			if ((index - end_state) < arr_len) {
				/*Terminated in the terminated state
				*/
				string sub_str = src_t->substr(bs_p, i-bs_p);
				bs_p = i;
				return sub_str;
			}
			else {
				state = errState;
			}
		}
		int sfx_len;//the length of suffix
		switch (state) {
		case 0:
			if (c == '0')
				state = 2;
			else if (is_nzdigit(c))
				state = 1;
			break;
		case 1:
			if (is_digit(c))
				;
			else if ((sfx_len = is_int_suffix(i)) > 0) {
				i--;
				i += sfx_len;
				state = 5;
			}
			else
				state = errState;
			break;
		case 2:
			if (is_octdigit(c))
				state = 6;
			else if ((sfx_len = is_int_suffix(i)) > 0) {
				i--;
				i += sfx_len;
				state = 5;
			}
			else if (c == 'x' || c == 'X')
				state = 3;
			else
				state = errState;
			break;
		case 6:
			if (is_octdigit(c))
				;
			else if ((sfx_len = is_int_suffix(i)) > 0) {
				i--;
				i += sfx_len;
				state = 5;
			}
			else
				state = errState;
			break;
		case 3:
			if (is_hexdigit(c))
				state = 4;
			else
				state = errState;
			break;
		case 4:
			if (is_hexdigit(c))
				;
			else if ((sfx_len = is_int_suffix(i)) > 0) {
				i--;
				i += sfx_len;
				state = 5;
			}
			else
				state = errState;
			break;
		case 5:
			if (!is_space_break(c) || is_punc(c))
				state = errState;
			break;
		}
	}
	// 错误处理
	tokens[tokens.size() - 1]->ptn = ERR;
	errors.push_back(tokens.size() - 1);
	i--;
	int p2[2];
	get_position(bs_p, p2);
	string errMsg = to_string(p2[0]) + ":" + to_string(p2[1]) + " : undefined integer";

	while (true) {
		c = ptr[i];
		if (is_space_break(c) || is_punc(c)) {
			bs_p = i;
			break;
		}
		i++;
	}
	return errMsg;
}
string get_dec_flo() {
	// 判定当前lexeme是否是十进制浮点数
	int state = 0;
	const char* ptr = src_t->c_str();
	int i;
	char c;
	int errState = -1;
	int endState = 8;

	for (i = bs_p; state != endState && state != errState; i++) {

		c = ptr[i];

		switch (state) {
		case 0:
			if (c == '.')
				state = 1;
			else if (is_digit(c))
				state = 3;
			else
				state = errState;
			break;
		case 1:
			if (is_digit(c))
				state = 2;
			else
				state = errState;
			break;
		case 2:
			if (is_digit(c))
				;
			else if (c == 'e' || c == 'E')
				state = 4;
			else if (is_flo_suffix(c))
				state = 7;
			else if (is_space_break(c) || is_punc1(c))
				state = endState;
			else
				state = errState;
			break;
		case 3:
			if (is_digit(c))
				;
			else if (c == '.')
				state = 2;
			else if (c == 'e' || c == 'E')
				state = 4;
			else
				state = errState;
			break;
		case 4:
			if (c == '+' || c == '-')
				state = 6;
			else if (is_digit(c))
				state = 5;
			else
				state = errState;
			break;
		case 5:
			if (is_digit(c))
				;
			else if (is_flo_suffix(c))
				state = 7;
			else if (is_space_break(c) || is_punc1(c))
				state = endState;
			else
				state = errState;
			break;
		case 6:
			if (is_digit(c))
				state = 5;
			else
				state = errState;
			break;
		case 7:
			if (is_space_break(c) || is_punc1(c))
				state = endState;
			else
				state = errState;
		break;
		}
	}

	if (state == endState) {
		 // 判定结果为十进制浮点数
			string sub_str = src_t->substr(bs_p, i - bs_p);
			bs_p = i;
			return sub_str;
	}
	// 错误处理
	tokens[tokens.size() - 1]->ptn = ERR;
	errors.push_back(tokens.size() - 1);
	i--;
	int p2[2];
	get_position(bs_p, p2);
	string errMsg = to_string(p2[0]) + ":" + to_string(p2[1]) + " : undefined decimal float";

	while (true) {
		c = ptr[i];
		if (is_space_break(c) || is_punc1(c)) {
			bs_p = i;
			break;
		}
		i++;
	}
	return errMsg;
}
string get_hex_flo() {
	// 判定当前的lexeme是否为十六进制浮点数
	int state = 0;
	const char* ptr = src_t->c_str();
	int i;
	char c;
	int errState = -1;
	int endState = 8;

	for (i = bs_p+2; state != endState && state != errState; i++) {

		c = ptr[i];

		switch (state) {
		case 0:
			if (is_hexdigit(c))
				state = 1;
			else if (c == '.')
				state = 3;
			else
				state = errState;
			break;
		case 1:
			if (is_hexdigit(c))
				;
			else if (c == '.')
				state = 2;
			else if (c == 'p' || c == 'P')
				state = 4;
			else
				state = errState;
			break;
		case 2:
			if (is_hexdigit(c))
				;
			else if (c == 'p' || c == 'P')
				state = 4;
			else
				state = errState;
			break;
		case 3:
			if (is_hexdigit(c))
				state = 2;
			else
				state = errState;
			break;
		case 4:
			if (is_digit(c))
				state = 6;
			else if (c == '+' || c == '-')
				state = 5;
			else
				state = errState;
			break;
		case 5:
			if (is_digit(c))
				state = 6;
			else
				state = errState;
			break;
		case 6:
			if (is_digit(c))
				;
			else if (is_flo_suffix(c))
				state = 7;
			else if (is_space_break(c) || is_punc1(c))
				state = endState;
			else
				state = errState;
			break;
		case 7:
			if (is_space_break(c) || is_punc1(c))
				state = endState;
			else
				state = errState;
		}

	}

	if (state == endState) {
		// 判定结果：当前lexeme是十六进制小数
			string sub_str = src_t->substr(bs_p, i - bs_p);
			bs_p = i;
			return sub_str;
	}
	// 错误处理
	tokens[tokens.size() - 1]->ptn = ERR;
	errors.push_back(tokens.size() - 1);
	int p2[2];
	get_position(bs_p, p2);
	string errMsg = to_string(p2[0]) + ":" + to_string(p2[1]) + " : undefined hexadecimal float";

	i--;
	while (!is_space_break(ptr[i]) || is_punc1(ptr[i]) )
		i++;
	bs_p = i;

	return errMsg;
}
string get_flo() {
	// 判定并获取浮点型lexeme
	const char* ptr = src_t->c_str();
	int i = bs_p;
	if (ptr[i] == '0' && ptr[i+1] == 'x')
		return get_hex_flo();
	else
		return get_dec_flo();
}
string get_char() {
	// 判定当前lexeme是否是char
	const char* ptr = src_t->c_str();
	int i;
	char c;
	int state = 0;
	int errState = -1;
	int endState = 4;

	for (i = bs_p; (state != endState && state != errState); i++) {

		c = ptr[i];

		switch (state) {
		case 0:
			if (c == '\'')
				state = 2;
			else if (c == 'L') {
				i++;
				state = 2;
			}
			break;
		case 2:
			if (c == '\0' || c == '\n' || c == '\'')
				state = errState;
			else if (c == '\\')
				state = 5;
			else
				state = 3;
			break;
		case 3:
			if (c == '\0' || c == '\n')
				state = errState;
			else if (c == '\\')
				state = 5;
			else if (c == '\'')
				state = endState;
			else
				;
			break;
		case 5:
			state =  is_escape_sequence(i);
			break;
		}
	}
	
	if (state == endState) {
		// 到达终止状态
		int temp = bs_p;
		bs_p = i;
		return src_t->substr(temp, i - temp);
	}
	else {
		// 错误
		tokens[tokens.size() - 1]->ptn = ERR;
		errors.push_back(tokens.size() - 1);
		i--;
		while (!is_space_break(ptr[i]))
			i++;
		int p2[2];
		get_position(bs_p, p2);
		string errMsg = to_string(p2[0]) + ":" + to_string(p2[1]) + " : undefined charactor";
		bs_p = i;
		return errMsg;
	}

}
string get_str() {
	// 判定当前lexeme是否是string
	const char* ptr = src_t->c_str();
	int i;
	char c;
	int state = 0;
	int errState = -1;
	int endState = 4;

	for (i = bs_p; (state != endState && state != errState); i++) {

		c = ptr[i];

		switch (state) {
		case 0:
			if (c == '\"')
				state = 3;
			else if (c == 'L') {
				i++;
				state = 3;
			}
			break;
		case 3:
			if (c == '\0' || c == '\n')
				state = errState;
			else if (c == '\\')
				state = 5;
			else if (c == '\"')
				state = endState;
			else
				;
			break;
		case 5:
			state = is_escape_sequence(i);
			break;
		}
	}

	if (state == endState) {
		// 到达正常终止状态
		int temp = bs_p;
		bs_p = i;
		return src_t->substr(temp, i - temp);
	}
	else {
		// 错误处理
		tokens[tokens.size() - 1]->ptn = ERR;
		errors.push_back(tokens.size() - 1);
		i--;
		while (!is_space_break(ptr[i]))
			i++;
		int p2[2];
		get_position(bs_p, p2);
		string errMsg = to_string(p2[0]) + ":" + to_string(p2[1]) + " : undefined string";
		bs_p = i;
		return errMsg;
	}

}
string get_err() {
	// 跳过当前的未定义的lexeme
	const char* ptr = src_t->c_str();
	int i;
	i = bs_p;
	while (!is_space_break(ptr[i]))
		i++;
	int p2[2];
	get_position(bs_p, p2);
	string errMsg = to_string(p2[0]) + ":" + to_string(p2[1]) + " : undefined type";
	bs_p = i;
	return errMsg;
}
string get_com() {
	// 获取这条注释

	int fwd_p = bs_p + 2;
	const char* ptr = src_t->c_str();
	char c = ptr[bs_p +1];
	string comment;

	if (c == '*') {
		while (ptr[fwd_p] != '\0') {
			if (ptr[fwd_p++] != '*')
				;
			else if (ptr[fwd_p++] == '/') {
				break;
			}
		}
	}
	else {
		while (ptr[fwd_p] != '\0') {
			if (ptr[fwd_p++] != '\n')
				;
			else {
				int temp = bs_p;
				bs_p = fwd_p;
				return src_t->substr(temp, fwd_p-1 - temp);
				break;
			}
		}
	}
	int temp = bs_p;
	bs_p = fwd_p;
	return src_t->substr(temp, fwd_p- temp);
}
string get_pattern(int i) {
	// 获取序号对应的类型的名称
	switch (i) {
	case 0: return "ID";
	case 1: return "INT";
	case 2:	return "FLO";
	case 3: return "CHAR";
	case 4:return "STR";
	case 5: return "COM";
	case 6: return "ERR";
	case 7: return "KEY";
	case 8: return "PUNC";
	case -1:return "DEF";
	}
}
int get_test_code_num() {
	// 获取源文本的编号
	string type;
	cout << endl;
	for (int i = 0; i < PTN_NUM + 1; i++) {
		cout << "If you want to test for ";
		switch (i) {
		case 0:
			type = "all types";
			break;
		case 1:
			type = "identifiers";
			break;
		case 2:
			type = "integer";
			break;
		case 3:
			type = "float";
			break;
		case 4:
			type = "char";
			break;
		case 5:
			type = "string";
			break;
		case 6:
			type = "comment";
			break;
		case 7:
			type = "key";
			break;
		case 8:
			type = "the code in which there may be '\\' and line breaks at the end of the line";
			break;
		case 9:
			type = "the code of this tool";
			break;
		}
		cout << type;
		cout << ", please enter ";
		cout << i;
		cout << endl;
	}

	cout << endl;
	int i;
	cin >> i;
	if (i >= 0 && i <= 9)
		return i;
	else {
		cout << "The entered number is not between 0 and 8. Please restart.";
		exit(1);
	}

}




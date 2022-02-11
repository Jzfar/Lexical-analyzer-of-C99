#include"lexeme_class_and_func.h"

bool is_simple_escape_sequence(char c) {

	set<char> sip_set = { '\'','\"','\?','\\'\
		, 'a','b','f','n','r','t','v' };

	if (sip_set.find(c) != sip_set.end())
		return true;

	return false;
}
bool is_punc() {
	const set<string> punc_set = { "[","]","(",")","{","}",".","->",\
													"++","--","&","*","+", "-", "~", "!", \
													"/", "%", "<<", ">>", "<", ">", "<=", ">=", "==", "!=", "^", "|", "&&", "||",\
													"?", ":", ";", "...",\
													"=", "*=", "/=", "%=", "+=", "-=", "<<=", ">>=", "&=", "^=", "|=", \
													",", "#", "##", \
													"<:", ":>", "<%", "%>", "%:", "%:%:" };
	string sub_str;
	set<string>::iterator it;
	int i_max = 0;
	/*
	* 待测的sub_str如<<=，其前缀<,<<,<<=都是合法punc
	* 以最长的为准
	* i_max存最大的长度
	*/
	const char* ptr = src_t->c_str();
	for (int i = 1; i <= 3 && ptr[i - 1]; i++) {
		if (bs_p + i > src_t->length())
			break;
		sub_str = src_t->substr(bs_p, i);
		it = punc_set.find(sub_str);

		if (it != punc_set.end()) {
			if (i_max < i) {
				i_max = i;
			}
		}
	}

	if (i_max != 0) {
		int temp = bs_p;
		bs_p = bs_p + i_max;
		Token* token = new Token(PUNC, src_t->substr(temp, i_max));
		tokens.push_back(token);
		return true;
	}

	return false;
}
bool is_punc(char c) {

	const set<char> punc_set = { '!','#','%','&','(',')',
		'*','+',',','-','.','/',':',';','<','=','>','?','[',']','^','{','|','}','~' };

	if (punc_set.find(c) != punc_set.end())
		return true;

	return false;
}
bool is_punc1(char c) {
	// with + -, without .
	const set<char> punc_set = { '!','#','%','&','(',')',
		'*',',','+','-','/',':',';','<','=','>','?','[',']','^','{','|','}','~' };

	if (punc_set.find(c) != punc_set.end())
		return true;

	return false;
}
bool is_hexdigit(char c) {
	if (c >= '0' && c <= '9' || c >= 'a' && c <= 'f' || c >= 'A' && c <= 'F') {
		return true;
	}
	else {
		return false;
	}
}
bool is_octdigit(char c) {
	if (c >= '0' && c <= '7')
		return true;
	else
		return false;
}
bool is_nondigit(char c) {
	if (c >= 'a' && c <= 'z' || c >= 'A' && c <= 'Z' || c == '_')
		return true;
	else
		return false;
}
bool is_digit(char c) {
	if (c >= '0' && c <= '9')
		return true;
	else
		return false;
}
bool is_space_break(char c) {
	if (c == ' ' || c == '\t' || c == '\n' || c == '\0')
		return true;
	else
		return false;
}
bool is_nzdigit(char c) {
	if (c >= '1' && c <= '9')
		return true;
	else
		return false;

}
int is_int_suffix(int i) {
	set<string> int_sfx = {
		"u","U","l","L","ll","LL",
		"lu","lU","Lu","LU","llu","llU","LLu","LLU",
		"ul","uL","ull","uLL","Ul","UL","Ull","ULL" };
	int i_max;
	i_max = 0;
	set<string>::iterator it;
	for (int len = 1; len <= 3; len++) {
		int end_p = i + len;
		if (i + len > src_t->length())
			break;
		string sub_str = src_t->substr(i, len);
		int det = sub_str.length();
		it = int_sfx.find(sub_str);
		if (it != int_sfx.end() && i_max < len)
			i_max = len;
	}
	return i_max;
}
int is_flo_suffix(char c) {
	if (c == 'f' || c == 'F' || c == 'l' || c == 'L')
		return true;
	else
		return false;
}
int is_escape_sequence(int& i) {
	const char* ptr = src_t->c_str();
	char c = ptr[i];
	int state = 5;
	int errState = -1;
	if (is_simple_escape_sequence(c))
		state = 3;
	else if (is_octdigit(c)) {
		if (is_octdigit(ptr[i + 1]))
			i += 1;
		else if (is_octdigit(ptr[i + 1]))
			i += 1;
		state = 3;
	}
	else if (c == 'x') {

		if (is_hexdigit(ptr[++i]))
			state = 3;
		else
			state = errState;

		while (is_hexdigit(ptr[i + 1]))
			i++;
	}
	else if (c == 'u' || c == 'U') {
		int p;
		int len = c == 'u' ? 4 : 8;
		for (p = i + 1; p <= (i + len) && is_hexdigit(ptr[p]); p++)
			;
		if (p == (i + len + 1)) {
			i += len;
			state = 3;
		}
		else
			state = errState;
	}
	return state;
}
bool is_str() {

	const char* ptr = src_t->c_str();
	int i = bs_p;

	if (ptr[i] == '\"' ||
		ptr[i] == 'L' && ptr[i + 1] == '\"')
		return true;
	else
		return false;
}
bool is_id() {
	const char* ptr = src_t->c_str();
	char fir = ptr[bs_p];
	if (is_nondigit(fir))
		return true;
	else if (fir == '\\') {
		if (ptr[bs_p + 1] == 'u' || ptr[bs_p + 1] == 'U')
			return true;
	}
	return false;
}
bool is_com() {

	string sub_str = src_t->substr(bs_p, 2);
	if (!sub_str.compare("//") || !sub_str.compare("/*")) {
		return true;
	}
	return false;
}
bool is_key() {
	/*
	* the maximum length among members in key_set is 8
	* each of the prefixes of the first 8 characters may be a key,
	* and is not same as others
	*/
	set<string> key_set = {
		"auto","enum","restrict","unsigned",\
		"break", "extern", "return", "void", \
		"case", "float", "short", "volatile", \
		"char", "for", "signed", "while", \
		"const", "goto", "size", "_Bool", \
		"continue", "if", "static", "_Complex", \
		"default", "inline", "struct", "_Imaginary", \
		"do", "int", "switch",  \
		"double", "long", "typedef", \
		"else", "register", "union"
	};
	int max_len = 10;
	for (int i = 1; i <= max_len; i++) {
		if (bs_p + i > src_t->length())
			break;
		string sub_str = src_t->substr(bs_p, i);
		set<string>::iterator it = key_set.find(sub_str);
		if (it != key_set.end()) {
			bs_p = bs_p + i;
			Token* token = new Token(KEY, sub_str);
			tokens.push_back(token);
			return true;
		}
	}
	return false;
}
bool contain_flo_sign() {

	const char* ptr = src_t->c_str();
	int i = bs_p;
	char c = ptr[i];
	while (true) {
		if (c == ' ' || c == '\n' || c == '\t' || c == '\0' || is_punc1(c))
			return false;
		if (c == '.' || c == 'e' || c == 'p')
			return true;

		i++;
		c = ptr[i];

	}
}

#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
#include<fstream>
#include<sstream>
#include<set>
#include<vector>
#include<algorithm>
#include<cstdlib>
#include<stdio.h>
using namespace std;

enum PTN
{
	ID, INT, FLO, CHAR, STR, COM, ERR, KEY, PUNC, PTN_NUM
};
// the last element is the amount of patterns

extern string(*get_lexeme[PTN_NUM]) ();
extern void (*add_lexeme[PTN_NUM])(string);
extern vector<int> line_num;// the positions of each line's beginning
extern unsigned int bs_p;// base_position: points to the current lexeme's first character
extern const string* src_t;// the pointer of source text
extern unsigned int each_ptn_num[PTN_NUM];// the numbers of lexemes of each pattern
extern unsigned int sum_num;// total number of characters

struct id_info {
	string id;
	string position;
};
typedef struct id_info id_info;
extern vector<id_info> id_vec;  // id table

class Token {
public:
	int ptn;
	string lexeme;
	Token(int ptn, string lex);
	Token();
};
extern vector<Token*> tokens;// token table
extern vector<int> errors;// error is stored in "tokens" whose "ptn" is ERR

string get_id();
string get_int();
string get_flo();
string get_char();
string get_str();
string get_com();
string get_err();
string get_dec_flo();
string get_hex_flo();
string get_pattern(int i);// get the name of pattern whose serial number is i
void get_position(int p, int* p2);// p: bs_p of the lexeme; p2[0]: row number, p2[1]: column number
int get_test_code_num();

void add_lex(string);// add lexeme to "tokens"
void add_num();// add the number of the type of the new added lexeme

bool is_nondigit(char);
bool is_hexdigit(char);
bool is_digit(char);
bool is_punc();
// punctuation set is the standard punctuation set
bool is_punc(char);
// the set of punctuation whose length is 1 in the standard punctuation set
bool is_punc1(char);
//  the set of punctuation whose length is 1 in the standard punctuation set except '.'
bool is_simple_escape_sequence(char c);
bool is_octdigit(char c);
bool is_space_break(char c);// space break: ' ' , '\n' , '\t', or '\0'
bool is_nzdigit(char c);// nzdigit: nonzore-digit
int is_int_suffix(int i);
int is_flo_suffix(char c);
int is_escape_sequence(int& i);
bool is_str();
bool is_id();
bool is_com();
bool is_key();
bool contain_flo_sign();

void print_identifiers();
void print_tokens();
void print_each_num();// print the numbers of lexemes of each pattern
void print_basic_info();// print line number and total number of charaters
void print_error();
void print_result();// print the result of lexical analysis
void print_greetings();

void proc_lexeme();
string pre_process();
void skip_space();

int judge_type();
int judge_cons_type();// return one of the constant type

string get_src_file();
void init();
void init_funcs();
#include"lexeme_class_and_func.h"
void init_funcs() {

	get_lexeme[ID] = get_id;
	get_lexeme[INT] = get_int;
	get_lexeme[FLO] = get_flo;
	get_lexeme[CHAR] = get_char;
	get_lexeme[STR] = get_str;
	get_lexeme[COM] = get_com;
	get_lexeme[ERR] = get_err;

	add_lexeme[ID] = add_lex;
	add_lexeme[INT] = add_lex;
	add_lexeme[FLO] = add_lex;
	add_lexeme[CHAR] = add_lex;
	add_lexeme[STR] = add_lex;
	add_lexeme[COM] = add_lex;
	add_lexeme[ERR] = add_lex;

}
void init() {
	bs_p = 0;
	init_funcs();
}
string get_src_file() {

	print_greetings();

	int sign;
	cin >> sign;
	int i;
	string path;
	switch(sign) {
		case 0:
			i = get_test_code_num();
			path = (string)"test\\test" + to_string(i) + (string)".c";
			break;
		case 1:
			cout << "Please enter the path of test code\n";
			cin >> path;
			break;
		default:
			cout << "The entered number is not 0 or 1. Please restart.";
			exit(1);
	}

	ifstream infile;
	infile.open(path);
	if (!infile.is_open()) {
		cout << "Fail to open file.";
		exit(0);
	}
	string str;
	getline(infile, str, (char)'\0');
	infile.close();
	return str;
}
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
#include"lexeme_class_and_func.h"
int judge_cons_type() {

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
	int pattern;
	if (is_com())
		return COM;
	else if (is_id())
		return ID;
	else if ((pattern = judge_cons_type()) != -1)
		return pattern;
	else if (is_str())
		return STR;
	return -1;
}
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
	int i;
	for (i = 0; i < line_num.size()  && p >= line_num[i]; i++)
		;
	i--;
	p2[0] = i + 1;
	p2[1] = p - line_num[i] + 1;

}
string get_id() {

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
			string sub_str = src_t->substr(bs_p, i - bs_p);
			bs_p = i;
			return sub_str;
	}

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
			string sub_str = src_t->substr(bs_p, i - bs_p);
			bs_p = i;
			return sub_str;
	}

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
	const char* ptr = src_t->c_str();
	int i = bs_p;
	if (ptr[i] == '0' && ptr[i+1] == 'x')
		return get_hex_flo();
	else
		return get_dec_flo();
}
string get_char() {

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
		int temp = bs_p;
		bs_p = i;
		return src_t->substr(temp, i - temp);
	}
	else {
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
		int temp = bs_p;
		bs_p = i;
		return src_t->substr(temp, i - temp);
	}
	else {
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
	/*
	* the paramenter c is the second charactor which is '*' or '/'
	* when c is '*', the end charactor of the word is "* /"
	* when c is '/', the end charactor of the word is line break
	*/
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
	string type;
	cout << endl;
	for (int i = 0; i < PTN_NUM; i++) {
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
		}
		cout << type;
		cout << ", please enter ";
		cout << i;
		cout << endl;
	}

	cout << endl;
	int i;
	cin >> i;
	if (i >= 0 && i <= 8)
		return i;
	else {
		cout << "The entered number is not between 0 and 8. Please restart.";
		exit(1);
	}

}
#include"lexeme_class_and_func.h"

void add_lex(string lexeme) {
	int size = tokens.size();
	tokens[size - 1]->lexeme = lexeme;
}
void add_num() {
	int n = tokens.size() - 1;
	int ptn = tokens[n]->ptn;
	each_ptn_num[ptn]++;
}
#include"lexeme_class_and_func.h"

void print_identifiers() {
	cout << "identifiers:\n";
	char pos[] = "position(row:column)";
	char lex[] = "id";
	char num[] = "serial number";
	printf("%-20s%-25s%s\n", num, pos, lex);

	for (unsigned int i = 0; i < id_vec.size(); i++) {
		printf("%-20d%-25s%s\n", i + 1,
			id_vec[i].position.c_str(), id_vec[i].id.c_str());
	}
	cout << endl;
}
void print_tokens() {

	cout << "tokens" << endl;

	for (unsigned int i = 0; i < tokens.size(); i++) {
		if (tokens[i]->ptn == ERR)
			continue;
		cout << '[';
		cout << get_pattern(tokens[i]->ptn);
		cout << ',';
		cout << tokens[i]->lexeme;
		cout << ']';
		cout << endl;
	}

	cout << endl;
}
void print_each_num() {
	cout << "The number of each pattern:" << endl;
	for (int i = 0; i < PTN_NUM; i++) {
		if (i == ERR)
			continue;
		cout << (string)"The number of " + get_pattern(i) + (string)": ";
		cout << each_ptn_num[i] << endl;
	}
	cout << endl;
}
void print_basic_info() {
	cout << "Basic Information:" << endl;
	cout << "THe number of lines: " << line_num.size() << endl;
	cout << "The total number of characters: " << sum_num << endl;
	cout << endl;
}
void print_error() {
	cout << "The number of errors is ";
	cout << each_ptn_num[ERR];
	cout << endl;
	char pos_err[] = "position(row:column) and error";
	char num[] = "serial number";
	printf("%-20s%-25s\n", num, pos_err);
	for (int i = 0; i < errors.size(); i++) {
		printf("%-20d%-25s\n", i + 1, (tokens[errors[i]]->lexeme).c_str());
	}
	cout << endl;
}
void print_result() {
	cout << "The result of lexical analysis: " << endl << endl;
	print_basic_info();
	print_each_num();
	print_identifiers();
	print_tokens();
	print_error();
}
void print_greetings() {
	cout << "Welcome to the Lexical analysis assistant\n";
	cout << endl;
	cout << "If you want to use existing c source code of test, please enter 0 \nor if you want to use your own test code, please enter 1\n\n";
}
#include"lexeme_class_and_func.h"

void proc_lexeme() {

	int pattern;

	if (is_key()) {
		;
	}
	else if ((pattern = judge_type()) != -1) {
		Token* token = new Token();
		tokens.push_back(token);
		token->ptn = pattern;
		string lex = get_lexeme[pattern]();
		add_lexeme[token->ptn](lex);
	}
	else if (is_punc()) {
		;
	}
	else {
		Token* token = new Token(ERR, "error char");
		tokens.push_back(token);
		errors.push_back(tokens.size() - 1);
		string lex = get_lexeme[ERR]();
		add_lexeme[token->ptn](lex);
	}

	add_num();
}
string pre_process() {

	string src = get_src_file();
	sum_num = src.length();

	int i = 0;

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
#include"lexeme_class_and_func.h"

unsigned int bs_p;
const string* src_t;
vector<int> line_num;
vector<id_info> id_vec;
unsigned int each_ptn_num[PTN_NUM] = { 0 };
unsigned int sum_num;
vector<int> errors;
vector<Token*> tokens;
vector<string> comments;
string(*get_lexeme[PTN_NUM]) ();
void (*add_lexeme[PTN_NUM])(string);

int main() {

	string src = pre_process();
	src_t =  &src;

	init();

	skip_space();

	while ((*src_t)[bs_p] != '\0') {

		proc_lexeme();

		skip_space();

	}

	print_result();

	return 0;
}



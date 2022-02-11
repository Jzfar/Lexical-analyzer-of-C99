#include"lexeme_class_and_func.h"
void init_funcs() {
	// 初始化函数指针
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
	// 获取源文本
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

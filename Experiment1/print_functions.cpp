#include"lexeme_class_and_func.h"

void print_identifiers() {
	cout << "----------------------------------------------------------------------------------------\n";
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
	cout << "----------------------------------------------------------------------------------------\n";

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
	cout << "----------------------------------------------------------------------------------------\n";
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
	cout << "========================================================================================\n";
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

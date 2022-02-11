#include"lexeme_class_and_func.h"

unsigned int bs_p;// base_position: points to the current lexeme's first character
const string* src_t;// the pointer of source text
vector<int> line_num;// the positions of each line's beginning
vector<id_info> id_vec;// id table
unsigned int each_ptn_num[PTN_NUM] = { 0 };// the numbers of lexemes of each pattern
unsigned int sum_num;// total number of characters
vector<int> errors;// error is stored in "tokens" whose "ptn" is ERR
vector<Token*> tokens;// token table
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
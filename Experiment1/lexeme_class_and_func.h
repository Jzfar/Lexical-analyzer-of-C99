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
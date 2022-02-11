// Test for identifiers

// legal
// The following 3 are all a
int a;
int \u0061;
int \U00000061;
// The following 3 are all _d6a8
int _d6a8;
int _d6\u00618;
int _d6\U000000618;

// illegal
int 6a;// The 1st character should be nondigit or universal_character_name
int \u00;// Behind \u should be 4 consecutive octal numbers 
int a\U0000003;// Behind \U should be 4 consecutive octal numbers 
// test for char

// legal
char c[] = {'c','\n','\a','\0','\01','\012','\xabcd','\u0061','\U00000061','ab'};
char cc = L'c';

// illegal
char c1 = 'abc\';
char c2 = 'abc'';
char c3 = 'adfg
';


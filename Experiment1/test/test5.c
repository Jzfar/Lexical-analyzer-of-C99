// test for string

// legal
char* s1 = "hello world";
char* s2 = "a3\n\a\x89\u0061\777";
char* ss = L"asd";

// illegal
char* s3 = "hel"lo";
char* s4 = "hel
lo";
char* s5 = "hell\o";
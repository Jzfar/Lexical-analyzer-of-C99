// test for integer

// legal
// Decimal numbers without suffix
int arr1[] = { 9,90,923, 0 };
// Octal numbers without suffix
int arr2[] = { 01,035 };
// Hex numbers without suffix
int arr3[] = { 0xa6, 0x8fd1 };
// examples with suffix
int arr4[] = { 123u,123Ul,123llu,123LU,
					0123U,0123uL,
					0xabclu,0x12fU };

// illegal
int illegalArr[] = { 0x,0x3j,0x3uU,123lL,123uLl };
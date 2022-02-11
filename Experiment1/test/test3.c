// test for float

// legal
// Decimal float without suffix
float dec_flo_arr1[] = {5., 5.2, .1,
                        5.e8, 5.3E-2, .3e35, 5.E+9};
// Decimal float with suffix
float dec_flo_arr2[] = {5.f, 5.2l, 5.3E2L, .3e5F};
// Hex number without suffix
float hex_flo_arr1[] = {0x3.p1, 0x.4p3, 0xa.fp0};
// Hex number with suffix
float hex_flo_arr2[] = {0x3.p1f,0x.4p3F,0xa.fp0l,0xa.fp0L};

float sum = 5.3E-2 + 5.E+9;

// illegal
float illegal_dec_flo[] = {., 5.2e, 5.2e3.5, 5.2e3u, 5.2p2};
float illegal_hec_flo[] = {0x., 0x3.125e2, 0x3.3, 0x3.5p8U};
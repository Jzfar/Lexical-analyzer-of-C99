// test for key
_Bool sign = 1;
int a = 0;
if( sign == 1){
    a++;
}else{
    a--;
}
a -= 1;
a <<=1 ;
switch(a){
    case 1:
    a = 2;
    case 0:
    a >>= 1;
    default:
    a *= 2;
}
typedef struct sign{
    float f;
    long l;
}sign;
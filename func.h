#ifndef CHESS_FUNC_H
#define CHESS_FUNC_H
typedef long long int Long;
typedef unsigned long long int ULong;
int set_bits(Long val);
int leading_count(Long val);
int trailing_count(Long val);
int trailing_count_2(Long val);
Long reversed(Long val);
#define start(string, substring, length) (!strncmp(string, substring, length))
#define stringer(c) (string(1, (char)(c)))
#define zo_rand ((Long)rand() | (Long)rand() << 15 |(Long)rand() << 30 |(Long)rand() << 45 |((Long)rand() & 0xf) << 60 )
#define in_cell(a,b) ((a >> b & 1) == 1)
#endif //CHESS_FUNC_H

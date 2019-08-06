LDLIBS += -lcrypt -lcs50 -lm
CC := clang
CFLAGS += -fsanitize=signed-integer-overflow -fsanitize=undefined -ggdb3 -O0 -Wall -Wextra -Werror -pedantic -std=c11 -Wno-sign-compare -Wno-unused-parameter -Wno-unused-variable -Wshadow

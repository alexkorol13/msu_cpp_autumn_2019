#include <iostream>
#include <stdlib.h>

using namespace std;

int calc(const char *r, int sign)
{
    int left = 0;
    int was_mul = 0, was_div = 0, was_num = 0, was_m = 0;
    while ((*r > '9' || *r < '0') && *r != '\0') {
        if (*r == '-') {
            sign *= -1;
        } else if (*r != '+' && !isspace(*r)) {
            cerr << "Wrong data format\n";
            exit(1);
        }
        r++;
    }
    if (*r == '\0') {
        cerr << "too few numbers\n";
        exit(1);
    }
    was_num = 1;
    while (*r <= '9' && *r >= '0') {
        left = left * 10 + (*r - '0');
        r++;
    }
    cout << left << endl;
    while (true) {
        while (isspace(*r)) {
            r++;
        }
        if (*r == '+') {
            if (was_num) {
                return left * sign + calc(r + 1, 1);
            }
            if (was_mul || was_div || was_m) {
                cerr << "Wrong data format\n";
                exit(1);
            }
        } else if (*r == '-') {
            was_m = 1;
            if (was_num) {
                return sign * left + calc(r + 1, -1);
            } else {
                sign *= -1;
            }
        } else if (*r == '*') {
            was_m = 0;
            was_div = 0;
            if (was_num) {
                was_mul = 1;
                was_num = 0;
            } else {
                cerr << "'*' requires left operand\n";
                exit(1);
            }
        } else if (*r == '/') {
            was_m = 0;
            if (was_num) {
                was_num = 0;
                was_div = 1;
            } else {
                cerr << "'/' requires left operand\n";
                exit(1);
            }
        } else if (*r <= '9' && *r >= '0') {
            was_m = 0;
            was_num = 1;
            if (was_mul || was_div) {
                int tmp = 0;
                while (*r <= '9' && *r >= '0') {
                    tmp = tmp * 10 + (*r - '0');
                    r++;
                }
                was_num = 1;
                if (was_mul) {
                    left *= tmp;
                    was_mul = 0;
                } else {
                    if (tmp == 0) {
                        cerr << "ZeroDivisionError\n";
                        exit(1);
                    }
                    left /= tmp;
                    was_div = 0;
                }
            }
        } else if (*r == '\0') {
            if (!was_num) {
                cerr << "Wrong data format\n";
                exit(1);
            }
            return left * sign;
        } else {
            cerr << "Wrong data format\n";
            exit(1);
        }
        r++;
    }
}

int main(int argc, char **argv)
{
    if (argc > 2) {
        cerr << "too many arguments\n";
        exit(1);
    }
    if (argc == 1) {
        cerr << "too few arguments\n";
        exit(1);
    }
    cout << calc(argv[1], 1) << endl;
    return 0;
}

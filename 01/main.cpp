#include <iostream>
#include <stdlib.h>

using namespace std;

const char *skip_spaces(char *r)
{
    while (isspace(*r)) {
        r++;
    }
    return r;
}

const char *str_to_number(const char *str, int *n)
{
    *n = 0;
    while (isdigit(*str)) {
        *n = 10 * (*n) + *str - '0';
        str++;
    }
    return str;
}

int calc(const char *r)
{
    int was_mul = 0, was_div = 0, was_num = 0, sign = 1, left = 0;
    while (true) {
        r = skip_spaces((char *) r);
        if (*r == '+') {
            if (was_num) {
                return left * sign + calc(r + 1);
            } else {
                throw runtime_error("Wrong data format\n");
            }
        } else if (*r == '-') {
            if (was_num) {
                return sign * left + calc(r);   /* the minus to which r points will
                                                be considered in the sign of calc(r)*/
            } else {
                sign *= -1;
                r++;
            }
        } else if (*r == '*') {
            was_div = 0;
            if (was_num) {
                was_mul = 1;
                was_num = 0;
                r++;
            } else {
                throw runtime_error("'*' requires left operand\n");
            }
        } else if (*r == '/') {
            if (was_num) {
                was_num = 0;
                was_div = 1;
                r++;
            } else {
                throw runtime_error("'/' requires left operand\n");
            }
        } else if (isdigit(*r)) {
            was_num = 1;
            const char *t = r;
            int tmp = 0;
            if (was_mul || was_div) {
                r = str_to_number(t, &tmp);
                if (was_mul) {
                    left *= tmp;
                    was_mul = 0;
                } else {
                    if (tmp == 0) {
                        throw runtime_error("ZeroDivisionError\n");
                    }
                    left /= tmp;
                    was_div = 0;
                }
            } else {
                r = str_to_number(t, &left);
            }
        } else if (*r == '\0') {
            if (!was_num) {
                throw runtime_error("Wrong data format\n");
            }
            return left * sign;
        } else {
            throw runtime_error("Wrong data format\n");
        }
    }
}

int main(int argc, char **argv)
{
    if (argc != 2) {
        cerr << "too many arguments\n";
    } else {
        try {
            cout << calc(argv[1]) << endl;
        } catch (const exception &exc) {
            cerr << exc.what() << endl;
            return 1;
        }
    }
    return 0;
}

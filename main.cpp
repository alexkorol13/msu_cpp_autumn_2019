#include <iostream>
#include <stdlib.h>

using namespace std;

int calc(const char *r, int sign)
{
    //cout << "branch started\n";
    int left = 0;
    int was_mul = 0, was_div = 0, was_num = 0, was_m = 0;
    while ((*r > '9' || *r < '0') && *r != '\0') {
        if (*r == '-') {
            sign *= -1;
        } else if (*r == '+' && !isspace(*r)) {
            throw runtime_error("Wrong data format\n");
        }
        r++;
    }
    //cout << "all beginning - and spaces read\n";
    if (*r == '\0') {
        throw runtime_error("too few numbers\n");
    }
    was_num = 1;
    while (*r <= '9' && *r >= '0') {
        left = left * 10 + (*r - '0');
        r++;
    }
    //cout << "first number " << left << "\n";
    while (true) {
        while (isspace(*r)) {
            r++;
        }
        if (*r == '+') {
            //cout << "+ detected\n";
            if (was_num) {
              //  cout << left*sign << ", sign = " << sign << endl;
                return left * sign + calc(r + 1, 1);
            }
            if (was_mul || was_div || was_m) {
                throw runtime_error("Wrong data format\n");
            }
            r++;
        } else if (*r == '-') {
            //cout << "- detected\n";
            was_m = 1;
            if (was_num) {
                return sign * left + calc(r + 1, -1);
            } else {
                sign *= -1;
            }
            r++;
        } else if (*r == '*') {
            //cout << "* detected\n";
            was_m = 0;
            was_div = 0;
            if (was_num) {
                was_mul = 1;
                was_num = 0;
            } else {
                throw runtime_error("'*' requires left operand\n");
            }
            r++;
        } else if (*r == '/') {
            //cout << "/detected\n";
            was_m = 0;
            if (was_num) {
                was_num = 0;
                was_div = 1;
            } else {
                throw runtime_error("'/' requires left operand\n");
            }
            r++;
        } else if (*r <= '9' && *r >= '0') {
            //cout << "number detected\n";
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
                        throw runtime_error("ZeroDivisionError\n");
                    }
                    left /= tmp;
                    was_div = 0;
                }
            }
        } else if (*r == '\0') {
          //  cout << "\0 reached\n";
            if (!was_num) {
                throw runtime_error("Wrong data format\n");
            }
        //    cout << left*sign << ", sign = " << sign << endl;
            return left * sign;
        } else {
            throw runtime_error("Wrong data format\n");
        }
    }
}

int main(int argc, char **argv)
{
    if (argc != 2) {
        throw runtime_error("wrong number of arguments\n");
    }
    try {
        cout << calc(argv[1], 1) << endl;
    } catch (const exception &exc) {
        cerr << exc.what() << endl;
        return 1;
    }
    return 0;
}

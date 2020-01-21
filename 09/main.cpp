#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <cstdio>
#include <cstdint>
#include <cstddef>
#include <fstream>
#include "sort.h"

using namespace std;

enum { TOTAL = 5, SIZE = 10000000 };

int main(void) {
    int done_flag = 1;
    srand(time(NULL));
    for (int j = 1; j <= TOTAL; j++) {
        ofstream fill_source_file("for_sort.bin", ios::binary | ios::out);
        ofstream fill_test_file("for_test.bin", ios::binary | ios::out);

        for (size_t i = 0; i < SIZE; i++) {
            uint64_t tmp = (1llu << 31) * rand() + rand();
            fill_source_file.write((char *) &tmp, sizeof(tmp));
            fill_test_file.write((char *) &tmp, sizeof(tmp));
        }

        sort_("for_sort.bin", "result.bin");

        ifstream for_test_in("for_test.bin", ios::binary | ios::in);
        ifstream file_sort_result("result.bin", ios::binary | ios::in);
        vector<uint64_t> numbers;
        uint64_t tmp;

        while (for_test_in.read((char *) &tmp, sizeof(tmp))) {
            numbers.push_back(tmp);
        }

        sort(numbers.begin(), numbers.end());

        for (size_t i = 0; i < numbers.size(); i++) {
            if (file_sort_result.read((char *) &tmp, sizeof(tmp))) {
                if (tmp != numbers[i]) {
                    cout << "elements' order is not correct" << endl;
                    done_flag = 0;
                    break;
                }
            } else {
                cout << "some elements had been missed while sorting" << endl;
                done_flag = 0;
                break;
            }
        }

        if (done_flag) {
            cout << "random test " << j << " of " << TOTAL << endl;
            for_test_in.close();
            file_sort_result.close();
            remove("for_test.bin");
            remove("for_sort.bin");
            remove("result.bin");
        } else {
            break;
        }
    }

    remove("for_test.bin");
    remove("for_sort.bin");
    remove("result.bin");

    if (done_flag) cout << "done" << endl;

    return 0;
}

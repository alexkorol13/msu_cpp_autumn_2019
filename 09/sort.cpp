#include <mutex>
#include <vector>
#include <queue>
#include <thread>
#include <cstdint>
#include <cstddef>
#include <cstdio>
#include <string>
#include <fstream>
#include <sstream>
#include <functional>
#include <algorithm>

#include "sort.h"

enum { SIZE = 1048576 / sizeof(size_t) };

void file_sort(std::ifstream &fin, std::mutex &file_mutex, size_t &part, std::mutex &mutex_p) {
    size_t cur_part, size = 0;
    auto *array = new uint64_t[SIZE]();
    while (true) {
        size = 0;
        for (size_t i = 0; i < SIZE; i++) {
            std::unique_lock <std::mutex> unique(file_mutex);
            uint64_t tmp;
            if (!fin.read((char *) &tmp, sizeof(tmp))) {
                break;
            }
            unique.unlock();
            array[size] = tmp;
            size++;
        }
        if (size == 0) {
            break;
        }
        std::lock_guard <std::mutex> guard(mutex_p);
        cur_part = part;
        part++;

        std::sort(array, array + size);
        std::stringstream my_out;
        my_out << "sorted" << cur_part << ".bin";

        std::ofstream fout(my_out.str(), std::ios::out | std::ios::binary);
        for (size_t i = 0; i < size; i++) {
            fout.write((char *) &array[i], sizeof(array[i]));
        }
    }
    delete[](array);
}


void sort_(const std::string &input, const std::string &output) {
    std::vector <std::string> filenames;
    std::vector <std::ifstream> files;
    std::ifstream fin(input, std::ios::in | std::ios::binary);
    std::ofstream fout(output, std::ios::binary | std::ios::out);
    std::size_t parts = 0;
    std::mutex mfile, mpart;

    std::thread t1(file_sort, ref(fin), ref(mfile), std::ref(parts), ref(mpart));
    std::thread t2(file_sort, ref(fin), ref(mfile), std::ref(parts), ref(mpart));
    t1.join();
    t2.join();
    fin.close();

    for (size_t i = 0; i < parts; i++) {
        std::stringstream my_out;
        my_out << "sorted" << i << ".bin";
        filenames.push_back(my_out.str());
    }

    for (auto &name : filenames) {
        files.emplace_back(name, std::ios::in | std::ios::binary);
    }

    std::priority_queue <std::pair <uint64_t, size_t>,
            std::vector <std::pair <uint64_t, size_t>>,
                    std::greater<>> queue;

    for (size_t i = 0; i < files.size(); i++) {
        if (files[i].is_open()) {
            uint64_t tmp;
            if (files[i].read((char *) &tmp, sizeof(tmp))) {
                queue.push(std::make_pair(tmp, i));
            }
        }
    }

    while (not queue.empty()) {
        size_t n;
        uint64_t tmp;
        std::tie(tmp, n) = queue.top();
        queue.pop();
        fout.write((char *) &tmp, sizeof(tmp));
        if (files[n].read((char *) &tmp, sizeof(tmp))) {
            queue.push(std::make_pair(tmp, n));
        }
    }

    fout.close();

    for (const auto &name : filenames) {
        remove(name.c_str());
    }
}
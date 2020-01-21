#pragma once

#include <mutex>

void file_sort(std::ifstream &fin, std::mutex &file_m, size_t &num, std::mutex &num_m);

void sort_(const std::string &input, const std::string &output);
//
// Created by Hamager Y on 2022/1/29.
//

#ifndef MD5_H
#define MD5_H

#define _CRT_SECURE_NO_WARNINGS

#include <string>
#include <cstring>

using std::string;

void md5bin(const void *dat, size_t len, unsigned char out[16]);
std::string md5(const void *dat, size_t len);


#endif //MD5_H

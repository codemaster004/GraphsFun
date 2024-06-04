/**
 * Created by Filip Dabkowski on 25/04/2024.
 */

#ifndef UTILITIES_H
#define UTILITIES_H

#include <cstdio>

int inputNumber(char divider = 0);

template<typename T>
T min(T a, T b) {
	return a < b ? a : b;
}

#endif //UTILITIES_H

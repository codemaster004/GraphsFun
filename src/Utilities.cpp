/**
 * @file Utilities.cpp
 * @brief [Description of file]
 *
 * @author Filip Dabkowski
 * @date 25/04/2024
 */
#include "Utilities.h"

#include <cstdio>


int inputNumber(char divider) {
	int number = 0;

	int ch;
	while ((ch = getchar()) != divider) {
		if (ch == EOF || (divider == 0 && (ch == ' ' || ch == '\n'))) {
			break;
		}
		if (ch >= '0' && ch <= '9') {
			number = number * 10 + (ch - '0');
		}
	}

	return number;
}

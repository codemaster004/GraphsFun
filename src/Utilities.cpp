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

	char ch;
	while ((ch = (char) getchar()) != divider) {
		if (!divider && (ch == ' ' || ch == '\n')) {
			break;
		}
		number = number * 10 + (ch - '0');
	}

	return number;
}

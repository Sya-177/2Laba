#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

typedef struct {
    char letter;
    int digit;
} LetterMap;

LetterMap unique_letters[10];
int letter_count = 0;
bool used_digits[10] = { false };

bool is_leading_letter(char c, const char* equation) {
    if (equation[0] == c) return true;
    for (int i = 1; equation[i] != '\0'; i++) {
        if (isalpha((unsigned char)equation[i]) && !isalpha((unsigned char)equation[i - 1])) {
            if (equation[i] == c) return true;
        }
    }
    return false;
}

int get_digit(char c) {
    for (int i = 0; i < letter_count; i++) {
        if (unique_letters[i].letter == c) return unique_letters[i].digit;
    }
    return -1;
}

bool check_equation(const char* equation) {
    char buffer[256];
    strcpy(buffer, equation);

    long long sum_left = 0;
    long long current_num = 0;
    long long right_side = 0;
    bool is_right_side = false;

    char* token = strtok(buffer, " ");
    while (token != NULL) {
        if (strcmp(token, "+") == 0) {
            sum_left += current_num;
            current_num = 0;
        }
        else if (strcmp(token, "=") == 0) {
            sum_left += current_num;
            current_num = 0;
            is_right_side = true;
        }
        else if (isalpha((unsigned char)token[0])) {
            current_num = 0;
            for (int i = 0; token[i] != '\0'; i++) {
                current_num = current_num * 10 + get_digit(token[i]);
            }
            if (is_right_side) {
                right_side = current_num;
            }
        }
        token = strtok(NULL, " ");
    }
    return sum_left == right_side;
}

bool solve_rebus(int index, const char* equation, char* result_str) {
    if (index == letter_count) {
        if (check_equation(equation)) {
            int pos = 0;
            for (int i = 0; equation[i] != '\0'; i++) {
                if (isalpha((unsigned char)equation[i])) {
                    result_str[pos++] = '0' + get_digit(equation[i]);
                }
                else {
                    result_str[pos++] = equation[i];
                }
            }
            result_str[pos] = '\0';
            return true;
        }
        return false;
    }

    char current_letter = unique_letters[index].letter;
    bool leading = is_leading_letter(current_letter, equation);

    for (int d = 0; d <= 9; d++) {
        if (d == 0 && leading) continue;
        if (!used_digits[d]) {
            used_digits[d] = true;
            unique_letters[index].digit = d;

            if (solve_rebus(index + 1, equation, result_str)) {
                return true;
            }

            used_digits[d] = false;
        }
    }
    return false;
}

void solve(const char* equation) {
    letter_count = 0;
    memset(used_digits, 0, sizeof(used_digits));

    bool seen[26] = { false };
    for (int i = 0; equation[i] != '\0'; i++) {
        if (isalpha((unsigned char)equation[i])) {
            int idx = equation[i] - 'A';
            if (!seen[idx]) {
                seen[idx] = true;
                unique_letters[letter_count++].letter = equation[i];
            }
        }
    }

    char result[256];
    if (solve_rebus(0, equation, result)) {
        printf("Input: %s\nSolution: %s\n", equation, result);
    }
    else {
        printf("Solution not found for: %s\n", equation);
    }
}

int main() {
    solve("SEND + MORE = MONEY");
    solve("BE + BE = MOO");
    return 0;
}

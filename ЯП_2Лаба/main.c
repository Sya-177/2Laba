#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <time.h>

#define MAX_UNIQUE 10

typedef struct {
    char letter;
    long long weight;
    int digit;
    bool is_leading;
} Letter;

Letter unique_letters[MAX_UNIQUE];
int letter_count = 0;

void add_letter_weight(char c, long long weight, bool is_leading) {
    for (int i = 0; i < letter_count; i++) {
        if (unique_letters[i].letter == c) {
            unique_letters[i].weight += weight;
            if (is_leading) {
                unique_letters[i].is_leading = true;
            }
            return;
        }
    }

    if (letter_count < MAX_UNIQUE) {
        unique_letters[letter_count].letter = c;
        unique_letters[letter_count].weight = weight;
        unique_letters[letter_count].digit = -1;
        unique_letters[letter_count].is_leading = is_leading;
        letter_count++;
    }
}

void calculate_weights(const char* equation) {
    char buffer[256];
    strcpy(buffer, equation);

    letter_count = 0;
    bool is_right_side = false;

    char* token = strtok(buffer, " ");
    while (token != NULL) {
        if (strcmp(token, "+") == 0) {
        }
        else if (strcmp(token, "=") == 0) {
            is_right_side = true;
        }
        else if (isalpha((unsigned char)token[0])) {
            int len = (int)strlen(token);
            long long current_multiplier = 1;

            for (int i = len - 1; i >= 0; i--) {
                char c = token[i];
                bool is_leading = (i == 0);

                long long final_weight = is_right_side ? -current_multiplier : current_multiplier;

                add_letter_weight(c, final_weight, is_leading);
                current_multiplier *= 10;
            }
        }
        token = strtok(NULL, " ");
    }
}

int compare_letters(const void* a, const void* b) {
    const Letter* la = (const Letter*)a;
    const Letter* lb = (const Letter*)b;

    long long wa = llabs(la->weight);
    long long wb = llabs(lb->weight);

    if (wa < wb) return 1;
    if (wa > wb) return -1;
    return 0;
}

bool check_weights_sum(void) {
    long long total_sum = 0;
    for (int i = 0; i < letter_count; i++) {
        total_sum += unique_letters[i].weight * unique_letters[i].digit;
    }
    return total_sum == 0;
}

bool solve_backtracking(int index, int used_mask) {
    if (index == letter_count) {
        return check_weights_sum();
    }

    for (int d = 0; d <= 9; d++) {
        if (d == 0 && unique_letters[index].is_leading) {
            continue;
        }

        if (!(used_mask & (1 << d))) {
            unique_letters[index].digit = d;

            if (solve_backtracking(index + 1, used_mask | (1 << d))) {
                return true;
            }
        }
    }
    return false;
}

void print_result(const char* equation) {
    char result_str[256];
    int pos = 0;

    for (int i = 0; equation[i] != '\0'; i++) {
        if (isalpha((unsigned char)equation[i])) {
            for (int j = 0; j < letter_count; j++) {
                if (unique_letters[j].letter == equation[i]) {
                    result_str[pos++] = '0' + unique_letters[j].digit;
                    break;
                }
            }
        }
        else {
            result_str[pos++] = equation[i];
        }
    }
    result_str[pos] = '\0';
    printf("Result: %s\n", result_str);
}

void process_rebus(const char* equation) {
    printf("Input:  %s\n", equation);

    calculate_weights(equation);

    qsort(unique_letters, letter_count, sizeof(Letter), compare_letters);

    clock_t start_time = clock();

    if (solve_backtracking(0, 0)) {
        clock_t end_time = clock();
        double time_spent = (double)(end_time - start_time) / CLOCKS_PER_SEC;

        print_result(equation);
        printf("Execution time: %.6f seconds\n\n", time_spent);
    }
    else {
        printf("No solution found.\n\n");
    }
}

int main(void) {
    process_rebus("SEND + MORE = MONEY");
    process_rebus("BE + BE = MOO");
    process_rebus("BIG + CAT = LION");
    process_rebus("ELEVEN + NINE + FIVE + FIVE = THIRTY");

    return 0;
}

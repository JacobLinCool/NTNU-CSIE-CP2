// #define DEBUG
#include "hw0204.h"
#include "helper.h"
#include "./lib/structures/vector.h"
#include "fraction.h"
#define MAX_INPUT_LEN 4096

StructVector(OperandStack, sMixedNumber*, NULL);
StructVector(OperatorStack, char, '\0');

i32 main() {
    OperandStack* operands = create_OperandStack();
    OperatorStack* operators = create_OperatorStack();

    printf("Q: ");
    string input = (string)calloc(MAX_INPUT_LEN + 1, sizeof(char));
    scanf("%s", input);

    string head = input;
    while (true) {
        if (head[0] == '\0') {
            break;
        }
        if (head[0] == ' ') {
            head++;
            continue;
        }

        i64 end = 0;
        sMixedNumber* operand = parse_mixed(head, &end);
        if (end) {
            operands->push(operands, operand);

            if (operand->denominator == 0) {
                printf(CLR_RED "Error: Denominator is zero.\n" CLR_NONE);
                exit(EXIT_FAILURE);
            }

            DBG_PRINT("Parsed Operand: %s\n", mixed_to_string(operand));

            if (operators->size && strchr("*/", operators->back(operators))) {
                sMixedNumber* operand_b = operands->pop(operands);
                sMixedNumber* operand_a = operands->pop(operands);
                char operator = operators->pop(operators);

                sMixedNumber* result = (sMixedNumber*)calloc(1, sizeof(sMixedNumber));

                switch (operator) {
                case '*':
                    mixed_mul(result, *operand_a, *operand_b);
                    break;
                case '/':
                    mixed_div(result, *operand_a, *operand_b);
                    break;
                }

                free(operand_a);
                free(operand_b);

                DBG_PRINT("%s %c %s = %s\n", mixed_to_string(operand_a), operator, mixed_to_string(operand_b), mixed_to_string(result));
                operands->push(operands, result);
            }

            head += end;
        }
        else {
            free(operand);
        }

        if (head[0] == '\0') {
            break;
        }

        if (strchr("+-*/", head[0])) {
            DBG_PRINT("Parsed Operator: %c\n", head[0]);
            operators->push(operators, head[0]);
            head++;
        }
        else {
            printf(CLR_RED "Error: Invalid operator '%c' at position %ld.\n" CLR_NONE, head[0], head - input);
            exit(EXIT_FAILURE);
        }
    }

    DBG_PRINT("All Parsed.\n");

    while (operators->size) {
        sMixedNumber* operand_b = operands->pop(operands);
        sMixedNumber* operand_a = operands->pop(operands);
        char operator = operators->pop(operators);

        sMixedNumber* result = (sMixedNumber*)calloc(1, sizeof(sMixedNumber));

        switch (operator) {
        case '+':
            mixed_add(result, *operand_a, *operand_b);
            break;
        case '-':
            mixed_sub(result, *operand_a, *operand_b);
            break;
        }

        free(operand_a);
        free(operand_b);

        DBG_PRINT("%s %c %s = %s\n", mixed_to_string(operand_a), operator, mixed_to_string(operand_b), mixed_to_string(result));
        operands->push(operands, result);
    }

    sMixedNumber* result = operands->pop(operands);
    printf("A: %s\n", mixed_to_string(result));

    free(result);
    free(operands);
    free(operators);
    free(input);

    return 0;
}

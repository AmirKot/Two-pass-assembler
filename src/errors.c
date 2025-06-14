
#include "libary.h"

void print_error(int line,ERROR_ENUM err)
 {
    fprintf(stderr, "ERROR: line %d : ",line);
    switch (err) {
        case JMP_EXPECTED_PARENTHESIS:
            fprintf(stderr, "first character after the label must be parenthesis'(' .\n");
            break;
        case EXPECTED_SPACE_AFTER_LABEL:
            fprintf(stderr, "After the label \":\" there is no space \n");
            break;
        case EXPECTED_LABEL_AFTER_OPCODE:
            fprintf(stderr, "After the opcode label is expected \n");
            break;

        case LABEL_ALREADY_EXISTS:
            fprintf(stderr, "label already exists.\n");
            break;
        case LABEL_NAME_DOSENT_EXISTS_ENTRY:
            fprintf(stderr, ".entry directive must be followed by an existing label\n");
            break;
        case LABEL_OPERANDS_DOSENT_EXISTS:
            fprintf(stderr, "the label operand dosen't exist \n");
            break;
        case EXTRA_STRING_B4_STRING:
            fprintf(stderr, "There is extra string before \".string\" and it is not a label.\n");
            break;
        case EXTRA_STRING_B4_DATA:
            fprintf(stderr, "There is extra string before \".data\" and it is not a label.\n");
            break;
        case EXTRA_STRING_B4_EXTERN:
            fprintf(stderr, "There is extra string before \".extern\" and it is not a label.\n");
            break;
        case EXTRA_STRING_AFTER_OPERAND:
            fprintf(stderr, "There is extra string after the operand, it must only be white-space.\n");
            break;
        case EXTRA_STRING_AFTER_COMMAND:
            fprintf(stderr, "There is extra string after the comand, it must only be white-space.\n");
            break;
        case LABEL_TOO_LONG_MISSING:
            fprintf(stderr, "label is too long or label is missing\n");
            break;
        case LABEL_CANT_BE_SAVED_WORD:
            fprintf(stderr, "label can't have the same name as a assmbly saved word.\n");
            break;

        case DATA_DOUBLE_COMMAS_ROW:
            fprintf(stderr, "After .data too many commas in a row.\n");
            break;

        case DATA_EXPECTED_NUM:
            fprintf(stderr, ".data expected a numeric parameter.\n");
            break;
        case DATA_UNEXPECTED_DECIMAL_POINT:
            fprintf(stderr, " .data unexpected a decimal point.\n");
            break;
        case OPERANDS_UNEXPECTED_DECIMAL_POINT:
            fprintf(stderr, " operand unexpected a decimal point.\n");
            break;
       case OPERANDS_INVALID_NUM:
            fprintf(stderr, " operand illegal number detected .\n");
            break;
        
        case DATA_OUT_RANGE:
            fprintf(stderr, "The integer .data is out of range.\n");
            break;
        case OPERANDS_NUMBER_OUT_RANGE:
            fprintf(stderr, "The integer operand  is out of range.\n");
            break;
        case DATA_UNEXPECTED_COMMA:
            fprintf(stderr, ".data  expected an intiger after the comma (instated of the white-space) .\n");

            break;
        case DATA_START_COMMA:
            fprintf(stderr, ".data  expected an intiger at first instesd started with comma.\n");

            break;
        case STRING_OPERAND_NOT_VALID:
            fprintf(stderr, ".string operand is invalid.\n");
            break;
        case EXTERN_INVALID_LABEL:
            fprintf(stderr, ".extern directive received an invalid label.\n");

            break;

        case EXTERN_TOO_MANY_OPERANDS:
            fprintf(stderr, ".extern must only have one operand that is a label.\n");

            break;

        case COMMAND_NOT_FOUND:
            fprintf(stderr, "invalid command or directive.\n");

            break;
        case EXTRA_STRING_OPCODE:
            fprintf(stderr, "There is extra string before  the opcode.\n");
            break;
        case EXTRA_STRING_OPCODE_LABEL:
            fprintf(stderr, "There is extra string between the label and the opcode.\n");
            break;
        case STRING_B4_ENTRY:
            fprintf(stderr, "There is extra string before(not label) .entry can have only white-space or label.\n");

            break;
        case ENTRY_NO_LABEL:
            fprintf(stderr, ".entry directive must be followed by a label.\n");
            break;
        case EXTERN_NO_LABEL:
            fprintf(stderr, ".extern directive must be followed by a label.\n");
            break;
        case ENTRY_TOO_MANY_OPERANDS:
            fprintf(stderr, ".entry must only have one operand that is a label.\n");

            break;
        case EXPECTED_COMMA_BETWEEN_OPERANDS:
            fprintf(stderr, "command must have 2 operands with a comma between them.\n");

            break;
        case COMMAND_INVALID_OPERANDS_METHODS:
            fprintf(stderr, "operands' addressing methods do not match command requirements.\n");

            break;
        case COMMAND_INVALID_NUMBER_OF_OPERANDS:
            fprintf(stderr, "number of operands does not match command requirements.\n");

            break;

        case DIRECTIVE_NO_PARAMS:
            fprintf(stderr, "directive must have operands.\n");
            break;
        case OPERAND_CANT_B_NUMBER:
            fprintf(stderr, " operand can't be a number .\n");
            break;
        case COMMAND_INVALID_NUMBER_OF_PARAMTERS:
            fprintf(stderr, "number of paramters does not match command requirements.\n");

            break;
        case CANT_BE_WHITE_SPACE_IN_PARAMTERS:
            fprintf(stderr, "between the parenthesis must be only the paramters no white-space.\n");

            break;
        case LABEL_INVALID_FIRST_CHAR:
            fprintf(stderr, "label must start with an alphanumeric character.\n");
            break;
        case LABEL_ONLY_ALPHANUMERIC_DIGIT:
            fprintf(stderr, "label must only contain alphanumeric characters ane digits.\n");
            break;
         case STRING_TOO_MISSING_OPERANDS:
            fprintf(stderr, ".string missing operand.\n");
            break;
        case LABEL_CANT_BE_EXTERN:
            fprintf(stderr, "label already declared, can't apply  extern to a declare label .\n");
            break;
        case NO_ERROR:
            break;
        
    }
}

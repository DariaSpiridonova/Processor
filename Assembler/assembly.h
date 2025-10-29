#ifndef ASSEMBLY
#define ASSEMBLY

#include <stdio.h>
#include <stdlib.h>

#define RED_COLOR_BEGIN "\033[31m"
#define RED_COLOR_END   "\033[0m\n"

#define ASSERTS() \
    assert(commands_for_reading != NULL);\
    assert(commands_for_record != NULL);\
    assert(data_buffer != NULL);\
    

typedef int used_type;

const size_t SIZE_INPUT = 10;
const size_t SIZE_LABELS_BUFFER = 16;
const size_t NUM_OF_COMPILATIONS = 2;

enum CommandsNums
{
    PUSH,
    ADD,
    DIV,
    MUL,
    SUB,
    SQRT,
    OUT,
    HLT,
    IN,
    PUSHR = 33,
    POPR = 42,
    JB = 50,
    JA,
    JBE,
    JAE,
    JE,
    JNE,
    JMP,
    CALL,
    RET,
    COLON,
    COMMENT,
    PUSHM = 80,
    POPM,
    PRINT_RAM = 100,
    NON_EXISTING_COMMAND = -1
};

enum RegistersCommands
{
    AX,
    BX,
    CX,
    DX,
    SP,
    BP,
    SI,
    DI,
    NON_EXISTING_ARGUMENT
};

struct data_of_buffer
{
    size_t num_of_bytes_in_file;
    char *buffer_with_original_commands;
    char *byte_code_buffer;
    size_t num_of_strings_in_file;
};

struct asm_data
{
    FILE *commands_for_reading;
    FILE *commands_for_record;
    int res;
};

struct commands_for_change
{
    char command[SIZE_INPUT];
    ssize_t labels[SIZE_LABELS_BUFFER];
    char string_from_num[SIZE_INPUT];
    CommandsNums num_of_command;
    RegistersCommands reg_command;
    char reg_argument[SIZE_INPUT];
    int argument;
    ssize_t num_of_parameters;
    size_t num_of_strings;
    size_t number_of_compilation;
};

bool open_files_success(FILE *fp, size_t file_num);
bool close_files_success(FILE *commands_for_reading, FILE *commands_for_record);

bool identify_the_command(struct commands_for_change *data_buffer);
bool def_reg_arg(struct commands_for_change *data_buffer);
bool is_command(const char *received_command, const char *existing_command, bool *command_exist);
int get_argument(FILE *commands_for_reading, FILE *commands_for_record, struct commands_for_change *data_buffer);
int check_and_get_label_argument(FILE *commands_for_reading, FILE *commands_for_record, struct commands_for_change *data_buffer);
int check_and_get_reg(FILE *commands_for_reading, FILE *commands_for_record, struct commands_for_change *data_buffer);
int check_and_get_arg(FILE *commands_for_reading, FILE *commands_for_record, struct commands_for_change *data_buffer);
int check_and_get_label(struct commands_for_change *data_buffer);
int skip(FILE *commands_for_reading, struct commands_for_change *data_buffer);
int check_and_get_index(FILE *commands_for_reading, FILE *commands_for_record, struct commands_for_change *data_buffer);
void labels_init(struct commands_for_change *data_buffer);
int opening_files_and_initializing_data(FILE **commands_for_reading, FILE **commands_for_record, struct commands_for_change *data_buffer, size_t num_of_compilation);
int compile_file_content(FILE *commands_for_reading, FILE *commands_for_record, struct commands_for_change *data_buffer);
int read_command(FILE *commands_for_reading, struct commands_for_change *data_buffer);
int writing_to_a_file(FILE *commands_for_reading, FILE *commands_for_record, struct commands_for_change *data_buffer);
#endif

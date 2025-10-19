#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "assembly.h"

int main()
{
    struct commands_for_change data_buffer;

    labels_init(&data_buffer);

    for (size_t j = 1; j <= NUM_OF_COMPILATIONS; j++)
    {
        int res = 0;
        FILE *commands_for_reading = NULL;
        FILE *commands_for_record = NULL;

        if ((res = opening_files_and_initializing_data(&commands_for_reading, &commands_for_record, &data_buffer, j)))
        {
            return res;
        }
        if (commands_for_record == NULL) 
        {
            printf("hf");
            return 9;
        }
        fprintf(commands_for_record, "                                    \n");
        
        if ((res = compile_file_content(commands_for_reading, commands_for_record, &data_buffer)))
        {
            printf(RED_COLOR_BEGIN "in commands.asm: line %zu: An error occurred during compilation №%zu" RED_COLOR_END, data_buffer.num_of_strings, j);
            return res;
        }

        printf("STOP\n");
        fseek(commands_for_record, 0, SEEK_SET);
        fprintf(commands_for_record, "%zd", data_buffer.num_of_parameters++);
        data_buffer.number_of_compilation++;
        if (!close_files_success(commands_for_reading, commands_for_record))
        {
            return -2;
        }
    }
    return 0;
}
#include <string.h>
#include <stdio.h>
#include "commands.h"

command_t *commands = (command_t *) &__COMMANDS_START;

/* find a command with name 'command' and return the index */
int get_command_index(char *command) {
	unsigned int i;
	for (i = 0; i < NUM_COMMANDS; i++)
		if (!strncmp(command, commands[i].name, strlen(commands[i].name)))
			return i;
	return -1;
}

/* locate and attempt to execute a command given a tokenized array */
void exec_command(int argc, char **argv) {

	int command_ind;
	command_status result;

	/* search for a command with name argv[0] */
	command_ind = get_command_index(argv[0]);
	if (command_ind < 0) {
		printf("'%s' not a known command\r\n", argv[0]);
		return;
	}

	/* execute desired command */
	result = commands[command_ind].fp(argc, argv);
	if (result == FAIL)
		printf("%s failed\r\n", argv[0]);
	else if (result == USAGE)
		printf("%s usage: %s\r\n", argv[0], commands[command_ind].usage);
}

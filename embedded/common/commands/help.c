#include <stdio.h>
#include "commands.h"

command_status do_help(int argc, char *argv[]) {
	unsigned int i;
	int command_index;
	if (argc == 1)
		for (i = 0; i < NUM_COMMANDS; i++)
			printf("%s\t- %s\r\n", commands[i].name, commands[i].help);
	else {
		command_index = get_command_index(argv[1]);
		if (command_index >= 0) 
			printf("%s\r\nUsage: %s\r\n", commands[command_index].help,
					commands[command_index].usage);
		else
			printf("'%s' not a command.\r\n", argv[1]);
	}
	return SUCCESS;
}

COMMAND_ENTRY(
	"help",
	"help [command_name]",
	"Display a command's help message.",
	do_help
)

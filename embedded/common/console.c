#include <stdio.h>
#include "console.h"
#include "commands.h"
#include "usart.h"

/* return number of tokens found */
int tokenize(char **tok, int max_tokens, char *buf, int buflen) {

	int curr_token = 0, curr_char = 0;
	bool whitespace_flag = true;

	/* continue while we have characters left and space for tokens */
	while (curr_token < max_tokens && curr_char < buflen && buf[curr_char] != '\0') {

		/* previous has been whitespace, we have a new command */
		if (whitespace_flag && buf[curr_char] != ' ') {
			tok[curr_token++] = &buf[curr_char];
			whitespace_flag = false;
		}
		/* end of a command reached, write '\0' here to terminate */
		else if (!whitespace_flag && buf[curr_char] == ' ') {
			buf[curr_char] = '\0';
			whitespace_flag = true;
		}
		curr_char++;
	}

	/* if any unused token spaces remain, indicate this with NULL */
	if (curr_token != max_tokens)
		tok[curr_token] = NULL;

	return curr_token;
}

char *tokens[MAX_TOKENS];
void process_input(char *buf) {

	int num_tokens;

	/* tokenize the input (i.e. 'argv' style array) */
	num_tokens = tokenize(tokens, MAX_TOKENS, buf, USART_BUF);
	if (!num_tokens) return;

	exec_command(num_tokens, tokens);
}

char buffer[USART_BUF];
void check_input(PC_Buffer *rx) {

	/* only something to do if a message is waiting */
	if (pc_buffer_messageAvailable(rx)) {

		pc_buffer_getMessage(rx, buffer, USART_BUF);

		/* process the command string if it's not empty */
		if (buffer[0] != '\0') process_input(buffer);

		printPrompt();
	}
}

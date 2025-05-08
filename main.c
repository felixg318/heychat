#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include "handle_cli.h"

enum STATE { NO_PROMPT, PROMPT_IN_ARGS };

int main (int argc, char* argv[]) {
	int state;
	int c;

	if (argc < 2) 
		state = NO_PROMPT;
	else 
		state = PROMPT_IN_ARGS;

	while (1) {
		static struct option long_options[] = {
			{"key", 	required_argument, 0, 'k' },
			{"help", 	no_argument, 0, 'h' }
		};

		int option_index = 0;
		
		c = getopt_long(argc, argv, "hk:", long_options, &option_index);
	
		if (c == -1)
			break;

		switch (c) {
			case 'k':
				handle_key(optarg);
				exit(0);
			case 'h':
				printf("heychat requires an API key from ChatGPT\nUse --key to paste API Key\nMake sure there are no spaces!\n");
				printf("\nEnclose your prompt in quotes!\n");
				exit(0);
			case '?':	
			default:
				exit(0);
		}
	}

	if (argc > 2){
		printf("Too many args\n");
		exit(0);
	}

	switch (state){
		case NO_PROMPT:
			handle_no_prompt();
			break;
		case PROMPT_IN_ARGS:
			handle_prompt(argv[1]);
			break;	
	}
	exit(0);
}

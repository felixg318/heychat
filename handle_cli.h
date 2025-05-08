#ifndef HANDLE_CLI_H
#define HANDLE_CLI_H

#define MAX_LENGTH 1024
void parse_response_from_json(const char *);
void get_filepath(char*, size_t);
char* get_key();
void handle_key(const char*);
void handle_no_prompt();
void handle_prompt(const char*);
#endif

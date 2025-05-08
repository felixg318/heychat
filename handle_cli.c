#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cJSON.h"
#include "handle_cli.h"
#include "requests.h"

char buffer[MAX_LENGTH];

void parse_response_from_json(const char* response){
	cJSON* json = cJSON_Parse(response);
	if (!json) {
        printf("Error parsing JSON\n");
        exit(0);
    }

	cJSON* output = cJSON_GetObjectItemCaseSensitive(json,"output");
	if (!cJSON_IsArray(output)) {
        printf("Output is not an array\n");
        cJSON_Delete(json);
        exit(0);
    }

    cJSON *first_output_item = cJSON_GetArrayItem(output, 0);

	cJSON *content = cJSON_GetObjectItem(first_output_item, "content");
    if (!cJSON_IsArray(content)) {
        printf("Content is not an array\n");
        cJSON_Delete(json);
        exit(0);
    }

	cJSON *first_content_item = cJSON_GetArrayItem(content, 0);

	cJSON *text = cJSON_GetObjectItem(first_content_item, "text");
    if (cJSON_IsString(text)) {
        printf("\"%s\"\n", text->valuestring);
    }

	cJSON_Delete(json);
}

void get_filepath(char* buffer, size_t size){
	snprintf(buffer, size, "%s/.heychat", getenv("HOME"));
}

char* get_key(){
	get_filepath(buffer, sizeof(buffer));
	FILE *fptr = fopen(buffer, "r");
	if (fptr == NULL) {
		perror("Error opening file");
		return NULL;
	}
	char* key = malloc(MAX_LENGTH);
	
	if (fgets(key, MAX_LENGTH, fptr) == NULL) {
		perror("Error reading file");
		free(key);
		fclose(fptr);
		exit(1);
	}
	fclose(fptr);
	return key;
}


void handle_key(const char* key){
	get_filepath(buffer, sizeof(buffer));
	printf("Created file to store API Key in %s\n", buffer);

	FILE *fptr = fopen(buffer, "w");
	if (fptr == NULL)
		perror("Error opening file");
	else 
		fprintf(fptr,"%s", key);	

	fclose(fptr);	
}

void handle_no_prompt(){
	printf("Hi %s\n", getenv("USER"));
}

void handle_prompt(const char* prompt){
	char* response = curl_main(prompt);
	if (response == NULL) {
		free(response);
		perror("Error: No response from server.\n");
		exit(1);
	}
	parse_response_from_json(response);
	free(response);
}

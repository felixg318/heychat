#ifndef REQUESTS_H
#define REQUESTS_H

#include <curl/curl.h>
#include <stdlib.h>
#include <string.h>

#define MAX_PAYLOAD_LENGTH (1024 * 512)
#define MAX_RESPONSE_LENGTH (1024 * 512)

extern const char* OPENAI_URL;
extern const char* OPENAI_MODEL;

struct MemoryStruct {
    char *memory;
    size_t size;
};

size_t WriteMemoryCallback(void* contents, size_t size, size_t nmemb, void *userp);

char* curl_main(const char* input);
#endif 

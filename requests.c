#include "requests.h"
#include "handle_cli.h"

const char* OPENAI_URL = "https://api.openai.com/v1/responses";
const char* OPENAI_MODEL = "gpt-4.1";

size_t WriteMemoryCallback(void* contents, size_t size, size_t nmemb, void *userp) {
    size_t realsize = size * nmemb;
    struct MemoryStruct *mem = (struct MemoryStruct *)userp;

    char* ptr = (char *)realloc(mem->memory, mem->size + realsize + 1);
    if (!ptr) {
        printf("not enough memory (realloc returned NULL)\n");
        return 0;
    }

    mem->memory = ptr;
    memcpy(&(mem->memory[mem->size]), contents, realsize);
    mem->size += realsize;
    mem->memory[mem->size] = 0;

    return realsize;
}

char* curl_main(const char* input) {
    CURL *curl;
    CURLcode res;
    struct MemoryStruct chunk;

    char* response = malloc(MAX_RESPONSE_LENGTH);
    
    char* key = get_key();
    if (key == NULL) {
        free(key);
        perror("Error: API key not found.\n");
        exit(1);
    }

    char auth_header[MAX_LENGTH];
    snprintf(auth_header, sizeof(auth_header), "Authorization: Bearer %s", key);

    char payload[MAX_PAYLOAD_LENGTH];
    snprintf(payload, sizeof(payload), "{\"model\": \"%s\", \"store\": true, \"input\": \"%s\"}", OPENAI_MODEL, input);

    chunk.memory = malloc(1); 
    chunk.size = 0;
    curl_global_init(CURL_GLOBAL_ALL);
    curl = curl_easy_init();
    if (curl) {
        struct curl_slist *headers = NULL;
        headers = curl_slist_append(headers, "Content-Type: application/json");
        headers = curl_slist_append(headers, auth_header);


        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(curl, CURLOPT_URL, OPENAI_URL);
/*      curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L); */

        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&chunk);
        curl_easy_setopt(curl, CURLOPT_USERAGENT, "libcurl-agent/1.0");
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, payload);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, (long)strlen(payload));

        res = curl_easy_perform(curl);
        if (res != CURLE_OK)
            fprintf(stderr, "curl_easy_setopt() failed: %s\n", curl_easy_strerror(res));
        else {
            /*
            extract the response from chunk.memory
            and store it in response
            printf("%s\n",chunk.memory);
	    */
            snprintf(response, (chunk.size + 1), "%s", chunk.memory);
        }
        curl_easy_cleanup(curl);
        curl_slist_free_all(headers);
    }
    free(chunk.memory);
    curl_global_cleanup();
    free(key);

    return response;
}

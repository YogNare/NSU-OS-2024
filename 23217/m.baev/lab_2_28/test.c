#include <stdio.h>
#include <curl/curl.h>

int main() {
    CURL *curl;
    CURLcode res;
    char *url = "http://example.com";
    FILE *fp = NULL;

    curl_global_init(CURL_GLOBAL_ALL);
    curl = curl_easy_init();

    if(curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url);

        fp = fopen("output.html", "wb");
        if (!fp) {
            perror("fopen");
            curl_easy_cleanup(curl);
            curl_global_cleanup();
            return 1;
        }

        curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);

        res = curl_easy_perform(curl);

        if(res != CURLE_OK) {
            fprintf(stderr, "curl failed: %s\n", curl_easy_strerror(res));
        } else {
            printf("GET запрос успешно выполнен и ответ сохранен в output.html\n");
        }
        fclose(fp);

        curl_easy_cleanup(curl);
    } else {
        fprintf(stderr, "curl_easy_init() failed\n");
    }

    curl_global_cleanup();
    return 0;
}

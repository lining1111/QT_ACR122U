//
// Created by lining on 7/7/21.
//

#include <time.h>
#include <stdio.h>
#include <cjson/cJSON.h>
#include <stdlib.h>

//字符串转化时间戳
long metis_strptime(char *str_time) {
    struct tm *stm;
    time_t now;
    time(&now);
    stm = localtime(&now);
    strptime(str_time, "%Y-%m-%d %H:%M:%S", stm);
    long t = mktime(stm);
    return t;
}


int main(int argc, const char *argv[]) {
    char *string1 = "2021-07-07 14:42:11";
    long ltime = metis_strptime(string1);

    cJSON *root = cJSON_CreateObject();

//    cJSON_AddNumberToObject(root, "abc", metis_strptime(string1));
    cJSON_AddNumberToObject(root, "abc", (double )ltime);
    char *pjson = cJSON_PrintUnformatted(root);

    printf("json:%s\n", pjson);

    cJSON_Delete(root);
    return 0;
}
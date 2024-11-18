#include "car_co_ut.h"
#include "ut_def.h"
#include <stdio.h>
#include <time.h>
#include <stdint.h>

UtStats_t gUtStats = {0};

UtSuit_t utSuits[] = {
    {"gCarCoSuit", gCarCoSuit},
};

int utRun(void)
{
    for (int32_t suitIdx = 0; suitIdx < (int32_t)(sizeof(utSuits) / sizeof(utSuits[0])); suitIdx++) {
        printf("[UTSuit] begin: %s\n", utSuits[suitIdx].pName);
        int caseIdx = 0;
        while (utSuits[suitIdx].pUtCase[caseIdx].pUtTest != NULL)
        {
            printf("    Enter Case: %s\n", utSuits[suitIdx].pUtCase[caseIdx].pName);
            clock_t start_t = clock();
            utSuits[suitIdx].pUtCase[caseIdx].pUtTest();
            clock_t total_t = clock() - start_t;
            printf("    Exit Case : %s, %ldus\n", utSuits[suitIdx].pUtCase[caseIdx].pName, total_t);
            caseIdx++;
        }
        printf("[UTSuit] end  : %s\n\n", utSuits[suitIdx].pName);
    }
    printf("[UTStats] begin\n");
    printf("    |     total|      succ|      fail|\n");
    printf("    |%10d|%10d|%10d|\n", gUtStats.total, gUtStats.succ, gUtStats.fail);
    printf("[UTStats] end\n");
    printf("\n");

    return 0;
}

int main(void)
{
    int rc;
    rc = utRun();
    if (rc != 0)
    {
        printf("Ut run failed, rc: %d\n", rc);
    }
    return 0;
}

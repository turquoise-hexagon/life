#include <err.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "utils.h"

static const short RULES[][17] = {
    {-1, -1,  1, -1, -1, -1, 0, 0, 0, 0, 0, 1, -1, -1, -1, 1, 1},
    {-1, -1, -1, -1, -1, -1, 0, 0, 0, 0, 0, 1,  1,  1,  1, 1, 1},
    {-1, -1,  1,  1,  1, -1, 0, 0, 0, 0, 0, 1,  1,  1, -1, 1, 1}
};

static const char *COLORS[] = {
    "237 238 240",
    "016 032 041",
    "113 170 197"
};

int
main(int argc, char **argv)
{
    /* argument parsing */
    if (argc != 5)
        usage("[height] [width] [init] [iter]", argv[0]);

    unsigned *args = argstous(argc, argv);

    /* init array */
    short ***uni = malloc(args[0] * sizeof *uni);

    if (uni == NULL)
        errx(1, "program failed to allocate memory");

    for (unsigned i = 0; i < args[0]; ++i) {
        uni[i] = malloc(args[1] * sizeof *uni[i]);

        if (uni[i] == NULL)
            errx(1, "program failed to allocate memory");

        for (unsigned j = 0; j < args[1]; ++j) {
            uni[i][j] = calloc(2, sizeof *uni[i][j]);

            if (uni[i][j] == NULL)
                errx(1, "program failed to allocate memory");
        }
    }

    bool flag = 0;
    unsigned x, y;
    srand(time(NULL));

    for (unsigned i = 0; i < args[2]; ++i) {
        x = rand() % args[0];
        y = rand() % args[1];

        uni[x][y][flag] = rand() % 2 == 0 ? -1 : 1;
    }

    /* run pinwheels */
    short cnt;

    for (unsigned n = 0; n < args[3]; ++n) {
        printf("P3\n%u %u\n255\n", args[1], args[0]);

        for (unsigned i = 0; i < args[0]; ++i)
            for (unsigned j = 0; j < args[1]; ++j) {
                puts(COLORS[uni[i][j][flag] + 1]);

                cnt = 0;

                for (short u = -1; u < 2; ++u)
                    for (short v = -1; v < 2; ++v)
                        if (u != 0 || v != 0) {
                            x = ((long)i + u + args[0]) % args[0];
                            y = ((long)j + v + args[1]) % args[1];

                            cnt += uni[x][y][flag];
                        }

                uni[i][j][!flag] = RULES[uni[i][j][flag] + 1][cnt + 8];
            }

        flag ^= 1;
    }

    /* cleanup */
    for (unsigned i = 0; i < args[0]; ++i) {
        for (unsigned j = 0; j < args[1]; ++j)
            free(uni[i][j]);

        free(uni[i]);
    }

    free(args);
    free(uni);

    return 0;
}

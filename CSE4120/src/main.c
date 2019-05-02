#include "globals.h"
#include "util.h"

/* allocate global variables */
int lineno = 0;
FILE *source;
FILE *listing;
FILE *code;

/* allocate and set tracing flags */
int TraceScan = TRUE;

TokenType getToken(void);

int main(int argc, char *argv[])
{
        /* source code file name */
        char pgm[120];
        if (argc != 2) {
                fprintf(stderr, "usage: %s <filename>\n", argv[0]);
                exit(1);
        }
        strcpy(pgm, argv[1]);
        if (strchr(pgm, '.') == NULL)
                strcat(pgm, ".tny");
        source = fopen(pgm, "r");
        if (source == NULL) {
                fprintf(stderr, "File %s not found\n", pgm);
                exit(1);
        }
        /* send listing to screen */
        listing = stdout;
        fprintf(listing, "   line number\t\ttoken\t\tlexeme\n");
        fprintf(listing, 
                "----------------------------------------------------\n");
        while (getToken() != ENDFILE);
        fclose(source);
        return 0;
}

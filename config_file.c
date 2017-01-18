#include "config_file.h"
#include "stdio.h"

int shock_parse_config(shock_config_t* conf, char* filename)
{
    FILE *fp = fopen(filename, "r");
    if (!fp){
        printf("-------------------------------------------\n");
        printf("ERROR: Cannot open config file for reading!\n");
        printf("-------------------------------------------\n");
        return -1;
    }

    size_t len = 0;
    int read = 0;
    char line[256];
    int lineNum = 1;

    while (fgets (line, sizeof(line), fp ) != NULL ) {
        //Okay, so let's parse

        //If line is comment, skip it
        if (line[0] == '#' || line[0] == '\n') {
            lineNum++;
            continue;
        }


        size_t lineSize = strlen(line);
        shock_config_keypair_t entry;
        char token[512] = "";
        int phase = 0;
        for (int i=0;i<lineSize;i++) {
            //Skip space
            if (line[i] == ' ') {
                continue;
            }

            //If it is line ending, then let's set entry value
            if (line[i] == '\n') {
                if (phase == 0) {
                    printf("Config Error: unexpected ending of line %d\n", lineNum);
                    break;
                }
                strcpy(entry.val, token);
                break;
            }

            //Update token
            if (line[i] != ':'){
                token[strlen(token)] = line[i];
                token[strlen(token)+1] = '\0';
                continue;
            }

            if (phase == 1) {
                printf("Config Warning: unexpected colon in value on line %d\n", lineNum);
            }

            //Set entry name and reset token (strcpy doesn't work)
            strcpy(entry.name, token);
            memset(token, 0, sizeof(token));
            phase = 1;
            continue;
        }

        lineNum++;
    }

    return 0;
}

void shock_default_config(shock_config_t* conf)
{
    conf->port = SHOCK_DEFAULT_PORT;
    conf->clearLogs = SHOCK_DEAFULT_CLEARLOGS;
    strcpy(conf->root, SHOCK_DEFAULT_ROOT);
}

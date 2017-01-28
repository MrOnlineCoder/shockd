#include "config_file.h"
#include "stdio.h"
#include "stdlib.h"

#include "log.h"

int shock_parse_config(shock_config_t* conf, char* filename)
{
    FILE *fp = fopen(filename, "r");
    if (!fp){
        shock_log(SHOCK_ERROR, "Cannot open config file for reading!");
        printf("-------------------------------------------\n");
        printf("[Error] Cannot open config file for reading!\n");
        printf("-------------------------------------------\n");
        return -1;
    }

    size_t len = 0;
    int read = 0;
    char line[512];
    int lineNum = 1;

    while (fgets (line, sizeof(line), fp ) != NULL ) {
        //Okay, so let's parse

        //If line is comment or empty line, skip it
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
                    shock_log(SHOCK_ERROR, "Unexpected ending of line %d in config!", lineNum);
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
                shock_log(SHOCK_ERROR, "Unexpected colon in value on line %d in config!", lineNum);
                printf("Config Warning: unexpected colon in value on line %d\n", lineNum);
            }

            //Set entry name and reset token (strcpy doesn't work)
            strcpy(entry.name, token);
            memset(token, 0, sizeof(token));
            phase = 1;
            continue;
        }

        lineNum++;
        shock_parse_config_token(conf, entry);
    }

    return 0;
}

void shock_default_config(shock_config_t* conf)
{
    conf->port = SHOCK_DEFAULT_PORT;
    conf->clearLogs = SHOCK_DEAFULT_CLEARLOGS;
    strcpy(conf->root, SHOCK_DEFAULT_ROOT);
    strcpy(conf->logFile, SHOCK_DEFAULT_LOG);
}


int shock_parse_config_token(shock_config_t* conf, shock_config_keypair_t entry)
{
    // OK, so you noticed this bad hardcoded IFs
    // But sorry so :)

    if (strcasecmp(entry.name, "Port") == 0) {
        conf->port = strtol(entry.val, NULL, 10);
        if (conf->port == 0) {
            conf->port = SHOCK_DEFAULT_PORT;
            shock_log(SHOCK_ERROR, "Invalid value for %s option in config!", entry.name);
            printf("Config Error: Invalid value for %s option. \n", entry.name);
        }
        return;
    }

    if (strcasecmp(entry.name, "ServerRoot") == 0) {
        strncpy(conf->root, entry.val, sizeof(conf->root));
        return;
    }

    if (strcasecmp(entry.name, "ClearLogs") == 0) {
        if(entry.val[0] == '1') {
            conf->clearLogs = 1;
        } else if(entry.val[0] == '0') {
            conf->clearLogs = 0;
        } else {
            printf("Config Error: Invalid value for %s option. \n", entry.name);
            shock_log(SHOCK_ERROR, "Invalid value for %s option in config!", entry.name);
        }
        return;
    }

    if (strcasecmp(entry.name, "ETagCache") == 0) {
        if(entry.val[0] == '1') {
            conf->etagCache = 1;
        } else if(entry.val[0] == '0') {
            conf->etagCache = 0;
        } else {
            printf("Config Error: Invalid value for %s option. \n", entry.name);
            shock_log(SHOCK_ERROR, "Invalid value for %s option in config!", entry.name);
        }
        return;
    }

    if (strcasecmp(entry.name, "LogFile") == 0) {
        strncpy(conf->logFile, entry.val, sizeof(conf->logFile));
        return;
    }

    shock_log(SHOCK_ERROR, "Unknown config option: %s", entry.name);
    printf("Config Error: Unknown option %s\n", entry.name);
}

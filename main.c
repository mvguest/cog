#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef __ANDROID__
    #define LUA_DISABLE
#else
    #define LUA_ENABLE
#endif

#ifdef LUA_ENABLE
    #include <lua.h>
    #include <lualib.h>
    #include <lauxlib.h>
#endif

#define MAX_LINES 1000
#define MAX_LEN 256

char *buffer[MAX_LINES];
int num_lines = 0;
int current_line = 0;
FILE *output_file = NULL;
char filename[256] = "foo._";

void append() {
    char line[MAX_LEN];
    while (1) {
        if (fgets(line, sizeof(line), stdin) == NULL) break;
        if (strcmp(line, ".\n") == 0) break;

        if (num_lines >= MAX_LINES) {
            printf("Buffer full.\n");
            return;
        }

        buffer[num_lines] = strdup(line);
        num_lines++;
    }
    current_line = num_lines - 1;
}

void modify() {
    if (current_line < 0 || current_line >= num_lines) {
        printf("Invalid current line.\n");
        return;
    }

    char input[MAX_LEN];
    if (fgets(input, MAX_LEN, stdin) != NULL) {
        input[strcspn(input, "\n")] = '\0'; 
        free(buffer[current_line]);

        char formatted[MAX_LEN];
        snprintf(formatted, sizeof(formatted), "%s\n", input);

        buffer[current_line] = strdup(formatted);
        printf("Line %d modified.\n", current_line + 1);
    }
}

void print_current() {
    if (current_line < 0 || current_line >= num_lines) {
        printf("?\n");
        return;
    }
    printf("%s", buffer[current_line]);
}

void numbered_print_current() {
    if (current_line < 0 || current_line >= num_lines) {
        printf("?\n");
        return;
    }
    printf("\e[0;33m%d\e[0m\t%s", current_line + 1, buffer[current_line]);
}

void print_all() {
    for (int i = 0; i < num_lines; i++) {
        if (i == current_line)
            printf("* %s", buffer[i]);
        else
            printf("  %s", buffer[i]);
    }
}

void numbered_print_all() {
    for (int i = 0; i < num_lines; i++) {
        if (i == current_line)
            printf("\e[1;36m*\e[0m \e[0;33m%d\e[0m\t%s", i + 1, buffer[i]);
        else
            printf("  \e[0;33m%d\e[0m\t%s", i + 1, buffer[i]);
    }
}

void delete_current() {
    if (current_line < 0 || current_line >= num_lines) {
        printf("?\n");
        return;
    }
    free(buffer[current_line]);
    for (int i = current_line; i < num_lines - 1; i++) {
        buffer[i] = buffer[i + 1];
    }
    num_lines--;
    if (current_line >= num_lines) {
        current_line = num_lines - 1;
    }
}

void os_clear(const char *os) {
    if (strcmp(os, "Windows") == 0) {
        printf("\e[1;1H\e[2J");
    } else {
        system("clear");
    }
}

int main(int argc, char *argv[]) {
    #ifdef LUA_ENABLE
        lua_State *L = luaL_newstate(); 
        luaL_openlibs(L); 

        const char *lua_os_path = "/usr/local/share/cog-lua/os_detec.lua";
        if (luaL_dofile(L, lua_os_path) != LUA_OK) {
            fprintf(stderr, "[os_detec.lua] - Error loading Lua script: %s\n", lua_tostring(L, -1));
            lua_close(L);
            return 1;
        } 

        lua_getglobal(L, "os_detec");
        if (lua_pcall(L, 0, 1, 0) != LUA_OK) {
            fprintf(stderr, "[os_detec.lua] - Error calling os_detec(): %s\n", lua_tostring(L, -1));
            lua_close(L);
            return 1;
        }

        const char *lua_update_path = "/usr/local/share/cog-lua/update.lua"; 
        if (luaL_dofile(L, lua_update_path) != LUA_OK) {
            fprintf(stderr, "[update.lua] - Error loading Lua script: %s\n", lua_tostring(L, -1));
            lua_pop(L, 1);
            lua_close(L);
            return 1;
        }

        lua_getglobal(L, "update");
        if (lua_pcall(L, 0, 1, 0) != LUA_OK) {
            fprintf(stderr, "[update.lua] - Error calling update(): %s\n", lua_tostring(L, -1));
            lua_pop(L, 1);
            lua_close(L);
            return 1;
        }
        
        const char *os_name = lua_tostring(L, -1);
        char *c_os = strdup(os_name);   
        
        lua_pop(L, 1);
        lua_close(L); 

        if (strcmp(c_os, "NUPDT") == 0) {
            char *yn = malloc(sizeof(char));
            printf("[ ! ] - Your cog editor is not updated to the latest version\n\n");
            printf("Do you want to update cog editor to the latest version avaliable? (y/n): ");
            scanf("%c", yn);
            free(yn);
        } 

        os_clear(c_os);
    #else
        system("clear");
    #endif 

    if (argc > 1) {
        strncpy(filename, argv[1], sizeof(filename) - 1);
        filename[sizeof(filename) - 1] = '\0';
    }

    output_file = fopen(filename, "r");
    if (!output_file) {
        output_file = fopen(filename, "w");
        if (!output_file) {
            perror("Error opening file");
            return 1;
        }
    }

    char line[MAX_LEN];
    rewind(output_file);
    while (fgets(line, sizeof(line), output_file) != NULL) {
        if (num_lines >= MAX_LINES) break;
        buffer[num_lines++] = strdup(line);
    }
    rewind(output_file);

    if (num_lines > 0) {
        current_line = num_lines - 1;
    }

    printf("cog 0.6 - simple line editor\n");
    printf("Licensed under the MIT License\n");
    printf("Opened file: %s\n\n", filename);

    char cmd[512]; 

    while (1) {
        printf(": ");
        if (fgets(cmd, sizeof(cmd), stdin) == NULL) break;

        size_t len = strlen(cmd);
        if (len > 0 && cmd[len - 1] == '\n') {
            cmd[len - 1] = '\0';
        }

        if (strncmp(cmd, "bsh$", 4) == 0) {
            char *bash_cmd = cmd + 4;
            if (strlen(bash_cmd) > 0) {
                int result = system(bash_cmd);
                printf("\033[90m[exit code: %d]\033[0m\n", result);
            } else {
                printf("No shell command provided.\n");
            }
            continue;
        }

        if (strcmp(cmd, "clear") == 0) {
            #ifdef LUA_ENABLE
                os_clear(c_os);
            #else
                system("clear");
            #endif
            continue;
        }

        if (strncmp(cmd, "l.", 2) == 0) {
            int line = atoi(cmd + 2) - 1;
            if (line >= 0 && line < num_lines) {
                current_line = line;
                printf("Moved to line %d\n", line + 1);
            } else {
                printf("Invalid line number.\n");
            }
            continue;
        }

        switch (cmd[0]) {
            case 'a':
                append();
                break;
            case 'm':
                modify();
                break;
            case 'p':
                print_current();
                break;
            case 'n':
                numbered_print_current();
                break;
            case 'P':
                print_all();
                break;
            case 'N':
                numbered_print_all();
                break;
            case 'd':
                delete_current();
                break;
            case 'q':
                goto end;
            case 'w': {
                FILE *f = fopen(filename, "w");
                if (!f) {
                    perror("Failed to save file");
                } else {
                    for (int i = 0; i < num_lines; i++) {
                        if (buffer[i]) {
                            fputs(buffer[i], f);
                        }
                    }
                    fclose(f);
                    printf("Buffer saved to '%s'\n", filename);
                }
                break;
            }
            case '\0':
                if (current_line < num_lines - 1) current_line++;
                break;
            case '-':
                if (current_line < num_lines - 1) current_line++;
                else printf("At end of buffer.\n");
                break;
            case '+':
                if (current_line > 0) current_line--;
                else printf("At start of buffer.\n");
                break;
            default:
                printf("Unknown command.\n");
        }
    }

end:
    if (output_file) {
        fclose(output_file);
        output_file = NULL;
    }

    for (int i = 0; i < num_lines; i++) {
        free(buffer[i]);
    }
    
    #ifdef LUA_ENABLE
        os_clear(c_os);
        free(c_os);
    #else
        system("clear");
    #endif 

    return 0;
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINES 1000
#define MAX_LEN 256

char *buffer[MAX_LINES];
int num_lines = 0;
int current_line = 0;
FILE *output_file = NULL;
char filename[256] = "foo._";
char version[32] = "unknown";

void load_version(const char *json_file) {
    FILE *f = fopen(json_file, "r");
    if (!f) return;

    char line[256];
    while (fgets(line, sizeof(line), f)) {
        if (strstr(line, "\"version\"")) {
            char *start = strchr(line, ':');
            if (start) {
                start += 1;
                while (*start == ' ' || *start == '\"') start++;
                char *end = strchr(start, '\"');
                if (end) *end = '\0';
                strncpy(version, start, sizeof(version) - 1);
                version[sizeof(version) - 1] = '\0';
                break;
            }
        }
    }

    fclose(f);
}

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
        if (output_file) {
            fputs(line, output_file);
        }
        num_lines++;
    }
    current_line = num_lines - 1;
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

int main(int argc, char *argv[]) {
    system("clear");
    load_version("manifest.json");

    if (argc > 1) {
        strncpy(filename, argv[1], sizeof(filename) - 1);
        filename[sizeof(filename) - 1] = '\0';
    }

    output_file = fopen(filename, "r+");
    if (!output_file) {
        output_file = fopen(filename, "w+");
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

    printf("cog %s - simple line editor\n", version);
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
                printf("[exit code: %d]\n", result);
            } else {
                printf("No shell command provided.\n");
            }
            continue;
        }

        if (strcmp(cmd, "clear") == 0) {
            system("clear");
            continue;
        }

        switch (cmd[0]) {
            case 'a':
                fseek(output_file, 0, SEEK_END);
                append();
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
    }

    for (int i = 0; i < num_lines; i++) {
        free(buffer[i]);
    }

    return 0;
}

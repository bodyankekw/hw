#include <ncurses.h>
#include <dirent.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>

#define MAX_FILES 1024
#define MIN_WIDTH 107
#define MIN_HEIGHT 24

typedef struct File {
    char name[256];
    off_t size;
    int is_dir;
} File;

typedef struct Panel {
    File files[MAX_FILES];
    int file_count;
    int current_file;
    int scroll_offset;
    char current_path[1024];
} Panel;

Panel panels[2];
int active_panel = 0;

void list_files(Panel *panel, const char *path) {
    DIR *dir;
    struct dirent *entry;
    struct stat file_stat;

    dir = opendir(path);
    if (!dir) {
        perror("opendir");
        return;
    }

    panel->file_count = 0;
    while ((entry = readdir(dir)) != NULL) {
        snprintf(panel->files[panel->file_count].name, sizeof(panel->files[panel->file_count].name), "%s",
                 entry->d_name);

        char full_path[1024];
        snprintf(full_path, sizeof(full_path), "%s/%s", path, entry->d_name);

        if (stat(full_path, &file_stat) == 0) {
            panel->files[panel->file_count].size = file_stat.st_size;
            panel->files[panel->file_count].is_dir = S_ISDIR(file_stat.st_mode);
        } else {
            panel->files[panel->file_count].size = 0;
            panel->files[panel->file_count].is_dir = 0;
        }

        panel->file_count++;
    }
    closedir(dir);
}

void display_panel(int panel_index, int start_x, int start_y, int width, int height) {
    Panel *panel = &panels[panel_index];

    if (panel_index == active_panel) {
        attron(A_BOLD);
    }
    mvprintw(start_y - 1, start_x, "Name");
    mvprintw(start_y - 1, start_x + width - 10, "Size");
    if (panel_index == active_panel) {
        attroff(A_BOLD);
    }

    for (int i = 0; i < height; i++) {
        int file_index = i + panel->scroll_offset;
        if (file_index < panel->file_count) {
            if (file_index == panel->current_file && panel_index == active_panel) {
                attron(A_REVERSE);
            }
            if (panel->files[file_index].is_dir) {
                mvprintw(start_y + i, start_x, "%-40s      <DIR>", panel->files[file_index].name);
            } else {
                mvprintw(start_y + i, start_x, "%-40s %10ld", panel->files[file_index].name,
                         panel->files[file_index].size);
            }
            if (file_index == panel->current_file && panel_index == active_panel) {
                attroff(A_REVERSE);
            }
        } else {
            mvprintw(start_y + i, start_x, "%s", " ");
        }
    }
}

void handle_navigation(Panel *panel, int ch, int height) {
    switch (ch) {
        case KEY_UP:
            if (panel->current_file > 0) {
                panel->current_file--;
                if (panel->current_file < panel->scroll_offset) {
                    panel->scroll_offset--;
                }
            }
            break;
        case KEY_DOWN:
            if (panel->current_file < panel->file_count - 1) {
                panel->current_file++;
                if (panel->current_file >= panel->scroll_offset + height) {
                    panel->scroll_offset++;
                }
            }
            break;
        case '\n':
            if (panel->files[panel->current_file].is_dir) {
                if (strcmp(panel->files[panel->current_file].name, "..") == 0) {
                    chdir("..");
                } else {
                    chdir(panel->files[panel->current_file].name);
                }
                getcwd(panel->current_path, sizeof(panel->current_path));
                list_files(panel, panel->current_path);
                panel->current_file = 0;
                panel->scroll_offset = 0;
            }
            break;
    }
}

void check_terminal_size() {
    if (COLS < MIN_WIDTH || LINES < MIN_HEIGHT) {
        endwin();
        fprintf(stderr, "Terminal size must be at least %d x %d\n", MIN_WIDTH, MIN_HEIGHT);
        exit(1);
    }
}

int main() {
    initscr();
    noecho();
    cbreak();
    keypad(stdscr, TRUE);
    curs_set(0);

    check_terminal_size();

    int width = COLS / 2 - 2;
    int height = LINES - 3;
    getcwd(panels[0].current_path, sizeof(panels[0].current_path));
    getcwd(panels[1].current_path, sizeof(panels[1].current_path));
    list_files(&panels[0], panels[0].current_path);
    list_files(&panels[1], panels[1].current_path);

    while (1) {
        clear();
        box(stdscr, 0, 0);

        if (active_panel == 0) {
            mvprintw(0, COLS / 4, "Window 1");
            attron(A_BOLD);
            mvprintw(0, 3 * COLS / 4, "Window 2");
            attroff(A_BOLD);
        } else {
            attron(A_BOLD);
            mvprintw(0, COLS / 4, "Window 1");
            attroff(A_BOLD);
            mvprintw(0, 3 * COLS / 4, "Window 2");
        }

        display_panel(0, 1, 2, width, height);
        display_panel(1, COLS / 2 + 2, 2, width, height);

        for (int i = 1; i < LINES - 1; i++) {
            mvprintw(i, COLS / 2, "|");
        }

        int ch = getch();
        switch (ch) {
            case KEY_UP:
            case KEY_DOWN:
            case '\n':
                handle_navigation(&panels[active_panel], ch, height);
                break;
            case '\t':
                active_panel = 1 - active_panel;
                break;
            case 'q':
                endwin();
                exit(0);
        }
    }

    endwin();
    return 0;
}

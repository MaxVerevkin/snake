#include <ncurses.h>
#include <unistd.h>
#include <time.h>
#include <locale.h>
#include <stdlib.h>

// Delay between frames
// [µs]
#define FRAME_DELAY 10000 * 10

// Snake linked list
typedef struct snake_list{
    int x, y;
    struct snake_list *head
} snake_list;

// Console size
int HEIGHT, WIDTH;

int main(int argc, char **argv) {
    setlocale (LC_ALL, "");

    // Random setup
    srand(time(NULL));

    // Setup
    initscr();  // Init
    cbreak();   // Char mode
    noecho();   // No echo
    timeout(0); // Use non blocking mode
    keypad(stdscr, TRUE); // Catch special chars
    curs_set(0); // No blinking
    getmaxyx(stdscr, HEIGHT, WIDTH); // Get window size

    // Gave vars
    snake_list snake; // The actual snake
    snake.x = 0;
    snake.y = 0;
    snake.head = NULL;
    snake_list *snake_head = &snake;
    snake_list *_snake; // Temp pointer
    int x_dir = 0;
    int y_dir = 0;
    int food_x = 2;
    int food_y = 0;

    // Game loop
    while (1) {

        /* User input */

        switch (getch()) {
        case 'q':
            clear();
            curs_set(1);
            refresh();
            return 0;
        case KEY_UP:
            if (y_dir != 1) {
                x_dir = 0;
                y_dir = -1;
            } break;
        case KEY_RIGHT:
            if (x_dir != -1) {
                x_dir = 1;
                y_dir = 0;
            } break;
        case KEY_DOWN:
            if (y_dir != -1) {
                x_dir = 0;
                y_dir = 1;
            } break;
        case KEY_LEFT:
            if (x_dir != 1) {
                x_dir = -1;
                y_dir = 0;
            } break;
        }

        /* Game logic */

        // Find new head position
        int new_head_x = snake_head->x + x_dir;
        int new_head_y = snake_head->y + y_dir;
        if (new_head_x < 0)
            new_head_x = WIDTH - 1;
        else if (new_head_x >= WIDTH)
            new_head_x = 0;
        if (new_head_y < 0)
            new_head_y = HEIGHT - 1;
        else if (new_head_y >= HEIGHT)
            new_head_y = 0;

        // Eat food!
        if (new_head_x == food_x && new_head_y == food_y) {
            _snake = (snake_list*) malloc(sizeof(snake_list));
            if (!_snake) { // Allcation error
                curs_set(1);
                return 1;
            }
            _snake->x = new_head_x;
            _snake->y = new_head_y;
            _snake->head = NULL;
            snake_head->head = _snake;
            snake_head = _snake;

            food_x = rand() % WIDTH;
            food_y = rand() % HEIGHT;
        }
        // Just move
        else {
            _snake = &snake;
            while (_snake) {
                snake_list *head = _snake->head;
                if (head) {
                    _snake->x = head->x;
                    _snake->y = head->y;
                } else {
                    _snake->x = new_head_x;
                    _snake->y = new_head_y;
                }
                _snake = head;
            }
        }

        /* Drawing */

        clear();

        _snake = &snake;
        while (_snake) {
            // Game over
            if (_snake->x == snake_head->x && _snake->y == snake_head->y && _snake != snake_head) {
                clear();
                curs_set(1);
                refresh();
                return 0;
            }

            move(_snake->y, _snake->x);
            addch(_snake->head ? '*' : '@');
            _snake = _snake->head;
        }
        move(food_y, food_x);
        addch('#');

        refresh();
        usleep(FRAME_DELAY);
    }

    clear();
    curs_set(1);
    refresh();
    return 1;
}

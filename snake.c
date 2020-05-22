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
        if (snake_head->x + x_dir == food_x && snake_head->y + y_dir == food_y) {
            _snake = (snake_list*) malloc(sizeof(snake_list));
            if (!_snake) { // Allcation error
                curs_set(1);
                return 1;
            }
            _snake->x = snake_head->x + x_dir;
            _snake->y = snake_head->y + y_dir;
            _snake->head = NULL;
            snake_head->head = _snake;
            snake_head = _snake;

            food_x = rand() % WIDTH;
            food_y = rand() % HEIGHT;
        } else {
            _snake = &snake;
            while (_snake) {
                snake_list *head = _snake->head;
                if (head) {
                    _snake->x = head->x;
                    _snake->y = head->y;
                } else {
                    _snake->x += x_dir;
                    _snake->y += y_dir;
                }
                _snake = head;
            }
        }
        if (snake_head->x < 0)
            snake_head->x = WIDTH - 1;
        else if (snake_head->x >= WIDTH)
            snake_head->x = 0;
        if (snake_head->y < 0)
            snake_head->y = HEIGHT - 1;
        else if (snake_head->y >= HEIGHT)
            snake_head->y = 0;

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

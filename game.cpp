#include <iostream>
#include <SFML/Graphics.hpp>
#include <ctime>
#include <cstdlib>
#include <termios.h>
#include <ncurses.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/wait.h>

using namespace std;

const int WIDTH = 800;
const int HEIGHT = 600;
const int GRID_SIZE = 30;
const int GRID_ROWS = HEIGHT / GRID_SIZE;
const int GRID_COLS = WIDTH / GRID_SIZE;


//Board global var
int board_size;
char** board_grid; 
const int NO_PLAYER = 2;
const int NO_ITEMS = 15;
const char ITEM = '*';
const char EMPTY = '-';

//Player key global var
//Player 1
const int I = 73;
const int i = 105;
const int K = 75;
const int k = 107;
const int J = 74;
const int j = 106;
const int L = 76;
const int l = 108; 

//Player 2
const int W = 85;
const int w = 119;
const int S = 83;
const int s = 115;
const int A = 65;
const int a = 97;
const int D = 68;
const int d = 100; 

struct Player
{
    int pos_x, pos_y;
    int p_score;
};

Player players[NO_PLAYER];

pthread_t player_1;
pthread_t player_2;

//Function to generate random number 
int random_number()
{
    srand (time(NULL));
    int random_num = rand() % 99;
    if (random_num < 10)
    {
        random_num += 10;
    }
    return random_num;
}

void init_game_board()
{
    //Creating Multiplayer Game Board
    int random_num = random_number();
    int roll_no = 2502;
    int last_digit = roll_no % 10;
    int mult_num = random_num * last_digit;
    int div_num = roll_no / mult_num;
    int mod_num = div_num % 25;

    if (mod_num < 10)
    {
        mod_num += 15;
    } 

    board_size =  mod_num;

    srand(time(NULL));

    cout << "Random number generated: " << random_number() << endl;
    cout << "Roll No: " << roll_no << endl;
    cout << "Board size: "<< board_size << endl;
     
    board_grid = new char*[board_size];

    for (int i = 0; i < board_size; i++)
    {
        board_grid[i] = new char[board_size];
    } 

    //srand(time(NULL));

    for (int i = 0; i < board_size; i++)
    {
        for (int j = 0; j < board_size; j++)
        {
            board_grid[i][j] = EMPTY;
        }
    }

    for (int i = 0; i < NO_PLAYER; i++)
    {
        players[i].pos_x = rand() % board_size;
        players[i].pos_y =  rand() % board_size;
        players[i].p_score = 0;
    }

    for (int i = 0; i < NO_ITEMS; i++)
    {
        int x = rand() % board_size;
        int y = rand() % board_size;

        board_grid[x][y] = ITEM;
    }
}

void print_board() 
{
    system("clear");

    cout << "\033[1;36m|-----MULTIPLAYER GAME BOARD-----|\033[0m" << endl;
    cout << "\033[1;31m| Player 1 (\033[1;31mX\033[1;31m)          Score: " << players[0].p_score << " |\033[0m" << endl;
    cout << "\033[1;32m| Player 2 (\033[1;32mO\033[1;32m)          Score: " << players[1].p_score << " |\033[0m" << endl;
    cout << "\033[1;36m________________________________________________________________________________\033[0m" << endl;


    for (int i = 0; i < board_size; i++) 
    {
        //cout << "| ";
        for (int j = 0; j < board_size; j++) 
        {
            bool pflag = false;

            for (int p = 0; p < NO_PLAYER; p++) 
            {
                if (players[p].pos_x == i && players[p].pos_y == j)
                {
                    if (p == 0)
                    {
                        cout << "\033[1;31mX\033[0m";
                    }
                    else if (p == 1)
                    {
                        cout << "\033[1;32mO\033[0m";
                    }
                    
                    pflag = true;
                    break;
                }
            }
            if (!pflag) 
            {
                cout << "\033[1;34m" << board_grid[i][j] << "\033[0m";
            }
        }
        //cout << "\033[1;36m|\033[0m";
        cout << endl;
    }
    cout << "\033[1;36m________________________________________________________________________________\033[0m" << endl;
}

void* playerthread1(void* arg)
{
    //print_board();
    initscr();  

    noecho();   
    cbreak();   

    keypad(stdscr, TRUE);  


    int ch;
    while ((ch = getch()) != 'q')
    {
        switch (ch)
        { 
            case w:
            case W:
                if (players[0].pos_x > 0)
                {
                    players[0].pos_x--;
                }
                break;
            case s:
            case S:
                if (players[0].pos_x < board_size - 1)
                {
                    players[0].pos_x++;
                }
                break;
            case a:
            case A:
                if (players[0].pos_y > 0)
                {
                    players[0].pos_y--;
                }
                break;
            case d:
            case D:
                if (players[0].pos_y < board_size - 1)
                {
                    players[0].pos_y++;
                }
                break;
        }
        // check if player has collected an item
        if (board_grid[players[0].pos_x][players[0].pos_y] == ITEM)
        {
            players[0].p_score++;
            board_grid[players[0].pos_x][players[0].pos_y] = EMPTY;
        }

        refresh();
    }

    
    pthread_exit(NULL);

}

void* playerthread2(void* arg)
{
    //print_board();
    initscr();  

    noecho();   
    cbreak();   

    keypad(stdscr, TRUE);  

    int ch;
    while ((ch = getch()) != 'q')
    {
        switch (ch)
        {
            case i:
            case I:
                if (players[1].pos_x > 0)
                {
                    players[1].pos_x--;
                }
                break;
            case k:
            case K:
                if (players[1].pos_x < board_size - 1)
                {
                    players[1].pos_x++;
                }
                break;
            case j:
            case J:
                if (players[1].pos_y > 0)
                {
                    players[1].pos_y--;
                }
                break;
            case l:
            case L:
                if (players[1].pos_y < board_size - 1)
                {
                    players[1].pos_y++;
                }
                break;
        }
        // check if player has collected an item
        if (board_grid[players[1].pos_x][players[1].pos_y] == ITEM)
        {
            players[1].p_score++;
            board_grid[players[1].pos_x][players[1].pos_y] = EMPTY;
        }
        refresh();
        //print_board();
        
    }

    //endwin();   // restore terminal settings
    pthread_exit(NULL);

}

int main()
{
    sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "Multiplayer Game");
    window.setFramerateLimit(10);

    
    sf::RectangleShape grid[GRID_ROWS][GRID_COLS];

    for (int i = 0; i < GRID_ROWS; i++) {
        for (int j = 0; j < GRID_COLS; j++) {
            grid[i][j].setSize(sf::Vector2f(GRID_SIZE, GRID_SIZE));
            grid[i][j].setPosition(j * GRID_SIZE, i * GRID_SIZE);
            grid[i][j].setFillColor(sf::Color::Black);
            grid[i][j].setOutlineThickness(1);
            grid[i][j].setOutlineColor(sf::Color::White);
        }
    }

    
    sf::RectangleShape player1(sf::Vector2f(GRID_SIZE, GRID_SIZE));
    player1.setPosition(2 * GRID_SIZE, 2 * GRID_SIZE);
    player1.setFillColor(sf::Color::Red);

    sf::RectangleShape player2(sf::Vector2f(GRID_SIZE, GRID_SIZE));
    player2.setPosition((GRID_COLS - 3) * GRID_SIZE, (GRID_ROWS - 3) * GRID_SIZE);
    player2.setFillColor(sf::Color::Green);

   
    sf::RectangleShape items[NO_ITEMS];
    for (int i = 0; i < NO_ITEMS; i++) {
        int x = rand() % (GRID_COLS - 2) + 1;
        int y = rand() % (GRID_ROWS - 2) + 1;
        items[i].setSize(sf::Vector2f(GRID_SIZE, GRID_SIZE));
        items[i].setPosition(x * GRID_SIZE, y * GRID_SIZE);
        items[i].setFillColor(sf::Color::Blue);
    }

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        window.clear(sf::Color::Black);

        // Draw grid
        for (int i = 0; i < GRID_ROWS; i++) {
            for (int j = 0; j < GRID_COLS; j++) {
                window.draw(grid[i][j]);
            }
        }

        // Draw player objects
        window.draw(player1);
        window.draw(player2);

        // Draw item objects
        for (int i = 0; i < NO_ITEMS; i++) {
            window.draw(items[i]);
        }

        window.display();
    }

    init_game_board();
    print_board();

    pthread_create (&player_1, NULL, playerthread1, NULL);
    pthread_create (&player_2, NULL, playerthread2, NULL);
    
    pthread_join(player_1, NULL);
    pthread_join(player_2, NULL);

    endwin();
    //print_board();
}
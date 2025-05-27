# Multiplayer Grid Game

## Description

**Multiplayer Grid Game** is a simple concurrent game simulation implemented using threads and message passing in C/C++. Multiple players navigate a shared 2D grid board, collect items, and earn points. The project emphasizes thread-safety without using mutexes or semaphores by using a message-queue-based communication system between threads.

## Features

- **Dynamic Game Board**: The game board is generated using a unique algorithm based on your roll number and a random number, resulting in an `n x n` grid.
- **Multiple Players**: Multiple threads represent individual players, each capable of moving independently on the board.
- **Item Collection**: Players collect items by moving onto cells containing them, earning points.
- **Message Passing**: Instead of using traditional synchronization methods, message queues are used for thread communication to ensure a safe and consistent game state.
- **Priority-based Threads**: Player thread priorities can be adjusted based on scores or performance, giving an edge to high-scoring players.
- **Main Thread Logic**: A central thread handles game logic, board updates, and score tracking based on messages received from player threads.

## Game Board Size Calculation

The board size `n x n` is calculated using the following steps:

1. Generate a random number between **10–99**.
2. Multiply this number by the **last digit of your roll number**.
3. Divide your **roll number** by the result from step 2.
4. Take **modulo 25** of the result. If the result is **less than 10**, add 15.
5. The final value is `n`, which defines the size of the square board.

## How It Works

1. The **main thread** initializes the board, places items randomly, and spawns player threads.
2. Each **player thread**:
   - Listens for arrow key input.
   - Moves across the board.
   - Sends messages to the main thread when collecting items.
3. The **main thread**:
   - Polls player message queues.
   - Updates the board and player scores accordingly.
   - Displays the game state.

## Technologies Used

- Language: **C/C++**
- Concurrency: **Threads (POSIX / C++ threads)**
- Input Handling: **Keyboard input (platform-dependent)**

## Requirements

- GCC / g++ compiler
- POSIX threads (pthreads) if using C
- Platform libraries for keyboard input (e.g., `ncurses` or `conio.h`)
- Linux/Unix or Windows system

## Future Improvements

- Add a GUI or use a text-based game interface.
- Include real-time scoreboard.
- Add AI-controlled players.
- Implement power-ups and special items.

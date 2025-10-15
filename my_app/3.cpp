#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>

using namespace std;

const int mazeWidth = 9;
const int mazeHeight = 9;
vector<vector<int>> maze(mazeHeight, vector<int>(mazeWidth, 1)); // 1 represents walls, 0 represents paths
int px = 1, pz = 1; // Player's position
int exitX, exitZ; // Exit position (random but reachable)

// Function to shuffle an array (used for random maze generation)
void shuffleArray(int* array, int size) {
    for (int i = size - 1; i > 0; i--) {
        int index = rand() % (i + 1);
        // Swap elements
        int temp = array[i];
        array[i] = array[index];
        array[index] = temp;
    }
}

// Recursive function to generate maze using backtracking
void carvePath(int x, int z) {
    int directions[] = {0, 1, 2, 3}; // 0: Up, 1: Down, 2: Left, 3: Right
    shuffleArray(directions, 4); // Shuffle directions for randomness

    for (int i = 0; i < 4; i++) {
        int nx = x, nz = z;

        switch (directions[i]) {
            case 0: // Up
                nz -= 2;
                break;
            case 1: // Down
                nz += 2;
                break;
            case 2: // Left
                nx -= 2;
                break;
            case 3: // Right
                nx += 2;
                break;
        }

        // Check if the new position is within bounds and is a wall
        if (nx > 0 && nx < mazeWidth - 1 && nz > 0 && nz < mazeHeight - 1 && maze[nz][nx] == 1) {
            maze[nz][nx] = 0; // Make the path
            maze[(nz + z) / 2][(nx + x) / 2] = 0; // Carve the wall in between

            // Recursively carve paths from the new position
            carvePath(nx, nz);
        }
    }
}

// Function to print the maze with player's position and exit (in different color)
void printMaze() {
    for (int z = 0; z < mazeHeight; z++) {
        for (int x = 0; x < mazeWidth; x++) {
            if (x == px && z == pz)
                cout << "P "; // Player position
            else if (x == exitX && z == exitZ)
                cout << "\033[1;32mE \033[0m"; // Exit in green (colored)
            else if (maze[z][x] == 1)
                cout << "# "; // Wall
            else
                cout << ". "; // Path
        }
        cout << endl;
    }
}

// Function to reset the maze and player position
void resetMaze() {
    // Reset player position
    px = 1;
    pz = 1;

    // Clear previous maze
    for (int i = 0; i < mazeHeight; i++) {
        for (int j = 0; j < mazeWidth; j++) {
            maze[i][j] = 1; // Fill maze with walls
        }
    }

    // Step 2: Start maze generation at position (1, 1)
    maze[1][1] = 0;
    carvePath(1, 1);

    // Step 3: Ensure exit is placed on a path
    // Randomly select exit position, ensuring it’s on a valid path
    do {
        exitX = rand() % (mazeWidth - 2) + 1; // Random X within bounds (1 to mazeWidth-2)
        exitZ = rand() % (mazeHeight - 2) + 1; // Random Z within bounds (1 to mazeHeight-2)
    } while (maze[exitZ][exitX] != 0); // Ensure exit is on a path (not a wall)
    maze[exitZ][exitX] = 0; // Ensure the exit is an open space
}

int main() {
    srand(time(0)); // Seed the random number generator

    bool keepPlaying = true;

    while (keepPlaying) {
        // Generate a new maze each time the player chooses to continue
        resetMaze();

        // Game loop
        string direction;
        while (true) {
            // Step 4: Print the maze
            printMaze();

            // Step 5: Check if the player has reached the exit
            if (px == exitX && pz == exitZ) {
                cout << "🎉 You Win! You've reached the exit." << endl;

                // Ask if the player wants to keep going
                char choice;
                cout << "Do you want to keep going? (y/n): ";
                cin >> choice;

                if (choice == 'n' || choice == 'N') {
                    keepPlaying = false; // Exit the game
                    break;
                } else if (choice == 'y' || choice == 'Y') {
                    // Reset the maze and continue playing
                    break;
                } else {
                    cout << "Invalid input. Continuing the game..." << endl;
                }
            }

            // Step 6: Ask the user for movement input
            cout << "Use WASD to move, type 'exit' to quit: ";
            cin >> direction;

            // Step 7: Handle movement based on user input
            if (direction == "w" && pz > 0 && maze[pz - 1][px] == 0) pz--;
            if (direction == "s" && pz < mazeHeight - 1 && maze[pz + 1][px] == 0) pz++;
            if (direction == "a" && px > 0 && maze[pz][px - 1] == 0) px--;
            if (direction == "d" && px < mazeWidth - 1 && maze[pz][px + 1] == 0) px++;

            // Step 8: Quit the game
            if (direction == "exit") {
                cout << "Exiting the game. Thanks for playing!" << endl;
                keepPlaying = false;
                break;
            }
        }
    }

    return 0;
}


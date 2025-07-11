using System;

public class MazeMono
{
    const int WIDTH = 20;
    const int HEIGHT = 10;
    static char[,] maze = new char[HEIGHT, WIDTH];
    static int px, py, ex, ey;

    public static void Main()
    {
        Console.Clear();
        Console.CursorVisible = false;
        GenerateMaze();
        DrawMaze();

        while (true)
        {
            if (!Console.KeyAvailable) continue;

            var key = Console.ReadKey(true).Key;
            int nx = px, ny = py;

            if (key == ConsoleKey.W) ny--;
            else if (key == ConsoleKey.S) ny++;
            else if (key == ConsoleKey.A) nx--;
            else if (key == ConsoleKey.D) nx++;

            if (maze[ny, nx] == ' ' || maze[ny, nx] == 'E')
            {
                maze[py, px] = ' ';
                px = nx;
                py = ny;
                if (maze[py, px] == 'E')
                {
                    maze[py, px] = 'P';
                    DrawMaze();
                    Console.WriteLine("\nYou escaped!");
                    break;
                }
                maze[py, px] = 'P';
                DrawMaze();
            }
        }
    }

    static void DrawMaze()
    {
        Console.SetCursorPosition(0, 0);
        for (int y = 0; y < HEIGHT; y++)
        {
            for (int x = 0; x < WIDTH; x++)
                Console.Write(maze[y, x]);
            Console.WriteLine();
        }
        Console.WriteLine("\nWASD to move.");
    }

    static void GenerateMaze()
    {
        var rand = new Random();
        for (int y = 0; y < HEIGHT; y++)
        {
            for (int x = 0; x < WIDTH; x++)
                maze[y, x] = (y == 0 || y == HEIGHT - 1 || x == 0 || x == WIDTH - 1) ? '#' :
                             (rand.Next(4) == 0 ? '#' : ' ');
        }

        px = rand.Next(1, WIDTH - 1);
        py = rand.Next(1, HEIGHT - 1);
        maze[py, px] = 'P';

        do
        {
            ex = rand.Next(1, WIDTH - 1);
            ey = rand.Next(1, HEIGHT - 1);
        } while (ex == px && ey == py);

        int cx = px, cy = py;
        while (cx != ex || cy != ey)
        {
            maze[cy, cx] = ' ';
            if (cx < ex) cx++;
            else if (cx > ex) cx--;
            else if (cy < ey) cy++;
            else if (cy > ey) cy--;
            maze[cy, cx] = ' ';
        }

        maze[ey, ex] = 'E';
    }
}


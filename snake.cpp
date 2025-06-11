#include <GL/glut.h>
#include <deque>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <string>

// Grid settings
const int GRID_SIZE = 20; // 20x20 grid
const float CELL_SIZE = 2.0f / GRID_SIZE; // Map grid to OpenGL [-1, 1]
const int WINDOW_SIZE = 600; // Window size in pixels

// Snake and game settings
struct Point {
    int x, y;
    Point(int _x, int _y) : x(_x), y(_y) {}
    bool operator==(const Point& other) const { return x == other.x && y == other.y; }
};

std::deque<Point> snake; // Snake body (deque for O(1) head/tail operations)
Point apple(0, 0); // Apple position
enum Direction { UP, DOWN, LEFT, RIGHT, NONE };
Direction dir = RIGHT; // Current direction
Direction nextDir = RIGHT; // Next direction (for input buffering)
bool gameOver = false;
int score = 0;
float speed = 0.1f; // Seconds per move (default: medium difficulty)
int difficulty = 1; // 0: easy (0.15s), 1: medium (0.1s), 2: hard (0.05s)

// Forward declarations
void generateApple();

// Initialize game state
void initGame() {
    snake.clear();
    snake.push_back(Point(GRID_SIZE / 2, GRID_SIZE / 2)); // Start at center
    dir = RIGHT;
    nextDir = RIGHT;
    gameOver = false;
    score = 0;
    generateApple();
}

// Generate apple at random valid position
void generateApple() {
    std::vector<Point> emptyCells;
    for (int x = 0; x < GRID_SIZE; x++) {
        for (int y = 0; y < GRID_SIZE; y++) {
            Point p(x, y);
            bool inSnake = false;
            for (const auto& segment : snake) {
                if (p == segment) {
                    inSnake = true;
                    break;
                }
            }
            if (!inSnake) emptyCells.push_back(p);
        }
    }
    if (!emptyCells.empty()) {
        int idx = rand() % emptyCells.size();
        apple = emptyCells[idx];
    }
}

// Convert grid coordinates to OpenGL coordinates
float gridToOpenGL(int coord) {
    return -1.0f + coord * CELL_SIZE + CELL_SIZE / 2.0f;
}

// Draw a rectangle at grid position (x, y) with specified color
void drawRect(int x, int y, float r, float g, float b) {
    float glX = gridToOpenGL(x) - CELL_SIZE / 2;
    float glY = gridToOpenGL(y) - CELL_SIZE / 2;
    glColor3f(r, g, b);
    glBegin(GL_QUADS);
    glVertex2f(glX, glY);
    glVertex2f(glX + CELL_SIZE, glY);
    glVertex2f(glX + CELL_SIZE, glY + CELL_SIZE);
    glVertex2f(glX, glY + CELL_SIZE);
    glEnd();
}

// Render the game
void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    // Draw grid (light gray lines)
    glColor3f(0.2f, 0.2f, 0.2f);
    glBegin(GL_LINES);
    for (int i = 0; i <= GRID_SIZE; i++) {
        float pos = -1.0f + i * CELL_SIZE;
        glVertex2f(pos, -1.0f); glVertex2f(pos, 1.0f); // Vertical
        glVertex2f(-1.0f, pos); glVertex2f(1.0f, pos); // Horizontal
    }
    glEnd();

    // Draw snake (green)
    for (const auto& segment : snake) {
        drawRect(segment.x, segment.y, 0.0f, 1.0f, 0.0f);
    }

    // Draw apple (red)
    drawRect(apple.x, apple.y, 1.0f, 0.0f, 0.0f);

    // Draw game over message and score
    if (gameOver) {
        glColor3f(1.0f, 1.0f, 1.0f);
        glRasterPos2f(-0.3f, 0.0f);
        std::string msg = "Game Over! Score: " + std::to_string(score) + " (R to restart)";
        for (char c : msg) glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);
    } else {
        glColor3f(1.0f, 1.0f, 1.0f);
        glRasterPos2f(-0.9f, 0.9f);
        std::string msg = "Score: " + std::to_string(score);
        for (char c : msg) glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, c);
    }

    glutSwapBuffers();
}

// Update game state
void update(int value) {
    if (!gameOver) {
        // Update direction
        dir = nextDir;

        // Calculate new head position
        Point head = snake.front();
        if (dir == UP) head.y++;
        else if (dir == DOWN) head.y--;
        else if (dir == LEFT) head.x--;
        else if (dir == RIGHT) head.x++;

        // Check boundary collision
        if (head.x < 0 || head.x >= GRID_SIZE || head.y < 0 || head.y >= GRID_SIZE) {
            gameOver = true;
        } else {
            // Check self-collision
            for (const auto& segment : snake) {
                if (head == segment) {
                    gameOver = true;
                    break;
                }
            }
        }

        if (!gameOver) {
            // Move snake
            snake.push_front(head);

            // Check apple collision
            if (head == apple) {
                score += 10;
                generateApple();
            } else {
                snake.pop_back(); // Remove tail if no apple eaten
            }
        }
    }

    glutPostRedisplay();
    glutTimerFunc(static_cast<int>(speed * 1000), update, 0);
}

// Handle keyboard input
void keyboard(unsigned char key, int x, int y) {
    switch (key) {
        case 'w':
        case 'W':
            if (dir != DOWN) nextDir = UP;
            break;
        case 's':
        case 'S':
            if (dir != UP) nextDir = DOWN;
            break;
        case 'a':
        case 'A':
            if (dir != RIGHT) nextDir = LEFT;
            break;
        case 'd':
        case 'D':
            if (dir != LEFT) nextDir = RIGHT;
            break;
        case 'r':
        case 'R':
            if (gameOver) {
                initGame();
                speed = (difficulty == 0) ? 0.15f : (difficulty == 1) ? 0.1f : 0.05f;
            }
            break;
        case '1': // Easy
            difficulty = 0;
            speed = 0.15f;
            if (gameOver) initGame();
            break;
        case '2': // Medium
            difficulty = 1;
            speed = 0.1f;
            if (gameOver) initGame();
            break;
        case '3': // Hard
            difficulty = 2;
            speed = 0.05f;
            if (gameOver) initGame();
            break;
    }
}

// Handle special keys (arrows)
void special(int key, int x, int y) {
    switch (key) {
        case GLUT_KEY_UP:
            if (dir != DOWN) nextDir = UP;
            break;
        case GLUT_KEY_DOWN:
            if (dir != UP) nextDir = DOWN;
            break;
        case GLUT_KEY_LEFT:
            if (dir != RIGHT) nextDir = LEFT;
            break;
        case GLUT_KEY_RIGHT:
            if (dir != LEFT) nextDir = RIGHT;
            break;
    }
}

// Initialize OpenGL
void init() {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Black background
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0); // 2D orthographic projection
    srand(time(0));
    initGame();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(WINDOW_SIZE, WINDOW_SIZE);
    glutCreateWindow("Snake Game");
    init();
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(special);
    glutTimerFunc(0, update, 0);
    glutMainLoop();
    return 0;
}
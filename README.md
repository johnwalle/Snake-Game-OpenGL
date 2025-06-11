# 🐍 Snake Game in OpenGL (C++ & FreeGLUT)

## 🎮 Overview

This project is a simple yet complete implementation of the **classic Snake game** using **C++ with OpenGL and FreeGLUT**. The game runs on a 20x20 grid where the player controls the snake to collect apples, grow in size, and avoid collisions with walls or itself.

---

## ✨ Features

- 🟩 **Smooth 2D Rendering** using OpenGL
- 🎯 **Apple Spawning** with collision-safe placement
- ⌨️ **WASD / Arrow Key Controls**
- 🚦 **3 Difficulty Levels**: Easy (0.25s), Medium (0.2s), Hard (0.1s)
- 💀 **Self and Wall Collision Detection**
- 🧠 **Buffered Direction Input**
- 🧾 **Score Tracking** (+10 per apple)
- 🔁 **Restart with 'R' key** after game over

- ![Snake Game Screenshot](https://res.cloudinary.com/dso7gnmps/image/upload/v1749628020/Screenshot_2025-06-11_104207_tqlbth.png)


---

## ⚙️ How It Works (Process)

### 🧠 Game Logic

- **Snake Body**: Managed with a `std::deque` for efficient head/tail operations.
- **Grid Mapping**: Each grid cell is mapped to OpenGL's `[-1, 1]` using `CELL_SIZE = 2.0 / 20`.
- **Apple Placement**: Randomly selected from empty cells not occupied by the snake.
- **Movement**: Controlled using buffered `nextDir` to avoid reversing direction instantly.
- **Game Loop**: Updated with `glutTimerFunc` for consistent timing based on difficulty level.

### 🧪 Development Steps

1. **Installed**: MinGW-w64 (Windows) and FreeGLUT 3.0.0 (64-bit).
2. **Set Up**: OpenGL development environment with proper include/lib paths.
3. **Implemented**:
   - Snake structure, movement, collisions.
   - Apple generation and score tracking.
   - Input handling (WASD + arrows).
4. **Tested**:
   - Different speeds for playability.
   - Edge-case collisions.
   - Difficulty switching and reset handling.
5. **Polished**: Added grid rendering, score UI, game over message, and user-friendly restart.

---

## 🧱 How to Compile and Run (Windows – MinGW-w64 + FreeGLUT)
```bash
g++ snake.cpp -o snake -I"C:\mingw64\include" -L"C:\mingw64\lib" -lfreeglut -lopengl32 -lglu32
./snake.exe



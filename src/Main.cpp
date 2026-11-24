#include <string>
#include <vector>

#include "ui/Window.hpp"
#include "ui/DrawComponent.hpp"
#include "ui/ColorNode.hpp"
#include "Math/Vector.hpp"
#include "Math/Random.hpp"
#include "input/Keyboard.hpp"
#include "time/Timer.hpp"
#include "core/Collision.hpp"

using Vector::Vec2D;
using UI::RGB;
using Input::Keyboard;

// The board is a grid of 40 x 30 cells, 20 pixels each = 800 x 600.
const int CELL = 20;
const int COLS = 40;
const int ROWS = 30;

struct Game {
    std::vector<Vec2D> snake;   // grid cells, snake[0] is the head
    Vec2D dir{1.0f, 0.0f};      // direction used for the last step
    Vec2D nextDir{1.0f, 0.0f};  // direction the player asked for
    Vec2D food;
    int score = 0;
    bool gameOver = false;
};

// A grid cell as an engine object (position and size in pixels), for collision.
Objects::Object2D cellObject(const Vec2D& cell)
{
    return Objects::Object2D(RGB(0, 0, 0),
                             Vec2D(cell.x * CELL, cell.y * CELL),
                             Vec2D(CELL, CELL));
}

void drawCell(UI::DrawComponent& draw, const Vec2D& cell, const RGB& color)
{
    // 1 pixel gap around each cell makes the grid visible.
    draw.DrawRect(Vec2D(cell.x * CELL + 1, cell.y * CELL + 1), CELL - 2, CELL - 2, color);
}

void placeFood(Game& g)
{
    if ((int)g.snake.size() >= COLS * ROWS) return;   // board is full, nothing to place

    bool onSnake;
    do {
        g.food = Vec2D((float)Math::Random::nextInt(0, COLS - 1),
                       (float)Math::Random::nextInt(0, ROWS - 1));
        onSnake = false;
        for (const Vec2D& part : g.snake)
            if (part == g.food) onSnake = true;
    } while (onSnake);
}

void reset(Game& g)
{
    g.snake = { Vec2D(20.0f, 15.0f), Vec2D(19.0f, 15.0f), Vec2D(18.0f, 15.0f) };
    g.dir = Vec2D(1.0f, 0.0f);
    g.nextDir = g.dir;
    g.score = 0;
    g.gameOver = false;
    placeFood(g);
}

// Moves the snake one cell.
void step(Game& g)
{
    g.dir = g.nextDir;
    Vec2D newHead = g.snake[0] + g.dir;

    // Hit a wall?
    if (newHead.x < 0 || newHead.x >= COLS || newHead.y < 0 || newHead.y >= ROWS) {
        g.gameOver = true;
        return;
    }

    Objects::Object2D head = cellObject(newHead);
    bool ate = Collision::Collide2D(head, cellObject(g.food));

    // Hit itself? The tail moves away this step, so it doesn't count (unless we just ate).
    int checkCount = (int)g.snake.size() - (ate ? 0 : 1);
    for (int i = 0; i < checkCount; ++i) {
        if (Collision::Collide2D(head, cellObject(g.snake[i]))) {
            g.gameOver = true;
            return;
        }
    }

    g.snake.insert(g.snake.begin(), newHead);   // grow at the head
    if (ate) {
        g.score++;
        placeFood(g);                           // keep the tail: the snake got longer
    } else {
        g.snake.pop_back();                     // drop the tail: the snake just moved
    }
}

int main()
{
    UI::Window win("Snake", COLS * CELL, ROWS * CELL);
    UI::DrawComponent draw(win);
    Input::Keyboard kb;

    win.setResizable(false);
    win.center();
    win.setBackground(RGB(20, 24, 30));
    win.setTitle("Snake | Score: 0");

    Game game;
    reset(game);

    Timing::Timer stepTimer(0.14f);   // the snake moves once per timer cycle
    stepTimer.start();

    win.run([&](float) {
        // ---- Input ----
        // A turn is only allowed if it isn't a reversal of the current direction.
        if ((kb.isKeyDown(Keyboard::Up)    || kb.isKeyDown(Keyboard::W)) && game.dir.y == 0) game.nextDir = Vec2D(0.0f, -1.0f);
        if ((kb.isKeyDown(Keyboard::Down)  || kb.isKeyDown(Keyboard::S)) && game.dir.y == 0) game.nextDir = Vec2D(0.0f, 1.0f);
        if ((kb.isKeyDown(Keyboard::Left)  || kb.isKeyDown(Keyboard::A)) && game.dir.x == 0) game.nextDir = Vec2D(-1.0f, 0.0f);
        if ((kb.isKeyDown(Keyboard::Right) || kb.isKeyDown(Keyboard::D)) && game.dir.x == 0) game.nextDir = Vec2D(1.0f, 0.0f);

        if (kb.isKeyPressed(Keyboard::Escape)) win.close();

        if (game.gameOver && (kb.isKeyPressed(Keyboard::Space) || kb.isKeyPressed(Keyboard::Enter))) {
            reset(game);
            stepTimer.setDuration(0.14f);
            stepTimer.start();
            win.setTitle("Snake | Score: 0");
        }

        // ---- Update ----
        if (!game.gameOver && stepTimer.isFinished()) {
            step(game);

            if (game.gameOver)
                win.setTitle("GAME OVER | Score: " + std::to_string(game.score) + " | Space to restart");
            else
                win.setTitle("Snake | Score: " + std::to_string(game.score));

            // Faster as the score grows (but never faster than 0.05 s per step).
            float interval = 0.14f - 0.004f * game.score;
            stepTimer.setDuration(interval > 0.05f ? interval : 0.05f);
            stepTimer.start();
        }

        // ---- Drawing ----
        drawCell(draw, game.food, RGB(230, 60, 60));

        for (size_t i = 0; i < game.snake.size(); ++i) {
            RGB color = (i == 0) ? RGB(120, 255, 120) : RGB(40, 200, 60);
            if (game.gameOver) color = (i == 0) ? RGB(200, 200, 200) : RGB(120, 120, 120);
            drawCell(draw, game.snake[i], color);
        }
    });
}
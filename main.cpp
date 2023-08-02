#include <SFML/Graphics.hpp>
#include <thread>

#define WALL 0
#define TAIL 1
#define WIN 2
#define CLOSED 3

using namespace sf;

struct inputsData
{
    char &direction;
    RenderWindow &window;
};

void gameover(int gameEndCode)
{
    exit(0);
}

void foo() {}

void inputs(inputsData &data)
{
    char &direction = data.direction;
    RenderWindow &window = data.window;

    Event event;

    Thread empty(foo);

    while (true)
    {
        window.pollEvent(event);

        switch (event.type)
        {
        case Event::Closed:
            gameover(CLOSED);
            break;

        case Event::KeyPressed:
            if (Keyboard::isKeyPressed(Keyboard::W))
                direction = 'w';
            else if (Keyboard::isKeyPressed(Keyboard::A))
                direction = 'a';
            else if (Keyboard::isKeyPressed(Keyboard::S))
                direction = 's';
            else if (Keyboard::isKeyPressed(Keyboard::D))
                direction = 'd';
            break;

        default:
            continue;
        }
    }
}

int main()
{
    int height = 25,
        width = 25;

    float tps = 5.0;

    int cellSize = 20;

    int margin = 3;

    int array[width][height];

    int pos[2] = {width / 2, height / 2};

    int food[2];
    bool foodExist = false;

    char direction = 'w';
    char correctPrevDirection = 'w';
    int lenght = 3;

    int emptyCellNum = 0;

    int foodCellNum;

    RenderWindow window(VideoMode(
                            width * (cellSize + margin) + margin,
                            height * (cellSize + margin) + margin),
                        "zmeyka");

    window.setFramerateLimit(tps);

    RectangleShape square;
    square.setSize(Vector2f(cellSize, cellSize));
    square.setFillColor(Color::Black);
    square.setOutlineColor(Color::White);
    square.setOutlineThickness(1);

    for (int x = 0; x < width; x += 1)
    {
        for (int y = 0; y < height; y += 1)
        {
            square.setPosition(Vector2f(margin + x * (margin + cellSize),
                                        margin + y * (margin + cellSize)));
            window.draw(square);
            array[x][y] = 0;
        }
    }

    array[width / 2][height / 2] = lenght;

    inputsData data({direction, window});
    Thread input(inputs, data);
    input.launch();

    while (true)
    {
        // logic
        if (lenght == (width * height))
        {
            gameover(WIN);
        }

        switch (direction)
        {
        case 'w':
            pos[1] -= 1;
            break;
        case 'd':
            pos[0] += 1;
            break;
        case 's':
            pos[1] += 1;
            break;
        case 'a':
            pos[0] -= 1;
            break;
        }

        if (pos[0] < 0 || pos[0] > width)
        {
            gameover(WALL);
        }

        if (pos[1] < 0 || pos[1] > height)
        {
            gameover(WALL);
        }

        if (!foodExist)
        {
            foodCellNum = rand() % (width * height - lenght);
            foodExist = true;
        }

        switch (array[pos[0]][pos[1]])
        {
        case -1:
            lenght += 1;
            foodExist = false;
            break;
        case 0:
            break;
        default:
            gameover(TAIL);
        }

        emptyCellNum = 0;

        for (int x = 0; x < width; x += 1)
        {
            for (int y = 0; y < height; y += 1)
            {
                if (foodExist)
                {
                    if (array[x][y] > 0)
                    {
                        array[x][y] -= 1;
                        emptyCellNum -= 1;
                    }

                    emptyCellNum += 1;

                    if (emptyCellNum == foodCellNum)
                    {
                        array[x][y] = -1;
                        foodCellNum = -1;
                    }
                }
            }
        }
        array[pos[0]][pos[1]] = lenght;

        // draw
        for (int x = 0; x < width; x += 1)
        {
            for (int y = 0; y < height; y += 1)
            {
                if (array[x][y] > 0)
                    square.setFillColor(Color::White);
                else if (array[x][y] < 0)
                    square.setFillColor(Color::Red);
                else
                    square.setFillColor(Color::Black);
                square.setPosition(Vector2f(margin + x * (margin + cellSize),
                                            margin + y * (margin + cellSize)));
                window.draw(square);
            }
        }
        window.display();
    }
}
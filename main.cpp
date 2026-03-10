#include <SFML/Graphics.hpp>    //setup
#include <fstream>
#include <string>

// Grid settings
const int TILE_SIZE = 40;
const int GRID_WIDTH = 20;
const int GRID_HEIGHT = 15;

// Tile types
enum TileType
{
    GRASS,
    FOREST,
    WATER,
    HILL,
    CONCRETE
};

// Tile grid
TileType grid[GRID_WIDTH][GRID_HEIGHT];

bool isWalkable(TileType tile)
{
    if(tile == WATER)
        return false;

    return true;
}

void loadMap(std::string filename)
{
    std::ifstream mapFile(filename);

    for(int y = 0; y < GRID_HEIGHT; y++)
    {
        std::string line;
        std::getline(mapFile, line);

        for(int x = 0; x < GRID_WIDTH; x++)
        {
            char tile = line[x];

            if(tile == 'G') grid[x][y] = GRASS;
            if(tile == 'F') grid[x][y] = FOREST;
            if(tile == 'W') grid[x][y] = WATER;
            if(tile == 'H') grid[x][y] = HILL;
            if(tile == 'C') grid[x][y] = CONCRETE;
        }
    }
}

int main()  //int main you dumbass
{
    int currentLevel = 1;
    loadMap("maps/level1.txt");

    sf::RenderWindow window(sf::VideoMode(800, 600), "Strategy Grid");

    sf::Font font;
    font.loadFromFile("fonts/arial.ttf");

    sf::Text debugText;
    debugText.setFont(font);
    debugText.setCharacterSize(16);
    debugText.setFillColor(sf::Color::White);
    debugText.setPosition(10, 10);

    // Load textures
    sf::Texture grassTex;
    sf::Texture forestTex;
    sf::Texture waterTex;
    sf::Texture hillTex;
    sf::Texture concreteTex;

    grassTex.loadFromFile("textures/grass.png");
    forestTex.loadFromFile("textures/forest.png");
    waterTex.loadFromFile("textures/water.png");
    hillTex.loadFromFile("textures/hill.png");
    concreteTex.loadFromFile("textures/concrete.png");
    
    int selectedX = -1;
    int selectedY = -1;

    while (window.isOpen())
    {
        sf::Event event;

        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();

            if(event.type == sf::Event::KeyPressed)
            {
                if(event.key.code == sf::Keyboard::N)
                {
                    currentLevel++;

                    std::string levelFile = "maps/level" + std::to_string(currentLevel) + ".txt";
                    loadMap(levelFile);
                }
            }

            if (event.type == sf::Event::MouseButtonPressed &&
                event.mouseButton.button == sf::Mouse::Left)
            {
                selectedX = event.mouseButton.x / TILE_SIZE;
                selectedY = event.mouseButton.y / TILE_SIZE;
            }
        }

        sf::Vector2i mousePos = sf::Mouse::getPosition(window);
        int hoverX = mousePos.x / TILE_SIZE;
        int hoverY = mousePos.y / TILE_SIZE;

        window.clear();

        for(int x = 0; x < GRID_WIDTH; x++)
        {
            for(int y = 0; y < GRID_HEIGHT; y++)
            {
                sf::Sprite sprite;

                if(grid[x][y] == GRASS)
                    sprite.setTexture(grassTex);

                if(grid[x][y] == FOREST)
                    sprite.setTexture(forestTex);

                if(grid[x][y] == WATER)
                    sprite.setTexture(waterTex);

                if(grid[x][y] == HILL)
                    sprite.setTexture(hillTex);

                if(grid[x][y] == CONCRETE)
                    sprite.setTexture(concreteTex);

                sprite.setPosition(x * TILE_SIZE, y * TILE_SIZE);

                window.draw(sprite);

                // Hover highlight
                if(x == hoverX && y == hoverY)
                {
                    sf::RectangleShape highlight(sf::Vector2f(TILE_SIZE, TILE_SIZE));
                    highlight.setPosition(x * TILE_SIZE, y * TILE_SIZE);
                    highlight.setFillColor(sf::Color(255,255,255,80));
                    window.draw(highlight);
                }

                // Selected tile
                if(x == selectedX && y == selectedY)
                {
                    sf::RectangleShape select(sf::Vector2f(TILE_SIZE, TILE_SIZE));
                    select.setPosition(x * TILE_SIZE, y * TILE_SIZE);
                    select.setFillColor(sf::Color(100,100,255,120));
                    window.draw(select);
                }
            }
        }

        std::string tileName = "Unknown";

        TileType tile = GRASS;

        if(hoverX >= 0 && hoverX < GRID_WIDTH &&
        hoverY >= 0 && hoverY < GRID_HEIGHT)
        {
            tile = grid[hoverX][hoverY];
        }

        if(tile == GRASS) tileName = "Grass";
        if(tile == FOREST) tileName = "Forest";
        if(tile == WATER) tileName = "Water";
        if(tile == HILL) tileName = "Hill";
        if(tile == CONCRETE) tileName = "Concrete";

        debugText.setString(
            "Tile: (" + std::to_string(hoverX) + ", " + std::to_string(hoverY) + 
            ")  Type: " + tileName +
            "  Level: " + std::to_string(currentLevel)
        );
        window.draw(debugText);
        window.display();
    }

    return 0;
}
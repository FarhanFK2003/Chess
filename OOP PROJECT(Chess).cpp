#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp> 
#include <iostream>
#include <string.h>
using namespace sf;
using namespace std;
// Class representing an image loader
class ImageLoader
{
public:
    // Load texture from file
    static bool loadTexture(Texture& texture, const string& filename)
    {
        if (!texture.loadFromFile(filename))
        {
            return false;
        }
        return true;
    }
};
//Function to display stating page
bool displayStartingPage(RenderWindow& window) {
    // Load background texture
    Texture backgroundTexture;
    if (!backgroundTexture.loadFromFile("C:\\Users\\DELL\\OneDrive\\Pictures\\Saved Pictures\\background3.jpg")) {
        return false;
    }
    Sprite backgroundSprite(backgroundTexture);

    // Scale background sprite to fit window size
    backgroundSprite.setScale(window.getSize().x / backgroundSprite.getLocalBounds().width,
        window.getSize().y / backgroundSprite.getLocalBounds().height);

    Font font;
    if (!font.loadFromFile("C:\\Users\\DELL\\OneDrive\\Pictures\\Saved Pictures\\font.ttf")) {
        return false;
    }
    Text startText("START", font, 50);
    startText.setFillColor(Color::White);
    startText.setPosition(320, 300);

    Text exitText("EXIT", font, 50);
    exitText.setFillColor(Color::White);
    exitText.setPosition(330, 400);

    Text madeByText("Made by Zain and Shafan", font, 40);
    madeByText.setFillColor(Color::White);
    madeByText.setPosition(90, 700);

    // Load music
    Music music;
    if (!music.openFromFile("C:\\Users\\DELL\\OneDrive\\Pictures\\Saved Pictures\\gameStart.wav")) {
        // Handle error loading music file
        return false;
    }
    music.play(); // Play music when starting the game
    while (window.isOpen()) {
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed) {
                window.close();
                return false;
            }
            else if (event.type == Event::MouseButtonPressed) {
                if (event.mouseButton.button == Mouse::Left) {
                    Vector2f mousePosition = window.mapPixelToCoords(Mouse::getPosition(window));
                    if (startText.getGlobalBounds().contains(mousePosition)) {
                        return true; // Start the game
                    }
                    else if (exitText.getGlobalBounds().contains(mousePosition)) {
                        return false; // Exit the game
                    }
                }
            }
            else if (event.type == Event::KeyPressed) {
                if (event.key.code == Keyboard::Enter) {
                    return true; // Start the game
                }
                else if (event.key.code == Keyboard::E) {
                    return false; // Exit the game
                }
            }
        }

        window.clear();
        window.draw(backgroundSprite);
        window.draw(startText);
        window.draw(exitText);
        window.draw(madeByText);
        window.display();
    }

    return false;
}
// Class representing the highlighter square
class Highlighter
{
private:
    RectangleShape square;

public:
    Highlighter()
    {
        square.setSize(Vector2f(100, 100));
        square.setFillColor(Color::Transparent);
        square.setOutlineThickness(8.f);
        square.setOutlineColor(Color::Red);
        square.setScale(1.f, 1.f);
    }

    void setPosition(float x, float y)
    {
        square.setPosition(x, y);
    }

    const Vector2f& getPosition() const
    {
        return square.getPosition();
    }

    void draw(RenderWindow& window)
    {
        window.draw(square);
    }

    void moveUp()
    {
        if (square.getPosition().y > 0)
        {
            square.move(0, -100);
        }
    }

    void moveDown()
    {
        if (square.getPosition().y < 700)
        {
            square.move(0, 100);
        }
    }

    void moveLeft()
    {
        if (square.getPosition().x > 0)
        {
            square.move(-100, 0);
        }
    }

    void moveRight()
    {
        if (square.getPosition().x < 700)
        {
            square.move(100, 0);
        }
    }
};

class Chessboard
{
private:
    RectangleShape squares[8][8];
    bool highlighted[8][8]; // Array to track highlighted squares

public:
    Chessboard()
    {
        // Initialize the squares of the chessboard
        for (int i = 0; i < 8; ++i)
        {
            for (int j = 0; j < 8; ++j)
            {
                squares[i][j] = RectangleShape(Vector2f(100, 100));
                squares[i][j].setPosition(j * 100, i * 100);
                if ((i + j) % 2 == 0)
                {
                    squares[i][j].setFillColor(Color(255, 255, 255));
                }
                else
                {
                    squares[i][j].setFillColor(Color(200, 155, 100));
                }
                highlighted[i][j] = false; // Initialize highlighted squares to false
            }
        }
    }

    // Method to draw the chessboard
    void draw(RenderWindow& window)
    {
        // Draw the squares of the chessboard
        for (int i = 0; i < 8; ++i)
        {
            for (int j = 0; j < 8; ++j)
            {
                if (highlighted[i][j]) {
                    // Highlighted square color
                    squares[i][j].setFillColor(Color(0, 255, 0, 128)); // Semi-transparent green
                }
                else {
                    // Normal square color
                    if ((i + j) % 2 == 0)
                    {
                        squares[i][j].setFillColor(Color(255, 255, 255));
                    }
                    else
                    {
                        squares[i][j].setFillColor(Color(200, 155, 100));
                    }
                }
                window.draw(squares[i][j]);
            }
        }
    }

    // Method to highlight a square at position (x, y)
    void highlightSquare(int x, int y)
    {
        if (x >= 0 && x < 8 && y >= 0 && y < 8) {
            highlighted[y][x] = true; // Flip x and y indices because of row-major order
        }
    }

    // Method to reset all highlighted squares
    void resetHighlightedSquares()
    {
        for (int i = 0; i < 8; ++i)
        {
            for (int j = 0; j < 8; ++j)
            {
                highlighted[i][j] = false;
            }
        }
    }
};
// Class representing a chess piece
class Piece
{
protected:
    Sprite sprite;
    bool colorCode; // 0 -> Black, 1 -> White
    bool isVisible; // Flag to indicate if the piece is visible
public:
    // Default constructor
    Piece() {}

    // Constructor with parameters
    Piece(const Texture& texture, float scale, Vector2f position, bool colorCode)
    {
        sprite.setTexture(texture);
        sprite.setScale(scale, scale);
        sprite.setPosition(position);
        this->colorCode = colorCode;
        this->isVisible = true;
    }

    // Method to set texture
    void setTexture(const Texture& texture)
    {
        if (isVisible) // Check if the piece is visible before setting its texture
            sprite.setTexture(texture);
    }

    void setScale(float x, float y)
    {
        if (isVisible) // Check if the piece is visible before setting its scale
            sprite.setScale(x, y);
    }

    void setPosition(float x, float y)
    {
        if (isVisible) // Check if the piece is visible before setting its position
            sprite.setPosition(x, y);
    }

    void setColorCode(bool colorCode)
    {
        this->colorCode = colorCode;
    }

    void setVisible(bool visible)
    {
        isVisible = visible;
    }

    const Texture* getTexture() const
    {
        if (isVisible) // Check if the piece is visible before returning its texture
            return sprite.getTexture();
        else
            return nullptr;
    }

    const Vector2f& getScale() const
    {
        static Vector2f defaultScale(0, 0);
        if (isVisible) // Check if the piece is visible before returning its scale
            return sprite.getScale();
        else
            return defaultScale; // Return default scale if the piece is not visible
    }

    const Vector2f& getPosition() const
    {
        static Vector2f defaultPosition(0, 0);
        if (this != nullptr && isVisible) // Check if the object is valid and visible
            return sprite.getPosition();
        else
            return defaultPosition; // Return default position if the object is not valid or not visible
    }


    bool getColorCode() const
    {
        if (this != nullptr)
        {
        return colorCode;
        }
        return false;
    }

    bool getVisible() const
    {
        return isVisible;
    }

    void draw(RenderWindow& window)
    {
        if (this != nullptr && isVisible && sprite.getTexture() != nullptr)
        {
            window.draw(sprite);
        }
    }

    virtual bool validMove(Vector2f oldPos, Vector2f newPos, Piece* pieces[])
    {
        if ((newPos.x < 0 || newPos.x >= 800 || newPos.y < 0 || newPos.y >= 800))
        {
            return false;
        }

        return true;
    }
    virtual string getType()const {
        return "Piece";
    }
};
class Pawn : public Piece
{
public:
    // Default constructor
    Pawn() {}

    // Constructor with parameters
    Pawn(Texture& texture, float scale, Vector2f position, bool colorCode) : Piece(texture, scale, position, colorCode)
    {

    }
    // Method to override move function for pawn
    bool validMove(Vector2f oldPos, Vector2f newPos, Piece* pieces[]) override
    {
        if (newPos.x < 0 || newPos.x >= 800 || newPos.y < 0 || newPos.y >= 800)
        {
            cout << "Invalid move: Position out of bounds!" << endl;
            return false;
        }

        if (newPos == oldPos)
        {
            cout << "Invalid move: Same position!" << endl;
            return false;
        }

        if (!getColorCode())
        {
            // Check if the new position is one square forward
            if (newPos.x == oldPos.x && newPos.y == oldPos.y + 100)
            {
                // Check if the square in front of the pawn is occupied by another piece
                for (int i = 0; i < 32; ++i)
                {
                    if (pieces[i] != nullptr && pieces[i]->getPosition() == Vector2f(newPos.x, newPos.y) && pieces[i]->getVisible())
                    {
                        cout << "Invalid move: Piece blocking the way!" << endl;
                        return false;
                    }
                }

                // If the loop completes without finding any piece at the new position, the move is valid
                cout << "Valid move: One square forward." << endl;
                return true;
            }

            // Check if the new position is two squares forward (only valid from the starting position)
            if (oldPos.y == 100 && newPos.x == oldPos.x && newPos.y == oldPos.y + 200)
            {
                // Check if the square in front of the pawn is occupied by another piece
                for (int i = 0; i < 32; ++i)
                {
                    if (pieces[i] != nullptr && pieces[i]->getPosition() == Vector2f(newPos.x, newPos.y) && pieces[i]->getVisible())
                    {
                        cout << "Invalid move: Piece blocking the way!" << endl;
                        return false;
                    }
                }

                // If the loop completes without finding any piece at the new position, the move is valid
                cout << "Valid move: Two squares forward." << endl;
                return true;
            }

            if ((newPos.x == oldPos.x + 100) && newPos.y == oldPos.y + 100)
            {
                // Check if there is an opponent's piece at the diagonal position
                for (int i = 0; i < 32; ++i)
                {
                    if (pieces[i] != nullptr && pieces[i]->getPosition() == Vector2f(oldPos.x + 100, oldPos.y + 100) && pieces[i]->getVisible())
                    {
                        if (pieces[i]->getTexture() != this->getTexture() && pieces[i]->getVisible())
                        {
                            if (pieces[i]->getColorCode() != getColorCode() && pieces[i]->getVisible())
                            {
                                cout << "Valid move: Diagonal attack." << endl;
                                // Mark the piece at the diagonal position as invisible
                                pieces[i]->setVisible(false);
                                delete pieces[i]; // Free memory of the captured piece
                                pieces[i] = nullptr;
                                return true;
                            }
                        }
                    }
                }
            }

            if ((newPos.x == oldPos.x - 100) && newPos.y == oldPos.y + 100)
            {
                // Check if there is an opponent's piece at the diagonal position
                for (int i = 0; i < 32; ++i)
                {
                    if (pieces[i] != nullptr && pieces[i]->getPosition() == Vector2f(oldPos.x - 100, oldPos.y + 100) && pieces[i]->getVisible())
                    {
                        if (pieces[i]->getTexture() != this->getTexture() && pieces[i]->getVisible())
                        {
                            if (pieces[i]->getColorCode() != getColorCode() && pieces[i]->getVisible())
                            {
                                cout << "Valid move: Diagonal attack." << endl;
                                // Mark the piece at the diagonal position as invisible
                                pieces[i]->setVisible(false);
                                delete pieces[i]; // Free memory of the captured piece
                                pieces[i] = nullptr;
                                return true;
                            }
                        }
                    }
                }
            }
        }

        else
        {
            // Check if the new position is one square forward
            if (newPos.x == oldPos.x && newPos.y == oldPos.y - 100)
            {
                // Check if the square in front of the pawn is occupied by another piece
                for (int i = 0; i < 32; ++i)
                {
                    if (pieces[i] != nullptr && pieces[i]->getPosition() == Vector2f(newPos.x, newPos.y) && pieces[i]->getVisible())
                    {
                        cout << "Invalid move: Piece blocking the way!" << endl;
                        return false;
                    }
                }

                // If the loop completes without finding any piece at the new position, the move is valid
                cout << "Valid move: One square forward." << endl;
                return true;
            }


            // Check if the new position is two squares forward (only valid from the starting position)
            if (oldPos.y == 600 && newPos.x == oldPos.x && newPos.y == oldPos.y - 200)
            {
                // Check if the square in front of the pawn is occupied by another piece
                for (int i = 0; i < 32; ++i)
                {
                    if (pieces[i] != nullptr && pieces[i]->getPosition() == Vector2f(newPos.x, newPos.y) && pieces[i]->getVisible())
                    {
                        cout << "Invalid move: Piece blocking the way!" << endl;
                        return false;
                    }
                }

                // If the loop completes without finding any piece at the new position, the move is valid
                cout << "Valid move: Two squares forward." << endl;
                return true;
            }

            if ((newPos.x == oldPos.x + 100) && newPos.y == oldPos.y - 100)
            {
                // Check if there is an opponent's piece at the diagonal position
                for (int i = 0; i < 32; ++i)
                {
                    if (pieces[i] != nullptr && pieces[i]->getPosition() == Vector2f(oldPos.x + 100, oldPos.y - 100) && pieces[i]->getVisible())
                    {
                        if (pieces[i]->getTexture() != this->getTexture() && pieces[i]->getVisible())
                        {
                            if (pieces[i]->getColorCode() != getColorCode() && pieces[i]->getVisible())
                            {
                                cout << "Valid move: Diagonal attack." << endl;
                                // Mark the piece at the diagonal position as invisible
                                pieces[i]->setVisible(false);
                                delete pieces[i]; // Free memory of the captured piece
                                pieces[i] = nullptr;
                                return true;

                            }
                        }
                    }
                }
            }

            if ((newPos.x == oldPos.x - 100) && newPos.y == oldPos.y - 100)
            {
                // Check if there is an opponent's piece at the diagonal position
                for (int i = 0; i < 32; ++i)
                {
                    if (pieces[i] != nullptr && pieces[i]->getPosition() == Vector2f(oldPos.x - 100, oldPos.y - 100) && pieces[i]->getVisible())
                    {
                        if (pieces[i]->getTexture() != this->getTexture() && pieces[i]->getVisible())
                        {
                            if (pieces[i]->getColorCode() != getColorCode() && pieces[i]->getVisible())
                            {
                                cout << "Valid move: Diagonal attack." << endl;
                                // Mark the piece at the diagonal position as invisible
                                pieces[i]->setVisible(false);
                                delete pieces[i]; // Free memory of the captured piece
                                pieces[i] = nullptr;
                                return true;
                            }
                        }
                    }
                }
            }
        }

        return false;
    }
    string getType()const override {
        return "Pawn";
    }

};

class Rook : public Piece
{
public:
    // Default constructor
    Rook() {}

    // Constructor with parameters
    Rook(Texture& texture, float scale, Vector2f position, bool colorCode) : Piece(texture, scale, position, colorCode)
    {

    }

    // Method to override move function for rook
    bool validMove(Vector2f oldPos, Vector2f newPos, Piece* pieces[]) override
    {
        // Check if the new position is out of bounds
        if (newPos.x < 0 || newPos.x >= 800 || newPos.y < 0 || newPos.y >= 800)
        {
            cout << "Invalid move: Position out of bounds!" << endl;
            return false;
        }

        // Check if the new position is the same as the old position
        if (newPos == oldPos)
        {
            cout << "Invalid move: Same position!" << endl;
            return false;
        }

        // Check if the new position is on the same row or column as the old position
        if (newPos.x != oldPos.x && newPos.y != oldPos.y)
        {
            cout << "Invalid move: Rook can only move horizontally or vertically!" << endl;
            return false;
        }

        // Check if there are any pieces blocking the path
        float x = oldPos.x;
        float y = oldPos.y;
        bool captureOccurred = false; // Flag to indicate if a capture has occurred

        while (x != newPos.x || y != newPos.y)
        {
            // Move along the path
            if (x != newPos.x)
            {
                x += (newPos.x > oldPos.x) ? 100.0f : -100.0f;
            }
            if (y != newPos.y)
            {
                y += (newPos.y > oldPos.y) ? 100.0f : -100.0f;
            }

            for (int i = 0; i < 32; ++i)
            {
                if (pieces[i]!=nullptr&&pieces[i]->getPosition() == Vector2f(x, y))
                {
                    if (pieces[i]->getColorCode() == getColorCode())
                    {
                        // A piece of the same color is blocking the path
                        cout << "Invalid move: Piece of the same color blocking the way at (" << x << ", " << y << ")" << endl;
                        return false;
                    }
                    else
                    {
                        // Set the piece at the new position to invisible only if no capture has occurred yet
                        if (!captureOccurred)
                        {
                            pieces[i]->setVisible(false);
                            delete pieces[i]; // Free memory of the captured piece
                            pieces[i] = nullptr;
                            captureOccurred = true;
                        }
                        else
                        {
                            // If a capture has already occurred, the rook cannot move further
                            cout << "Invalid move: Rook cannot move past captured piece at (" << x << ", " << y << ")" << endl;
                            return false;
                        }
                    }
                }
            }
        }

        // If no pieces are blocking the path, or a capture has occurred, the move is valid
        cout << "Valid move: Rook moves from (" << oldPos.x << ", " << oldPos.y << ") to (" << newPos.x << ", " << newPos.y << ")" << endl;
        return true;
    }
    string getType()const override {
        return "Rook";
    }
};

class Knight : public Piece
{
public:
    // Default constructor
    Knight() {}

    // Constructor with parameters
    Knight(Texture& texture, float scale, Vector2f position, bool colorCode) : Piece(texture, scale, position, colorCode)
    {

    }

    // Method to override move function for knight
    bool validMove(Vector2f oldPos, Vector2f newPos, Piece* pieces[]) override
    {
        // Check if the new position is out of bounds
        if (newPos.x < 0 || newPos.x >= 800 || newPos.y < 0 || newPos.y >= 800)
        {
            cout << "Invalid move: Position out of bounds!" << endl;
            return false;
        }

        // Check if the new position is the same as the old position
        if (newPos == oldPos)
        {
            cout << "Invalid move: Same position!" << endl;
            return false;
        }

        // Calculate the horizontal and vertical distance between old and new positions
        float dx = abs(newPos.x - oldPos.x);
        float dy = abs(newPos.y - oldPos.y);

        // Check if the move is L-shaped (2 squares in one direction, 1 square in the other)
        if ((dx == 100 && dy == 200) || (dx == 200 && dy == 100))
        {
            // If the move is valid, check if there is no piece at the new position or if it's of the opposite color
            for (int i = 0; i < 32; ++i)
            {
                if (pieces[i]!=nullptr&&pieces[i]->getPosition() == newPos)
                {
                    if (pieces[i]->getColorCode() == getColorCode())
                    {
                        cout << "Invalid move: Piece of the same color blocking the way!" << endl;
                        return false;
                    }
                    else
                    {
                        // Set the piece at the new position to invisible
                        pieces[i]->setVisible(false);
                        delete pieces[i]; // Free memory of the captured piece
                        pieces[i] = nullptr;
                        break;
                    }
                }
            }

            // If no pieces are blocking the way, the move is valid
            cout << "Valid move: Knight moves from (" << oldPos.x << ", " << oldPos.y << ") to (" << newPos.x << ", " << newPos.y << ")" << endl;
            return true;
        }
        else
        {
            cout << "Invalid move: Knight moves in an L-shape!" << endl;
            return false;
        }
    }
    string getType()const override {
        return "Knight";
    }
};

class Bishop : public Piece
{
public:
    // Default constructor
    Bishop() {}

    // Constructor with parameters
    Bishop(Texture& texture, float scale, Vector2f position, bool colorCode) : Piece(texture, scale, position, colorCode)
    {

    }

    // Method to override move function for bishop
    bool validMove(Vector2f oldPos, Vector2f newPos, Piece* pieces[]) override
    {
        // Check if the new position is out of bounds
        if (newPos.x < 0 || newPos.x >= 800 || newPos.y < 0 || newPos.y >= 800)
        {
            cout << "Invalid move: Position out of bounds!" << endl;
            return false;
        }

        // Check if the new position is the same as the old position
        if (newPos == oldPos)
        {
            cout << "Invalid move: Same position!" << endl;
            return false;
        }

        // Calculate the horizontal and vertical distance between old and new positions
        float dx = abs(newPos.x - oldPos.x);
        float dy = abs(newPos.y - oldPos.y);

        // Check if the move is diagonally (equal horizontal and vertical distance)
        if (dx == dy)
        {
            // Determine the direction of movement
            float xStep = (newPos.x > oldPos.x) ? 100.0f : -100.0f;
            float yStep = (newPos.y > oldPos.y) ? 100.0f : -100.0f;

            // Iterate over the path to the new position
            float x = oldPos.x;
            float y = oldPos.y;
            while (Vector2f(x, y) != newPos)
            {
                // Move to the next position
                x += xStep;
                y += yStep;

                // Check if there is a piece in the path
                for (int i = 0; i < 32; ++i)
                {
                    if (pieces[i]!=nullptr&&pieces[i]->getPosition() == Vector2f(x, y))
                    {
                        // If the piece is of the same color, block the move
                        if (pieces[i]->getColorCode() == this->getColorCode())
                        {
                            cout << "Invalid move: Piece of the same color blocking the way!" << endl;
                            return false;
                        }
                        // If the piece is of a different color, make it invisible and continue the move
                        else
                        {
                            cout << "Valid move: Bishop captures the opponent's piece at (" << x << ", " << y << ")" << endl;
                            pieces[i]->setVisible(false);
                            delete pieces[i]; // Free memory of the captured piece
                            pieces[i] = nullptr;
                        }
                    }
                }
            }

            // If no pieces are blocking the way, the move is valid
            cout << "Valid move: Bishop moves from (" << oldPos.x << ", " << oldPos.y << ") to (" << newPos.x << ", " << newPos.y << ")" << endl;
            return true;
        }
        else
        {
            cout << "Invalid move: Bishop moves diagonally!" << endl;
            return false;
        }
    }
    string getType()const override {
        return "Bishop";
    }
};

class Queen : public Piece
{
public:
    // Default constructor
    Queen() {}

    // Constructor with parameters
    Queen(Texture& texture, float scale, Vector2f position, bool colorCode) : Piece(texture, scale, position, colorCode)
    {

    }

    // Method to override move function for queen
    bool validMove(Vector2f oldPos, Vector2f newPos, Piece* pieces[]) override
    {
        // Check if the new position is out of bounds
        if (newPos.x < 0 || newPos.x >= 800 || newPos.y < 0 || newPos.y >= 800)
        {
            cout << "Invalid move: Position out of bounds!" << endl;
            return false;
        }

        // Check if the new position is the same as the old position
        if (newPos == oldPos)
        {
            cout << "Invalid move: Same position!" << endl;
            return false;
        }

        // Calculate the horizontal and vertical distance between old and new positions
        float dx = abs(newPos.x - oldPos.x);
        float dy = abs(newPos.y - oldPos.y);

        // Check if the move is horizontally, vertically, or diagonally
        if (dx == 0 || dy == 0 || dx == dy)
        {
            // If the move is horizontally or vertically, check if it's blocked
            if (dx == 0 || dy == 0)
            {
                float step = (dx == 0) ? ((newPos.y > oldPos.y) ? 100.0f : -100.0f) : ((newPos.x > oldPos.x) ? 100.0f : -100.0f);
                float x = oldPos.x;
                float y = oldPos.y;
                while (Vector2f(x, y) != newPos)
                {
                    x += (dx == 0) ? 0 : step;
                    y += (dy == 0) ? 0 : step;
                    for (int i = 0; i < 32; ++i)
                    {
                        if (pieces[i]!=nullptr&&pieces[i]->getPosition() == Vector2f(x, y))
                        {
                            // If the piece is of the same color, block the move
                            if (pieces[i]->getColorCode() == this->getColorCode())
                            {
                                cout << "Invalid move: Piece of the same color blocking the way!" << endl;
                                return false;
                            }
                            // If the piece is of a different color, make it invisible and continue the move
                            else
                            {
                                cout << "Valid move: Queen captures the opponent's piece at (" << x << ", " << y << ")" << endl;
                                    pieces[i]->setVisible(false);
                                    delete pieces[i]; // Free memory of the captured piece
                                    pieces[i] = nullptr;
                            }
                        }
                    }
                }
            }
            // If the move is diagonally, check if it's blocked
            else
            {
                float xStep = (newPos.x > oldPos.x) ? 100.0f : -100.0f;
                float yStep = (newPos.y > oldPos.y) ? 100.0f : -100.0f;
                float x = oldPos.x;
                float y = oldPos.y;
                while (Vector2f(x, y) != newPos)
                {
                    x += xStep;
                    y += yStep;
                    for (int i = 0; i < 32; ++i)
                    {
                        if (pieces[i]!=nullptr&&pieces[i]->getPosition() == Vector2f(x, y))
                        {
                            // If the piece is of the same color, block the move
                            if (pieces[i]->getColorCode() == this->getColorCode())
                            {
                                cout << "Invalid move: Piece of the same color blocking the way!" << endl;
                                return false;
                            }
                            // If the piece is of a different color, make it invisible and continue the move
                            else
                            {
                                cout << "Valid move: Queen captures the opponent's piece at (" << x << ", " << y << ")" << endl;
                                pieces[i]->setVisible(false);
                                delete pieces[i]; // Free memory of the captured piece
                                pieces[i] = nullptr;
                            }
                        }
                    }
                }
            }

            // If no pieces are blocking the way, the move is valid
            cout << "Valid move: Queen moves from (" << oldPos.x << ", " << oldPos.y << ") to (" << newPos.x << ", " << newPos.y << ")" << endl;
            return true;
        }
        else
        {
            cout << "Invalid move: Queen moves horizontally, vertically, or diagonally!" << endl;
            return false;
        }
    }
    string getType()const override {
        return "Queen";
    }
};

class King : public Piece
{
public:
    // Default constructor
    King() {}

    // Constructor with parameters
    King(Texture& texture, float scale, Vector2f position, bool colorCode) : Piece(texture, scale, position, colorCode)
    {

    }

    // Method to override move function for king
    bool validMove(Vector2f oldPos, Vector2f newPos, Piece* pieces[]) override
    {
        // Check if the new position is out of bounds
        if (newPos.x < 0 || newPos.x >= 800 || newPos.y < 0 || newPos.y >= 800)
        {
            cout << "Invalid move: Position out of bounds!" << endl;
            return false;
        }

        // Check if the new position is the same as the old position
        if (newPos == oldPos)
        {
            cout << "Invalid move: Same position!" << endl;
            return false;
        }

        // Calculate the horizontal and vertical distance between old and new positions
        float dx = abs(newPos.x - oldPos.x);
        float dy = abs(newPos.y - oldPos.y);

        // Check if the move is one square in any direction
        if ((dx == 100 && dy == 0) || (dx == 0 && dy == 100) || (dx == 100 && dy == 100))
        {
            // Check if the new position is occupied by a piece of the same color
            for (int i = 0; i < 32; ++i)
            {
                if (pieces[i]->getPosition() == newPos)
                {
                    if (pieces[i]->getColorCode() == this->getColorCode())
                    {
                        cout << "Invalid move: Piece of the same color occupies the new position!" << endl;
                        return false;
                    }
                    // If the piece is of a different color, make it invisible
                    else
                    {
                        cout << "Valid move: King captures the opponent's piece at (" << newPos.x << ", " << newPos.y << ")" << endl;
                        pieces[i]->setVisible(false);
                        delete pieces[i]; // Free memory of the captured piece
                        pieces[i] = nullptr;
                    }
                }
            }

            // If the new position is not occupied, the move is valid
            cout << "Valid move: King moves from (" << oldPos.x << ", " << oldPos.y << ") to (" << newPos.x << ", " << newPos.y << ")" << endl;
            return true;
        }
        else
        {
            cout << "Invalid move: King moves one square in any direction!" << endl;
            return false;
        }
    }

    string getType()const override
    {
        return "King";
    }
};

int displayGameOverWindow(int currentPlayerColor)
{

    // Load music
    Music music;
    if (!music.openFromFile("C:\\Users\\DELL\\OneDrive\\Pictures\\Saved Pictures\\gameOver.wav")) {
        // Handle error loading music file
        return false;
    }
    music.play(); // Play music when ending the game
    // Create a new RenderWindow for the game over window
    RenderWindow gameOverWindow(sf::VideoMode(800, 600), "Game Over\n");

    // Create a font for the text
    Font font;
    if (!font.loadFromFile("C:\\Users\\DELL\\OneDrive\\Pictures\\Saved Pictures\\font.ttf"))
    {
        // Handle font loading failure
        return 0;
    }

    // Create a Text object to display the game over message
    sf::Text gameOverText;
    gameOverText.setFont(font);
    gameOverText.setCharacterSize(25);
    gameOverText.setFillColor(sf::Color::Red);

    // Set the game over message based on the current player color
    if (currentPlayerColor == 0) {
        gameOverText.setString("GAME OVER\nCongratulations! Player 2 (Black) wins!");
    }
    else {
        gameOverText.setString("GAME OVER\nCongratulations! Player 1 (White) wins!");
    }
    // Center the text on the window
    gameOverText.setPosition((gameOverWindow.getSize().x - gameOverText.getLocalBounds().width) / 2,
        (gameOverWindow.getSize().y - gameOverText.getLocalBounds().height) / 2);

    // Main loop for the game over window
    while (gameOverWindow.isOpen()) {
        sf::Event event;
        while (gameOverWindow.pollEvent(event))
        {
            if (event.type == sf::Event::Closed) {
                gameOverWindow.close();
            }
        }

        // Clear the window
        gameOverWindow.clear();

        // Draw the game over text
        gameOverWindow.draw(gameOverText);

        // Display the window
        gameOverWindow.display();
    }
    return 0;
}
bool isKingCaptured(Piece* pieces[], int numPieces) {
    int found = 0;
    for (int i = 0; i < 32; ++i) {
       
        if (pieces[i] != nullptr && pieces[i]->getType() == "King") 
        {
           found++; // King is captured
        }
    }
    if (found == 1)
       return true;
    return false; // King is not captured
}

int selectedPieceIndex;

int main()
{
    RenderWindow window(sf::VideoMode(800, 800), "Chess Game");

    bool startGame = displayStartingPage(window);

    if (!startGame)
    {
        window.close();
        return 0;
    }
    cout << "------WHITE AS PLAYER 1 AND BLACK AS PLAYER 2-----" << endl;
    cout << "---------FIRST TURN IS OF PLAYER 1(WHITE)---------" << endl;
    // Load images into the array
    Texture images[32];
    if (!images[0].loadFromFile("C:\\Users\\DELL\\OneDrive\\Pictures\\Saved Pictures\\br.png") ||
        !images[1].loadFromFile("C:\\Users\\DELL\\OneDrive\\Pictures\\Saved Pictures\\bn.png") ||
        !images[2].loadFromFile("C:\\Users\\DELL\\OneDrive\\Pictures\\Saved Pictures\\bb.png") ||
        !images[3].loadFromFile("C:\\Users\\DELL\\OneDrive\\Pictures\\Saved Pictures\\bq.png") ||
        !images[4].loadFromFile("C:\\Users\\DELL\\OneDrive\\Pictures\\Saved Pictures\\bk.png") ||
        !images[5].loadFromFile("C:\\Users\\DELL\\OneDrive\\Pictures\\Saved Pictures\\bb.png") ||
        !images[6].loadFromFile("C:\\Users\\DELL\\OneDrive\\Pictures\\Saved Pictures\\bn.png") ||
        !images[7].loadFromFile("C:\\Users\\DELL\\OneDrive\\Pictures\\Saved Pictures\\br.png") ||
        !images[8].loadFromFile("C:\\Users\\DELL\\OneDrive\\Pictures\\Saved Pictures\\bp.png") ||
        !images[9].loadFromFile("C:\\Users\\DELL\\OneDrive\\Pictures\\Saved Pictures\\bp.png") ||
        !images[10].loadFromFile("C:\\Users\\DELL\\OneDrive\\Pictures\\Saved Pictures\\bp.png") ||
        !images[11].loadFromFile("C:\\Users\\DELL\\OneDrive\\Pictures\\Saved Pictures\\bp.png") ||
        !images[12].loadFromFile("C:\\Users\\DELL\\OneDrive\\Pictures\\Saved Pictures\\bp.png") ||
        !images[13].loadFromFile("C:\\Users\\DELL\\OneDrive\\Pictures\\Saved Pictures\\bp.png") ||
        !images[14].loadFromFile("C:\\Users\\DELL\\OneDrive\\Pictures\\Saved Pictures\\bp.png") ||
        !images[15].loadFromFile("C:\\Users\\DELL\\OneDrive\\Pictures\\Saved Pictures\\bp.png") ||
        !images[16].loadFromFile("C:\\Users\\DELL\\OneDrive\\Pictures\\Saved Pictures\\wp.png") ||
        !images[17].loadFromFile("C:\\Users\\DELL\\OneDrive\\Pictures\\Saved Pictures\\wp.png") ||
        !images[18].loadFromFile("C:\\Users\\DELL\\OneDrive\\Pictures\\Saved Pictures\\wp.png") ||
        !images[19].loadFromFile("C:\\Users\\DELL\\OneDrive\\Pictures\\Saved Pictures\\wp.png") ||
        !images[20].loadFromFile("C:\\Users\\DELL\\OneDrive\\Pictures\\Saved Pictures\\wp.png") ||
        !images[21].loadFromFile("C:\\Users\\DELL\\OneDrive\\Pictures\\Saved Pictures\\wp.png") ||
        !images[22].loadFromFile("C:\\Users\\DELL\\OneDrive\\Pictures\\Saved Pictures\\wp.png") ||
        !images[23].loadFromFile("C:\\Users\\DELL\\OneDrive\\Pictures\\Saved Pictures\\wp.png") ||
        !images[24].loadFromFile("C:\\Users\\DELL\\OneDrive\\Pictures\\Saved Pictures\\wr.png") ||
        !images[25].loadFromFile("C:\\Users\\DELL\\OneDrive\\Pictures\\Saved Pictures\\wn.png") ||
        !images[26].loadFromFile("C:\\Users\\DELL\\OneDrive\\Pictures\\Saved Pictures\\wb.png") ||
        !images[27].loadFromFile("C:\\Users\\DELL\\OneDrive\\Pictures\\Saved Pictures\\wq.png") ||
        !images[28].loadFromFile("C:\\Users\\DELL\\OneDrive\\Pictures\\Saved Pictures\\wk.png") ||
        !images[29].loadFromFile("C:\\Users\\DELL\\OneDrive\\Pictures\\Saved Pictures\\wb.png") ||
        !images[30].loadFromFile("C:\\Users\\DELL\\OneDrive\\Pictures\\Saved Pictures\\wn.png") ||
        !images[31].loadFromFile("C:\\Users\\DELL\\OneDrive\\Pictures\\Saved Pictures\\wr.png"))

    {
        // Handle loading failure
        return 0;
    }
    // Create the pieces collection
    Piece* pieces[32];

    // Allocate pieces for black pieces (0 to 15)
    pieces[0] = new Rook(images[0], 0.3f, Vector2f(0, 0), 0);
    pieces[1] = new Knight(images[1], 0.3f, Vector2f(100, 0), 0);
    pieces[2] = new Bishop(images[2], 0.3f, Vector2f(200, 0), 0);
    pieces[3] = new Queen(images[3], 0.3f, Vector2f(300, 0), 0);
    pieces[4] = new King(images[4], 0.3f, Vector2f(400, 0), 0);
    pieces[5] = new Bishop(images[5], 0.3f, Vector2f(500, 0), 0);
    pieces[6] = new Knight(images[6], 0.3f, Vector2f(600, 0), 0);
    pieces[7] = new Rook(images[7], 0.3f, Vector2f(700, 0), 0);

    // Allocate pieces for black pawns (index 8 to 15)
    for (int i = 8; i < 16; ++i)
        pieces[i] = new Pawn(images[i], 0.3f, Vector2f((i % 8) * 100, 100), 0);

    // Allocate pieces for white pawns (index 24 to 31)
    for (int i = 16; i < 24; ++i)
        pieces[i] = new Pawn(images[i], 0.3f, Vector2f((i % 8) * 100, 600), 1);

    // Allocate pieces for white pieces (16 to 31)
    pieces[24] = new Rook(images[24], 0.3f, Vector2f(0, 700), 1);
    pieces[25] = new Knight(images[25], 0.3f, Vector2f(100, 700), 1);
    pieces[26] = new Bishop(images[26], 0.3f, Vector2f(200, 700), 1);
    pieces[27] = new Queen(images[27], 0.3f, Vector2f(300, 700), 1);
    pieces[28] = new King(images[28], 0.3f, Vector2f(400, 700), 1);
    pieces[29] = new Bishop(images[29], 0.3f, Vector2f(500, 700), 1);
    pieces[30] = new Knight(images[30], 0.3f, Vector2f(600, 700), 1);
    pieces[31] = new Rook(images[31], 0.3f, Vector2f(700, 700), 1);



    // Create the chessboard
    Chessboard chessboard;

    // Create the highlighter
    Highlighter highlighter;
    highlighter.setPosition(0, 0); // Initialize at the top-left corner


    bool enterKeyPressed = false;
    Vector2f highlighterInitialPosition; // Store initial position of highlighter
    int currentPlayer = 1;
    Music music;
    if (!music.openFromFile("C:\\Users\\DELL\\OneDrive\\Pictures\\Saved Pictures\\move.wav")) {
        // Handle error loading music file
        return false;
    }
    // Main loop
    while (window.isOpen()) {
        Event event;
        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed)
                window.close();

            else if (event.type == Event::KeyPressed)
            {
                // Move the highlighter using keyboard inputs
                switch (event.key.code)
                {
                case Keyboard::Up:
                    highlighter.moveUp();
                    break;
                case Keyboard::Down:
                    highlighter.moveDown();
                    break;
                case Keyboard::Left:
                    highlighter.moveLeft();
                    break;
                case Keyboard::Right:
                    highlighter.moveRight();
                    break;
                case Keyboard::Enter:
                    // Highlight
                    if (!enterKeyPressed)
                    {
                        // Store initial position of highlighter on first enter press
                        highlighterInitialPosition = highlighter.getPosition();
                        enterKeyPressed = true;

                        // Determine which piece is selected based on the highlighter's initial position
                        for (int i = 0; i < 32; ++i)
                        {
                            if (pieces[i]->getPosition() == highlighterInitialPosition)
                            {
                                selectedPieceIndex = i;
                                break;
                            }
                        }
                    }
                    else
                    {
                        // Move the piece to the new position if a valid move is selected
                        for (int i = 0; i < 32; ++i)
                        {
                            if (pieces[i]->getPosition() == highlighterInitialPosition && pieces[i]->getColorCode() == currentPlayer &&
                                pieces[selectedPieceIndex]->validMove(pieces[i]->getPosition(), highlighter.getPosition(), pieces))
                            {
                                pieces[selectedPieceIndex]->setPosition(highlighter.getPosition().x, highlighter.getPosition().y); // Move the piece
                                cout << "mUSIC IS PLAYING\n";
                                music.play(); // Play music when moving the piece
                                if (currentPlayer == 0) {
                                    cout << "CURRENT TURN: PLAYER 1(WHITE)" << endl;
                                }
                                else {
                                    cout << "CURRENT TURN: PLAYER 2(BLACK)" << endl;
                                }
                                //Check if the opponent's king is captured
                                if (isKingCaptured(pieces, 32) && pieces[i]->getColorCode() == currentPlayer)
                                {
                                    // Display game over message
                                  displayGameOverWindow(currentPlayer); // Pass the current player's color code
                                  window.close(); // Close the main game window
                                  return 0; // Exit the main function
                                }
                                // Switch to the next player's turn
                                currentPlayer = (currentPlayer + 1) % 2;


                                //Pawn Promotion//
                                if (pieces[i]->getType() == "Pawn" &&
                                    ((pieces[i]->getColorCode() == 0 && pieces[i]->getPosition().y == 700) ||
                                        (pieces[i]->getColorCode() == 1 && pieces[i]->getPosition().y == 0)))
                                {
                                    // Replace the pawn with the selected promoted piece
                                    Vector2f origPos = pieces[i]->getPosition();
                                    int origColor = pieces[i]->getColorCode();
                                    delete pieces[i]; // Free memory of the pawn

                                    char promotionChoice; // Variable to store user's promotion choice
                                    cout << "Pawn Promotion: Enter Q for Queen, R for Rook, B for Bishop, or K for Knight: ";
                                    cin >> promotionChoice;

                                    switch (promotionChoice)
                                    {
                                    case 'Q': // Queen
                                        if (currentPlayer == 0)
                                        {
                                            pieces[i] = new Queen(images[27], 0.3f, origPos, origColor);
                                            cout << "White Pawn has been converted into White Queen" << endl;
                                        }
                                        else
                                        {
                                            pieces[i] = new Queen(images[3], 0.3f, origPos, origColor);
                                            cout << "Black Pawn has been converted into Black Queen" << endl;
                                        }
                                        break;
                                    case 'R': // Rook
                                        if (currentPlayer == 0)
                                        {
                                            pieces[i] = new Rook(images[31], 0.3f, origPos, origColor);
                                            cout << "White Pawn has been converted into White Rook" << endl;
                                        }
                                        else
                                        {
                                            pieces[i] = new Rook(images[7], 0.3f, origPos, origColor);
                                            cout << "Black Pawn has been converted into Black Rook" << endl;
                                        }
                                        break;
                                    case 'B': // Bishop
                                        if (currentPlayer == 0)
                                        {
                                            pieces[i] = new Bishop(images[29], 0.3f, origPos, origColor);
                                            cout << "White Pawn has been converted into White Bishop" << endl;
                                        }
                                        else
                                        {
                                            pieces[i] = new Bishop(images[5], 0.3f, origPos, origColor);
                                            cout << "Black Pawn has been converted into Black Bishop" << endl;
                                        }
                                        break;
                                    case 'K': // Knight
                                        if (currentPlayer == 0)
                                        {
                                            pieces[i] = new Knight(images[30], 0.3f, origPos, origColor);
                                            cout << "White Pawn has been converted into White Knight" << endl;
                                        }
                                        else
                                        {
                                            pieces[i] = new Knight(images[6], 0.3f, origPos, origColor);
                                            cout << "Black Pawn has been converted into Black Knight" << endl;
                                        }
                                        break;
                                    default:
                                        cout << "Invalid choice. Pawn remains unchanged." << endl;
                                        // Create a default behavior, like promoting to a queen
                                        if (currentPlayer == 0)
                                        {
                                            pieces[i] = new Queen(images[27], 0.3f, origPos, origColor);
                                            cout << "White Pawn has been converted into White Queen by default" << endl;
                                        }
                                        else
                                        {
                                            pieces[i] = new Queen(images[3], 0.3f, origPos, origColor);
                                            cout << "Black Pawn has been converted into Black Queen by default" << endl;
                                        }
                                        break;
                                    }
                                }
                                break;
                            }
                        }
                        // Reset enterKeyPressed
                        enterKeyPressed = false;
                    }
                    break;
                default:
                    break;
                }
            }
        }
        window.clear();
        // Draw the chessboard
        chessboard.draw(window);

        // Draw the pieces
        for (int i = 0; i < 32; ++i)
        {
            pieces[i]->draw(window);
        }
        // Draw the highlighter
        highlighter.draw(window);
        window.display();
    }
    // Delete dynamically allocated pieces
    for (int i = 0; i < 32; ++i) {
        delete pieces[i];
    }
    // Reset pieces array to nullptr
    memset(pieces, 0, sizeof(pieces));
    return 0;
}
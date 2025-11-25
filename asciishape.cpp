#include <iostream> 
#include <vector> 
#include <string> 
#include <cmath> 
#include <memory> // For std::unique_ptr
#include <iomanip> // For std::setw (formatting numbers)

// ==========================================
// 0. ANSI Color Codes & Pixel Structure
// ==========================================
namespace Color { //namespace for color codes
    const std::string RESET = "\033[0m";
    const std::string GRAY = "\033[90m";
    const std::string WHITE = "\033[37m";
    const std::string RED = "\033[31m";
    const std::string GREEN = "\033[32m";
    const std::string BLUE = "\033[34m";
    const std::string YELLOW = "\033[33m";
    const std::string CYAN = "\033[36m";
    const std::string PINK = "\033[95m";
}

struct Pixel { // user defined datatype that represents a single pixel on the canvas containing a character and its color
    char symbol;
    std::string colorCode;
};

// ==========================================
// 1. Canvas Class (encapsulation)
// ==========================================
class Canvas { // manages the drawing area
private: // Encapsulation
    int width, height;
    std::vector<std::vector<Pixel>> buffer; // 2d vector called buffer is used to store the current state of the canvas

public:
    Canvas(int w, int h) : width(w), height(h) { // Constructor used to initialize data members
        buffer.resize(height, std::vector<Pixel>(width)); // Resize buffer to height x width. buffer can now be called as buffer[row][column]
        clear(); 
    }

    void clear() {
        Pixel emptyPixel = {'.', Color::GRAY};
        for (int i = 0; i < height; ++i) {
            for(int j = 0; j < width; ++j) {
                buffer[i][j] = emptyPixel; // Fill with empty pixel
            }
        }
    }

    void drawPoint(int x, int y, char brush, const std::string& color) { // Draw a point if within bounds
        if (x >= 0 && x < width && y >= 0 && y < height) {
            buffer[y][x] = {brush, color};
        }
    }

    void render() const { // clears the console and renders the canvas with rulers
        #ifdef _WIN32 
            system("cls");
        #else
            system("clear");
        #endif

        // --- RENDER X-AXIS RULER (top) ---
        std::cout << "    "; // offset for Y-axis numbers
        for (int i = 0; i < width; ++i) { //iterating through each column
            if (i % 10 == 0) std::cout << Color::GRAY << (i) << Color::RESET;
            else std::cout << " ";
        }
        std::cout << "\n";

        // --- RENDER Y-AXIS RULER (left) ---
        for (int i = 0; i < height; ++i) {
            // %2d ensures single digits are padded with a space for alignment
            std::cout << Color::GRAY << std::setw(2) << i << ": " << Color::RESET;
            
            for (int j = 0; j < width; ++j) { 
                std::cout << buffer[i][j].colorCode << buffer[i][j].symbol << Color::RESET;
            }
            std::cout << "\n";
        }
    }
    int getWidth() const { return width; } // Getter for width
    int getHeight() const { return height; } // Getter for height
};

// ==========================================
// 2. Shape Base Class (abstraction/polymorphism)
// ==========================================
class Shape {
protected: //can be accessed by derived classes
    int x, y;
    char brush;
    std::string color; 
public:
    Shape(int x, int y, char b, std::string c) : x(x), y(y), brush(b), color(c) {} // Constructor
    virtual ~Shape() {} // virtual destructor to access derived class destructors
    virtual void draw(Canvas& canvas) const = 0; // Pure virtual function making Shape an abstract class
};

// ==========================================
// 3. Derived Classes (inheritance)
// ==========================================
class Rectangle : public Shape {
    int width, height;
public:
    Rectangle(int x, int y, int w, int h, char b, std::string c) 
        : Shape(x, y, b, c), width(w), height(h) {} // Constructor that initializes base Shape class

    void draw(Canvas& canvas) const override { // Override draw method
        for (int i = 0; i < height; ++i) { //row
            for (int j = 0; j < width; ++j) { //column
                canvas.drawPoint(x + j, y + i, brush, color); // Draw each point of the rectangle
            }
        }
    }
};

class Circle : public Shape {
    int radius;
public:
    Circle(int cx, int cy, int r, char b, std::string c) 
        : Shape(cx, cy, b, c), radius(r) {}

    void draw(Canvas& canvas) const override {
        float aspectRatio = 0.8f; // adjust for character height vs width
        for (int i = -radius; i <= radius; ++i) { //rows
            for (int j = -radius; j <= radius; ++j) { //columns
                if ((j*j*aspectRatio) + (i*i) <= (radius*radius)) { // circle equation x2+y2=r2
                    canvas.drawPoint(x + j, y + i, brush, color);
                }
            }
        }
    }
};

class Triangle : public Shape {
    int height;
public:
    Triangle(int tip_x, int tip_y, int h, char b, std::string c) 
        : Shape(tip_x, tip_y, b, c), height(h) {}

    void draw(Canvas& canvas) const override {
        for (int i = 0; i < height; ++i) {
            for (int j = -i; j <= i; ++j) {
                canvas.drawPoint(x + j, y + i, brush, color); 
            }
        }
    }
};

// ==========================================
// Helper Functions
// ==========================================
std::string chooseColor() {
    int cChoice;
    std::cout << "Color [1.Red 2.Green 3.Blue 4.Yel 5.Cyn 6.Wht]: ";
    std::cin >> cChoice;
    switch(cChoice) {
        case 1: return Color::RED;
        case 2: return Color::GREEN;
        case 3: return Color::BLUE;
        case 4: return Color::YELLOW;
        case 5: return Color::CYAN;
        default: return Color::WHITE;
    }
}

// ==========================================
// Main
// ==========================================
int main() {
    const int CANVAS_W = 60; // Adjustable width
    const int CANVAS_H = 25; // Adjustable height

    Canvas myCanvas(CANVAS_W, CANVAS_H); // Create canvas object
    std::vector<std::unique_ptr<Shape>> layers; // Vector to hold shapes. it is a vector of unique pointers to Shape objects. // polymorphic behavior
    int choice = 0;

    // Ensure formatting works
    std::cout << std::fixed;

    while (true) {
        myCanvas.clear();
        for (const auto& shape : layers) shape->draw(myCanvas); // Draw each shape onto the canvas
        myCanvas.render();

        std::cout << "\n--- ASCII PAINTER ---\n";
        std::cout << "1. Rectangle\n";
        std::cout << "2. Circle\n";
        std::cout << "3. Triangle\n";
        std::cout << "4. Undo\n5. Clear\n6. Demo\n7. Exit\nSelect: ";
        
        if (!(std::cin >> choice)) { // Catch invalid input (letters instead of numbers)
            std::cin.clear();
            std::cin.ignore(1000, '\n');
            continue;
        }

        if (choice >= 1 && choice <= 3) {
            std::string pickedColor = chooseColor();
            
            if (choice == 1) {
                int x, y, w, h;
                std::cout << "Coords (x y w h): ";
                std::cin >> x >> y >> w >> h;
                layers.push_back(std::make_unique<Rectangle>(x, y, w, h, '#', pickedColor)); //rectangle object is created dynamically and is pointed to by a smart pointer 'unique_ptr'. this ensures that the rectangle deconstructor is called when the unique_ptr goes out of scope. 
            }
            else if (choice == 2) {
                int x, y, r;
                std::cout << "Coords (cx cy r): ";
                std::cin >> x >> y >> r;
                layers.push_back(std::make_unique<Circle>(x, y, r, '@', pickedColor));
            }
            else if (choice == 3) {
                int x, y, h;
                std::cout << "Coords (tip_x tip_y h): ";
                std::cin >> x >> y >> h;
                layers.push_back(std::make_unique<Triangle>(x, y, h, '^', pickedColor));
            }
        }
        else if (choice == 4) { 
            if (!layers.empty()) layers.pop_back(); //undo last shape
        }
        else if (choice == 5) layers.clear();
        else if (choice == 6) { 
            layers.clear(); 
            // --- DEMO SCENE: House at Sunset ---
            // 1. Sunset Background (large rectangle)
            layers.push_back(std::make_unique<Rectangle>(0, 0, CANVAS_W, CANVAS_H, '~', Color::BLUE)); // Sky
            layers.push_back(std::make_unique<Rectangle>(0, 15, CANVAS_W, CANVAS_H - 15, '#', Color::GREEN)); // Ground
            // 2. Sun (large circle, partially hidden by ground)
            layers.push_back(std::make_unique<Circle>(CANVAS_W / 2, 4, 3, 'O', Color::YELLOW));
            // 3. House Body (rectangle)
            layers.push_back(std::make_unique<Rectangle>(10, 10, 15, 8, '#', Color::WHITE));
            // 4. House Roof (triangle)
            layers.push_back(std::make_unique<Triangle>(17, 4, 6, '^', Color::RED)); 
            // 5. Door (small rectangle)
            layers.push_back(std::make_unique<Rectangle>(15, 14, 5, 4, '|', Color::CYAN)); 
            // 6. Window (small rectangle)
            layers.push_back(std::make_unique<Rectangle>(12, 11, 3, 3, '+', Color::BLUE));
            layers.push_back(std::make_unique<Rectangle>(20, 11, 3, 3, '+', Color::BLUE));
            // 7. Tree Trunk (rectangle)
            layers.push_back(std::make_unique<Rectangle>(40, 12, 3, 7, '|', Color::YELLOW)); // Brownish-yellow for trunk
            // 8. Tree Canopy (large circle)
            layers.push_back(std::make_unique<Circle>(41, 9, 5, '%', Color::GREEN));

        }
        else if (choice == 7) break;
    }
    return 0;
}
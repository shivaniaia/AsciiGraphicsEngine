# AsciiGraphicsEngine
C++ Console Application that functions as a Vector Graphics ASCII Art Engine.

Its a Drawing Program that operates entirely within the terminal, allowing users to create colored geometric shapes using ascii characters by inputing its position/coordinates on a grid and the desired dimensions. It is a Vector Graphics system, meaning shapes are stored as mathematical blueprints rather than individual pixels.

Features
- Pure OOP Architecture: Rigorously applies Abstraction, Encapsulation, Inheritance, and Polymorphism to create a highly modular and extensible system.
- Vector Rendering: Stores shapes as mathematical definitions rather than fixed pixels, enabling precise layering and rendering onto the character-based canvas.
- Memory Safety: Utilizes C++ smart pointers to manage dynamic memory allocation for all shapes
- Decoupled Design: Features a clear separation between the core shape logic and the display handling, making the system ready for future migration to a GUI.
- Real-time Output: Renders the layered scene using ANSI color codes in a continuous input-process-render loop.

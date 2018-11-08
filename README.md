# Graphics/Game Engine

### TL;DR
A game engine I was working on during the summer which is now being repurposed as a palce to build my own graphics framework for the "Advanced Graphics for Games" Module at Newcastle University 

This repo is what I managed to accomplish in the free time I had during summer. **Many** parts of engine are unfinished. 

This was mainly a learning experience and not an actual attempt to create a polished game engine. Because of this, parts of the engine are hard coded in and are definitely not best practice! For example, the scene that is loaded in is hard coded in the Graphics Node. 
This is great for quickly testing features and fancy graphics stuff, but not actual games! 



### Where I'm at now
- So far the engine can render 3D objects onto the screen using .obj files.
- Can load in random terrain using a heightmap
- simulate basics looking water by using a heightmap and Perlin Noise
- Basic phong lighting
- Basic blurring post processing effect using FBOs.
- Pass around important events in the engine by using an event system.

### Still need to implement
- A logger that logs errors and warnings in the system
- An basic Physics subsystem to move around objects (collisions and actual physics later)
- an ECS (Entity Component System) to create game objects.
- Add shadow mapping.
- Cube mapping/Refections
- Data driven system to load levels and other data
- An audio subsystem for sound. 
- And a million other things!

The following are notes about each subsystem and any incomplete code that I would like to flesh out in future.

## Initialisation
Where the main method is and handles initialisation and shutdown of the program

## Graphics
At the start of a level, a level is loaded by a class in the Game subsystem and the picked up my other subsystems such as Graphics and Physics. The graphics subsystem takes the Level and turns it into graphics objects (or RenderObjects). This is what's used to render the object onto the screen. 

### RendererNodes
An idea I had to separate various rendering technologies into different classes to avoid having a huge Renderer class. Each node could be 'attached' to a Renderer and provide a specific effect. For example the Skybox class could be added to a renderer to add a skybox into the scene. Files where created for this but never finished.

### Graphics Node
Handles communication between subsystems. This is where Levels are loaded in and turned into RenderObjects to be rendered to the screen.

## Game
Where the game logic will be. At the moment it only contains a class defining what a GameObject is, i.e a any object that is inside of the game. 

## Maths
Any mathsy class that the game might need. Things like Vector3Ds, Perlin Noise etc...

## Utilities
Useful tools that the program can use. ID generator, Logger, Timer

## EventSystem
The event system that any subsystem can be attached to. Events (or messages) can be sent from any subsystem to another subsystem. Each event can also have an optional void pointer to send data, although this probably should be avoided where possible due to all the casting. 

## Interface
Handles user input. Only include mouse and keyboard at the moment.

## FileIO
Most of the File IO code should be here. Also includes the tinyOBJ reader.

## SOIL2
The SOIL2 library, can be found here: https://bitbucket.org/SpartanJ/soil2

## Scene
Handles the different types of levels that can be present in a game. At the moment there is a basic level with position data of objects

## ECS
A not yet implemented Entity Component System.

## Physics
Handles physics information for gameObjects. A physics object should be created when a level is loaded (similar to how RenderObjects are loading in the graphics subsystem), and that should control how objects interact in 3D space. 




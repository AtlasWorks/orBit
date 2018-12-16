# orBit

orBit is a singleplayer platfomer game developed by Andres Saladrigas and Aitor Simona, two students at second year of Video Game Design and Development degree in UPC Barcelona, Spain.
The game includes two levels, with usual mechanics. player can move right, left and jump 
The game has been made using C++, using SDL and pugi libraries. The maps have been made using Tiled portable v_0.17.

### Installation

Decompress the .zip file in any directory. Open the file directoy and double click on the executable.

### Controls
 Player movement:

- A: move to the left

- D: move to the right

- W: while in GodMode going up

- S: while in GodMode going down

- SPACE: jump If on air, press SPACE again to DOUBLE JUMP


### Debug keys:

- F1: start from the very first level
- F2: start from the beginning of the current level
- F5: save current state
- F6: Load the previous state
- F8: view UI Rectangles (UI Debug mode)
- F9: view colliders and pathfinding
- F10: GodMode (allows to fly around)
- F11: Enable/Disable FPS cap to 30
- 1: Go to 1st Stage
- 2: Go to 2nd Stage
- ESC: Open In-Game Menu

### Authors
 - Andrés Saladrigas
github: https://github.com/TheArzhel

- Aitor Simona
github: https://github.com/AitorSimona

### Project Github
	
	https://github.com/AtlasWorks/orBit

### Project Trello

	https://trello.com/b/M2TV7zXz/development

### Project Website

	https://atlasworks.github.io/orBit/

### Disclosure

We do not own any of the sprites, tilesets, music or sfx presented in this game. Authors credited below

- Player sprites:
https://jesse-m.itch.io/jungle-pack

- Tilesets & songs:
https://ansimuz.itch.io/sunnyland-forest
https://soumitrashewale.itch.io/sunnyland

- SFX were generated with BFXR
https://www.bfxr.net/

### License

© Andrés Ricardo Saladrigas Pérez and Aitor Simona Bouzas

Licensed under the [MIT License](LICENSE.txt)

### Apart from the minimum requirements, we have added the following features

### Innovation First Assignment

 Additional fetures added:

- 1: Go to 1st Stage

- 2: Go to 2nd Stage

- Checkpoint collider: creating checkpoints to save progress. If player dies he will start from checkpoints

- Player animations on Tiled: player animations are in Tiled. Inside folder Game/animation. player animation is a xml file we read to interpret data

- Camera: Only tiles inside the camera are printed 

- Colliders: Only colliders that are in camera are checked in module collisions Update

### Innovation Second Assignment

Additional fetures added:

- Collisions: Only colliders that are near an entity are checked. Update in module collisions.

- Enemies Update Area: enemies update just when the enemy is in the player depth of field.

- Enemies animations on Tiled: animations are loaded from a tmx defined in tiled.

- Enemies position on Tiled: enemies position defined in tiled and loaded from tmx

- FX: implemented sound effects to player.

### Innovation Second Assignment

Additional fetures added:

- Checkpoint Areas: creating checkpoints to save progress.


- Animations Loaded from Tiled: loaded animations from xml document save from Tiled.


- Collisions system: collisions are only checked near entities.


- Entity Update Area: enemies update just when the enemy is in the player depth of field.


- Tiled: Animations, collisions, entities, and areas loaded from tiled.


- UI from xml: Elements from the Ui are created from a xml.


- Entity management: avoid destroying and creating entities by active or inactive system.

### Task List

## Tasks

### Aitor


- Entity System 

- UI System 

- Pathfinding
 
- Player 

- Collisions  

- Metadata  

- Framerate control  

- Camera, parallax  

- Volume Control  


### Andres


- Enemies  

- Loading TMX System (sprites, game objects)  

- Collectibles, Score, UI Timer, Lifes  

- Maps   

- Website  

- Change_Scene Management  

- Collisions near entity

- Load & Save  

- Art Management (sprites, tileset, atlas, logo)





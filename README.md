
<h1>Maze.cpp / Maze.h</h1>
    
<!-- ---------------------------------------------------------------------------------------------------------- -->

<h2>Maze Constructor</h2>

The Maze class includes two constructors: a default constructor that initializes member variables and a parameterized constructor that sets dimensions, initializes the base point, calculates the end coordinate, and prepares the maze structure.

<!-- ---------------------------------------------------------------------------------------------------------- -->

<h2>Maze Generation</h2>

<h3>generateMaze()</h3>
* Generates the maze structure by calling the following methods depending on the mode:

**initializeMaze()**
* Initializes the maze structure, identifying open positions.

<h3>Normal Mode:</h3>
    
**placeRandomDot()**
* Randomly places an Pointer within open positions in the maze to start the algorithm from.

**findClosestExit()**
* Finds the closest exit from the placed Pointer.

**movePointerRandomly()**

* Check Valid Moves: It finds possible moves from the current position (currX, currY).

* No Valid Moves: If there are no valid moves, it marks the current cell as visited ('.') and searches for an unvisited cell, updating the position if found.

* Random Move: If valid moves exist, it randomly selects one, marks the current position as visited, removes the wall between the current and new position, and updates the maze.

* Recursive Call: If further moves are available, it recursively calls itself to continue moving; otherwise, it marks the position as visited.

**findPossibleMoves()**
* Identifies valid moves within the maze for the moveRandomly() Function.

<h3>Test Mode:</h3>
    
**placeDotandExitTest()**
* Places a Pointer at [1][1] and marks the exit location at [1][0] for testing.

**movePointerTest()**
* This function is very similar to movePointerRandomly(), but it prioritizes moves in this order: UP, RIGHT, DOWN, LEFT. If multiple directions are valid, it selects according to this priority—for example, UP if both UP and RIGHT are valid, or DOWN if only DOWN and LEFT are valid. This makes maze generation predictable for testing:

<!-- ---------------------------------------------------------------------------------------------------------- -->

<h2>Placing the Player within the Maze</h2>

**placePlayerAtBasePoint()**
* Places the player character at the maze’s starting (base) point.

**placePlayerTestMode()** 
* Places the player at the Position furthest from the exit.

<!-- ---------------------------------------------------------------------------------------------------------- -->

<h2>Maze methods</h2>

**displayMaze()** 
* Displays the current maze layout.

**setMaze()**
* Sets or updates the maze layout with a given 2D character vector.

**getMaze()** 
* Returns the current maze layout.

**checkMaze()** 
* Returns a boolean value indicating true if all characters are either 'x' or '.', and false otherwise.

<!-- ---------------------------------------------------------------------------------------------------------- -->

<h2>Maze Input</h2>

**readMaze():**

* Reads the maze layout with an optional validation option for Enchancement.

<h3>Enchancement 3 - Validation</h3>

**fixMaze():**

* Ensures that all positions in the maze are filled with either 'x' or '.'.
            
* Ensures that all Borders are Walls except the Gate and Places a Gate at [1][0] if a Gate is not found.

* Ensures that all Maze[Even][Even] Positions are Walls and Maze[Odd][Odd] Positions are Blank Spaces.

**validateMaze():**

* Initialization: It sets up movement directions and vectors for unvisited and wall coordinates.

* Find Start Point: It locates the first unvisited position ('.') in the maze and initializes it as the starting point.

* Flood-Fill: Using a flood-fill algorithm, it marks reachable cells with 'F'.

* Identify Walls: It checks for potential walls ('x') based on conditions such as adjacency to filled cells and their position within the maze. If conditions are met, it adds them to the wall coordinates for removal.

* Reset Maze: It restores all marked cells ('F') back to ('.').

* Modify Walls: If wall candidates exist, it randomly selects one to remove (sets it to '.') and recursively calls validateMaze() to revalidate.

**flipMaze():**

* It converts walls ('x') to open paths ('.') and vice versa, inverting the maze for inverted flood-fill validation.


<!-- --------------------------------------------- MAZE RUNNER ---------------------------------------------- -->

<h1>mazeRunner.cpp</h1>

<!-- ---------------------------------------------------------------------------------------------------------- -->

<h2>Menu Methods </h2>

**GenerateMazeMenu()**

* Invalid Input: If input is not between 1-5, it handles the error.

* Option 1 - Read Maze: Prompts the user for maze structure input and displays it.

* Option 2 - Generate Maze: Automatically generates a maze and displays the result.

* Option 3 - Validate Maze: Reads and validates a maze structure provided by the user, then displays it.

* Option 4 - Generate Maze Without Flattening: Generates a maze without flattening and displays it.

* Option 5 - Go back to Main Menu.

**getBasePoint()** 

* Asks player to set BasePoint Position for Normal Mode;

* Sets basePoint to (4848, 71, 4369) for Testing Mode;


<!-- --------------------------------------------- COORDINATE ---------------------------------------------- -->

<h1>Coordinate.h</h1>

This C++ Coordinate class represents a 3D point with an mcpp::BlockType block:

**Attributes:**

* x, y, z (coordinates) and block (default: AIR).

**Constructors:**

* Default: sets x, y, z to 0 and block to AIR.
* Parameterized: accepts x, y, z (with optional block).

**Methods:**
* Getters and setters for each attribute.

<!-- ------------------------------------------ COORDINATE ARRAY ------------------------------------------- -->

<h1>CoordinateArray.h</h1>

The CoordinateArray was utilized to store the types of blocks that were removed while flattening the terrain for maze construction, along with their coordinates, allowing for restoration at the end of the process.

**Advantages**

* Dynamic Resizing: It doubles capacity when full, reducing allocation frequency and improving efficiency.

* Memory Management: It manages memory explicitly with new and delete, which can lower overhead compared to STL containers.

* Type Safety: It stores only Coordinate objects, eliminating type checks and allowing for customized functionality.

* Performance: Offers amortized constant time for adding elements, similar to std::vector, while optimizing for specific access patterns.

**Attributes:**

* An array of Coordinate pointers, capacity to track allocated size, and size for the current number of elements.

**Constructors:**

* Initializes the array with a specified initial capacity (default is 10).

**Destructor:**

* Frees allocated memory to prevent memory leaks.

**Public Methods:**

* add(Coordinate): Adds a new Coordinate, resizing the array if the capacity is reached.

* get(int): Retrieves a Coordinate at a specified index.

* length(): Returns the current number of Coordinate objects stored.

**Private Methods:**

* resize(): Doubles the array's capacity and copies existing coordinates to the new array.

<!-- -------------------------------------------- MENU_UTILS ----------------------------------------------- -->

<h1>menuUtils.h</h1>

<h2>Error Handling && Input</h2>

**handleInputError()**
    
* Clears the input stream and displays an error message prompting the user to enter a number within a specified range.

**getLengthWidth()**
        
* Prompts the user to input the maze's length and width, ensuring both are odd numbers greater than zero. If the input is invalid, it clears the input stream and repeatedly prompts the user until valid values are provided.

<!-- ---------------------------------------------- TESTS --------------------------------------------------- -->

<h1>Tests</h1>

**EvenMazeLength:**

* This test ensures that the program correctly enforces the requirement for the user to enter odd numbers for the length and width of the maze. It verifies that the program rejects even dimensions and prompts the user for valid odd inputs.

**GenerateAndBuildMaze:**

* This test verifies that the program can generate and build a maze correctly based on specified parameters. It ensures that the maze structure is accurately created in memory and adheres to the defined maze generation rules.

**GenerateReadAndBuildMultipleMazes**

* This test evaluates the program's ability to generate, read, and build multiple mazes in succession. It ensures that the program can handle various maze configurations without data loss or performance degradation. Additionally, it verifies that unnecessary data is properly deleted after use by calling the destructor to free memory from the previous maze. The test also checks for the proper handling of null pointer references and ensures that no memory issues arise during maze processing. Overall, this test confirms robust memory management and error handling when dealing with multiple mazes.

**InvalidInputMaze**

* This test assesses how the program responds when the user enters invalid characters for the maze structure. It checks that the program appropriately handles inputs that are not 'x' or '.' by prompting the user to re-enter the maze structure. The test ensures that the old maze is discarded, and a clear error message is provided, confirming that the program correctly validates input and maintains the integrity of the maze configuration.

**MinimumMazeSize**

* This test ensures that the maze meets the minimum size requirements set by the program. It verifies that the program correctly enforces size constraints, checking that attempts to create mazes below the minimum specified dimensions are appropriately handled.

<!-- --------------------------------------------- MAKEFILE --------------------------------------------------- -->

<h1>MakeFile</h1>

<h2>Running all the Tests</h2>

* The runTests target in the Makefile automates the execution of all test cases, allowing users to run all tests with a single command. It provides immediate feedback on test results, indicating pass/fail status and similarity percentages. This design enhances usability by ensuring a standardized and efficient testing process, making it easy for users to validate the functionality of the mazeRunner program.


<h1> mazeRunner.cpp </h1>

SolveMazeMenu The SolveMazeMenu offers two options for users:

* Solve Manually: Places the player in a random location within the maze using the placePlayerRandomly() in maze.cpp.

* Show Escape Route: Prompts the user to select a search type after the player has been placed in the maze, ensuring the player is within bounds for exit searching.

<h1> Agent.cpp / Agent.h </h1>

<h2> Wall Follower </h2>

The WallFollower method implements the "right-hand rule" navigation technique for maze-solving. Here’s how it operates:

* Initialization: The agent checks surrounding blocks (front, right, left) to determine its orientation relative to walls.

* Direction Setting:

    - Test Mode: The agent assumes an initial orientation and rotates until a wall is on its right.

    - Regular Mode: The agent detects the nearest wall and adjusts its starting orientation accordingly. If all adjacent blocks are air, a random direction is chosen.
            
* Movement: The agent enters a loop, moving forward while following walls. It utilizes the findDirection helper method to determine the next move based on the right-hand rule (priority: right → forward → left). The path is marked with green carpet (LIME_CARPET), and previously visited blocks are cleared of carpet to avoid re-exploration.

* Goal Detection: The loop continues until the agent detects a blue carpet block (BLUE_CARPET), signaling it has reached the goal. The WallFollower is effective for certain maze layouts but may not always yield the shortest path.

<h2> Breadth-First Search (BFS) </h2> 
The BreadthFirstSearch method employs an exhaustive search strategy to guarantee finding the shortest path:

* Initialization: The agent ensures the player remains stationary for two seconds to establish a base coordinate.

* Exploration Structures:
Unvisited Queue: Stores paths yet to be explored.
Visited List: Tracks already explored paths to prevent revisits.

* Path Exploration: The agent examines neighboring coordinates in all four directions (positive and negative x, positive and negative z). Non-wall blocks are added to the unvisited queue, while processed blocks are marked as visited.

* Goal Detection: The search continues until a blue carpet block is encountered, indicating the goal has been reached.

* Path Reconstruction: Upon reaching the goal, BFS reconstructs the shortest path using pointers stored in each path node, marking the route with green carpets for guidance.

* Real-Time Adjustments: The agent monitors the player’s position relative to the marked path. If the player steps off, a message prompts them to return, and carpets are cleared from visited blocks to prevent loops.

* Helper Method: checkIfVisited is used to verify if a coordinate was already explored, facilitating the search for unvisited coordinates.
        

<h2> Agent Destructor </h2> 
The Agent destructor (~Agent()) ensures proper memory management by:

* Freeing Visited Paths: Iterates through the visited list, deleting each path to prevent memory leaks.

* Freeing Unvisited Paths: Uses a loop to dequeue and delete each path from the unvisited queue, ensuring all dynamically allocated paths are properly cleaned up.

<h1> Path.h </h1>
    
Simulates a linked list node by storing a coordinate and a pointer to the previous path
This is implemented in Breath First Search.
contains:
- Constructor
- Default Constructor
- get functions

Linked List for Path Management The implementation utilizes a linked list data structure (path) to manage coordinates effectively. Each path object stores the current location and a pointer to the previous path, enabling efficient backtracking and path reconstruction.

<h1> Maze.cpp / Maze.h </h1>

* placePlayerRandomly()
this function is triggered when solve manually is entered in the solveMazeMenu. It places the player at a random free location within the maze.
    
<h1> Tests </h1>

<h3> IncorrectInputValues </h3>

This test evaluated the Minecraft MazeRunner program’s functionality in handling user input, navigating menus, generating and validating a maze structure, and exiting gracefully. The test covered main menu navigation, correct error messaging for invalid inputs, and successful reading and printing of a custom maze structure entered by the user. Finally, the program’s exit functionality was tested, confirming it displays a closing message and terminates without errors.

<h3> NoMazeExit </h3>

This test evaluated the Minecraft MazeRunner program's ability to handle maze generation, building, solving, and error detection in cases where no solution exists. After navigating the main menu, a custom maze structure was entered, successfully read, and printed. The maze was then built in Minecraft, with confirmation of successful building. Upon selecting the "Solve Maze" option, the program attempted to find an escape route. When using the chosen search algorithm (Wall Follower or Breadth First Search), the program followed a series of steps but ultimately detected that no exit was reachable, displaying "No Exit Found!" and identifying the maze as incorrect. The agent was then destructed, freeing memory, and the program returned to the main menu and exited gracefully when requested.

<h3> readMazeTerminal </h3>

This test checked the Minecraft MazeRunner program’s ability to read, build, and exit a custom maze. After selecting "Generate Maze" and entering a custom maze layout, the program confirmed successful reading and displayed the maze structure with a specified base point. Returning to the main menu, the user selected the "Build Maze" option, which the program completed successfully, showing "Maze built successfully." Finally, the program exited without errors when prompted, displaying "The End!" as expected. This test confirmed correct functionality for maze generation, building, and clean exit operations.

<h3> solveMazeMenuLogic </h3>

This test demonstrated that the Minecraft MazeRunner program requires the "Solve Manually" option to be selected before allowing "Show Escape Route." After generating and building a maze, the user attempted to select "Show Escape Route" directly. The program displayed a prompt instructing the user to choose "Solve Manually" first to ensure the player is positioned within the maze. After following this prompt, the user selected the escape route option, and the solver algorithm successfully found the exit. This behavior confirms that the program includes safeguards to verify the player’s location within the maze before showing an escape route.

<h3> WallFollowerExitFound </h3>

This test validated that the Minecraft MazeRunner program’s Wall Follower algorithm is capable of finding the exit when a designated marker (blue carpet) is encountered. After generating and building a random maze, the program successfully positioned the player within the maze. Upon selecting the "Show Escape Route" option, the Wall Follower algorithm initiated and navigated through a series of coordinates until it encountered the blue carpet marking the exit. The program then displayed "Exit Found!" and correctly deallocated resources, returning to the main menu. This test confirms that the Wall Follower algorithm recognizes the blue carpet as the exit and successfully terminates when it is reached.

<!-- ----------------------------------------------- MAZE ----------------------------------------------------- -->

<h2>Build Maze</h2>

<h3>buildMaze()</h3>

* Builds the maze base on the structure by calling the following methods:

    **flattenLand()**
    * Flatten the area by build the block on the coordinate that has lower height than base height and remove the block that has higher height than base height. Follow by record the block that has been build or remove.

    **placeMaze()**
    * Place ACACIA_WOOD_PLANK base on the maze structure and place BLUE_CARPET on the entrance.

<h3>~Maze()</h3>

* Calls revertChunk() if the maze has been built.

    **revertChunk()**

    * Calls removeMaze() and revert the area by read the coordinate and block from the record (recorded when flattenLand() has been called) and place the block.

    **removeMaze()**

    * Remove the maze base on the maze structure.

<h2>Enhancement</h2>

<h3>generateMazeWithoutFlattening()</h3>

* Generate the maze without flattening by calling the following methods:

    **initializeMaze()**
    * Places wall around the object that has height different from nearby block(front, left, right, back blocks) more than 1 block.

    **placeRandomDot()**
    * Randomly places an Pointer within open positions in the maze to start the algorithm from.

    **findClosestExit()**
    * Finds the closest exit from the placed Pointer.

    **movePointerRandomly()**
    * Check Valid Moves: It finds possible moves from the current position (currX, currY).

    * No Valid Moves: If there are no valid moves, it marks the current cell as visited ('.') and searches for an unvisited cell, updating the position if found.

    * Random Move: If valid moves exist, it randomly selects one, marks the current position as visited, removes the wall between the current and new position, and updates the maze.

    * Recursive Call: If further moves are available, it recursively calls itself to continue moving; otherwise, it marks the position as visited.

    **findPossibleMoves()**
    * Identifies valid moves within the maze for the moveRandomly() Function.

    **removeWallAboveObj()**
    * remove wall on object that has height different more than 1 block.  

<h3>buildMaze()</h3>

* build maze for the enhancement by calling placeMaze() without calling flattenLand().

    **placeMaze()**
    * Place the wall and carpet base above current highest non-air block of each coordinate which base on the maze structure. Record the height of each wall for removeMaze().
 
<h3>removeMaze()</h3>

* Remove maze for the enhancement.

    **removeMaze()**
    * remove blocks in each coordinate depend on each wall height that has been record.

<!-- ---------------------------------------------- TESTS --------------------------------------------------- -->

<h1> Tests </h1>

**BuildMazeBeforeSolve:**

* This test ensures that the program will prevent player from access solve maze while the maze has not been placed.

**NonEnglishLanguage:**

* This test ensures that the program will handle properly when player input alplabets of languages besides english langauge.

**DuplicateMaze**

* This test ensure that user cannot build multiple maze without removing old maze, and prevent building new maze unintentionally.

**ShowTeamInfo**

* This test ensures that show team information function work properly by showing name and email of members.
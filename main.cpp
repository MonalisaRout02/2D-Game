#include <GL/freeglut.h>
#include <iostream>
#include <string>
#include <ctime>



// Window dimensions
int windowWidth = 1000;
int windowHeight = 800;

// Bird properties
float birdX = 300.0f;
float birdY = windowHeight / 2.0f;
float birdSpeed = 0.0f;
float birdGravity = 0.3f;
float birdSize = 20.0f;
float birdHorizontalSpeed = 1.0f;
// moon properties
float moonX = windowWidth / 4.0f;    // X position of the moon
float moonY = windowHeight - 100.0f; // Y position of the moon
float moonRadius = 50.0f;             // Radius of the moon

// Grass properties
float grassHeight = 50.0f;

// Obstacle properties
float gapSize = 400.0f;
float obstacleWidth = 100.0f;
float obstacleSpeed = 1.5f;
int numObstacles = 5;
float obstaclePositions[5];

// Food particle properties
int numFoodParticles = 10;
float foodSize = 15.0f;
float foodY = windowHeight / 2.0f;
float foodSpeed = 2.0f;
float foodPositions[10];
bool foodActive[10];

// Scoring
int score = 0;

// Game state
bool gameOver = false;

// Function to draw the bird
void drawBird() {
    glPushMatrix();
    glTranslatef(birdX, birdY, 0.0f);

    // Draw the bird's body (a round shape)
    glColor3f(1.0f, 0.0f, 0.0f);
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(0.0f, 0.0f); // Center of the body

    // Define the vertices of the body
    const int numBodyVertices = 50; // Number of vertices for the body's outline
    const float bodyRadius = birdSize / 1.5f; // Radius of the body

    for (int i = 0; i <= numBodyVertices; ++i) {
        float theta = static_cast<float>(i) / numBodyVertices * 2.0f * 3.14; // Angle around the body
        float x = bodyRadius * cos(theta);
        float y = bodyRadius * sin(theta);
        glVertex2f(x, y);
    }
    glEnd();

    // Draw the bird's wings (triangular shape)
    glColor3f(0.0f, 1.0f, 0.0f); // Green wings
    glBegin(GL_TRIANGLES);

    // Define the vertices of the wings
    const float wingWidth = birdSize * 0.5f; // Width of the wings
    const float wingHeight = birdSize * 0.3f; // Height of the wings

    // Left wing
    glVertex2f(-birdSize / 2.0f, 0.0f);
    glVertex2f(-birdSize / 2.0f - wingWidth, wingHeight);
    glVertex2f(-birdSize / 2.0f - wingWidth, -wingHeight);

    glEnd();

    // Draw the bird's beak (a triangle)
    glColor3f(1.0f, 1.0f, 0.0f); // Yellow beak
    glBegin(GL_TRIANGLES);

    // Define the vertices of the beak
    const float beakLength = birdSize * 0.4f; // Length of the beak

    // Beak tip
    glVertex2f(bodyRadius + beakLength, 0.0f);
    // Beak base
    glVertex2f(bodyRadius, beakLength / 2.0f);
    glVertex2f(bodyRadius, -beakLength / 2.0f);

    glEnd();

    // Draw the bird's eye (a black circle)
    glColor3f(0.0f, 0.0f, 0.0f); // Black eye
    const float eyeRadius = birdSize * 0.1f; // Radius of the eye
    const float eyeOffsetX = birdSize * 0.15f; // X offset of the eye
    const float eyeOffsetY = birdSize * 0.1f; // Y offset of the eye
    glBegin(GL_TRIANGLE_FAN);
    for (int i = 0; i <= numBodyVertices; ++i) {
        float theta = static_cast<float>(i) / numBodyVertices * 2.0f * 3.14; // Angle around the eye
        float x = eyeOffsetX + eyeRadius * cos(theta);
        float y = eyeOffsetY + eyeRadius * sin(theta);
        glVertex2f(x, y);
    }

    glEnd();

    glPopMatrix();
}



void drawObstacles1() {
    for (int i = 0; i < numObstacles; ++i) {
        glPushMatrix();
        glTranslatef(obstaclePositions[i], 0.0f, 0.0f);

        // Set the color for the brick texture
        glColor3f(0.5f, 0.1f, 0.1f); // Dark reddish brick color

        // Draw the upper obstacle (brick texture)
        glBegin(GL_QUADS);
        glVertex2f(0.0f, 0.0f);
        glVertex2f(obstacleWidth / 2.0f, 0.0f); // Adjusted width to half of obstacleWidth
        glVertex2f(obstacleWidth / 2.0f, 300.0f);
        glVertex2f(0.0f, 300.0f);
        glEnd();

        // Set the color for the black outline
        glColor3f(1.0f, 1.0f, 1.0f);

        // Draw black outlines for each brick
        glLineWidth(2.0f);
        glBegin(GL_LINES);
        for (int y = 0; y < 300; y += 25) {
            for (int x = 0; x < obstacleWidth / 2.0f; x += 50) { // Adjusted width to half of obstacleWidth
                glVertex2f(x, y);
                glVertex2f(x + 50, y);
                glVertex2f(x + 50, y);
                glVertex2f(x + 50, y + 25);
                glVertex2f(x + 50, y + 25);
                glVertex2f(x, y + 25);
                glVertex2f(x, y + 25);
                glVertex2f(x, y);
            }
        }
        glEnd();

        // Set the color for the lower obstacle (brick texture)
        glColor3f(0.5f, 0.1f, 0.1f); // Dark reddish brick color

        // Draw the lower obstacle (brick texture)
        glBegin(GL_QUADS);
        glVertex2f(0.0f, 500.0f);
        glVertex2f(obstacleWidth / 2.0f, 500.0f); // Adjusted width to half of obstacleWidth
        glVertex2f(obstacleWidth / 2.0f, windowHeight);
        glVertex2f(0.0f, windowHeight);
        glEnd();

        // Set the color for the black outline
        glColor3f(1.0f, 1.0f, 1.0f);

        // Draw black outlines for each brick
        glLineWidth(2.0f);
        glBegin(GL_LINES);
        for (int y = 500; y < windowHeight; y += 25) {
            for (int x = 0; x < obstacleWidth / 2.0f; x += 50) { // Adjusted width to half of obstacleWidth
                glVertex2f(x, y);
                glVertex2f(x + 50, y);
                glVertex2f(x + 50, y);
                glVertex2f(x + 50, y + 25);
                glVertex2f(x + 50, y + 25);
                glVertex2f(x, y + 25);
                glVertex2f(x, y + 25);
                glVertex2f(x, y);
            }
        }
        glEnd();

        glPopMatrix();
    }
}





void drawMoon() {
    glPushMatrix();
    glTranslatef(moonX, moonY, 0.0f);

    // Draw the moon as a white circle
    glColor3f(1.0f, 1.0f, 1.0f); // White color for the moon
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(0.0f, 0.0f);
    for (int i = 0; i <= 360; ++i) {
        float angle = static_cast<float>(i) * 3.14159f / 180.0f;
        float x = moonRadius * cos(angle);
        float y = moonRadius * sin(angle);
        glVertex2f(x, y);
    }
    glEnd();

    glPopMatrix();
}

void drawStars() {
    // Draw stars as small white points scattered across the sky
    glColor3f(1.0f, 1.0f, 1.0f); // White color for stars
    glPointSize(2.0f); // Set the size of the stars

    // Draw each star as a point
    glBegin(GL_POINTS);
    // Define the number of stars and their random positions
    int numStars = 80;
    for (int i = 0; i < numStars; ++i) {
        float x = static_cast<float>(rand()) / RAND_MAX * windowWidth;
        float y = static_cast<float>(rand()) / RAND_MAX * windowHeight;
        glVertex2f(x, y);
    }
    glEnd();
}


void drawBackground() {
    // Set background color
    glColor3f(0.0f, 0.0f, 0.1f); // Dark blue color for night mode

    // Draw background rectangle
    glBegin(GL_QUADS);
    glVertex2f(0.0f, 0.0f);
    glVertex2f(windowWidth, 0.0f);
    glVertex2f(windowWidth, windowHeight);
    glVertex2f(0.0f, windowHeight);
    glEnd();

    // Draw stars
    drawStars();
    drawMoon();

    // Draw grass at the base
    glColor3f(0.0f, 0.6f, 0.0f); // Green color for grass

    // Define the number of grass segments
    int numSegments = 100; // Increase the number of segments
    float segmentWidth = windowWidth / static_cast<float>(numSegments);
    float grassWidth = segmentWidth * 0.5f; // Adjust the grass width as a fraction of segmentWidth

    // Set the seed for randomization
    srand(static_cast<unsigned int>(time(0)));

    // Draw each grass segment as a triangle
    glBegin(GL_TRIANGLES);
    for (int i = 0; i < numSegments; ++i) {
        float x = i * segmentWidth;
        float xMid = x + segmentWidth / 2.0f;
        float y = grassHeight + static_cast<float>(rand()) / static_cast<float>(RAND_MAX) * 30.0f; // Randomize the grass height

        // Draw the triangle vertices with adjusted grass width
        glVertex2f(x + (segmentWidth - grassWidth) / 2.0f, 0.0f);
        glVertex2f(x + (segmentWidth + grassWidth) / 2.0f, 0.0f);
        glVertex2f(xMid, y);
    }
    glEnd();
}






void drawFoodParticles() {
    for (int i = 0; i < numFoodParticles; ++i) {
        if (foodActive[i]) {
            glPushMatrix();
            glTranslatef(foodPositions[i], foodY, 0.0f);

            // Set the color for the apple (e.g., red)
            glColor3f(1.0f, 1.0f, 0.0f); // Red color for the apple

            // Draw the apple particle
            glBegin(GL_TRIANGLE_FAN);
            glVertex2f(0.0f, 0.0f); // Center of the apple

            // Define the shape of the apple
            int numSlices = 12; // Number of apple slices
            float sliceAngle = 360.0f / numSlices;

            // Draw each slice of the apple
            for (int slice = 0; slice <= numSlices; ++slice) {
                float angle = static_cast<float>(slice) * sliceAngle;
                float radian = angle * 3.14159f / 180.0f;
                float x = foodSize * cos(radian) / 2.0f;
                float y = foodSize * sin(radian) / 2.0f;
                glVertex2f(x, y);
            }
            glEnd();

            // Draw the leaves around the apple
            glColor3f(0.0f, 0.8f, 0.0f); // Green color for leaves

            // Define the size and shape of the leaves
            float leafSize = foodSize * 0.8f; // Adjust the size of the leaves
            float leafAngle = 45.0f; // Angle of the leaf rotation

            // Draw each leaf using rotated rectangles
            glPushMatrix();
            glRotatef(leafAngle, 0.0f, 0.0f, 1.0f); // Rotate the leaves

            glBegin(GL_QUADS);
            glVertex2f(-leafSize / 2.0f, 0.0f);
            glVertex2f(leafSize / 2.0f, 0.0f);
            glVertex2f(leafSize / 4.0f, leafSize);
            glVertex2f(-leafSize / 4.0f, leafSize);
            glEnd();

            glPopMatrix();

            glPopMatrix();
        }
    }
}









// Function to handle keyboard input
void keyboard(unsigned char key, int x, int y) {
    if (key == ' ') { // Spacebar
        birdSpeed = -8.0f; // Flap the bird upward
    }
}

// Function to update game logic
void update(int value) {
    if (!gameOver) {
        // Update bird's position and speed
        birdSpeed += birdGravity;
        birdY -= birdSpeed;


        // Check for collision with the obstacles
        for (int i = 0; i < numObstacles; ++i) {
            obstaclePositions[i] -= obstacleSpeed;

            // Calculate the coordinates of the obstacle's bounding box
            float obstacleTop = 300.0f;
            float obstacleBottom = 500.0f;
            float obstacleLeft = obstaclePositions[i];
            float obstacleRight = obstaclePositions[i] + obstacleWidth;

            // Calculate the coordinates of the bird's bounding box
            float birdTop = birdY + birdSize / 2;
            float birdBottom = birdY - birdSize / 2;
            float birdLeft = birdX - birdSize / 2;
            float birdRight = birdX + birdSize / 2;

            // Check for collision with the upper and lower obstacles
            if (birdRight >= obstacleLeft && birdLeft <= obstacleRight) {
                if (birdTop >= obstacleBottom || birdBottom <= obstacleTop) {
                    // Bird collided with upper or lower obstacle, game over
                    gameOver = true;
                }

            }

            // Reset obstacle position if it goes off the screen
            if (obstaclePositions[i] + obstacleWidth <= 0) {
                obstaclePositions[i] = windowWidth;
            }
        }


        // Update food particles
        for (int i = 0; i < numFoodParticles; ++i) {
            foodPositions[i] -= foodSpeed;

            // Check for collision with the bird
            if (foodActive[i]) {
                if (birdX + birdSize / 2 >= foodPositions[i] - foodSize / 2 &&
                    birdX - birdSize / 2 <= foodPositions[i] + foodSize / 2 &&
                    birdY + birdSize / 2 >= foodY - foodSize / 2 &&
                    birdY - birdSize / 2 <= foodY + foodSize / 2) {
                    // Bird collected the food particle
                    score += 5;
                    foodActive[i] = false;
                }
            }

            // Reset food position if it goes off the screen
            if (foodPositions[i] + foodSize / 2 < 0) {
                foodPositions[i] = windowWidth;
                foodActive[i] = rand() % 2; // Randomly activate the food particle
            }
        }

        // Check for collision with ground or ceiling
        if (birdY - birdSize / 2 <= 0 || birdY + birdSize / 2 >= windowHeight) {
            gameOver = true;
        }

        // Check if bird is outside left boundary and reset its position
        if (birdX - birdSize / 2 >= windowWidth) {
            birdX = -birdSize / 2;
        }
    }
    if (score >= 50)
        gameOver = true;

    glutPostRedisplay();
    glutTimerFunc(10, update, 0);
}

// Function to display graphics


void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    drawBackground();
    // Draw game objects
    drawBird();
    drawObstacles1();

    drawFoodParticles();

    // Display score
    glColor3f(1.0f, 1.0f, 1.0f); // White color for score text
    glRasterPos2f(10, windowHeight - 30);
    std::string scoreText = "Score: " + std::to_string(score);
    for (char c : scoreText) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);
    }

    // Display game over or game won message if applicable
    if (gameOver == true and score < 50) {
        glColor3f(1.0f, 1.0f, 1.0f); // Red color for game over message
        glRasterPos2f(windowWidth / 2 - 40, windowHeight / 2);
        glutBitmapString(GLUT_BITMAP_HELVETICA_18, (const unsigned char*)"Game Over!");
    }
    else if (score >= 50) {
        glColor3f(1.0f, 1.0f, 1.0f); // Green color for game won message
        glRasterPos2f(windowWidth / 2 - 70, windowHeight / 2);
        glutBitmapString(GLUT_BITMAP_HELVETICA_18, (const unsigned char*)"Congratulations! You Won!");
    }

    glutSwapBuffers();
}

// Function to initialize OpenGL
void init() {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Set clear color to black

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, windowWidth, 0, windowHeight); // Set up 2D orthographic projection

    glutKeyboardFunc(keyboard);
    glutTimerFunc(10, update, 0); // Call update function every 10 milliseconds
}

int main(int argc, char** argv) {
    srand(time(NULL)); // Seed the random number generator

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(windowWidth, windowHeight);
    glutCreateWindow("Flappy Bird Game");

    // Initialize obstacle positions
    for (int i = 0; i < numObstacles; ++i) {
        obstaclePositions[i] = windowWidth + i * (windowWidth / numObstacles);
    }

    // Initialize food particle positions and activity
    for (int i = 0; i < numFoodParticles; ++i) {
        foodPositions[i] = windowWidth + i * (windowWidth / numFoodParticles);
        foodActive[i] = rand() % 2; // Randomly activate the food particle
    }

    glutDisplayFunc(display);
    init();
    glutMainLoop();

    return 0;
}
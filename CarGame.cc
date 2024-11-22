#include <graphics.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <windows.h>

#define SCREEN_WIDTH GetSystemMetrics(SM_CXSCREEN)
#define SCREEN_HEIGHT GetSystemMetrics(SM_CYSCREEN)
#define ROAD_WIDTH 400
#define CAR_WIDTH 40
#define CAR_HEIGHT 80
#define ENEMY_WIDTH 40
#define ENEMY_HEIGHT 80

int carPosX = SCREEN_WIDTH / 2 - CAR_WIDTH / 2;
int carPosY = SCREEN_HEIGHT - 150;
int enemyX[3], enemyY[3], enemyFlag[3], spawnDelay[3];
int score = 0;
int level = 1;
int speed = 10;

void drawBorder() {
    setcolor(WHITE);
    rectangle((SCREEN_WIDTH - ROAD_WIDTH) / 2, 0, (SCREEN_WIDTH + ROAD_WIDTH) / 2, SCREEN_HEIGHT);
}

void drawCar(const char* imagePath) {
    readimagefile(imagePath, carPosX, carPosY, carPosX + CAR_WIDTH, carPosY + CAR_HEIGHT);
}

void drawEnemy(int ind, const char* imagePath) {
    if (enemyFlag[ind]) {
        readimagefile(imagePath, enemyX[ind], enemyY[ind], enemyX[ind] + ENEMY_WIDTH, enemyY[ind] + ENEMY_HEIGHT);
    }
}

void resetEnemy(int ind) {
    enemyY[ind] = 0;

    int validPosition = 0;
    while (!validPosition) {
        enemyX[ind] = (SCREEN_WIDTH - ROAD_WIDTH) / 2 + rand() % (ROAD_WIDTH - ENEMY_WIDTH);
        validPosition = 1;

        for (int i = 0; i < 3; i++) {
            if (i != ind && enemyFlag[i] && abs(enemyX[ind] - enemyX[i]) < ENEMY_WIDTH) {
                validPosition = 0;
                break;
            }
        }
    }

    spawnDelay[ind] = rand() % 50 + 30;
}

int collision() {
    for (int i = 0; i < 3; i++) {
        if (enemyFlag[i] && enemyY[i] + ENEMY_HEIGHT >= carPosY && enemyX[i] < carPosX + CAR_WIDTH &&
            enemyX[i] + ENEMY_WIDTH > carPosX) {
            return 1;
        }
    }
    return 0;
}

void updateScoreAndLevel() {
    setcolor(WHITE);
    settextstyle(SANS_SERIF_FONT, HORIZ_DIR, 2);

    char scoreText[20], levelText[20];
    sprintf(scoreText, "Score: %d", score);
    sprintf(levelText, "Level: %d", level);

    outtextxy(10, 10, scoreText);
    outtextxy(SCREEN_WIDTH - 150, 10, levelText);

    if (score > 0 && score % 10 == 0) {
        level = score / 10 + 1;
        speed = 5 + level * 2;
    }
}

void drawBackground(const char* backgroundPath) {
    readimagefile(backgroundPath, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
}

void playGame(const char* carImage, const char* enemyImage, const char* backgroundPath) {
    cleardevice();
    score = 0;
    level = 1;
    speed = 10;

    for (int i = 0; i < 3; i++) {
        enemyFlag[i] = 0;
        resetEnemy(i);
    }

    enemyFlag[0] = 1;

    while (1) {
        if (kbhit()) {
            char ch = getch();
            if (ch == 'a' || ch == 'A') {
                if (carPosX > (SCREEN_WIDTH - ROAD_WIDTH) / 2)
                    carPosX -= 20;
            }
            if (ch == 'd' || ch == 'D') {
                if (carPosX < (SCREEN_WIDTH + ROAD_WIDTH) / 2 - CAR_WIDTH)
                    carPosX += 20;
            }
            if (ch == 27) {
                break;
            }
        }

        // Draw the background
        drawBackground(backgroundPath);

        // Draw other elements
        drawBorder();
        drawCar(carImage);

        for (int i = 0; i < 3; i++) {
            if (enemyFlag[i]) {
                drawEnemy(i, enemyImage);
                enemyY[i] += speed;

                if (enemyY[i] > SCREEN_HEIGHT) {
                    resetEnemy(i);
                    score++;
                    enemyFlag[i] = 0;
                }
            } else if (--spawnDelay[i] <= 0) {
                enemyFlag[i] = 1;
            }
        }

        if (collision()) {
            cleardevice();
            setcolor(RED);
            settextstyle(SANS_SERIF_FONT, HORIZ_DIR, 4);
            outtextxy(SCREEN_WIDTH / 2 - 100, SCREEN_HEIGHT / 2, "Game Over!");
            getch();
            return;
        }

        updateScoreAndLevel();
        delay(30);
    }
}

int main() {
    initwindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Car Game", -3, -3); // Full-screen window
    srand(time(0));

    const char* carImage = "red_car.bmp";
    const char* enemyImage = "enemy.jpg";
    const char* backgroundImage = "Road19.jpg";

    while (1) {
        cleardevice();
        settextstyle(SANS_SERIF_FONT, HORIZ_DIR, 3);
        outtextxy(SCREEN_WIDTH / 2 - 100, SCREEN_HEIGHT / 2 - 50, "Car Game");
        outtextxy(SCREEN_WIDTH / 2 - 100, SCREEN_HEIGHT / 2, "1. Play");
        outtextxy(SCREEN_WIDTH / 2 - 100, SCREEN_HEIGHT / 2 + 50, "2. Quit");

        char choice = getch();
        if (choice == '1') {
            playGame(carImage, enemyImage, backgroundImage);
        } else if (choice == '2') {
            closegraph();
            return 0;
        }
    }
    return 0;
}


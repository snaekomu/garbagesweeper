//
//  main.c
//  ma.sweeper
//
//  Created by Ignasi Granell on 03/11/2019.
//  Copyright © 2019 Cowb Oy. All rights reserved.
//

#include <stdio.h>
#include <time.h>
#include <stdlib.h>

typedef struct {
    int val;
    int open;
} cell;

int gameOver = 0;

void populateBombs(size_t size_x, size_t size_y, cell map[size_y][size_x]) {
    printf("Enter amount of bombs:\n");
    int maxBombs, chance, bombs = 0;
    scanf("%d", &maxBombs);
    while (bombs <= maxBombs) {
        chance = (int)(size_y * size_x / (maxBombs - bombs));
        for (int y = 0; y < size_y; y++) {
            for (int x = 0; x < size_x; x++) {
                if ((rand() % chance) + 1 == 1) {
                    map[y][x].val = -1;
                    if (++bombs == maxBombs) {
                        return;
                    }
                }
            }
        }
    }
}

void populateRest(size_t size_x, size_t size_y, cell map[size_y][size_x]) {
    for (int y = 0; y < size_y; y++) {
        for (int x = 0; x < size_x; x++) {
            if (map[y][x].val != -1) {
                int neighbours = 0;
                for (int oy = y-1; oy <= y+1; oy++) {
                    for (int ox = x-1; ox <= x+1; ox++) {
                        if (oy >= 0 && oy < size_y && ox >= 0 && ox < size_x && map[oy][ox].val == -1) {
                            neighbours++;
                        }
                    }
                }
                map[y][x].val = neighbours;
            }
        }
    }
}

void initMap(size_t size_x, size_t size_y, cell map[size_y][size_x]) {
    for (int y = 0; y < size_y; y++) {
        for (int x = 0; x < size_x; x++) {
            map[y][x].val = -2;
            map[y][x].open = 0;
        }
    }
}

void display(size_t size_x, size_t size_y, cell map[size_y][size_x]) {
    printf(" |");
    for (int x = 0; x < size_x; x++) {
        printf("%d", x);
    }
    printf("\n-+");
    for (int x = 0; x < size_x; x++) {
        printf("-");
    }
    printf("\n");
    for (int y = 0; y < size_y; y++) {
        printf("%d|", y);
        for (int x = 0; x < size_x; x++) {
            if (map[y][x].open == 0) {
                printf("█");
            } else if (map[y][x].val == -1) {
                printf("X");
            } else {
                printf("%d", map[y][x].val);
            }
        }
        printf("\n");
    }
}

void openCell (size_t size_x, size_t size_y, cell map[size_y][size_x], int sel_x, int sel_y) {
    printf("Checking cell %d, %d\n", sel_x, sel_y);
    if (map[sel_y][sel_x].val == -1) {
        map[sel_y][sel_x].open = 1;
        gameOver = 1;
    } else if (map[sel_y][sel_x].val > 0) {
        map[sel_y][sel_x].open = 1;
        printf("Value of cell %d, %d is %d\n", sel_x, sel_y, map[sel_y][sel_x].val);
    } else {
        printf("Empty cell\n");
        for (int oy = sel_y-1; oy <= sel_y+1; oy++) {
            for (int ox = sel_x-1; ox <= sel_x+1; ox++) {
                if (oy >= 0 && oy < size_y && ox >= 0 && ox < size_x && map[oy][ox].open == 0) {
                    printf("Value of cell %d, %d is %d\n", ox, oy, map[oy][ox].val);
                    map[oy][ox].open = 1;
                    openCell(size_x, size_y, map, ox, oy);
                }
            }
        }
    }
}

int main(int argc, const char * argv[]) {
    srand((int)time(NULL));
    int size_x, size_y;
    
    printf("Enter grid width and height:\n");
    scanf("%d %d", &size_x, &size_y);
    cell map[size_y][size_x];
    
    initMap(size_x, size_y, map);
    populateBombs(size_x, size_y, map);
    populateRest(size_x, size_y, map);
    
    while (!gameOver) {
        display(size_x, size_y, map);
        int sel_x, sel_y;
        printf("Enter coords:\n");
        scanf("%d %d", &sel_x, &sel_y);
        openCell(size_x, size_y, map, sel_x, sel_y);
    }
    display(size_x, size_y, map);
    over: printf("GAME OVER\n");
    
    return 0;
}

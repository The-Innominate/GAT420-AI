#pragma once
#include "GameOfLife.h"
#include "Texture.h"
#include <algorithm>
#include <vector>
#include <SDL.h>
#include "Random.h"

    bool GameOfLife::Initialize() {
        // You may need to perform any specific initialization for GameOfLife here
        bufferA.resize(size.x * size.y);
        bufferB.resize(size.x * size.y);
        return true;
    }

    void GameOfLife::Step() {
        frame++;
        std::vector<uint8_t>& readBuffer = (frame % 2 == 0) ? bufferA : bufferB;
        std::vector<uint8_t>& writeBuffer = (frame % 2 == 0) ? bufferB : bufferA;

        // Clear write buffer
        std::fill(writeBuffer.begin(), writeBuffer.end(), 0);

        for (int y = 0; y < size.y; y++) {
            for (int x = 0; x < size.x; x++) {
                int weight = 0;

                // Read surrounding 8 cells, accumulate weight
                weight += Read<uint8_t>(readBuffer, x - 1, y - 1);
                weight += Read<uint8_t>(readBuffer, x, y - 1);
                weight += Read<uint8_t>(readBuffer, x + 1, y - 1);
                weight += Read<uint8_t>(readBuffer, x - 1, y);
                weight += Read<uint8_t>(readBuffer, x + 1, y);
                weight += Read<uint8_t>(readBuffer, x - 1, y + 1);
                weight += Read<uint8_t>(readBuffer, x, y + 1);
                weight += Read<uint8_t>(readBuffer, x + 1, y + 1);

                // Game of Life rules
                if (Read<uint8_t>(readBuffer, x, y)) {
                    if (weight == 2 || weight == 3) {
                        Write<uint8_t>(writeBuffer, x, y, 1); // Cell dies
                    }
                    else {
                        Write<uint8_t>(writeBuffer, x, y, 0); // Cell dies
                    }
                }
                else {
                    if (weight == 3) {
                        Write<uint8_t>(writeBuffer, x, y, 1); // Cell becomes alive
                    }
                    else {
                        Write<uint8_t>(writeBuffer, x, y, 0); // Cell stays dead
                    }                    
                }
            }
        }
        // convert buffer data format to color buffer
        std::transform((writeBuffer).begin(), (writeBuffer).end(), color_buffer.begin(), [](uint8_t v)
            {
                { return  v ? 0xffffffff : 0; }
            });

        // copy color buffer to texture
        texture->Copy(color_buffer);
    }

    
    void GameOfLife::KeyDown(SDL_Keycode keycode) {
        if (keycode == SDLK_r) {
                // Write random 'alive' cells to buffer
                std::vector<uint8_t>& writeBuffer = (frame % 2 == 0) ? bufferB : bufferA;
                for (int i = 0; i < writeBuffer.size(); i++) {
                    writeBuffer[i] = (rand() % 10 == 0) ? 1 : 0;
                }
        }
    }

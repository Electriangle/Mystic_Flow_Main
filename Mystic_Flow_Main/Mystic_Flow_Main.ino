/*
 * Title: Mystic_Flow_Main
 * 
 * Description:
 *      - This sketch was created for controling a WS2812B LED light strip (or similar) using the FastLED library. 
 *      
 * Author: Electriangle
 *      - Channel: https://www.youtube.com/@Electriangle
 *      
 * License: MIT License
 *      - Copyright (c) 2026 Electriangle
 *
 * Date Created: 1/24/2026
 * Last Updated: 1/24/2026
*/


// ====================================================================
//                     SETUP & CONFIGURATION PARAMETERS
// ====================================================================

#include "FastLED.h"

// --- HARDWARE & CONFIGURATION ---
#define NUM_LEDS    300    // Total number of LEDs on the strip
#define PIN         2      // Data pin connected to the LEDs

// --- STATE AND TIME TRACKING ---
const unsigned long COLOR_CYCLE_DURATION = 10000;
unsigned long lastColorChangeTime = 0;
int currentColorIndex = 0; 
int nextColorIndex = 1;    

// --- LED AND HEAT ARRAYS ---
#define NUM_COLORS  8
const byte MIN_HEAT = 20;   // Minimum heat to prevent LEDs from fully turning off
CRGB leds[NUM_LEDS];
static byte heat[NUM_LEDS];


// ====================================================================
//                           SETUP FUNCTION
// ====================================================================

void setup() {
  FastLED.addLeds<WS2812B, PIN, GRB>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.setMaxPowerInVoltsAndMilliamps(5, 1500);  
  FastLED.clear();                                    
  
  // Initial Ignition
  for (int i = 0; i < 10 && i < NUM_LEDS; i++) {
    heat[i] = 255;
  }
  FastLED.show();
}


// ====================================================================
//                         MAIN LOOP FUNCTION
// ====================================================================

void loop() {
  MysticFlow(150, 50, 50, false);    // Default example
}



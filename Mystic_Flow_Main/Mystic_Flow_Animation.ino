// ====================================================================
//                         ANIMATION FUNCTION
// ====================================================================

/*
  *  ========== Mystic_Flow_Animation ==========
  *  FUNCTION CALL:
  *   MysticFlow(Cooling, Sparks, DelayDuration, ReverseDirection);
  *  
  *  PARAMETERS:
  *   Cooling - use a larger value a less active flowing movement [default = 30]
  *   Sparks - use a larger value for more spark ignitions and a more active pixel count [range = 0 to 255; default = 150]
  *   DelayDuration - use larger value for slower speed [default = 30]
  *   ReverseDirection - change the direction of flow [default = false]
  *  
  *  EXAMPLES:
  *  - You can get a variety of different results by changing the values. Here are some examples to try:
  *      MysticFlow(150, 50, 50, false);   // Higher cooling, lower spark quantity, slower speed
  *      MysticFlow(30, 150, 30, false);   // Moderate cooling, moderate spark quantity, moderate speed
  *      MysticFlow(10, 225, 15, false);   // Lower cooling, higher spark quantity, faster speed
*/

void MysticFlow(int Cooling, int Sparks, int DelayDuration, bool ReverseDirection) {
  unsigned long currentTime = millis();
  unsigned long elapsedTime = currentTime - lastColorChangeTime;

  // 0) TIME-BASED COLOR CYCLING LOGIC
  if (elapsedTime >= COLOR_CYCLE_DURATION) {
    currentColorIndex = (currentColorIndex + 1) % NUM_COLORS;
    nextColorIndex = (currentColorIndex + 1) % NUM_COLORS;
    lastColorChangeTime = currentTime;
    elapsedTime = 0;
  }
  float blendFactor = (float)elapsedTime / COLOR_CYCLE_DURATION;
  if (blendFactor > 1.0) blendFactor = 1.0; 
  if (blendFactor < 0.0) blendFactor = 0.0;
  
  // 1) Slight cool down for each cell 
  int cooldown;
  for (int i = 0; i < NUM_LEDS; i++) {
    cooldown = random(0, ((Cooling * 10) / NUM_LEDS) + 2);
    
    // Perform standard cooling
    if(cooldown > heat[i]) { 
      heat[i] = 0;
    } else { 
      heat[i] = heat[i] - cooldown; 
    }
    
    // Ensure minimum heat level (to keep the testing glow)
    if (heat[i] < MIN_HEAT) {
        heat[i] = MIN_HEAT;
    }
  }
  
  // 2) Heat from each cell drifts up and diffuses slightly (Simplified)
  for (int k = (NUM_LEDS - 1); k >= 1; k--) {
    heat[k] = heat[k - 1]; 
  }
  
  // 3) Randomly ignite new Sparks anywhere on the strip
  if (random(255) < Sparks) {
    int y = random(NUM_LEDS);
    heat[y] = heat[y] + random(160, 255);
  }
  // Guaranteed minimum ignition at the base (to sustain the main ignition of the flow)
  heat[0] = heat[0] + 50; 
  if (heat[0] > 255) heat[0] = 255; 
  
  // 4) Convert heat cells to LED colors (unchanged)
  for (int n = 0; n < NUM_LEDS; n++) {
    
    byte temperature = heat[n];
    byte t192 = round((temperature / 255.0) * 191);
    byte heatramp = t192 & 0x3F; 
    heatramp <<= 2; 
    
    int BottomColor[8][3] = { {heatramp/2, heatramp/2, 255}, {255, 255, heatramp}, {255, heatramp, heatramp}, {heatramp, heatramp, 255}, {heatramp, heatramp, 255}, {255, 255, heatramp}, {255, heatramp, heatramp}, {255, heatramp, heatramp}, };
    int MiddleColor[8][3] = { {heatramp/2, heatramp/2, heatramp}, {255, heatramp, 0}, {heatramp, heatramp, 0}, {0, 255, heatramp/2}, {0, heatramp, heatramp}, {0, heatramp, 255}, {heatramp/3, 0, heatramp/2}, {heatramp, heatramp/4, heatramp*2/3}, };
    int TopColor[8][3] = { {heatramp, heatramp, heatramp}, {heatramp, 0, 0}, {heatramp, heatramp, 0}, {0, heatramp, 0}, {0, heatramp, heatramp}, {0, 0, heatramp}, {heatramp/3, 0, heatramp/2}, {heatramp, heatramp/4, heatramp*2/3}, };

    int Pixel = n;
    if (ReverseDirection) {Pixel = (NUM_LEDS - 1) - n;}

    int R, G, B;
    
    // Color blending logic 
    if (t192 > 0x80) { R = lerpFloat(BottomColor[currentColorIndex][0], BottomColor[nextColorIndex][0], blendFactor); G = lerpFloat(BottomColor[currentColorIndex][1], BottomColor[nextColorIndex][1], blendFactor); B = lerpFloat(BottomColor[currentColorIndex][2], BottomColor[nextColorIndex][2], blendFactor); }
    else if (t192 > 0x40) { R = lerpFloat(MiddleColor[currentColorIndex][0], MiddleColor[nextColorIndex][0], blendFactor); G = lerpFloat(MiddleColor[currentColorIndex][1], MiddleColor[nextColorIndex][1], blendFactor); B = lerpFloat(MiddleColor[currentColorIndex][2], MiddleColor[nextColorIndex][2], blendFactor); }
    else { R = lerpFloat(TopColor[currentColorIndex][0], TopColor[nextColorIndex][0], blendFactor); G = lerpFloat(TopColor[currentColorIndex][1], TopColor[nextColorIndex][1], blendFactor); B = lerpFloat(TopColor[currentColorIndex][2], TopColor[nextColorIndex][2], blendFactor); }

    leds[Pixel].setRGB(R, G, B);
  }

  FastLED.show();
  delay(DelayDuration);
}


// ====================================================================
//                           HELPER FUNCTION
// ====================================================================

int lerpFloat(int A, int B, float t) {
  return (int)((float)A * (1.0 - t) + (float)B * t);
}


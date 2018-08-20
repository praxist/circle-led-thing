#define NUM_COLORS 255

#define MIN_GHOST 2
#define MAX_GHOST 100
uint8_t curr_ghosts = MIN_GHOST;

// switches the current color [0-8]
void color_me(uint8_t num)
{
  color_idx = num;
  curr_color = color_generator(num);
}

// sets curr_color to the next color of the rainbow
void color_me_next_rainbow()
{
  if(color_idx == NUM_COLORS - 1)
    color_me(0);
  else
    color_me(color_idx + 1);
}

void color_me_random()
{
  color_me(random(NUM_COLORS));
}

void update_ghosts()
{
  for(uint8_t i=0; i<3; i++)
    for(uint8_t j=0; j<16; j++)
    {
      if(circles_ghost[i][j] == 0)
        strip.setPixelColor(circles[i][j], 0);
      else
        circles_ghost[i][j] -= 1;
    }
}

// updates pixel at circle[i] with current color
void color_update(uint8_t idx, uint8_t circle_num)
{
  // update given pixel with color
  strip.setPixelColor(circles[circle_num][idx], curr_color);
  // update ghost for given pixel
  circles_ghost[circle_num][idx] = curr_ghosts;
  // push updates to strip
  update_ghosts();
  strip.show();
  delay((unsigned long)t_delay);
}
void decrement_ghosts()
{
  if(curr_ghosts > MIN_GHOST)
    curr_ghosts -= 1;
}
void increment_ghosts()
{
  if(curr_ghosts < MAX_GHOST+1)
    curr_ghosts += 1;
}
uint8_t get_curr_ghosts()
{
  return curr_ghosts;
}
void set_curr_ghosts(uint8_t n)
{
  curr_ghosts = n;
}
void reset_strip()
{
  for(uint8_t i=0; i<TOTAL_PIXELS; i++)
    strip.setPixelColor(i, 0);
  strip.setBrightness(BRIGHTNESS);
}
// Based off Adafruit's color wheel function
// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t color_generator(uint16_t color_location) {
  color_location = 255 - color_location;
  if(color_location < 85) {
    return strip.Color(255 - color_location * 3, 0, color_location * 3);
  }
  if(color_location < 170) {
    color_location -= 85;
    return strip.Color(0, color_location * 3, 255 - color_location * 3);
  }
  color_location -= 170;
  return strip.Color(color_location * 3, 255 - color_location * 3, 0);
}


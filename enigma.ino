#include <Adafruit_NeoPixel.h>
#include <utils.c>
#define PIN 0 //marked D1 on GEMMA
#define TOTAL_PIXELS 48
#define BRIGHTNESS 15
#define DEFAULT_DELAY 25

#define DONT_CHANGE_COLOR 0
#define USE_RANDOM_COLOR 1
#define USE_RAINBOW_COLOR 2

// macros for pulse pattern
#define PULSE_MAX_BRIGHTNESS 32
#define PULSE_MIN_BRIGHTNESS 5
#define PULSE_DELAY 45
Adafruit_NeoPixel strip = Adafruit_NeoPixel(TOTAL_PIXELS, PIN);

// 0 = top, 1 = left, 2 = right
uint8_t circles[3][16] = {{0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15},{27,28,29,30,31,16,17,18,19,20,21,22,23,24,25,26},{38,39,40,41,42,43,44,45,46,47,32,33,34,35,36,37}};
uint8_t circles_ghost[3][16] = {{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}};

uint8_t t_delay = DEFAULT_DELAY;

uint8_t color_idx = 0;
uint32_t curr_color = 0xFFFFFF;
// navigates circles in reverse order
bool reversed = false;
// keeps pixels on, even after the ghosting period has expired

void setup() {
  strip.begin();
  strip.setBrightness(BRIGHTNESS);
  strip.show();
}
// lights a circle arr, can provide option to randomize color before lighting
// color_change_interval = how many pixels light up with the same color before changing to the next one
void light_circle(uint8_t circle_num, uint8_t change_color, uint8_t color_change_interval)
{
  if(reversed)
    for(uint8_t i=16, color_change_counter=color_change_interval; i>0; i--, color_change_counter--)
    {
      color_update(i-1, circle_num);
      if(color_change_counter == 0)
      {
        if(change_color == USE_RANDOM_COLOR)
          color_me_random();
        else if(change_color == USE_RAINBOW_COLOR)
          color_me_next_rainbow();
        color_change_counter = color_change_interval;
      }
    }
  else
    for(uint8_t i=0, color_change_counter=color_change_interval; i<16; i++, color_change_counter--)
    {
      color_update(i, circle_num);
      if(color_change_counter == 0)
      {
        if(change_color == USE_RANDOM_COLOR)
          color_me_random();
        else if(change_color == USE_RAINBOW_COLOR)
          color_me_next_rainbow();
        color_change_counter = color_change_interval;
      }
    }
}
void pattern_snakey()
{
  t_delay = 20;
  //reversed = random(2);
  for(uint8_t i=0; i<3; i++)
  {
    light_circle(i, USE_RAINBOW_COLOR, 1);
    reversed = !reversed;
  }
}
void pattern_random_snake()
{
  t_delay = 20;
  reversed = random(2);
  light_circle(random(3), USE_RAINBOW_COLOR, 1);
}
// makes a color "pulse" by adjusting brightness rapidly
void pattern_pulse(bool reset)
{
  color_me_random();
  // start at one color
  for(uint8_t i=0; i<TOTAL_PIXELS; i++)
    strip.setPixelColor(i, curr_color);
  // pulse up
  for(uint8_t i=PULSE_MIN_BRIGHTNESS; i<PULSE_MAX_BRIGHTNESS; i++)
  {
    strip.setBrightness(i);
    strip.show();
    delay(PULSE_DELAY);
  }
  // pulse down
  for(uint8_t i=PULSE_MAX_BRIGHTNESS; i>=BRIGHTNESS; i--)
  {
    strip.setBrightness(i);
    strip.show();
    delay(PULSE_DELAY);
  }
  // reset
  //if(reset)
   // reset_strip();
}
void pattern_rainbow() {
  t_delay = 12;

  for(uint8_t i=0; i<255; i++)
  {
    for(uint8_t j=0; j<3; j++)
      for(uint8_t k=0; k<16; k++)
      {
        strip.setPixelColor(circles[j][k], color_generator((i+(j*3)+k)));
        circles_ghost[j][k] = TOTAL_PIXELS;
      }
    strip.show();
    delay(t_delay);
  }
}
void pattern_magic_stolen() {
  t_delay = 7;
  for(uint8_t i=0; i<255; i++)
  {
    for(uint8_t j=0; j<48; j++)
    {
      strip.setPixelColor(j, color_generator(i+(j*7) & 255));
    }
    strip.show();
    delay(t_delay);
  }
}
void pattern_magic() {
  t_delay = 7;
  for(uint8_t i=0; i<255; i++)
  {
    for(uint8_t j=0; j<3; j++)
      for(uint8_t k=0; k<16; k++)
      {
        strip.setPixelColor(circles[j][k], color_generator(i+((j+k)*7) & 255));
        circles_ghost[j][k] = TOTAL_PIXELS;
      }
    strip.show();
    delay(t_delay);
  }
}
void pattern_bloom() {
  strip.show();
  t_delay = 25;
  reversed = true;
  uint8_t last_ghosts = get_curr_ghosts();
  set_curr_ghosts(TOTAL_PIXELS);
  color_me_random();
  for(uint8_t i=0; i<3; i++)
  {
    light_circle(i, USE_RANDOM_COLOR, TOTAL_PIXELS);
    delay(100);
  }
  reversed = !reversed;
  set_curr_ghosts(last_ghosts);
}
void loop() {
  // the show begins!
  for(uint8_t i=0; i<16; i++)
  {
    pattern_random_snake();
    increment_ghosts();
    increment_ghosts();
  }
  for(uint8_t i=0; i<4; i++)
  {
    pattern_snakey();
    increment_ghosts();
    increment_ghosts();
  }
  pattern_magic_stolen();
  pattern_magic_stolen();
  pattern_magic_stolen();
  pattern_magic_stolen();
  pattern_rainbow();
  pattern_rainbow();
  pattern_rainbow();
  pattern_pulse(false);
  pattern_pulse(false);
  pattern_pulse(false);
  pattern_bloom();
  pattern_bloom();
  pattern_bloom();
  pattern_magic();
  pattern_magic();
  pattern_magic();
  pattern_magic();
  pattern_magic();
  set_curr_ghosts(TOTAL_PIXELS/3);
  pattern_snakey();
  set_curr_ghosts(TOTAL_PIXELS);
  for(uint8_t i=0; i<16; i++)
  {
    pattern_random_snake();
    decrement_ghosts();
    decrement_ghosts();
  }
  for(uint8_t i=0; i<8; i++)
  {
    pattern_snakey();
    decrement_ghosts();
    decrement_ghosts();
  }
}
//flowers petals
//double sine


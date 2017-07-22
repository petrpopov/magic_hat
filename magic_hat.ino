#include <Adafruit_NeoPixel.h>
#include <Adafruit_NeoMatrix.h>
#include <Adafruit_GFX.h>
 
//5 meters, 60 led/m, total 300 count
//8 rows by 37 shift

#define GLITCH_COUNT 200
#define BRIGHTNESS 30
#define LED_PIN     6 // Arduino pin to NeoPixel data input
#define WIDTH  37 // Hat circumference in pixels
#define HEIGHT  8 // Number of pixel rows (round up if not equal)
#define TOTAL 300
#define OFFSET  (((WIDTH * HEIGHT) - TOTAL) / 2)
 
// Pixel strip must be coiled counterclockwise, top to bottom, due to
// custom remap function (not a regular grid).
Adafruit_NeoMatrix matrix(WIDTH, HEIGHT, LED_PIN,
  NEO_MATRIX_TOP  + NEO_MATRIX_LEFT +
  NEO_MATRIX_ROWS + NEO_MATRIX_PROGRESSIVE,
  NEO_GRB         + NEO_KHZ800);

const unsigned char* messages[] = {
  "INSOMNIA 2017",
  "Psycho hat ver. 0.1",
  "Hi there!",
  "Wanna see some magic?",
  "I have a lot of magic for ya!",
  
  "Come here, babe! Don't be afraid!",
  "My magic is soooooooooo big!",
  "You're gonna like it!",
  "Yammy! Oh, yeah..tasty magic",
  "Yeah, take some magic from me!",
  
  "Damn you're so bad girl! I like it!",
  "I like your boobs, by the way!",
  "You're so sexy",
  "I'm sexy too. And I know it!",
  "Come to my place?",

  "I will not marry you. But I like tits",
  "Do you think we are not alone in the Universe?",
  "I know we're not alone. You and me and the Universe",
  "I can show you the whole internal Universe of you.",
  "It is called unconsciousness.",

  "And there is a whole world deep inside you",
  "This is a beautiful world full of LOVE"
};

int mesIndex = 0;
int mesArrayLength = 22;

String msg = messages[mesIndex];            
uint8_t msgLen = msg.length();              
int msgX = matrix.width(); // Start off right edge
unsigned long prevFrameTime = 0L;             // For animation timing
#define FPS 20                                // Scrolling speed

uint16_t remapXY(uint16_t x, uint16_t y) {
  return y * WIDTH + x - OFFSET;
}
 
void setup() {
  matrix.begin();
  matrix.setRemapFunction(remapXY);
  matrix.setTextWrap(true);   
  matrix.setTextColor(0xF800); // Red
  matrix.setBrightness(BRIGHTNESS);
}
 
void loop() {
  
  unsigned long t = millis(); 
  
  if((t - prevFrameTime) >= (1000L / FPS)) { // Handle scrolling
    matrix.fillScreen(0);
    matrix.setCursor(msgX, 0);
    matrix.print(msg);
    
    if(--msgX < (msgLen * -6)) {
      //msgX = matrix.width(); //repeat

      //next message
      if(mesIndex % 4 == 0) {
        glitch(); 
      }
      
      if(++mesIndex >= mesArrayLength) {
        mesIndex = 0;
      }

      matrix.setTextColor(getRandomColor());
      matrix.setBrightness(BRIGHTNESS);    
      
      msg = messages[mesIndex];
      msgX = matrix.width();
      msgLen = msg.length();
    }
    
    matrix.show();
    prevFrameTime = t;
  }
}

void glitch() {

  for(int i = 0; i < GLITCH_COUNT; i++) {
    int pixel = random(TOTAL);
    matrix.setPixelColor(pixel, random(255), random(255), random(255), BRIGHTNESS);
    matrix.show();
  }

  delay(500);
}

unsigned long getRandomColor() {
  int r = random(255);
  int g = random(255);
  int b = random(255);

  return createRGB(r, g, b);
}

unsigned long createRGB(int r, int g, int b)
{   
    return ((r & 0xff) << 16) + ((g & 0xff) << 8) + (b & 0xff);
}


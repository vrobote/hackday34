//
// test LED matrix 8x8 over SPI
//
// http://robocraft.ru
//

#include <SPI.h>
#include "spi_led_matrix.h"

//
// https://learn.adafruit.com/animating-multiple-led-backpacks
//
// https://github.com/adafruit/Adafruit-LED-Backpack-Library/blob/master/examples/roboface/roboface.pde
//
static const uint8_t PROGMEM // Bitmaps are stored in program memory
  blinkImg[][8] = {    // Eye animation frames
  { B00111100,         // Fully open eye
    B01111110,
    B11111111,
    B11111111,
    B11111111,
    B11111111,
    B01111110,
    B00111100 },
  { B00000000,
    B01111110,
    B11111111,
    B11111111,
    B11111111,
    B11111111,
    B01111110,
    B00111100 },
  { B00000000,
    B00000000,
    B00111100,
    B11111111,
    B11111111,
    B11111111,
    B00111100,
    B00000000 },
  { B00000000,
    B00000000,
    B00000000,
    B00111100,
    B11111111,
    B01111110,
    B00011000,
    B00000000 },
  { B00000000,         // Fully closed eye
    B00000000,
    B00000000,
    B00000000,
    B10000001,
    B01111110,
    B00000000,
    B00000000 } };
uint8_t blinkIndex[] = { 1, 2, 3, 4, 3, 2, 1 }; // Blink bitmap sequence
uint8_t closeIndex[] = { 1, 2, 3, 4 }; // Close bitmap sequence
uint8_t openIndex[] = { 3, 2, 1 }; // Open bitmap sequence


// выходной каскад из двух сдвиговых регистров 74HC595
// SS на 53 pin
SPI_LedMatrix eyes(53);

void setup()
{
    Serial.begin(57600);

    Serial.println(F("[i] Start..."));
}

enum {ST_NONE=0, ST_CENTER=1, ST_UP, ST_DOWN, ST_LEFT, ST_RIGHT,
      ST_CLOSE, ST_OPEN, ST_BLINK};
int state = ST_CENTER;
int counter=0, idx=0;
int eyeX=3, eyeY=3;
int close_eye = EYE_BOTH;

void make_bitmap_sequence(uint8_t* seq, size_t seq_size)
{
    eyes.drawBitmap(close_eye, 0, 0, blinkImg[seq[idx]], 8, 8, LED_ON);
    eyes.drawRect(close_eye, eyeX, eyeY, eyeX+2, eyeY+2, LED_OFF);

    eyes.writeDisplay();
    delay(1);
    counter++;
    if(counter>5) {
        eyes.clear(close_eye);
        counter=0;
        idx++;
        if(idx >= seq_size) {
            idx=0;
            if(state == ST_BLINK || state == ST_OPEN) {
                state = ST_CENTER;
            }
            else if(state == ST_CLOSE) {
                state = ST_NONE;
                eyes.drawBitmap(close_eye, 0, 0, blinkImg[seq[seq_size-1]], 8, 8, LED_ON);
            }
        }
    }
}

void loop()
{
#if 0
    eyes.clear(0);
    for(int y=0; y<LEDS_MATRIX_L; y++) {
        for(int x=0; x<LEDS_MATRIX_L; x++) {
            eyes.clear(0);
            eyes.drawPixel(0, x, y, LED_ON);
            delay(50);
        }
    }
    eyes.clear(0);
    for(int x=0; x<LEDS_MATRIX_L; x++) {
        for(int y=0; y<LEDS_MATRIX_L; y++) {
            eyes.clear(0);
            eyes.drawPixel(0, x, y, LED_ON);
            delay(25);
        }
    }
#endif

#if 0
    eyes.drawPixel(0, 0, 0, LED_ON);
    eyes.drawPixel(0, 1, 1, LED_ON);
    eyes.writeDisplay();
    //delay(5);

    //Serial.println(F("."));

//    eyes.drawPixel(0, 0, 0, LED_OFF);
//    eyes.drawPixel(0, 1, 0, LED_ON);
//    delay(1000);

#endif

#if 0
    //eyes.drawRect(0, 0, 0, 8, 8, LED_ON);
    eyes.drawEye(0);
    //eyes.drawRect(0, 3, 3, 5, 5, LED_OFF); // center
    //eyes.writeDisplay();


    //eyes.drawRect(0, 3, 5, 5, 7, LED_OFF); // left
    //eyes.drawRect(0, 3, 1, 5, 3, LED_OFF); // right
    //eyes.drawRect(0, 1, 3, 3, 5, LED_OFF); // up
    //eyes.drawRect(0, 5, 3, 7, 5, LED_OFF); // down
    eyes.clear(0);
    eyes.drawRect(0, 3, 0, 5, 8, LED_ON);
    eyes.writeDisplay();
    //delay(100);
#endif

#if 1

    int c;
    if(Serial.available()) {
        c = Serial.read();
        if(c == 'w') {
            state = ST_UP;
            eyeX--;
        }
        else if(c == 's') {
            state = ST_DOWN;
            eyeX++;
        }
        else if(c == 'a') {
            state = ST_LEFT;
            eyeY--;
        }
        else if(c == 'd') {
            state = ST_RIGHT;
            eyeY++;
        }
        else if(c == 'c') {
            eyeX = 3; eyeY = 3;
            state = ST_CENTER;
        }
        else if(c == 'h') {
            state = ST_CLOSE;
            close_eye = EYE_LEFT;
        }
        else if(c == 'j') {
            state = ST_OPEN;
            close_eye = EYE_LEFT;
        }
        else if(c == 'k') {
            state = ST_CLOSE;
            close_eye = EYE_RIGHT;
        }
        else if(c == 'l') {
            state = ST_OPEN;
            close_eye = EYE_RIGHT;
        }
        else {
            state = ST_BLINK;
            close_eye = EYE_BOTH;
        }
    }

    if(eyeX < 0) eyeX=0;
    if(eyeY < 0) eyeY=0;
    if(eyeX > 6) eyeX=6;
    if(eyeY > 6) eyeY=6;

//    switch(state) {
//    case ST_UP:
//        eyes.drawRect(0, 1, 3, 3, 5, LED_OFF); // up
//        break;
//    case ST_DOWN:
//        eyes.drawRect(0, 5, 3, 7, 5, LED_OFF); // down
//        break;
//    case ST_LEFT:
//        eyes.drawRect(0, 3, 5, 5, 7, LED_OFF); // left
//        break;
//    case ST_RIGHT:
//        eyes.drawRect(0, 3, 1, 5, 3, LED_OFF); // right
//        break;
//    case ST_CLOSE:
//        eyes.clear(0);
//        eyes.drawRect(0, 3, 0, 5, 8, LED_ON);
//        break;
//    case ST_CENTER:
//    default:
//        eyes.drawRect(0, 3, 3, 5, 5, LED_OFF); // center
//        break;
//    }

    if(state == ST_NONE) {
        eyes.writeDisplay();
    }
    else if(state < ST_CLOSE) {
        eyes.drawEye(EYE_BOTH);
        eyes.drawRect(EYE_BOTH, eyeX, eyeY, eyeX+2, eyeY+2, LED_OFF);
        eyes.writeDisplay();
    }
    else if(state == ST_CLOSE) {
        make_bitmap_sequence(closeIndex, sizeof(closeIndex));
    }
    else if(state == ST_OPEN) {
        make_bitmap_sequence(openIndex, sizeof(openIndex));
    }
    else if(state == ST_BLINK) {
        make_bitmap_sequence(blinkIndex, sizeof(blinkIndex));
    }

    //delay(100);
#endif

#if 0
    eyes.drawBitmap(0, 0, blinkImg[blinkIndex[idx]], 8, 8, LED_ON);
    eyes.drawRect(0, eyeX, eyeY, eyeX+2, eyeY+2, LED_OFF);

    eyes.writeDisplay();
    delay(1);
    counter++;
    if(counter>20) {
        eyes.clear(0);
        counter=0;
        idx++;
        if(idx >= sizeof(blinkIndex)) {
            idx=0;
        }
    }
#endif

}

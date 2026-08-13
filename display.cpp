#include "display.h"
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);
bool displayInit()
{
    if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
        Serial.println("OLED ikke funnet!");
        return false;
    }
    return true;
}

void displayClear()
{
    display.clearDisplay();
}

void displayPrint(const char *text, int x, int y)
{
    display.setTextColor(SSD1306_WHITE);
    
    display.setTextSize(1);
    display.setCursor(x, y);
    display.println(text);

}

void displayShow()
{
    display.display();
}

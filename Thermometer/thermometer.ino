

#include <LiquidCrystal.h>
int tempPin = 0;
//                BS  E  D4 D5  D6 D7
LiquidCrystal lcd(2, 8, 9, 10, 11, 12);

#define BLUE 3
#define GREEN 5
#define RED 6

int tempType = 0;
int buttonPin = 16;
int buttonNew;
int buttonOld = 0;

void setup()
{
    lcd.begin(16, 2);

    pinMode(RED, OUTPUT);
    pinMode(GREEN, OUTPUT);
    pinMode(BLUE, OUTPUT);
}
void loop()
{
    int tempReading = analogRead(tempPin);
    double tempK = log(10000.0 * ((1024.0 / tempReading - 1)));
    tempK = 1 / (0.001129148 + (0.000234125 + (0.0000000876741 * tempK * tempK)) * tempK); //  Temp Kelvin
    float tempC = tempK - 273.15;                                                          // Convert Kelvin to Celcius
    float tempF = (tempC * 9.0) / 5.0 + 32.0;

    handleDisplayLCD(tempC, tempF);
    handleDisplayLed(tempC);
}

void handleDisplayLCD(float tempC, float tempF)
{
    buttonNew = digitalRead(buttonPin);

    if (buttonOld == 0 && buttonNew == 1)
    {
        if (tempType == 0)
        {
            lcd.setCursor(0, 0);
            lcd.print("Temp         C  ");
            lcd.setCursor(6, 0);
            lcd.print(tempC);
            tempType = 1;
            delay(500);
        }
        else
        {
            lcd.setCursor(0, 0);
            lcd.print("Temp         F  ");
            lcd.setCursor(6, 0);
            lcd.print(tempF);
            tempType = 0;
            delay(500);
        }
    }
    buttonOld = buttonNew;
    delay(100);
}

void handleDisplayLed(float tempC)
{
    if (tempC > 30)
    {
        setColor(100, 0, 0); // Red Color
    }
    else if ((tempC > 20) && (tempC <= 30))
    {
        setColor(100, 100, 0); // Yellow Color
    }
    else if ((tempC > 10) && (tempC <= 20))
    {
        setColor(0, 100, 0); // Green Color
    }
    else if (tempC <= 10)
    {
        setColor(0, 0, 100); // blue Color
    }
}

void setColor(int redValue, int greenValue, int blueValue)
{
    analogWrite(RED, redValue);
    analogWrite(GREEN, greenValue);
    analogWrite(BLUE, blueValue);
}

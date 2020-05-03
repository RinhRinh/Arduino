#include <LiquidCrystal.h>

LiquidCrystal lcd(2, 8, 9, 10, 11, 12);

#define BLUE 3
#define GREEN 5
#define RED 6

int tempPin = 0;
int tempType = 0;
int buttonPin = 16;
int buttonNew;
int buttonOld = 1;

unsigned long currentMillis = 0;
unsigned long previousMillisLCD = 0;
unsigned long previousMillisButton = 0;
const long intervalLCD = 500;
const long intervalButton = 100;

void setup()
{
    Serial.begin(9600);
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

    handleButton();
    handleDisplayLCD(tempC, tempF, tempK);
    handleDisplayLed(tempC);
}

void handleButton()
{
    buttonNew = digitalRead(buttonPin);

    currentMillis = millis();

    if (currentMillis - previousMillisButton >= intervalButton)
    {
        previousMillisButton = currentMillis;

        if (buttonOld == 0 && buttonNew == 1)
        {
            if (tempType == 0)
            {
                tempType = 1;
            }
            else if (tempType == 1)
            {
                tempType = 2;
            }

            else if (tempType == 2)
            {
                tempType = 0;
            }
        }
    }
    buttonOld = buttonNew;
    delay(100);
}

void handleDisplayLCD(float tempC, float tempF, double tempK)
{
    currentMillis = millis();

    if (currentMillis - previousMillisLCD >= intervalLCD)
    {
        previousMillisLCD = currentMillis;

        if (tempType == 0)
        {
            lcd.setCursor(0, 0);
            lcd.print("Temp         C  ");
            lcd.setCursor(6, 0);
            lcd.print(tempC);
        }
        else if (tempType == 1)
        {
            lcd.setCursor(0, 0);
            lcd.print("Temp         F  ");
            lcd.setCursor(6, 0);
            lcd.print(tempF);
        }
        else if (tempType == 2)
        {
            lcd.setCursor(0, 0);
            lcd.print("Temp          K ");
            lcd.setCursor(6, 0);
            lcd.print(tempK);
        }
    }
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

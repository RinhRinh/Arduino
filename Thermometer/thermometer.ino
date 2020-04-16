//www.elegoo.com
//2016.12.9

#include <LiquidCrystal.h>
int tempPin = 0;
//                BS  E  D4 D5  D6 D7
LiquidCrystal lcd(7, 8, 9, 10, 11, 12);

const uint8_t blue = 19;
const uint8_t green = 18;
const uint8_t yellow = 17;
const uint8_t red = 16;

void setup()
{
    /*lcd.begin(cols, rows)
    lcd: a variable of type LiquidCrystal
    cols: the number of columns that the display has
    rows: the number of rows that the display has 
  */

    lcd.begin(16, 2);

    pinMode(blue, OUTPUT);
    pinMode(green, OUTPUT);
    pinMode(yellow, OUTPUT);
    pinMode(red, OUTPUT);
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
    // Display Temperature in C
    lcd.setCursor(0, 0);
    lcd.print("Temp         C  ");
    // Display Temperature in F
    //lcd.print("Temp         F  ");
    lcd.setCursor(6, 0);
    // Display Temperature in C
    lcd.print(tempC);
    // Display Temperature in F
    //lcd.print(tempF);
    delay(500);
    lcd.setCursor(0, 1);
    lcd.print("Temp         F  ");
    lcd.setCursor(6, 1);
    lcd.print(tempF);
}

void handleDisplayLed(float tempC)
{
    if (tempC > 30)
    {
        clearAll();
        digitalWrite(red, HIGH);
    }
    else if ((tempC > 20) && (tempC <= 30))
    {
        clearAll();
        digitalWrite(yellow, HIGH);
    }
    else if ((tempC > 10) && (tempC <= 20))
    {
        clearAll();
        digitalWrite(green, HIGH);
    }
    else if (tempC <= 10)
    {
        clearAll();
        digitalWrite(blue, HIGH);
    }
}
void clearAll()
{
    digitalWrite(red, LOW);
    digitalWrite(yellow, LOW);
    digitalWrite(green, LOW);
    digitalWrite(blue, LOW);
}
#include <LiquidCrystal.h>
LiquidCrystal lcd(2, 8, 9, 10, 11, 12);

#define BLUE 3
#define GREEN 5
#define RED 6

const int tempPin = 0;
int tempType = 0;

const int buttonPinThermometer = 15;
const long intervalButtonThermometer = 100;
unsigned long previousMillisButtonThermometer = 0;
int buttonNewThermometer;
int buttonOldThermometer = 1;

const int buttonPinClockHour = 16;
const int buttonPinClockMinute = 17;
const long intervalClockButtonHour = 100;
const long intervalClockButtonMinute = 100;
unsigned long previousMillisClockButtonHour = 0;
unsigned long previousMillisClockButtonMinute = 0;
int buttonNewClockHour;
int buttonOldClockHour = 1;
int buttonNewClockMinute;
int buttonOldClockMinute = 1;

unsigned long currentMillis = 0;

unsigned long previousMillisThermometer = 0;
const long intervalThermometer = 650;

int tempReading;
double tempK;
float tempC;
float tempF;

unsigned long previousMillisClock = 0;
const long intervalClock = 1000;
int secondDisplay = 0;
int tenSecondDisplay = 0;
int minuteDisplay = 0;
int tenMinuteDisplay = 0;
int hourDisplay = 2;
int tenHourDisplay = 1;
int morningOrEvening = 0;

void setup()
{
    lcd.begin(16, 2);

    pinMode(RED, OUTPUT);
    pinMode(GREEN, OUTPUT);
    pinMode(BLUE, OUTPUT);
}

void loop()
{
    // process input
    processTemp();
    handleButtonThermometer();
    handleClockSetTimeButtons();
    calculateClockInfo();

    // action
    handleDisplayLCDForThermometer();
    handleDisplayLCDForClock();
    handleDisplayLed();
}

void processTemp()
{
    tempReading = analogRead(tempPin);

    tempK = log(10000.0 * ((1024.0 / tempReading - 1)));
    tempK = 1 / (0.001129148 + (0.000234125 + (0.0000000876741 * tempK * tempK)) * tempK); //  Temp Kelvin
    tempC = tempK - 273.15;                                                                // Convert Kelvin to Celcius
    tempF = (tempC * 9.0) / 5.0 + 32.0;
}

void handleButtonThermometer()
{
    buttonNewThermometer = digitalRead(buttonPinThermometer);

    currentMillis = millis();

    if (currentMillis - previousMillisButtonThermometer >= intervalButtonThermometer)
    {
        previousMillisButtonThermometer = currentMillis;

        if (buttonOldThermometer == 0 && buttonNewThermometer == 1)
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

        buttonOldThermometer = buttonNewThermometer;
    }
}

void handleDisplayLCDForThermometer()
{
    currentMillis = millis();

    if (currentMillis - previousMillisThermometer >= intervalThermometer)
    {
        previousMillisThermometer = currentMillis;

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

void calculateClockInfo()
{
    if (currentMillis - previousMillisClock >= intervalClock)
    {
        secondDisplay = secondDisplay + 1;

        if (secondDisplay > 0 && secondDisplay % 10 == 0)
        {
            secondDisplay = 0;
            tenSecondDisplay = tenSecondDisplay + 1;
        }

        if (tenSecondDisplay == 6)
        {
            secondDisplay = 0;
            tenSecondDisplay = 0;
            minuteDisplay = minuteDisplay + 1;
        }

        if (minuteDisplay > 0 && minuteDisplay % 10 == 0)
        {
            minuteDisplay = 0;
            tenMinuteDisplay = tenMinuteDisplay + 1;
        }

        if (tenMinuteDisplay == 6)
        {
            minuteDisplay = 0;
            tenMinuteDisplay = 0;
            hourDisplay = hourDisplay + 1;
        }

        if (hourDisplay == 10)
        {
            hourDisplay = 0;
            tenHourDisplay = tenHourDisplay + 1;
        }

        if ((tenHourDisplay == 1) && (hourDisplay == 3))
        {
            secondDisplay = 0;
            tenSecondDisplay = 0;
            minuteDisplay = 0;
            tenMinuteDisplay = 0;
            hourDisplay = 1;
            tenHourDisplay = 0;
        }

        if ((tenHourDisplay == 1) && (hourDisplay == 2) && (tenMinuteDisplay == 0) && (minuteDisplay == 0) && (tenSecondDisplay == 0) && (secondDisplay == 0))
        {
            morningOrEvening = morningOrEvening + 1;
        }

        previousMillisClock = currentMillis;
    }
}

void handleClockButtons()
{
    buttonNewClockHour = digitalRead(buttonPinClockHour);
    buttonNewClockMinute = digitalRead(buttonPinClockMinute);

    currentMillis = millis();

    if (currentMillis - previousMillisClockButtonHour >= intervalClockButtonHour)
    {
        if (buttonOldClockHour == 0 && buttonNewClockHour == 1)
        {
            hourDisplay = hourDisplay + 1;
        }

        if (buttonOldClockMinute == 0 && buttonNewClockMinute == 1)
        {
            minuteDisplay = minuteDisplay + 1;
        }
        previousMillisClockButtonHour = currentMillis;

        buttonOldClockHour = buttonNewClockHour;
        buttonOldClockMinute = buttonNewClockMinute;
    }
}

void handleDisplayLCDForClock()
{
    lcd.setCursor(0, 1);
    lcd.print("Time");
    lcd.setCursor(5, 1);
    lcd.print(tenHourDisplay);
    lcd.setCursor(6, 1);
    lcd.print(hourDisplay);
    lcd.setCursor(7, 1);
    lcd.print(":");
    lcd.setCursor(8, 1);
    lcd.print(tenMinuteDisplay);
    lcd.setCursor(9, 1);
    lcd.print(minuteDisplay);
    lcd.setCursor(10, 1);
    lcd.print(":");
    lcd.setCursor(11, 1);
    lcd.print(tenSecondDisplay);
    lcd.setCursor(12, 1);
    lcd.print(secondDisplay);

    if ((morningOrEvening % 2) == 0)
    {
        lcd.setCursor(14, 1);
        lcd.print("AM");
    }
    else
    {
        lcd.setCursor(14, 1);
        lcd.print("PM");
    }
}

void handleDisplayLed()
{
    if (tempC > 30)
    {
        setColor(100, 0, 0); // Red Color
    }
    else if ((tempC > 20) && (tempC <= 30))
    {
        setColor(85, 115, 0); // Yellow Color
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

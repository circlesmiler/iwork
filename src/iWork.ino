// This #include statement was automatically added by the Particle IDE.
#include <clickButton.h>

#include "Adafruit_GFX.h"
#include "Adafruit_SSD1306.h"

//PIN PHOTON => PIN OLED
#define OLED_MOSI   D0 //=>D1
#define OLED_CLK    D1 //=>D0
#define OLED_DC     D2 //=>DC
#define OLED_CS     D3 //=>CS
#define OLED_RESET  D4 //=>RST
Adafruit_SSD1306 display(OLED_MOSI, OLED_CLK, OLED_DC, OLED_RESET, OLED_CS);

#if (SSD1306_LCDHEIGHT != 64)
#error("Height incorrect, please fix Adafruit_SSD1306.h!");
#endif

// Icons
// Created by http://javl.github.io/image2cpp/
const unsigned char clockIcon [] PROGMEM = {
    0x3c, 0x4a, 0x89, 0x89, 0xb1, 0x81, 0x42, 0x3c
};

const unsigned char loginIcon [] PROGMEM = {
	0x3f, 0x01, 0x11, 0xf9, 0xf9, 0x11, 0x01, 0x3f
};

const unsigned char logoutIcon [] PROGMEM = {
    0x3f, 0x01, 0x21, 0x7d, 0x7d, 0x21, 0x01, 0x3f
};
const unsigned char tempIcon [] PROGMEM = {
    0x1c, 0x22, 0x2a, 0x2a, 0x2a, 0x5d, 0x5d, 0x3e
};
// WIFI Icons 1 = LOW, 4=FULL
const unsigned char wifiIcon1 [] PROGMEM = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x18
};
const unsigned char wifiIcon2 [] PROGMEM = {
    0x00, 0x00, 0x00, 0x00, 0x18, 0x24, 0x18, 0x18
};
const unsigned char wifiIcon3 [] PROGMEM = {
    0x00, 0x00, 0x3c, 0x42, 0x18, 0x24, 0x18, 0x18
};
const unsigned char wifiIcon4 [] PROGMEM = {
    0x7e, 0x81, 0x3c, 0x42, 0x18, 0x24, 0x18, 0x18
};
const unsigned char noWifiIcon [] PROGMEM = {
    0xff, 0xc3, 0x3c, 0x5a, 0x18, 0x24, 0x5a, 0x99
};

const unsigned char lampIcon [] PROGMEM = {
	0x00, 0x08, 0x1e, 0x2c, 0x48, 0x20, 0x10, 0x78
};

const unsigned char lampIcon24 [] PROGMEM = {
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x0f, 0x00, 0x00, 0x27, 0x80, 0x00,
	0x77, 0xf0, 0x00, 0x77, 0xf8, 0x00, 0xc7, 0xfc, 0x01, 0x8f, 0xf8, 0x03, 0x0f, 0xe0, 0x06, 0x0f,
	0x90, 0x38, 0x04, 0x60, 0x38, 0x00, 0x00, 0x1c, 0x00, 0x00, 0x06, 0x00, 0x00, 0x03, 0x00, 0x00,
	0x01, 0x80, 0x00, 0x00, 0xc0, 0x00, 0x00, 0x70, 0x00, 0x00, 0x78, 0x00, 0x00, 0x7c, 0x00, 0x01,
	0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

// Constants
#define ENTER_ADDR 0
#define EXIT_ADDR 4

// PIN LED-Button Constants
#define BTN_LOGIN   D5
#define BTN_LOGOUT  D6
#define BTN_MODE    D7
#define LED_LOGIN   A0
#define LED_LOGOUT  A1
#define LED_MODE    A2

// Debounce Stuff
#define T_TIMEOUT 100 // milliseconds

ClickButton loginButton(BTN_LOGIN, LOW, CLICKBTN_PULLUP);
ClickButton logoutButton(BTN_LOGOUT, LOW, CLICKBTN_PULLUP);
ClickButton modeButton(BTN_MODE, LOW, CLICKBTN_PULLUP);

void blinkModeLED()
{
    Serial.println("Blink LED");
    toggleModeLED();
}

Timer blinkModeLEDtimer(1000, blinkModeLED);

// VARIABLES
int enteredTime = 0;
int exitedTime = 0;
String enteredTimeStr;
String exitedTimeStr;

#define WEATHER_UPDATE_INTERVAL 3600 // 3600sec = 1h
#define Rostock_City_ID "2844588"
int lastWeatherUpdate = 0;
double tempHRO;

// For debugging
String dataStr;

// IFTTT EVENT HANDLER
void onWorkHandler(const char *event, const char *data) {
    dataStr = String(data).trim().toLowerCase();
    if (dataStr.equals("entered")) {
        setEnteredTime(Time.now());
    } else if (dataStr.equals("exited")) {
        setExitedTime(Time.now());
    }
}

// WEATHER HANDLER
void receiveWeather(const char *event, const char *data) {
    // Convert String to double
    tempHRO = atof(data);
}

int mode = 0;

// STARTUP(WiFi.selectAntenna(ANT_AUTO));
STARTUP(WiFi.selectAntenna(ANT_INTERNAL));

// SETUP
void setup()   {
    Serial.begin(9600);

    // Load data from EEPROM
    setEnteredTime(loadEEPROM(ENTER_ADDR));
    setExitedTime(loadEEPROM(EXIT_ADDR));

    // by default, we'll generate the high voltage from the 3.3v line internally! (neat!)
    display.begin(SSD1306_SWITCHCAPVCC);
    // init done

    display.display(); // show splashscreen
    delay(1000);
    display.clearDisplay();   // clears the screen and buffer

    // Time.zone(+2);
    Time.zone(+1);

    Particle.subscribe("on_work", onWorkHandler);
    Particle.subscribe("hook-response/weather/", receiveWeather, MY_DEVICES);

    Particle.variable("enteredTime", enteredTimeStr);
    Particle.variable("exitedTime", exitedTimeStr);
    Particle.variable("eventStr", dataStr);

    // PIN MODES
    pinMode(BTN_MODE, INPUT_PULLUP);
    // Setup button timers (all in milliseconds / ms)
    // (These are default if not set, but changeable for convenience)
    modeButton.debounceTime   = 30;   // Debounce timer in ms
    modeButton.longClickTime  = 1000; // time until "held-down clicks" register

    pinMode(LED_LOGIN, OUTPUT);
    pinMode(LED_LOGOUT, OUTPUT);
    pinMode(LED_MODE, OUTPUT);

    setModeLED(true);
}

int page = 0;
// PAINT STUFF IN LOOP
void loop() {
    // Update button state
    loginButton.Update();
    logoutButton.Update();
    modeButton.Update();

    // Save click codes in LEDfunction, as click codes are reset at next Update()
    int function = 0;
    if(modeButton.clicks != 0) function = modeButton.clicks;

    if(function == 1) {
        Serial.println("SINGLE click");
        if (mode == 0) {
            Serial.println("Change Page");
            // Normaler Modus > Seite weiterschalten
            page = (page + 1) % 4;
        }
    }
    if(function == -1) {
        Serial.println("SINGLE LONG click");
        if (mode == 0) {
            Serial.println("Edit Mode");
            mode = 1;
            blinkModeLEDtimer.start();
        } else if (mode == 1) {
            Serial.println("Normal Mode");
            mode = 0;
            blinkModeLEDtimer.stop();
            setModeLED(true);
        }
    }

    // Load weather data
    if ((lastWeatherUpdate + WEATHER_UPDATE_INTERVAL) < Time.now()) {
        Particle.publish("weather", Rostock_City_ID, PRIVATE);
        lastWeatherUpdate = Time.now();
    }

    display.clearDisplay();   // clears the screen and buffer

    updateLoginLED();
    updateLogoutLED();
    updateModeLED();

    if (page == 0) {
        printTime();
        printWifiState();
        printWorkStarted();
        printWorkDuration();
        printExitTime();
        printTemperature();
    }

    display.display();
}

void updateLoginLED() {
    digitalWrite(LED_LOGIN, enteredTime <= exitedTime);
}

void updateLogoutLED() {
    digitalWrite(LED_LOGOUT, enteredTime > exitedTime);
}

void updateModeLED() {
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setTextWrap(false);
    display.setCursor(60,56);

    display.print(page);
}

void printWorkStarted() {
    display.drawBitmap(0, 0, loginIcon, 8, 8, WHITE);

    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setTextWrap(false);

    display.setCursor(10,0);

    // Not initialized
    if (enteredTime == 0 && exitedTime == 0) {
        display.print("--:--");
        return;
    }

    if (enteredTime < exitedTime) {
        display.print("--:--");
    } else {
        display.print(Time.format(enteredTime, "%H:%M"));
    }
}

void printTime() {
    display.drawBitmap(88, 0, clockIcon, 8, 8, WHITE);

    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setTextWrap(false);

    display.setCursor(98,0);

    if (Time.now() % 2 == 0) {
        display.print(Time.format(Time.now(), "%H:%M"));
    } else {
        display.print(Time.format(Time.now(), "%H %M"));
    }
}

void printWifiState() {
    if (!WiFi.ready()) {
        display.drawBitmap(60, 0, noWifiIcon, 8, 8, WHITE);
        return;
    }

    // Signal strength
    int sig = WiFi.RSSI();
    if (sig < -95) {
        // Bad signal
        display.drawBitmap(60, 0, wifiIcon1, 8, 8, WHITE);
    } else if (-95 <= sig && sig < -63) {
        display.drawBitmap(60, 0, wifiIcon2, 8, 8, WHITE);
    } else if (-63 <= sig && sig < -31) {
        display.drawBitmap(60, 0, wifiIcon3, 8, 8, WHITE);
    } else if (-31 <= sig && sig < 0) {
        // Good signal
        display.drawBitmap(60, 0, wifiIcon4, 8, 8, WHITE);
    } else if (sig > 0) {
        // Error
        display.drawBitmap(60, 0, noWifiIcon, 8, 8, WHITE);
    }
}

void printWorkDuration() {
    display.drawBitmap(0, 20, lampIcon24, 24, 24, WHITE);

    display.setTextSize(3);
    display.setTextColor(WHITE);
    display.setTextWrap(false);

    display.setCursor(30,23);

    // Not initialized
    if (enteredTime == 0 && exitedTime == 0) {
        display.print("--:--");
        return;
    }

    if (enteredTime < exitedTime) {
        display.print("--:--");
    } else {
        int timeNow = Time.now();
        int timeDiff = timeNow - enteredTime;
        int minutes = timeDiff / 60;

        // Hours
        display.print(String::format("%02d", minutes / 60));

        if (timeNow % 2 == 0) {
            display.print(":");
        } else {
            display.print(" ");
        }

        // Minutes
        display.print(String::format("%02d", minutes % 60));
    }
}

void printExitTime() {
    display.drawBitmap(0, 56, logoutIcon, 8, 8, WHITE);

    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setTextWrap(false);

    display.setCursor(10, 56);

    if (exitedTime == 0) {
        display.print("--:--");
    } else {
        display.print(Time.format(exitedTime, "%H:%M"));
    }
}

void printTemperature() {
    display.drawBitmap(88, 56, tempIcon, 8, 8, WHITE);

    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setTextWrap(false);

    display.setCursor(98, 56);

    if (lastWeatherUpdate == 0) {
        display.print("-.-");
    } else {
        // display.print(String::format("%+5.1f", tempHRO));
        display.print(String::format("%+3.0f%cC", tempHRO, char(247)));
        //display.print(tempHRO);
    }
}

/*
* HELPER METHODS
*/
void toggleModeLED() {
    digitalWrite(LED_MODE, !digitalRead(LED_MODE));
}

void setModeLED(bool isOn) {
    digitalWrite(LED_MODE, isOn);
}

int loadEEPROM(int address) {
    int tmpVal;
    EEPROM.get(address, tmpVal);
    return tmpVal;
}

void setEnteredTime(long timeInSeconds) {
    if (timeInSeconds > 1483228800 && timeInSeconds < 4102444800) {
        enteredTime = timeInSeconds;
        EEPROM.put(ENTER_ADDR, enteredTime);
    }
    enteredTimeStr = Time.format(enteredTime, TIME_FORMAT_DEFAULT);
}

void setExitedTime(long timeInSeconds) {
    if (timeInSeconds > 1483228800 && timeInSeconds < 4102444800) {
        exitedTime = timeInSeconds;
        EEPROM.put(EXIT_ADDR, exitedTime);
    }
    exitedTimeStr = Time.format(exitedTime, TIME_FORMAT_DEFAULT);
}
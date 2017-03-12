// This #include statement was automatically added by the Particle IDE.
#include <clickButton.h>

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include "DataModel.h"
#include "Icons.h"

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

// PIN LED-Button Constants
#define BTN_LOGIN   D5
#define BTN_LOGOUT  D6
#define BTN_MODE    D7

#define LED_LOGIN   A0
#define LED_LOGOUT  A1
#define LED_MODE    A2

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
DataModel dataModel;

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
        dataModel.login();
    } else if (dataStr.equals("exited")) {
        dataModel.logout();
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
    dataModel.login(loadEEPROM(ENTER_ADDR));
    dataModel.logout(loadEEPROM(EXIT_ADDR));

    // by default, we'll generate the high voltage from the 3.3v line internally! (neat!)
    display.begin(SSD1306_SWITCHCAPVCC);
    // init done

    display.display(); // show splashscreen
    delay(500);
    display.clearDisplay();   // clears the screen and buffer

    // Time.zone(+2);
    Time.zone(+1);

    Particle.subscribe("on_work", onWorkHandler);
    Particle.subscribe("hook-response/weather/", receiveWeather, MY_DEVICES);

    pinMode(LED_LOGIN, OUTPUT);
    pinMode(LED_LOGOUT, OUTPUT);
    pinMode(LED_MODE, OUTPUT);

    digitalWrite(LED_LOGIN, HIGH);
    delay(200);
    digitalWrite(LED_LOGIN, LOW);

    digitalWrite(LED_LOGOUT, HIGH);
    delay(200);
    digitalWrite(LED_LOGOUT, LOW);

    digitalWrite(LED_MODE, HIGH);
}

int page = 0;
// PAINT STUFF IN LOOP
void loop() {
    checkButtonState();

    // Load weather data
    if ((lastWeatherUpdate + WEATHER_UPDATE_INTERVAL) < Time.now()) {
        Particle.publish("weather", Rostock_City_ID, PRIVATE);
        lastWeatherUpdate = Time.now();
    }

    display.clearDisplay();   // clears the screen and buffer

    updateLoginLED();
    updateLogoutLED();

    printPageNumber();
    if (page == 0) {
      printTime();
      printWifiState();
      printWorkStarted();
      printWorkDuration();
      printExitTime();
      printTemperature();
    } else if (page == 1) {
      printPage2();
    }

    display.display();
}

void checkButtonState() {
  performLoginButton();
  performLogoutButton();
  performModeButton();
}

void performLoginButton() {
  loginButton.Update();
  // Save click codes in LEDfunction, as click codes are reset at next Update()
  int function = 0;
  if(loginButton.clicks != 0) function = loginButton.clicks;

  if(function == 1) {
  Serial.println("Login button clicked.");
    if (mode == 0) {
      dataModel.login();
    }
  }
}

void performLogoutButton() {
  logoutButton.Update();
  // Save click codes in LEDfunction, as click codes are reset at next Update()
  int function = 0;
  if(logoutButton.clicks != 0) function = logoutButton.clicks;

  if(function == 1) {
    Serial.println("Logout button clicked.");
    if (mode == 0) {
      dataModel.logout();
    }
  }
}

void performModeButton() {
  modeButton.Update();
  // Save click codes in LEDfunction, as click codes are reset at next Update()
  int function = 0;
  if(modeButton.clicks != 0) function = modeButton.clicks;

  if(function == 1) {
      Serial.println("SINGLE click");
      if (mode == 0) {
          Serial.println("Change Page");
          // Normaler Modus > Seite weiterschalten
          page = (page + 1) % 2;
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
}

void updateLoginLED() {
    digitalWrite(LED_LOGIN, !dataModel.atWork());
}

void updateLogoutLED() {
    digitalWrite(LED_LOGOUT, dataModel.atWork());
}

void printPageNumber() {
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setTextWrap(false);
    display.setCursor(60,56);

    display.print(page + 1);
}

void printPage2() {
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setTextWrap(true);
  display.setCursor(0,10);

  display.println(" COMING");
  display.print("    SOON");
}

void printWorkStarted() {
    display.drawBitmap(0, 0, loginIcon, 8, 8, WHITE);

    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setTextWrap(false);

    display.setCursor(10,0);

    // Not initialized
    if (dataModel.getLoginTimeSec() == 0
          && dataModel.getLogoutTimeSec() == 0) {
        display.print("--:--");
        return;
    }

    if (!dataModel.atWork()) {
        display.print("--:--");
    } else {
        display.print(Time.format(dataModel.getLoginTimeSec(), "%H:%M"));
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
    if (dataModel.getLoginTimeSec() == 0
          && dataModel.getLogoutTimeSec() == 0) {
        display.print("--:--");
        return;
    }

    if (!dataModel.atWork()) {
        display.print("--:--");
    } else {
        int timeDiff = dataModel.getAtWorkTimeSec();
        int minutes = timeDiff / 60;

        // Hours
        display.print(String::format("%02d", minutes / 60));

        if (Time.now() % 2 == 0) {
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

    if (dataModel.getLogoutTimeSec() == 0) {
        display.print("--:--");
    } else {
        display.print(Time.format(dataModel.getLogoutTimeSec(), "%H:%M"));
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

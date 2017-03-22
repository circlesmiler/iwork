// This #include statement was automatically added by the Particle IDE.
#include <clickButton.h>

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include "DataModel.h"
#include "WeatherModel.h"
#include "Icons.h"
#include "PageOne.h"
#include "PageTwo.h"

//PIN PHOTON => PIN OLED
#define OLED_MOSI   D0 //=>D1
#define OLED_CLK    D1 //=>D0
#define OLED_DC     D2 //=>DC
#define OLED_CS     D3 //=>CS
#define OLED_RESET  D4 //=>RST
Adafruit_SSD1306* display = new Adafruit_SSD1306(OLED_MOSI, OLED_CLK, OLED_DC, OLED_RESET, OLED_CS);

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

// EEPROM addresses
#define ENTER_ADDR 0
#define EXIT_ADDR 4
#define OWM_CITY_ADDR 8

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
DataModel* dataModel = new DataModel(ENTER_ADDR, EXIT_ADDR);
WeatherModel* weatherModel = new WeatherModel(OWM_CITY_ADDR);

PageOne* pageOne = new PageOne(display, dataModel, weatherModel);
PageTwo* pageTwo = new PageTwo(display, dataModel, weatherModel);

#define WEATHER_UPDATE_INTERVAL 3600 // 3600sec = 1h
int lastWeatherUpdate = 0;

// For debugging
String dataStr;

// IFTTT EVENT HANDLER
void onWorkHandler(const char *event, const char *data) {
    dataStr = String(data).trim().toLowerCase();
    if (dataStr.equals("entered")) {
        dataModel->login();
    } else if (dataStr.equals("exited")) {
        dataModel->logout();
    }
}

void receiveWeather(const char *event, const char *data) {
  weatherModel->update(data);
}

int updateWeatherCityId(String data) {
  weatherModel->setCityId(atoi(data));
  Particle.publish("weather", String(weatherModel->getCityId()), PRIVATE);
  return 0;
}

int mode = 0;

// STARTUP(WiFi.selectAntenna(ANT_AUTO));
STARTUP(WiFi.selectAntenna(ANT_INTERNAL));

// SETUP
void setup()   {
    Serial.begin(9600);

    // Rotate display 180° (because it's build in in the other direction)
    display->setRotation(2);
    // by default, we'll generate the high voltage from the 3.3v line internally! (neat!)
    display->begin(SSD1306_SWITCHCAPVCC);
    // init done

    display->display(); // show splashscreen
    delay(500);
    display->clearDisplay();   // clears the screen and buffer

    // Time.zone(+2);
    Time.zone(+1);

    Particle.subscribe("on_work", onWorkHandler);
    Particle.subscribe("hook-response/weather/", receiveWeather, MY_DEVICES);
    Particle.function("owmCityId", updateWeatherCityId);

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
        Particle.publish("weather", String(weatherModel->getCityId()), PRIVATE);
        lastWeatherUpdate = Time.now();
    }

    display->clearDisplay();   // clears the screen and buffer

    updateLoginLED();
    updateLogoutLED();

    printPageNumber();
    if (page == 0) {
      pageOne->updateDisplay();
    } else if (page == 1) {
      pageTwo->updateDisplay();
    }

    display->display();
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
      dataModel->login();
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
      dataModel->logout();
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
    digitalWrite(LED_LOGIN, !dataModel->atWork());
}

void updateLogoutLED() {
    digitalWrite(LED_LOGOUT, dataModel->atWork());
}

void printPageNumber() {
    display->setTextSize(1);
    display->setTextColor(WHITE);
    display->setTextWrap(false);
    display->setCursor(60,56);

    display->print(page + 1);
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

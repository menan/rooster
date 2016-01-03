#include "MJSON.h"
#include "Adafruit_mfGFX.h"
#include "Adafruit_ILI9341.h"


Adafruit_ILI9341 tft = Adafruit_ILI9341(A2, A1, A0);
MJSON *parser;

#define DISPLAY_BACKLIT D2
#define TIME_DELAY 60 * 1000
#define QUOTE_DELAY 60 * 60 * 1000 * 12
#define WEATHER_DELAY 60 * 60  * 1000 * 2

unsigned int lastTimeSubscribe = 0;
unsigned int lastQuoteSubscribe = 0;
unsigned int lastWeatherSubscribe = 0;

// called once on startup
void setup() {
    Serial.begin(9600);

  	pinMode(DISPLAY_BACKLIT, OUTPUT);
  	digitalWrite(DISPLAY_BACKLIT, HIGH);
  	tft.begin();
  	tft.fillScreen(ILI9341_BLACK);
    tft.setFont(CENTURY_8);
    toggleLoading(true);
    parser = new MJSON();
    // Lets listen for the hook response
    Particle.subscribe("hook-response/get_weather", gotWeatherData, MY_DEVICES);
    Particle.subscribe("hook-response/get_quote", gotQuoteData, MY_DEVICES);
    Particle.subscribe("hook-response/get_time", gotTimeData, MY_DEVICES);

    // Lets give ourselves 10 seconds before we actually start the program.
    // That will just give us a chance to open the serial monitor before the program sends the request
    for(int i=0;i<10;i++) {
        Serial.println("waiting " + String(10-i) + " seconds before we publish");
        delay(1000);
    }
}


// called forever really fast
void loop() {
    Serial.println("Subscribing to events!");
    /*time_subscription();
    quote_subscription();
    weather_subscription();*/
}

void time_subscription(){
  unsigned long now = millis();
  if (lastWeatherSubscribe > 0 && (now - lastTimeSubscribe) < TIME_DELAY) {
    return;
  }

  Serial.println("Subscribing time.");
  Particle.publish("get_time");
  lastTimeSubscribe = now;
}

void quote_subscription(){
  unsigned long now = millis();

  if (lastWeatherSubscribe > 0 && (now - lastQuoteSubscribe) < QUOTE_DELAY) {
    return;
  }

  Serial.println("Subscribing quote.");
  Particle.publish("get_quote");
  lastQuoteSubscribe = now;
}

void weather_subscription(){
  unsigned long now = millis();

  if (lastWeatherSubscribe > 0 && (now - lastWeatherSubscribe) < WEATHER_DELAY) {
    return;
  }

  Serial.println("Subscribing weather.");
  Particle.publish("get_weather");
  lastWeatherSubscribe = now;
}

// This function will get called when weather data comes in
void gotWeatherData(const char *name, const char *data) {
    toggleLoading(false);
    parser->setJSON(String(data));

    int temp = atoi(parser->readValue("temp"));
    String description = parser->readValue("description");
    String location = parser->readValue("name");

    int descriptionWidth = tft.getStringWidth(description);
    int tempWidth = tft.getStringWidth(String(temp)) + tft.getStringWidth(" C");

    int y = 50;
    int x = tft.width()/2 - tempWidth/2 - 10;
    int height = 80;

    tft.fillRect(0, y, tft.width(), height - y, ILI9341_BLACK);

    tft.setCursor(x, y);
  	tft.setTextColor(ILI9341_YELLOW);
    tft.setTextSize(3);
    tft.println(String(temp) + " C");


  	tft.setTextColor(ILI9341_MAGENTA);
    tft.setTextSize(2);
    tft.advanceCursorY(8);
    tft.advanceCursorX(tft.width()/2 - descriptionWidth/2 - 50);
    tft.println(description);
}


// This function will get called when weather data comes in
void gotQuoteData(const char *name, const char *data) {

  toggleLoading(false);

  parser->setJSON(String(data));

  String quote = parser->readValue("body");
  String author = parser->readValue("author");

  int y = tft.height()/2 + 10;
  tft.fillRect(0, y, tft.width(), tft.height() - y, ILI9341_BLACK);

  tft.setCursor(8, y);

	tft.setTextColor(ILI9341_CYAN);
  tft.setTextSize(2);
  tft.println(quote);

	tft.setTextColor(ILI9341_MAGENTA);
  tft.advanceCursorY(8);
  tft.println(" - " + author);

}


// This function will get called when weather data comes in
void gotTimeData(const char *name, const char *data) {

  toggleLoading(false);
  int y = 10;
  int x = tft.width()/2 - tft.getStringWidth(data)/2 - 20;
  int height = 40;
	tft.setCursor(x, y);
  tft.fillRect(x, y, tft.width(), height-y, ILI9341_BLACK);
	tft.setTextColor(ILI9341_WHITE);
  tft.setTextSize(2);
  tft.println(String(data));

}


void toggleLoading(bool show){
  if (show){
    tft.setTextColor(ILI9341_WHITE);
  }
  else{
    tft.setTextColor(ILI9341_BLACK);
  }
  tft.setTextSize(2);
  tft.setCursor(75, tft.height()/2 - 20);
  tft.println("Loading...");
}

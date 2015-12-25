#include "MJSON.h"
#include "Adafruit_mfGFX.h"
#include "Adafruit_ILI9341.h"


Adafruit_ILI9341 tft = Adafruit_ILI9341(A2, A1, A0);
MJSON *parser;

#define DISPLAY_BACKLIT D2

// called once on startup
void setup() {
    // For simplicity, we'll format our weather data as text, and pipe it to serial.
    // but you could just as easily display it in a webpage or pass the data to another system.

    // Learn more about the serial commands at https://docs.particle.io/reference/firmware/photon/#serial
    //  for the Photon, or https://docs.particle.io/reference/firmware/core/#serial for the Core
    // You can also watch what's sent over serial with the particle cli with
    //  particle serial monitor
    Serial.begin(9600);

  	pinMode(DISPLAY_BACKLIT, OUTPUT);
  	digitalWrite(DISPLAY_BACKLIT, HIGH);

  	tft.begin();

  	tft.fillScreen(ILI9341_BLACK);

    toggleLoading(true);

    parser = new MJSON();

    // Lets listen for the hook response
    /*Particle.subscribe("hook-response/get_weather", gotWeatherData, MY_DEVICES);*/
    // Lets listen for the hook response
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

    // Let's request the weather, but no more than once every 60 seconds.
    /*Serial.println("Requesting Weather!");*/

    // publish the event that will trigger our Webhook
    /*Particle.publish("get_weather");*/


    // Let's request the weather, but no more than once every 60 seconds.
    Serial.println("Requesting Quote!");

    // publish the event that will trigger our Webhook
    Particle.publish("get_quote");
    // Let's request the weather, but no more than once every 60 seconds.
    Serial.println("Requesting Time!");

    // publish the event that will trigger our Webhook
    Particle.publish("get_time");

    // and wait at least 60 seconds before doing it again
    delay(60000);
}

// This function will get called when weather data comes in
void gotWeatherData(const char *name, const char *data) {
    // Important note!  -- Right now the response comes in 512 byte chunks.
    //  This code assumes we're getting the response in large chunks, and this
    //  assumption breaks down if a line happens to be split across response chunks.
    //

    /*
    {"coord":{"lon":-75.69,"lat":45.43},"weather":[{"id":803,"main":"Clouds","description":"broken clouds","icon":"04d"}],"base":"cmc stations","main":{"temp":280.7,"pressure":1021,"humidity":61,"temp_min":280.15,"temp_max":281.48},"wind":{"speed":4.6,"deg":250},"clouds":{"all":75},"dt":1451066269,"sys":{"type":1,"id":3694,"message":0.0058,"country":"CA","sunrise":1451047284,"sunset":1451078697},"id":7626289,"name":"ByWard Market","cod":200}
    */

    toggleLoading(false);
    String str = String(data);

    float temp = atof(parser->readValue("temp"));
    String description = parser->readValue("description");
    String location = parser->readValue("name");

    Serial.println("Temp: " + String(temp));
    Serial.println("Description: " + description);
    Serial.println("Location: " + location);
}


// This function will get called when weather data comes in
void gotQuoteData(const char *name, const char *data) {
  // Important note!  -- Right now the response comes in 512 byte chunks.
  //  This code assumes we're getting the response in large chunks, and this
  //  assumption breaks down if a line happens to be split across response chunks.
  //

  /*
  {"quote":{"body":"Dream is not that which you see while sleeping it is something that does not let you sleep.","author":"Dr. Abdul Kalam"},"time":"05:24"}
  */

  toggleLoading(false);

  parser->setJSON(String(data));

  String quote = parser->readValue("body");
  String author = parser->readValue("author");

  int y = tft.height()/2 + 10;
  tft.fillRect(0, y, tft.width(), tft.height() - y, ILI9341_BLACK);

  tft.setCursor(0, y);

	tft.setTextColor(ILI9341_CYAN);
  tft.setTextSize(2);
  tft.println(quote);

	tft.setTextColor(ILI9341_MAGENTA);
  tft.setTextSize(1);
  tft.advanceCursorY(5);
  tft.println(author);

}


// This function will get called when weather data comes in
void gotTimeData(const char *name, const char *data) {
  // Important note!  -- Right now the response comes in 512 byte chunks.
  //  This code assumes we're getting the response in large chunks, and this
  //  assumption breaks down if a line happens to be split across response chunks.
  //

  toggleLoading(false);
  int y = 10;
  int x = 50;
  int height = 40;
	tft.setCursor(x, y);
  tft.fillRect(x, y, tft.width()-x, height-y, ILI9341_BLACK);
	tft.setTextColor(ILI9341_WHITE);
  tft.setTextSize(3);
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
  tft.setCursor(60, tft.height()/2 - 20);
  tft.println("Loading...");
}

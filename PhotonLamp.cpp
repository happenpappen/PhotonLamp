#include "myLampConfig.h"
#include "myNoise.h"
#include "myNoisePlusPalette.h"
#include "myUtils.h"
#include "myRainbowCycle.h"
#include "mySingleColor.h"
#include "MQTT.h"
#include "MQTT_credentials.h"

CRGB leds[NUM_LEDS];

uint8_t brightness = 20;
CRGB fg_color = CRGB(200, 200, 200);
CRGB bg_color = CRGB(0, 0, 0);

bool displayEnabled = true;
uint16_t maxDistance = 0;
uint16_t lastDistance = 0;

void mqtt_callback(char *, byte *, unsigned int);
void saveSettings();
void loadSettings();

// TImer functions:
//
void publishState();
void setBrightnessByDistance();

MQTT client(MQTT_HOST, 1883, mqtt_callback);
Timer PublisherTimer(5000, publishState);
Timer DistanceSonarTimer(500, setBrightnessByDistance);

// functions to call via cloud:
int incBrightness(String command);
int decBrightness(String command);
int getBrightness(String command);
int setBrightness(String value);
int enableDisplay(String command);
int disableDisplay(String command);
int setDisplayMode(String command);
int getDisplayMode(String command);
int setFgColor(String command);
int setBgColor(String command);

#define ONE_DAY_MILLIS (24 * 60 * 60 * 1000)
unsigned long lastSync = millis();

// Display modes:
// 1: Noise
// 2: RainbowCycle
// 3: NoisePlusPalette
// 4: SingleColor

int dispMode = 1;

void mqtt_callback(char *topic, byte *payload, unsigned int length)
{
    // handle message arrived - we are only subscribing to one topic so assume all are led related

    String myTopic = String(topic);

    bool stateChanged = false;
    char *myPayload = NULL;

    myPayload = (char *) malloc(length + 1);

    memcpy(myPayload, payload, length);
    myPayload[length] = 0;


    if (!client.isConnected()) {
        client.connect("zigarre", MQTT_USER, MQTT_PASSWORD);
    }

    client.publish("/zigarre/state/LastPayload", "Last Payload: " + String(myPayload));

    if (myTopic == "/zigarre/set/DisplayMode") {
        setDisplayMode(String(myPayload));
        stateChanged = true;
    }

    if (myTopic == "/zigarre/set/Brightness") {
        setBrightness(String(myPayload));
        stateChanged = true;
    }

    if (myTopic == "/zigarre/set/ForgroundColor") {
        setFgColor(String(myPayload));
        stateChanged = true;
    }

    if (myTopic == "/zigarre/set/BackgroundColor") {
        setBgColor(String(myPayload));
        stateChanged = true;
    }

    if (stateChanged) {
        publishState();
    }

    free(myPayload);
    myPayload = NULL;
}

int incBrightness(String command)
{
    brightness++;
    FastLED.setBrightness(brightness);

    saveSettings();
    return brightness;
}

int decBrightness(String command)
{
    brightness--;
    brightness = (brightness < 0) ? 0 : brightness;
    FastLED.setBrightness(brightness);

    saveSettings();
    return brightness;
}

int getBrightness(String command)
{
    return brightness;
}

void setBrightnessByDistance() 
{
    uint16_t currentDistance = getDistance();
    
    if ((currentDistance >=5) && (currentDistance <=29))
    {
        setBrightness(String((currentDistance - 4) * 10));
    }
    
    lastDistance = currentDistance;
}

int setBrightness(String value)
{
    brightness = atoi(value);
    FastLED.setBrightness(brightness);

    saveSettings();
    return brightness;
}

int enableDisplay(String command)
{
    displayEnabled = true;

    saveSettings();
    return 1;
}

int disableDisplay(String command)
{
    displayEnabled = false;

    saveSettings();
    return 1;
}

String getFgColor()
{

    return String::format("#%0#2X%0#2X%0#2X", fg_color.red, fg_color.green, fg_color.blue);

}

int setFgColor(String command)
{
    char *rgbstr = (char *) malloc(command.length() + 1);

    rgbstr = strcpy(rgbstr, (const char *) command);

    if (command.length() == 11) {
        int r = atoi(strsep(&rgbstr, ","));
        int g = atoi(strsep(&rgbstr, ","));
        int b = atoi(strsep(&rgbstr, ","));

        fg_color = CRGB(g, r, b);
    }

    free(rgbstr);

    saveSettings();
    return 1;
}

String getBgColor()
{

    return String::format("#%0#2X%0#2X%0#2X", bg_color.red, bg_color.green, bg_color.blue);
}

int setBgColor(String command)
{
    char *rgbstr = (char *) malloc(command.length() + 1);

    rgbstr = strcpy(rgbstr, (const char *) command);

    if (command.length() == 11) {
        int r = atoi(strsep(&rgbstr, ","));
        int g = atoi(strsep(&rgbstr, ","));
        int b = atoi(strsep(&rgbstr, ","));

        bg_color = CRGB(r, g, b);
    }

    free(rgbstr);

    saveSettings();
    return 1;
}

int getDisplayMode(String command)
{
    return dispMode;
}

int setDisplayMode(String command)
{
    dispMode = atoi(command);

    switch (dispMode) {
    case 1:
        setupNoise();
        break;
    case 2:
        setupRainbowCycle();
        break;
    case 3:
        setupNoisePlusPalette();
        break;
    case 4:
        setupSingleColor();
        break;
    default:
        break;
    }

    saveSettings();
    return dispMode;
}

void loadSettings()
{
    int address = 1;

    brightness = EEPROM.read(address++);
    dispMode = EEPROM.read(address++);
    fg_color.r = EEPROM.read(address++);
    fg_color.g = EEPROM.read(address++);
    fg_color.b = EEPROM.read(address++);
    bg_color.r = EEPROM.read(address++);
    bg_color.g = EEPROM.read(address++);
    bg_color.b = EEPROM.read(address++);
    displayEnabled = EEPROM.read(address++);

    maxDistance = getDistance();

    for (int i = 0; i++; i < NUM_LEDS) {
        leds[i] = EEPROM.read(address++);
    }
}

void publishState()
{

    if (!client.isConnected()) {
        client.connect("zigarre", MQTT_USER, MQTT_PASSWORD);
    }

    if (client.isConnected()) {
        client.publish("/zigarre/state/DisplayMode", String(getDisplayMode("")));
        client.publish("/zigarre/state/Brightness", String(getBrightness("")));
        client.publish("/zigarre/state/ForgroundColor", getFgColor());
        client.publish("/zigarre/state/BackgroundColor", getBgColor());
        client.publish("/zigarre/state/MaxDistance", String(maxDistance));
        client.publish("/zigarre/state/LastDistance", String(lastDistance));
        client.publish("/zigarre/state/CurrentDistance", String(getDistance()));
    }
}

void saveSettings()
{
    int address = 1;

    EEPROM.write(address++, brightness);
    EEPROM.write(address++, dispMode);
    EEPROM.write(address++, fg_color.r);
    EEPROM.write(address++, fg_color.g);
    EEPROM.write(address++, fg_color.b);
    EEPROM.write(address++, bg_color.r);
    EEPROM.write(address++, bg_color.g);
    EEPROM.write(address++, bg_color.b);
    EEPROM.write(address++, displayEnabled);

    for (int i = 0; i++; i < NUM_LEDS) {
        EEPROM.write(address++, leds[i]);
    }
}

void setup()
{

    loadSettings();

    dispMode = 1;

    switch (dispMode) {
    case 1:
        setupNoise();
        break;
    case 2:
        setupRainbowCycle();
        break;
    case 3:
        setupNoisePlusPalette();
        break;
    case 4:
        setupSingleColor();
        break;
    default:
        break;
    }

    Time.zone(+1);
    FastLED.addLeds<WS2812B, DATA_PIN>(leds, NUM_LEDS);
    FastLED.setBrightness(brightness);

    client.connect("zigarre", MQTT_USER, MQTT_PASSWORD); // uid:pwd based authentication

    if (client.isConnected()) {
        PublisherTimer.start();
        DistanceSonarTimer.start();
        client.subscribe("/zigarre/set/+");
    }
}

void loop()
{

    // once a day sync time with cloud:
    if (millis() - lastSync > ONE_DAY_MILLIS) {
        // Request time synchronization from the Particle Cloud
        Particle.syncTime();
        lastSync = millis();
    }

    if (displayEnabled) {
        switch (dispMode) {
        case 1:
            loopNoise();
            break;
        case 2:
            loopRainbowCycle();
            break;
        case 3:
            loopNoisePlusPalette();
            break;
        case 4:
            loopSingleColor();
            break;
        default:
            break;
        }
    } else {
        for (int i = 0; i < kMatrixWidth; i++) {
            for (int j = 0; j < kMatrixHeight; j++) {
                leds[XY(i, j)] = 0;
            }
        }
    }

    LEDS.show();
    delay(10);

    if (client.isConnected()) {
        client.loop();
    }
}

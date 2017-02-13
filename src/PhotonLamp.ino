#include "myLampConfig.h"
#include "myFire.h"
#include "myNoise.h"
#include "myNoisePlusPalette.h"
#include "myUtils.h"
#include "myRainbowCycle.h"
#include "myRainSnow.h"
#include "myCylon.h"
#include "mySingleColor.h"
#include "myHorizontalSplit.h"
#include "myHorizontalDoubleSplit.h"
#include "myVerticalSplit.h"
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

// Timer functions:
//
void publishState();
void setBrightnessByDistance();

MQTT client(MQTT_HOST, 1883, mqtt_callback);
Timer PublisherTimer(5000, publishState);
Timer DistanceSonarTimer(500, setBrightnessByDistance);

ApplicationWatchdog wd(60000, System.reset);

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
// 5: Cylon
// 6: Rain/Snow
// 7: Fire
// 8: HorizontalSplit
// 9: HorizontalDoubleSplit
// 100: VerticalSplit

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

    String myID = System.deviceID();

    if (!client.isConnected()) {
        client.connect(myID, MQTT_USER, MQTT_PASSWORD);
    }

    client.publish("/"+myID+"/state/LastPayload", "Last Payload: " + String(myPayload));

    if (myTopic == "/"+myID+"/set/DisplayMode") {
        setDisplayMode(String(myPayload));
        stateChanged = true;
    }

    if (myTopic == "/"+myID+"/set/Brightness") {
        setBrightness(String(myPayload));
        stateChanged = true;
    }

    if (myTopic == "/"+myID+"/set/ForgroundColor") {
        setFgColor(String(myPayload));
        stateChanged = true;
    }

    if (myTopic == "/"+myID+"/set/BackgroundColor") {
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
    return String::format("%0.3d,%0.3d,%0.3d", fg_color.red, fg_color.green, fg_color.blue);
}

int setFgColor(String command)
{
    char *rgbstr = (char *) malloc(command.length() + 1);

    rgbstr = strcpy(rgbstr, (const char *) command);

    if (command.length() == 11) {
        int r = atoi(strsep(&rgbstr, ","));
        int g = atoi(strsep(&rgbstr, ","));
        int b = atoi(strsep(&rgbstr, ","));

        fg_color = CRGB(r, g, b);
    }

    free(rgbstr);

    saveSettings();
    return 1;
}

String getBgColor()
{
    return String::format("%0.3d,%0.3d,%0.3d", bg_color.red, bg_color.green, bg_color.blue);
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
    case 5:
        setupCylon();
        break;
    case 6:
        setupRainSnow();
        break;
    case 7:
        setupFire();
        break;
    case 8:
        setupHorizontalSplit();
        break;
    case 9:
        setupHorizontalDoubleSplit();
        break;
    case 10:
        setupVerticalSplit();
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

    for (int i = 0; i < NUM_LEDS; i++) {
        leds[i] = EEPROM.read(address++);
    }
}

void publishState()
{

    String myID = System.deviceID();

    if (!client.isConnected()) {
        client.connect(myID, MQTT_USER, MQTT_PASSWORD);
    }

    if (client.isConnected()) {
        client.publish("/"+myID+"/state/DisplayMode", String(getDisplayMode("")));
        client.publish("/"+myID+"/state/Brightness", String(getBrightness("")));
        client.publish("/"+myID+"/state/ForgroundColor", getFgColor());
        client.publish("/"+myID+"/state/BackgroundColor", getBgColor());
        client.publish("/"+myID+"/state/MaxDistance", String(maxDistance));
        client.publish("/"+myID+"/state/LastDistance", String(lastDistance));
        client.publish("/"+myID+"/state/CurrentDistance", String(getDistance()));
        client.publish("/"+myID+"/state/FirmwareVersion", System.version());
    }

    Particle.publish("Entfernung", String::format("Entfernung: %6d cm", lastDistance));
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

    for (int i = 0; i < NUM_LEDS; i++) {
        EEPROM.write(address++, leds[i]);
    }
}

void setup()
{
#ifdef DEBUG
    Serial.begin(9600);
#endif
    loadSettings();

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
    case 5:
        setupCylon();
        break;
    case 6:
        setupRainSnow();
        break;
    case 7:
        setupFire();
        break;
    case 8:
        setupHorizontalSplit();
        break;
    case 9:
        setupHorizontalDoubleSplit();
        break;
    case 10:
        setupVerticalSplit();
        break;
    default:
        break;
    }

    randomSeed(analogRead(A0) + micros());
    Time.zone(+1);
    FastLED.addLeds<WS2812B, DATA_PIN, COLOR_ORDER>(leds, NUM_LEDS);
    FastLED.setBrightness(brightness);

    client.connect(System.deviceID(), MQTT_USER, MQTT_PASSWORD); // uid:pwd based authentication

    if (client.isConnected()) {
        PublisherTimer.start();
        //DistanceSonarTimer.start();
        client.subscribe("/"+System.deviceID()+"/set/+");
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
        case 5:
            loopCylon();
            break;
        case 6:
            loopRainSnow();
            break;
        case 7:
            loopFire();
            break;
        case 8:
            loopHorizontalSplit();
            break;
        case 9:
            loopHorizontalDoubleSplit();
            break;
        case 10:
            loopVerticalSplit();
            break;
        default:
            break;
        }
    } else {
        for (int i = 0; i < kMatrixHeight; i++) {
            for (int j = 0; j < kMatrixWidth; j++) {
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

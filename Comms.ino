#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <RF24.h>
#include <SPI.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// Buttons
#define BUTTON_PIN 14
#define BUZZER_PIN 27

// which one it is
#define DEVICE_ID 1

// what's happenin at ze moment
bool isTransmitting = false;

// actual function radio stuff
RF24 radio(4, 5);
const byte address[6] = "00001";

// Who's who heheheh
String getName(int id) {
  switch(id) {
    case 1: return "Addy!";
    case 2: return "Taira!";
    case 3: return "Vienna!";
    default: return "Unknown?";
  }
}

struct Packet {
  int senderID;
  bool talking;
};

Packet data;

// beginning
void setup() {
  Serial.begin(115200);

  pinMode(BUTTON_PIN, INPUT_PULLUP);
  pinMode(BUZZER_PIN, OUTPUT);

  radio.begin();
  radio.openWritingPipe(address);
  radio.openReadingPipe(0, address);
  radio.setPALevel(RF24_PA_LOW);
  radio.startListening();

  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("OLED failed");
    while(true); // what happens when failsssss which def wont happen
  }

  // zee text
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  
  showStartup();
}
  // runs forever but when holding it won't send same command again
void loop() {
  bool buttonPressed = digitalRead(BUTTON_PIN) == LOW;

  // starting and stopping transmission
  if (buttonPressed && !isTransmitting) {
    delay(50);
    isTransmitting = true;

    playBeep();

    radio.stopListening();

    data.senderID = DEVICE_ID;
    data.talking = true;

  radio.write(&data, sizeof(data));

  radio.startListening();

    showTransmit();
  }

  if (!buttonPressed && isTransmitting) {
    isTransmitting = false;
    showIdle();
  }

  if (radio.available()) {
    radio.read(&data, sizeof(data));

    if (data.senderID != DEVICE_ID) {
      showIncoming(data.senderID);
    }
  }
}

// Startin'
void showStartup() {
  display.clearDisplay();
  display.setCursor(10, 20);
  display.setTextSize(2);
  display.println("TITAN");
  display.setTextSize(1);
  display.setCursor(25, 35);
  display.println("COMM");
  display.display();
  delay(1500);
  showIdle();
}

// Doin' Nothin'
void showIdle() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(0, 10);
  display.println("Channel: 1");
  display.display();
}

// Talkin'
void showTransmit() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(0, 10);
  display.println("You");
  display.setCursor(0, 30);
  display.println("Transmitting...");
  display.display();
}

void showIncoming(int sender) {
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(0, 10);
  display.println(getName(sender));
  display.setCursor(0, 30);
  display.println("Talking...");
  display.display();
}

// BEEEEEEEEEEEEEEEEEEEEEEEEP
void playBeep() {
  tone(BUZZER_PIN, 1000, 150);
}
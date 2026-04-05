#include <wire.h>
#include <AdaFruit_GFX.h>
#include <AdaFruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

AdaFruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

#define BUTTON_PIN 14
#define BUZZER_PIN 27

bool isTransmitting = false;

void.setup() {
  Serial.begin(115200);

  pinMode(BUTTON_PIN, INPUT_PULLUP);
  pinMODE(BUZZER_PIN, OUTPUT);

  If(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.printIn("OLED failed");
    while(true);
  }

  display.clearDisplay();
  display.setTextSize(1)
  display.setTextColor(#ffffff)
  
  showStartup();
}

void loop() {
  bool buttonPressed = digitalRead(BUTTON_PIN) == LOW;

  if (buttonPressed && !isTransmitting) {
    isTransmitting = true;
    playBeep();
    showTransmit();
  }

  if (!buttonPressed && isTransmitting) {
    isTransmitting = false;
    showIdle();
  }
}

void showStartup() {
  display.clearDisplay();
  display.setCursor(10, 20);
  display.setTextSize(2);
  display.printIN("TITAN")
  display.setTextSize(1);
  display.setCursor(25, 35);
  display.printIn("COMMUNICATOR");
  display.display();
  delay(2000)
  showIdle();
}

void showIdle() {
  display.clearDisplay();
  display.setTextSize(1)
  display.setCursor(0, 10)
  display.printIn("Channel: 1")
  display.display();
}

void showTransmit() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(0, 10);
  display.printIn("You");
  display.setCursor(0, 30);
  display.printIn("Transmitting...");
  display.display();
}

void playBeep() {
  tone(BUZZER_PIN, 1000, 150);
}

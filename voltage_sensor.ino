// #include <Arduino.h>

#include <Wire.h>
#include <Adafruit_ADS1X15.h>


#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>


#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels
#define OLED_RESET -1
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Scan for i2c devices using: https://playground.arduino.cc/Main/I2cScanner/


Adafruit_ADS1115 ads;

// float   multiplier = 3.0F;    /* ADS1015 @ +/- 6.144V gain (12-bit results) */
// float multiplier = 0.1875F; /* ADS1115  @ +/- 6.144V gain (16-bit results) */
float multiplier = 0.0625F; /* ADS1115  @ +/- 2.048V gain (16-bit results) */

void setup(void) {
    Serial.begin(9600);
    delay(100);
    Serial.println("Starting...");
    // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
    if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
        Serial.println(F("SSD1306 allocation failed"));
        for(;;); // Don't proceed, loop forever
    }

    // Show initial display buffer contents on the screen --
    // the library initializes this with an Adafruit splash screen.
    // display.display();
    // delay(2000); // Pause for 2 seconds
    // display.clearDisplay();

    // display.setTextSize(1);             // Normal 1:1 pixel scale
    // display.setTextColor(SSD1306_WHITE);        // Draw white text
    // display.setCursor(0,0);             // Start at top-left corner
    // display.clearDisplay();
    // display.println("0 +");
    // display.println("1 G");
    // display.println("3. footext");
    // display.println("4. footext");
    // display.println("5. footext");
    // display.setCursor(64,0);             // Start at top-left corner
    // display.setTextSize(2);             // Normal 1:1 pixel scale
    // display.println("#2");
    // display.setCursor(96,0);             // Start at top-left corner
    // display.setTextSize(3);             // Normal 1:1 pixel scale
    // display.println("#3");
    // display.display();


    // delay(2000);
    // display.clearDisplay();
    //     display.printf("foo %f", 1.2345f );

    // display.display();
  // The ADC input range (or gain) can be changed via the following
  // functions, but be careful never to exceed VDD +0.3V max, or to
  // exceed the upper and lower limits if you adjust the input range!
  // Setting these values incorrectly may destroy your ADC!
  //                                                                ADS1015  ADS1115
  //                                                                -------  -------
  // ads.setGain(GAIN_TWOTHIRDS);  // 2/3x gain +/- 6.144V  1 bit = 3mV      0.1875mV (default)
  // ads.setGain(GAIN_ONE);        // 1x gain   +/- 4.096V  1 bit = 2mV      0.125mV
  // ads.setGain(GAIN_TWO);        // 2x gain   +/- 2.048V  1 bit = 1mV      0.0625mV
  // ads.setGain(GAIN_FOUR);       // 4x gain   +/- 1.024V  1 bit = 0.5mV    0.03125mV
  // ads.setGain(GAIN_EIGHT);      // 8x gain   +/- 0.512V  1 bit = 0.25mV   0.015625mV
  // ads.setGain(GAIN_SIXTEEN);    // 16x gain  +/- 0.256V  1 bit = 0.125mV  0.0078125mV

    // ads.setDataRate(RATE_ADS1115_860SPS);
    ads.setDataRate(RATE_ADS1115_64SPS);
    ads.setGain(adsGain_t::GAIN_TWO);

    adsGain_t gain = ads.getGain();
    switch (gain) {
        case ADS1X15_REG_CONFIG_PGA_6_144V: Serial.printf("ADC Range is +/- to %fV ; 1 bit = %f mV\n", 6.144, multiplier); break;
        case ADS1X15_REG_CONFIG_PGA_4_096V: Serial.printf("ADC Range is +/- to %fV ; 1 bit = %f mV\n", 4.096, multiplier); break;
        case ADS1X15_REG_CONFIG_PGA_2_048V: Serial.printf("ADC Range is +/- to %fV ; 1 bit = %f mV\n", 2.048, multiplier); break;
        case ADS1X15_REG_CONFIG_PGA_1_024V: Serial.printf("ADC Range is +/- to %fV ; 1 bit = %f mV\n", 1.024, multiplier); break;
        case ADS1X15_REG_CONFIG_PGA_0_512V: Serial.printf("ADC Range is +/- to %fV ; 1 bit = %f mV\n", 0.512, multiplier); break;
        case ADS1X15_REG_CONFIG_PGA_0_256V: Serial.printf("ADC Range is +/- to %fV ; 1 bit = %f mV\n", 0.256, multiplier); break;
        default:  break;
    }
    Serial.printf("Reading data at %i Samples/Second\n", ads.getDataRate() );
    Serial.println("Getting differential reading from AIN0 (P) and AIN1 (N)");

    if (!ads.begin()) {
        while (1){
            Serial.println("Failed to initialize ADS.");
            delay(3000);
        }
    }

    display.setTextColor(SSD1306_WHITE);        // Draw white text
}

void loop(void) {
    int16_t result01;
    int16_t result23;

    display.clearDisplay();
    display.setTextSize(1);             // Normal 1:1 pixel scale
    display.setCursor(0,0);             // Start at top-left corner
    display.println("+");
    display.println("G");
    display.println("+");
    display.println("G");

    result01 = ads.readADC_Differential_0_1();
    result23 = ads.readADC_Differential_2_3();
    Serial.print("Differential 0_1: ");Serial.print(result01); Serial.print("("); Serial.print(result01 * multiplier); Serial.println("mV)");
    Serial.print("Differential 2_3: ");Serial.print(result23); Serial.print("("); Serial.print(result23 * multiplier); Serial.println("mV)");

    display.setTextSize(2);
    display.setCursor(8, 0);
    display.printf("%.6fmV", result01 * multiplier );
    display.setCursor(8, 16);
    display.printf("%.6fmV", result23 * multiplier );
    display.display();
        
    delay(25);
}
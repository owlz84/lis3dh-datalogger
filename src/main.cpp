// Basic demo for accelerometer readings from Adafruit LIS3DH

#include <SPI.h>
#include <Adafruit_LIS3DH.h>
#include <Adafruit_Sensor.h>
#include "RTClib.h"
#include "utils.h"
#include "logging.h"
#include "config.h"

// Chip Select for accelerometer
#define LIS3DH_CS 6

// hardware SPI
Adafruit_LIS3DH lis = Adafruit_LIS3DH(LIS3DH_CS, &SPI);

unsigned long previous_log_time = 0;
int sample_index = 0;
unsigned short arr_t[kSampleSize] = {}; // millisecond offset for measurements
short arr_X[kSampleSize] = {};
short arr_Y[kSampleSize] = {};
short arr_Z[kSampleSize] = {};
bool printed_headers = false;

RTC_PCF8523 rtc;
RTC_Millis SoftRTC;

void setup(void) {
  // Serial.begin(115200);
  // while (!Serial) delay(10);

  // initialise comms
  pinMode(SS, OUTPUT);
  SPI.begin();

  // get logging going
  SetupLogging();

  // initialise RTC
  if (! rtc.begin()) {
    // Serial.println("Couldn't find RTC");
    // Serial.flush();
    abort();
  }
  if (! rtc.initialized() || rtc.lostPower()) {
    // Serial.println("RTC time lost. Setting the time (this is bad).");
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }
  rtc.start();
  SoftRTC.begin(rtc.now());

  // initialize the accelerometer
  // Serial.print("LIS3DH test...");
  if (! lis.begin()) {
    // Serial.println("Couldn't start");
    while (1) yield();
  }
  // Serial.println("found!");

  lis.setRange(LIS3DH_RANGE_4_G);   // 2, 4, 8 or 16 G!
  // Serial.print("Range = "); // Serial.print(2 << lis.getRange());
  // Serial.println("G");

  lis.setDataRate(LIS3DH_DATARATE_LOWPOWER_5KHZ);
  // Serial.print("Data rate set to: ");
  // switch (lis.getDataRate()) {
  //   case LIS3DH_DATARATE_1_HZ: // Serial.println("1 Hz"); break;
  //   case LIS3DH_DATARATE_10_HZ: // Serial.println("10 Hz"); break;
  //   case LIS3DH_DATARATE_25_HZ: // Serial.println("25 Hz"); break;
  //   case LIS3DH_DATARATE_50_HZ: // Serial.println("50 Hz"); break;
  //   case LIS3DH_DATARATE_100_HZ: // Serial.println("100 Hz"); break;
  //   case LIS3DH_DATARATE_200_HZ: // Serial.println("200 Hz"); break;
  //   case LIS3DH_DATARATE_400_HZ: // Serial.println("400 Hz"); break;
  //   case LIS3DH_DATARATE_POWERDOWN: // Serial.println("Powered Down"); break;
  //   case LIS3DH_DATARATE_LOWPOWER_5KHZ: // Serial.println("5 Khz Low Power"); break;
  //   case LIS3DH_DATARATE_LOWPOWER_1K6HZ: // Serial.println("16 Khz Low Power"); break;
  // }
}

void ReadSensor(unsigned short i, unsigned long starting_millis) {
    lis.read();
    arr_t[i] = (short)(millis() - starting_millis);
    arr_X[i] = lis.x; arr_Y[i] = lis.y; arr_Z[i] = lis.z;
}

void PrintSummary(unsigned short sample_index) {

  if (!printed_headers) {
    // Serial.println(F("ix\tmin(t)\tmax(t)\tmin(X)\tmax(X)\tmean(X)\tmin(Y)\tmax(Y)\tmean(Y)\tmin(Z)\tmax(Z)\tmean(Z)\t"));
    printed_headers = true;
  }

  // Serial.print(sample_index); // Serial.print("\t");
  // Serial.print(array_min(arr_t)); // Serial.print("\t"); // Serial.print(array_max(arr_t)); // Serial.print("\t");
  // Serial.print(array_min(arr_X)); // Serial.print("\t"); // Serial.print(array_max(arr_X)); // Serial.print("\t"); // Serial.print(array_mean(arr_X)); // Serial.print("\t");
  // Serial.print(array_min(arr_Y)); // Serial.print("\t"); // Serial.print(array_max(arr_Y)); // Serial.print("\t"); // Serial.print(array_mean(arr_Y)); // Serial.print("\t");
  // Serial.print(array_min(arr_Z)); // Serial.print("\t"); // Serial.print(array_max(arr_Z)); // Serial.print("\t"); // Serial.println(array_mean(arr_Z));
}

void CollectAndLog(unsigned short sample_index) {

  unsigned short i = 0;
  unsigned long previous_micros = 0;
  unsigned long starting_millis = 0;
  
  starting_millis = millis();

  while (i < kSampleSize) {
    unsigned long current_micros = micros();
    // Read the sensor when needed.
    if (current_micros - previous_micros >= kSampleIntervalMicros) {
        previous_micros = current_micros;
        ReadSensor(i, starting_millis);
        i++;
    }
  }

  array_center(arr_X);
  array_center(arr_Y);
  array_center(arr_Z);

  LogToSD(arr_t, arr_X, arr_Y, arr_Z, sample_index, SoftRTC.now().unixtime());

}

void loop() {
  unsigned long current_log_time = SoftRTC.now().unixtime();

  if (current_log_time - previous_log_time >= kLoggingIntervalSeconds) {
    CollectAndLog(sample_index);
    sample_index++;
    PrintSummary(sample_index);
    previous_log_time = SoftRTC.now().unixtime();
  }

}
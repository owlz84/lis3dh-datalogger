#include <SD.h>
#include <config.h>

// SD card pin
#define ADALOGGER_SD 10

File logfile;
char filename[15];

void InitialiseCardReader(){
  Serial.print("Initializing SD card...");
  // see if the card is present and can be initialized:
  if (!SD.begin(ADALOGGER_SD)) {
    Serial.println("Card failed, or not present");
    // don't do anything more:
    return;
  }
  Serial.println("card initialized.");
}

void GetNextFilename(){
  // set up logging
  strcpy(filename, "/ACCEL000.TXT");
  for (uint8_t i = 0; i < 100; i++) {
    filename[7] = '0' + i/10;
    filename[8] = '0' + i%10;
    // create if does not exist, do not open existing, write, sync after write
    if (! SD.exists(filename)) {
      break;
    }
  }
}

void PrependHeaders() {
  logfile.println("eventIndex,eventTimestamp,sampleIndex,sampleOffset,X,Y,Z");
  logfile.flush();
}

void SetupLogging(){

  InitialiseCardReader();
  GetNextFilename();

  logfile = SD.open(filename, FILE_WRITE);
  if( ! logfile ) {
    Serial.print("Couldnt create "); 
    Serial.println(filename);
  }
  Serial.print("Writing to "); 
  Serial.println(filename);

  PrependHeaders();
}

void LogToSD(short unsigned* arr_t, short* arr_X, short* arr_Y, short* arr_Z, unsigned short sample_index, unsigned long current_log_time) {

  for (int i =0; i < kSampleSize; ++i) {
    logfile.print(sample_index); logfile.print(","); logfile.print(current_log_time); logfile.print(","); 
    logfile.print(i); logfile.print(","); logfile.print(arr_t[i]); logfile.print(",");
    logfile.print(arr_X[i]); logfile.print(","); logfile.print(arr_Y[i]); logfile.print(","); logfile.println(arr_Z[i]);
  }

  logfile.flush();
}
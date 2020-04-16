

#include <SD.h>
#include <SPI.h>
#include <AudioShield.h>

int filenumber = 1;

/******************************************************************************
 * Funktionen
 ******************************************************************************/
void setup() {
  //Einrichten der LEDs
  pinMode(LED_BLUE, OUTPUT);
  pinMode(LED_RED, OUTPUT);

  //beide LEDs ausschalten
  LED_BLUE_OFF;
  LED_RED_OFF;


  //SD-Karte initialisieren
  //SD_CS als parameter übergeben, da hier ChipSelect anders belegt
  if( SD.begin( SD_CS ) == false )
  {
    // Programm beenden, da keine SD-Karte vorhanden
    return;
  }

  //MP3-Decoder initialisieren
  VS1011.begin();
  VS1011.SetVolume(20,20);
  
  // Serielle Schnittstelle öffnen
  Serial.begin( 9600 );
  while( !Serial )
  {
    ; // warten bis Serielle Schnittstelle bereit
  }
}

void loop()
{
  //Puffer für MP3-Decoder anlegen
  //MP3-Decoder erwartet Daten immer in 32 Byte Blöcken
  unsigned char buffer[32];
  String track;
  bool yellow = false;
  bool white = false;
  bool blue = false;
  bool orange = false;
  bool green = false;
  bool red = false;
  bool black = false;
  bool yellow2 = false;
  int val1;
  int val2;
  if (filenumber < 10) track = "track00";
  else track = "track0"; 

  //Datei öffnen und abspielen
  if( File SoundFile = SD.open((track + String(filenumber) + ".mp3"), FILE_READ ) )
  {
    //Blaue LED während dem Abspielen leuchten lassen
    LED_BLUE_ON; 

    //Verstärker einschalten
    VS1011.UnsetMute();


    //Datei bis zum Ende abspielen
    while( SoundFile.available() )
    {
      val1 = analogRead(4);          // read the input pin
      if ((val1 > 100) && (val1 < 200))
      {
        delay(1000);
        yellow = true;
        break;
      }     
      if ((val1 >= 0) && (val1 < 100))
      {
        delay(1000);
        white = true;
        break;
      }
      if ((val1 > 300) && (val1 < 400))
      {
        delay(1000);
        green = true;
        break;
      }     
      if ((val1 > 450) && (val1 < 550))
      {
        delay(1000);
        red = true;
        break;
      }     
      val2 = analogRead(5);          // read the input pin
      if ((val2 > 100) && (val2 < 200))
      {
        delay(1000);
        blue = true;
        break;
      }     
      if ((val2 >= 0) && (val2 < 100))
      {
        delay(1000);
        orange = true;
        break;
      }
      if ((val2 > 300) && (val2 < 400))
      {
        delay(1000);
        black = true;
        break;
      }     
      if ((val2 > 450) && (val2 < 550))
      {
        delay(1000);
        yellow2 = true;
        break;
      }     
      
      //int val2 = analogRead(5);          // read the input pin
      //Serial.println(val2);             // debug value
      
      //Puffer mit Daten aus der Datei füllen
      SoundFile.read( buffer, sizeof(buffer) );
      //Daten aus Puffer an MP3-Decoder senden
      VS1011.Send32( buffer );
    }
    //Internen Datenpuffer vom MP3-Decoder mit Nullen füllen
    //damit sicher alles im Puffer abgespielt wird und Puffer leer ist
    //MP3-Decoder besitzt 2048 Byte großen Datenpuffer
    VS1011.Send2048Zeros();

    //Verstärker deaktivieren
    VS1011.SetMute();
    if (yellow == true)
    {
      filenumber++;
      if (filenumber > 5) filenumber = 1; 
      yellow = false;
    }
    else if (white == true)
    {
      filenumber++;
      if (filenumber < 6) filenumber = 6; 
      if (filenumber > 10) filenumber = 6; 
      white = false;
    }
    else if (blue == true)
    {
      filenumber++;
      if ((filenumber < 11) || (filenumber > 15)) filenumber = 11; 
      blue = false;
    }
    else if (orange == true)
    {
      filenumber++;
      if ((filenumber < 16) || (filenumber > 20)) filenumber = 16; 
      orange = false;
    }
    else if (green == true)
    {
      filenumber++;
      if ((filenumber < 21) || (filenumber > 25)) filenumber = 21; 
      green = false;
    }
    else if (red == true)
    {
      filenumber++;
      if ((filenumber < 26) || (filenumber > 30)) filenumber = 26; 
      red = false;
    }
    else if (black == true)
    {
      filenumber++;
      if ((filenumber < 31) || (filenumber > 35)) filenumber = 31; 
      black = false;
    }
    else if (yellow2 == true)
    {
      filenumber++;
      if ((filenumber < 36) || (filenumber > 40)) filenumber = 36; 
      yellow2 = false;
    }
    else filenumber++;

    //Blaue LED nach dem Abspielen ausschalten
    LED_BLUE_OFF;

    //Datei schliessen
    SoundFile.close();
    // Serial.println(val1);             // debug value
    // Serial.println(val2);             // debug value
    // Serial.println(filenumber);       // debug filenumber
    if (filenumber > 40) filenumber = 1; 
    delay(1000);
  }
  else
  {
    //Datei nicht gefunden
    //Rote LED für 2s leuchten lassen
    LED_RED_ON;
    delay( 2000 );
    LED_RED_OFF;
  }
}

// RGB LED modul WS2811

// připojení potřebné knihovny
#include <Adafruit_NeoPixel.h>
// nastavení propojovacího pinu
#define pinDI 11
// nastavení počtu LED modulů
#define pocetLED 4

const int buttonPin = 2;     // the number of the pushbutton pin
const int sensorPin = 3;     // the number of the pushbutton pin
int buttonState = 0;         // variable for reading the pushbutton status

long last_button_press = 0;
int button_Enabled = 1;

int red = 0;
int green = 0;
int blue = 0;
int color = 0;
int mode = 0;
int krok = 0;
int flash_speed = 10;
int move_phase = 0;
int phase = 0;
int fade_speed = 10;

// inicializace LED modulu z knihovny
Adafruit_NeoPixel rgbWS = Adafruit_NeoPixel(pocetLED, pinDI, NEO_GRB + NEO_KHZ800);

void setup() {
    Serial.begin(9600);
  // zahájení komunikace s LED modulem
  rgbWS.begin();
  // nastavení červené barvy na první LED diodu
  nastavRGB(255, 0  , 0  , 1);
  delay(500);
  // nastavení zelené barvy na první LED diodu
  nastavRGB(0, 0  , 0  , 1);
  nastavRGB(0  , 255, 0  , 2);
  delay(500);
  // nastavení modré barvy na první LED diodu
  nastavRGB(0, 0  , 0  , 2);
  nastavRGB(0  , 0  , 255, 3);
  delay(500);
  // nastavení černé barvy na první LED diodu = vypnutí
  nastavRGB(0, 0  , 0  , 3);
  nastavRGB(255  , 255  , 255  , 4);
  delay(500);
  nastavRGB(0, 0  , 0  , 4);


    // initialize the pushbutton pin as an input:
  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(sensorPin, INPUT);

  attachInterrupt(0, on_button_press, FALLING);
 
  mode = 0;
}
void loop() {
  switch (mode){
      case 0:
          red = 255;
          green = 255;
          blue = 255;
      break;
      case 1:
          red = 255;
          green = 255;
          blue = 0;                             
      break;
      case 2:
          red = 0;
          green = 255;
          blue = 0;                              
      break;
      case 3:
          red = 0;
          green = 255;
          blue = 255;                              
      break;
      case 4:
          red = 0;
          green = 0;
          blue = 255;                             
      break;
      case 5:
          red = 255;
          green = 0;
          blue = 255;                             
      break;
      case 6:
          red = 255;
          green = 0;
          blue = 0;                               
      break;
      case 7:
          flash_speed = 10;    
          flash_step();                         
      break;
      case 8:
          flash_speed = 100; 
          flash_step();                     
      break;
      case 9:
          fade_speed = 1; 
          fade_step();                                
      break;
      case 10:
          fade_speed = 2; 
          fade_step();                               
      break;
      case 11:
          fade_speed = 8; 
          fade_step();                               
      break;       
      

  }
  
  nastavRGB(red, green, blue, 0); 
  
  if(button_Enabled == 0) {
    // pokud je při zákazu stisku naměřen rozdíl času stisku
    // a momentálního času větší jak 1000 ms = 1 s,
    // tak je povolen další stisk
    if(millis()-last_button_press > 200) {
        button_Enabled = 1;
    }
  }
  delay(50); 
}
void fade_step(){

  switch (phase){
      case 0:
          red += fade_speed;
      break;
      case 1:
          green += fade_speed;                             
      break;
      case 2:
          red -= fade_speed;                       
      break;
      case 3:
          blue += fade_speed;                              
      break;
      case 4:
          green -= fade_speed;                             
      break;
      case 5:
          red += fade_speed;                        
      break;
      case 6:
          blue -= fade_speed;                               
      break;
   }

   move_phase = 0;
   if(red > 255){
      red = 255;
      move_phase = 1;
   }

   if(green > 255){
      green = 255;
      move_phase = 1;
   }

   if(blue > 255){
      blue = 255;
      move_phase = 1;
   }

   if(red < 0){
      red = 0;
      move_phase = 1;
   }

   if(green < 0){
      green = 0;
      move_phase = 1;
   }

    if(blue < 0){
      blue = 0;
      move_phase = 1;
   }

    if(move_phase){
        phase +=1;
        if(phase > 6){
          phase = 1;
        }
        move_phase = 0;
   }
  
  
}

void flash_step(){
  if(krok == 0){
    krok = flash_speed;
    color += 1;
    if(color==7)
       color = 0;
  }
  else
  {
    krok -= 1;
  }

  switch (color){
      case 0:
          red = 255;
          green = 0;
          blue = 0;
      break;
      case 1:
          red = 255;
          green = 255;
          blue = 0;                             
      break;
      case 2:
          red = 0;
          green = 255;
          blue = 0;                              
      break;
      case 3:
          red = 0;
          green = 255;
          blue = 255;                              
      break;
      case 4:
          red = 0;
          green = 0;
          blue = 255;                             
      break;
      case 5:
          red = 255;
          green = 0;
          blue = 255;                             
      break;
      case 6:
          red = 255;
          green = 255;
          blue = 255;                               
      break;
   }
  
}


void nastavRGB (byte r, byte g, byte b, int cislo) { //0 je pro vsechny LED
  // vytvoření proměnné pro ukládání barev
  uint32_t barva;
  // načtení barvy do proměnné
  if(digitalRead(sensorPin)){
     barva = rgbWS.Color(r, g, b);
  }
  else{
    barva = rgbWS.Color(0, 0, 0);
  }
  // nastavení barvy pro danou LED diodu,
  // číslo má pořadí od nuly
  if(cislo != 0){
      rgbWS.setPixelColor(cislo-1, barva);
  }
  else{
      rgbWS.setPixelColor(0, barva);
      rgbWS.setPixelColor(1, barva);
      rgbWS.setPixelColor(2, barva);
      rgbWS.setPixelColor(3, barva);
  }
      
  // aktualizace barev na všech modulech
  rgbWS.show();
}

void on_button_press()
{
  //Mode 0: R
  //Mode 1: Y
  //Mode 2: G
  //Mode 3: C
  //Mode 4: B
  //Mode 5: M
  //Mode 6: W
  //Mode 7: Ostre preblikavani pomale
  //Mode 8: Ostre preblikavani rychle
  //Mode 9: Pomale vyhasinaci blikani
  //Mode 10: Rychle vyhasinaci blikani

  if(button_Enabled == 1)
  {
 
   mode = mode + 1;

   //resetuj prok pro blikani
   krok = 0;

   if(mode == 12)
       mode = 0;
  

     Serial.print("mode: ");
     Serial.println(mode);

   last_button_press=millis();
   button_Enabled = 0;


    nastavRGB(128, 128, 128, 0);
    delay(50);
  }
}

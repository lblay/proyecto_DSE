#include <Servo.h>
#include <Adafruit_TCS34725.h>
#include <ColorConverterLib.h>
#include <Wire.h>


// Objetos
Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_50MS, TCS34725_GAIN_1X);
Servo servoColor;
Servo servoRampa;
String color;

// variables
#define sC 39
#define sR 38


void setup()
{
  Serial.begin(9600);
  servoColor.attach(sC);
  servoRampa.attach(sR);

  if (!tcs.begin())
  {
    Serial.println("Error al iniciar TCS34725");
    while (1) delay(1000);
  }
}

void loop()
{
  iniciar();
  color = medirColor();
  if(color.equals("Rojo")){
    servoRampa.write(30);
    delay(500);
    servoColor.write(73);
    delay(500);
  }else if(color.equals("Azul")){
    servoRampa.write(70);
    delay(500);
    servoColor.write(73);    
    delay(500);
  }else if(color.equals("Negro")){
    servoRampa.write(110);
    delay(500);
    servoColor.write(73);    
    delay(500);
  }/*else if(color.equals("Negro")){
    servoRampa.write(150);
    delay(500);
    servoColor.write(73);   
    delay(500);
  } */ 
  delay(1000);
}

void iniciar(){
  servoColor.write(175);
  delay(1000);
}
String medirColor(){
  uint16_t clear, red, green, blue;
  servoColor.write(115);
  delay(1500);
  tcs.setInterrupt(false);
  delay(500); // Cuesta 50ms capturar el color
  tcs.getRawData(&red, &green, &blue, &clear);
  tcs.setInterrupt(true);

  // Hacer rgb medición relativa
  uint32_t sum = clear;
  float r, g, b;
  r = red; r /= sum;
  g = green; g /= sum;
  b = blue; b /= sum;

  // Escalar rgb a bytes
  r *= 256; g *= 256; b *= 256;

  // Convertir a hue, saturation, value
  double hue, saturation, value;
  ColorConverter::RgbToHsv(static_cast<uint8_t>(r), static_cast<uint8_t>(g), static_cast<uint8_t>(b), hue, saturation, value);

  // Mostrar nombre de color
  return printColorName(hue * 360);
}

String printColorName(double hue)
{
  if (hue < 100)
  {
    Serial.println(hue);
    Serial.println("Rojo");
    return "Rojo";
  }
  /*else if (hue < 45)
  {
    Serial.println(hue);
    Serial.println("Naranja");
    return "Naranja";
  }*/
  /*else if (hue < 95)
  {
    Serial.println(hue);
    Serial.println("Amarillo");
    return "Amarillo";
  }*/
  /*else if (hue < 150)
  {
    Serial.println(hue);
    Serial.println("Verde");
    return "Verde";
  }*/
  else if (hue < 180)
  {
    Serial.println(hue);
    Serial.println("Negro");
    return "Negro";
  }
  else if (hue < 270)
  {
    Serial.println(hue);
    Serial.println("Azul");
    return "Azul";
  }
  /*else if (hue < 330)
  {
    Serial.println(hue);
    Serial.println("Magenta");
    return "Magenta";
  }*/
  /*else
  {
    Serial.println(hue);
    Serial.println("NC");
    return "NC";
  }*/
}

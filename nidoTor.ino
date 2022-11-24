#define BLYNK_TEMPLATE_ID "TMPLEjB0WCGZ"
#define BLYNK_DEVICE_NAME "Sistema de Monitoreo de Temperatura y Humedad"
#define BLYNK_AUTH_TOKEN "181duLHebomS-23Q1yaP1CtTv9Ha0DB_"
#define BLYNK_PRINT Serial

#include <OneWire.h>                
#include <DallasTemperature.h>
#include <SD.h>
#include <SPI.h>
#include <BlynkSimpleEsp32.h>

OneWire ourWire(4);


DallasTemperature sensors(&ourWire);

DeviceAddress address1 = {0x28, 0x8C, 0xE, 0x79, 0xA2, 0x0, 0x3, 0x4B};
DeviceAddress address2 = {0x28, 0xF3, 0x55, 0x79, 0xA2, 0x0, 0x3, 0x12};
DeviceAddress address3 = {0x28, 0xF, 0x87, 0x79, 0xA2, 0x0, 0x3, 0xC0};

float EA = 35; 
float humedad; 
float calculo;
float porcentaje;

const char WEBSITE[] = "api.pushingbox.com"; //pushingbox API server
const String devid = "v00DB1FF6420FBAE"; // ID del dispositivo en Pushingbox
const char* MY_SSID = "FABLAB"; // nombre del dispositivo de internet
const char* MY_PWD =  "#$IICA2019$#"; // contraseña del dispositivo de internet

char auth[] = BLYNK_AUTH_TOKEN;
char ssid[] = "FABLAB";
char pass[] = "#$IICA2019$#";

int status = WL_IDLE_STATUS;
// si no se quiere usar DNS
// es mejor usar el IP numerico en vez del nombre del servidor:
// IPAddress server(X,X,X,X);  // IP numerico de Google (no DNS)

void setup() {
  
  Serial.begin(115200);
  Serial.print("(SSID: ");
  sensors.begin();
  pinMode(32, OUTPUT);
  pinMode(33, OUTPUT);
  Blynk.begin(auth, ssid, pass);
}
/*
void printWifiStatus() {

  Serial.print("(SSID: ");
  Serial.print(WiFi.SSID());
  Serial.print(" / ");

  IPAddress ip = WiFi.localIP();
  Serial.print("Dirección IP: ");
  Serial.print(ip);
  Serial.print(" / ");

  long rssi = WiFi.RSSI();
  Serial.print("Fuerza de la señal (RSSI): ");
  Serial.print(rssi);
  Serial.println("dBm)");
}
 */


void loop() {
  // Establecer la conexion a internet:
  checkWIFI();
  Blynk.run();

  digitalWrite(33, HIGH);
  //status = WiFi.begin(MY_SSID, MY_PWD);
  delay(5000);
  Serial.println("\nConectado!!!\n");
  //printWifiStatus();

  Serial.println("\n"); 
  humedad = analogRead(EA);
  Serial.println(analogRead(EA));

  calculo = -0.0423728814*humedad+170.2542373;
  porcentaje = calculo;
  sensors.requestTemperatures();
  float temp1= sensors.getTempC(address1);
  float temp2= sensors.getTempC(address2);
  float temp3= sensors.getTempC(address3);
  Serial.print("Temperatura Sensor 1: ");
  Serial.print(temp1);
  Serial.println(" °C");
  Serial.print("Temperatura Sensor 2: ");
  Serial.print(temp2);
  Serial.println(" °C");
  Serial.print("Temperatura Sensor 3: ");
  Serial.print(temp3);
  Serial.println(" °C");
  Serial.print("Humedad: ");
  if (porcentaje > 100) {
    Serial.println("100%");
  } else if (porcentaje < 0) {
    Serial.println("0%");
  } else if (porcentaje > 0 && porcentaje < 100) {
    Serial.print(porcentaje);
    Serial.println("%");
  }
  Blynk.virtualWrite(V0,temp1);
  Blynk.virtualWrite(V1, temp2);
  Blynk.virtualWrite(V2, temp3);
  Blynk.virtualWrite(V3, porcentaje);
  
//  WiFiClient client;
//    if (client.connect(WEBSITE, 80)){ 
//         client.print("GET /pushingbox?devid=" + devid
//       + "&TemperaturaS1="   + (String) temp1
//       + "&TemperaturaS2="   + (String) temp2
//       + "&TemperaturaS3="   + (String) temp3
//       + "&Humedad="         + (String) porcentaje
//         );
//      client.println(" HTTP/1.1"); 
//      client.print("Host: ");
//      client.println(WEBSITE);
//      client.println("User-Agent: MKR1000/1.0");
//      client.println();
//      Serial.println("Datos Enviados!!!\n");
//      pinMode(5, OUTPUT);
//      digitalWrite(5, HIGH);
//      delay(5000);
//      client.stop();
//      digitalWrite(5, LOW);
//      delay(890000);
//      }
}


void checkWIFI(){
   while (!Blynk.connected()) {
    Serial.println("Lost Blynk server connection");
    Blynk.connect();
  } 
    digitalWrite(32, HIGH);
    Serial.println("Blynk OK");
}
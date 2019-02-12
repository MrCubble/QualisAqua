#include <ESP8266WiFi.h>//Biblioteca do WiFi.
#include <WiFiUdp.h>//Biblioteca do UDP.
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>  

#include "DHT.h" //Biblioteca do sensor Dht11
#define DHTTYPE DHT11
#define DHTPIN 2

DHT dht(DHTPIN, DHTTYPE);///Config Dht11

WiFiUDP udp;//Cria um objeto da classe UDP.
long x;//Variavel para ser enviada.
 
void setup()
{
   Serial.begin(115200);
   pinMode(D4, OUTPUT);//Habilita o LED onboard como saida.
   digitalWrite(D4, 1);//Desliga o LED.
   WiFiManager wifiManager; 
   wifiManager.autoConnect("ConnectSensor");
   WiFi.mode(WIFI_STA);//Define o ESP8266 como Station.
   //connect();
   dht.begin();
}
 
void loop()
{
   //Sub-rotina para conectar-se ao host.
 
   send();//Sub-rotina para enviar os dados ao host.
 
   delay(500);//Aguarda meio segundo.
}
 
void connect()//Sub-rotina para verificar a conexao com o host.
{
   if(WiFi.status() != WL_CONNECTED)//Caso nao esteja conectado ao host, ira se conectar.
   {
      Serial.print(".");
      WiFi.begin("My node", "");//Conecta à rede do host.
      delay(2000);//Espera ate que a conexao seja feita.
   }
}
 
void send()//Sub-rotina para enviar dados ao host.
{
   if (WiFi.status() == WL_CONNECTED)//Só ira enviar dados se estiver conectado.
   {
      int tmp = dht.readTemperature();
      Serial.print("Temperature = ");
      Serial.println(tmp);
      udp.beginPacket("192.168.4.1", 555);//Inicializa o pacote de transmissao ao IP e PORTA.
      udp.println(random(0,1024));//Adiciona-se o valor ao pacote.
      udp.endPacket();//Finaliza o pacote e envia.
 
      digitalWrite(D4, 0);//-
      delay(5);//-
      digitalWrite(D4, 1);//Pisca o led rapidamente apos enviar.
   }
   else{
    WiFiManager wifiManager; 
    wifiManager.autoConnect("ConnectSensor");
   }
 
}

#include <ESP8266WiFi.h>//Biblioteca do WiFi.
#include <WiFiUdp.h>//Biblioteca do UDP.
#include <DNSServer.h>
#include <ESP8266WebServer.h>//Biblioteca para WebServer
//Site de referencia https://lastminuteengineers.com/esp8266-dht11-dht22-web-server-tutorial/
ESP8266WebServer server(80);
WiFiUDP udp;//Cria um objeto da classe UDP.
String req = "";
double Temperature, Humidity;
void setup()
{
   pinMode(D4, OUTPUT);//Habilita o LED onboard como saida.
   digitalWrite(D4, 1);//Desliga o LED.
   Serial.begin(115200);//Habilita a comunicaçao serial para a string recebida ser lida no Serial monitor.
   delay(100);
   //WiFiManager wifiManager; 
   //wifiManager.autoConnect("ConnectBase");
   WiFi.mode(WIFI_AP_STA);//Define o ESP8266 como Acess Point.
   WiFi.softAP("local", "12345678");//Cria um WiFi de nome "NodeMCU" e sem senha.
   delay(2000);
   WiFi.begin("AP 901", "90135841775");
   delay(2000);//Aguarda 2 segundos para completar a criaçao do wifi.
   udp.begin(555);//Inicializa a recepçao de dados UDP na porta 555
   delay(2000);
   Serial.println("");
   Serial.println("WiFi connected..!");
   Serial.print("Got IP: ");  Serial.println(WiFi.localIP());

   server.on("/", handle_OnConnect);//Cria funcao para enviar ao encontrar pag
   server.onNotFound(handle_NotFound);//Cria funçao para caso nao encontre pag

   server.begin();//Liga o servidor
   Serial.println("HTTP server started");
}
 
void loop(){
   listen();//Sub-rotina para verificar a existencia de pacotes UDP.
   server.handleClient();//Sub-rotina para responder aos clientes da página html.
}
 
void listen()//Sub-rotina que verifica se há pacotes UDP's para serem lidos.
{
   if (udp.parsePacket() > 0)//Se houver pacotes para serem lidos
   {
       req = "";//Reseta a string para receber uma nova informaçao
       while (udp.available() > 0)//Enquanto houver dados para serem lidos
       {
           char f = udp.read();//Adiciona o byte lido em uma char
           req += f;//Adiciona o char à string
       }
       //Após todos os dados serem lidos, a String estara pronta.
       Serial.print("Temperatura: ");
       Serial.println(req);//Printa a string recebida no Serial monitor.
       digitalWrite(D4, 0);//-
       delay(5);//
       digitalWrite(D4, 1);//Pisca o LED rapidamente apos receber a string.
    }
}

void handle_OnConnect() {

 //Temperature = dht.readTemperature(); // Gets the values of the temperature
  //Humidity = dht.readHumidity(); // Gets the values of the humidity 
  server.send(200, "text/html", SendHTML(req.toInt(),random(1,1023))); 
}

void handle_NotFound(){
  server.send(404, "text/plain", "Not found");
}

String SendHTML(float Temperaturestat,float Humiditystat){
  String ptr = "<!DOCTYPE html> <html>\n";
  ptr +="<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=no\">\n";
  ptr +="<title>ESP8266 Weather Report</title>\n";
  ptr +="<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}\n";
  ptr +="body{margin-top: 50px;} h1 {color: #444444;margin: 50px auto 30px;}\n";
  ptr +="p {font-size: 24px;color: #444444;margin-bottom: 10px;}\n";
  ptr +="</style>\n";
  ptr +="<script>\n";
  ptr +="setInterval(loadDoc,200);\n";
  ptr +="function loadDoc() {\n";
  ptr +="var xhttp = new XMLHttpRequest();\n";
  ptr +="xhttp.onreadystatechange = function() {\n";
  ptr +="if (this.readyState == 4 && this.status == 200) {\n";
  ptr +="document.getElementById(\"webpage\").innerHTML =this.responseText}\n";
  ptr +="};\n";
  ptr +="xhttp.open(\"GET\", \"/\", true);\n";
  ptr +="xhttp.send();\n";
  ptr +="}\n";
  ptr +="</script>\n";
  ptr +="</head>\n";
  ptr +="<body>\n";
  ptr +="<div id=\"webpage\">\n";
  ptr +="<h1>Projeto QualisAqua ver.1</h1>\n";
  
  ptr +="<p>Temperature: ";
  ptr +=(int)Temperaturestat;
  ptr +="°C</p>";
  
  ptr +="<p>Humidity: ";
  ptr +=(int)Humiditystat;
  ptr +="%</p>";
  
  ptr +="</div>\n";
  ptr +="</body>\n";
  ptr +="</html>\n";
  return ptr;
}

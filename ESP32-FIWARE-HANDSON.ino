/*
------------------ FIAP --------------------
CP04 - Hands-On de Vinherias pelo Cloud IoT
EDGE COMPUTING & COMPUTER SYSTEMS
Participantes:
### - Guilherme Satler Macedo   || RM 563330
### - Laura Sousa Barreto       || RM 561965
### - Matheus Freitas Vieira    || RM 566198
### - Natalia Camargo de Souza  || RM 565769
--------------------------------------------
 
AZURE FIWARE - docker
*/

#include <WiFi.h>
#include <PubSubClient.h>
#include <DHT.h>

// Configurações - variáveis editáveis
const char* default_SSID = "GuiGuiGui"; // Nome da rede Wi-Fi
const char* default_PASSWORD = "iuGiuGiuG"; // Senha da rede Wi-Fi
const char* default_BROKER_MQTT = "###.###.###.##"; // IP do Broker MQTT
const int default_BROKER_PORT = 1883; // Porta do Broker MQTT
const char* default_TOPICO_SUBSCRIBE = "/TEF/SensorVinheria001/cmd"; // Tópico MQTT de escuta
const char* default_TOPICO_PUBLISH_1 = "/TEF/SensorVinheria001/attrs"; // Tópico MQTT de envio de informações para Broker
const char* default_TOPICO_PUBLISH_2 = "/TEF/SensorVinheria001/attrs/l"; // Tópico MQTT de envio de informações para Broker
const char* default_TOPICO_PUBLISH_3 = "/TEF/SensorVinheria001/attrs/t"; // Tópico MQTT de envio de informações para Broker
const char* default_TOPICO_PUBLISH_4 = "/TEF/SensorVinheria001/attrs/u"; // Tópico MQTT de envio de informações para Broker
const char* default_TOPICO_PUBLISH_5 = "/TEF/SensorVinheria001/attrs/a"; // Tópico MQTT de envio de informações para Broker
const char* default_ID_MQTT = "fiware_001"; // ID MQTT
const int default_D4 = 2; // Pino do LED onboard
// Declaração da variável para o prefixo do tópico
const char* topicPrefix = "SensorVinheria001";

// Variáveis para configurações editáveis
char* SSID = const_cast<char*>(default_SSID);
char* PASSWORD = const_cast<char*>(default_PASSWORD);
char* BROKER_MQTT = const_cast<char*>(default_BROKER_MQTT);
int BROKER_PORT = default_BROKER_PORT;
char* TOPICO_SUBSCRIBE = const_cast<char*>(default_TOPICO_SUBSCRIBE);
char* TOPICO_PUBLISH_1 = const_cast<char*>(default_TOPICO_PUBLISH_1);
char* TOPICO_PUBLISH_2 = const_cast<char*>(default_TOPICO_PUBLISH_2);
char* TOPICO_PUBLISH_3 = const_cast<char*>(default_TOPICO_PUBLISH_3);
char* TOPICO_PUBLISH_4 = const_cast<char*>(default_TOPICO_PUBLISH_4);
char* TOPICO_PUBLISH_5 = const_cast<char*>(default_TOPICO_PUBLISH_5);
char* ID_MQTT = const_cast<char*>(default_ID_MQTT);
int D4 = default_D4;
const int echo    = 21; 
const int trigger = 19; 
float dist = 0;

//CONFIG DHT
    #define DHTPIN 18
    #define DHTTYPE DHT11   
    DHT dht(DHTPIN, DHTTYPE);

WiFiClient espClient;
PubSubClient MQTT(espClient);
char EstadoSaida = '0';

//Monitor Serial
void initSerial() {
    Serial.begin(115200);
}

//Iniciar o wifi
void initWiFi() {
    delay(10);
    Serial.println("------Conexao WI-FI------");
    Serial.print("Conectando-se na rede: ");
    Serial.println(SSID);
    Serial.println("Aguarde");
    reconectWiFi();
}

//Iniciar o MQTT
void initMQTT() {
    MQTT.setServer(BROKER_MQTT, BROKER_PORT);
    MQTT.setCallback(mqtt_callback);
}

void setup() {
    InitOutput();
    initSerial();
    initWiFi();
    initMQTT();
    delay(5000);
    MQTT.publish(TOPICO_PUBLISH_1, "s|on");
}

void loop() {
    VerificaConexoesWiFIEMQTT();
    EnviaEstadoOutputMQTT();
    handleLuminosity();
    handleDHT();
    SensorAlagamento();
    MQTT.loop();
    delay(2000);
}

void reconectWiFi() {
    if (WiFi.status() == WL_CONNECTED)
        return;
    WiFi.begin(SSID, PASSWORD);
    while (WiFi.status() != WL_CONNECTED) {
        delay(100);
        Serial.print(".");
    }
    Serial.println();
    Serial.println("Conectado com sucesso na rede ");
    Serial.print(SSID);
    Serial.println("IP obtido: ");
    Serial.println(WiFi.localIP());

    // Garantir que o LED inicie desligado
    digitalWrite(D4, LOW);
}

void mqtt_callback(char* topic, byte* payload, unsigned int length) {
    String msg;
    for (int i = 0; i < length; i++) {
        char c = (char)payload[i];
        msg += c;
    }
    Serial.print("- Mensagem recebida: ");
    Serial.println(msg);

    // Forma o padrão de tópico para comparação
    String onTopic = String(topicPrefix) + "@on|";
    String offTopic = String(topicPrefix) + "@off|";

    // Compara com o tópico recebido
    if (msg.equals(onTopic)) {
        digitalWrite(D4, HIGH);
        EstadoSaida = '1';
    }

    if (msg.equals(offTopic)) {
        digitalWrite(D4, LOW);
        EstadoSaida = '0';
    }
}

void VerificaConexoesWiFIEMQTT() {
    if (!MQTT.connected())
        reconnectMQTT();
    reconectWiFi();
}

void AcionarBomba(float nivel) {
  if (nivel > 175) {
    digitalWrite(2, HIGH);
    EstadoSaida = '1';
  }
  else{
    digitalWrite(2, LOW);
    EstadoSaida = '0';
  }
}

void SensorAlagamento(){
  digitalWrite(trigger,LOW);
  delayMicroseconds(5);        
  digitalWrite(trigger,HIGH);  
  delayMicroseconds(10);      
  digitalWrite(trigger,LOW); 
  
  dist=pulseIn(echo,HIGH);      
  dist = dist/58;                  // Convertendo para centimetros

  dist = map(dist, 0, 200, 200, 0);
  AcionarBomba(dist);

  String m_agua = String(dist);
  Serial.print("Nivel d'agua: ");
  Serial.print(m_agua.c_str());
  Serial.println("cm");
  MQTT.publish(TOPICO_PUBLISH_5, m_agua.c_str());

}

void EnviaEstadoOutputMQTT() {
    if (EstadoSaida == '1') {
        digitalWrite(2, HIGH);        
        MQTT.publish(TOPICO_PUBLISH_1, "s|on");
        Serial.println("- Led Ligado");
    }

    if (EstadoSaida == '0') {
        digitalWrite(2, LOW);
        MQTT.publish(TOPICO_PUBLISH_1, "s|off");
        Serial.println("- Led Desligado");
    }
    Serial.println("- Estado do LED onboard enviado ao broker!");
    
}

void InitOutput() {
    pinMode(D4, OUTPUT);
    pinMode(trigger,OUTPUT);
    pinMode(echo,INPUT);
    digitalWrite(D4, HIGH);
    boolean toggle = false;

    for (int i = 0; i <= 10; i++) {
        toggle = !toggle;
        digitalWrite(D4, toggle);
        delay(200);
    }
}

void reconnectMQTT() {
    while (!MQTT.connected()) {
        Serial.print("* Tentando se conectar ao Broker MQTT: ");
        Serial.println(BROKER_MQTT);
        if (MQTT.connect(ID_MQTT)) {
            Serial.println("Conectado com sucesso ao broker MQTT!");
            MQTT.subscribe(TOPICO_SUBSCRIBE);
        } else {
            Serial.println("Falha ao reconectar no broker.");
            Serial.println("Haverá nova tentativa de conexão em 2s");
            delay(2000);
        }
    }
}

void handleLuminosity() {
    const int LuxPin = 34;
    int sensorValue = analogRead(LuxPin);
    //Serial.println("Valor do Sensor de lux: " + String(sensorValue));
    if (isnan(sensorValue)){
      Serial.print("Falha ao ler dados do sensor de luz.");
    }
    int luminosity = map(sensorValue, 0, 4095, 0, 100); // 100 significa escuridao, zero = muita luz
    String mensagem = String(luminosity);
    Serial.print("Valor da luminosidade: ");
    Serial.println(mensagem.c_str());
    MQTT.publish(TOPICO_PUBLISH_2, mensagem.c_str());
}

void handleDHT() {
  //Leitura do sensor de umidade
    float u = dht.readHumidity();
  //Leitura do sensor de temperatura (Celsius)
    float t = dht.readTemperature();

    if (isnan(u) || isnan(t))
    {
      Serial.println("Falha ao ler dados do sensor DHT !!!");
      return;
    }

    String m_temp = String(t);
    String m_umi = String(u);

    Serial.print("Valor da temperatura: ");
    Serial.println(m_temp.c_str());
    Serial.print("Valor da umidade: ");
    Serial.println(m_umi.c_str());

    MQTT.publish(TOPICO_PUBLISH_3, m_temp.c_str());
    MQTT.publish(TOPICO_PUBLISH_4, m_umi.c_str());
}
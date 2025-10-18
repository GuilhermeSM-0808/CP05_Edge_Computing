# ESPX-CP05-Edge-Computing
CP05 de Edge Computing - Monitoramente de uma Adega de uma Vinheria Teorica. Monitorando Temperatura, Luminosidade, Umidade e Nivel d'agua em casa de Inundação.

--- 

## Integrantes:

- Guilherme Satler Macedo     - RM 563330

- Laura Sousa Barreto         - RM 561965

- Matheus Freitas Vieira      - RM 566198

- Natalia Camargo de Souza    - RM 565769

---

#### Professor: Paulo Marcotti

### Docker FIWARE do professor Fábio Cabrini utilizado:
https://github.com/fabiocabrini/fiware

---

# Projeto Hands-On

Video demonstrando o funcionamento do ESP32 enviando dados de diversos sensores para o servidor Azure, com o FIWARE em funcionamento para a execução de uma IOT Cloud.

E com a confirmação do funcionamento do servidor e FIWARE coma utilização do PostMan.

https://youtu.be/6yw99pNxt34?si=EcjmgeKY3tj_qCBy

## Dispositivo


![Foto do Dispositivo](src/img/CP05-Dispositivo01.jpg)

![Foto do Dispositivo visto de cima](src/img/CP05-Dispositivo02.jpg)

### Diagrama do dispositivo (WOKWI)

Diagrama reproduzindo a fiação do dispositivo
![Diagrama reproduzindo o dispositivo](src/img/CP05-Diagrama-Wokwi.png)
https://wokwi.com/projects/444829650403562497


Diagrama simplificado do dispositivo
![Diagrama do dispositivo simplificado](src/img/CP05-Diagrama-Wokwi-Simplificado.png)
https://wokwi.com/projects/445188474601375745

---
### Screenshot da página do VM no Azure

![Screenshot da página do Azure exibindo as configurações da maquina virtual](src/img/CP05-Azure.png)

---

### Screenshots da Comunicação do PostMan com o FIWARE no servidor AZURE

#### MQTT Health Check
![MQTT Health Check](src/img/CP05-HealthCheck-MQTT.png)

#### Services Health Check
![Services Health Check](src/img/CP05-HealthCheck-Services.png)

#### Lising all devices
![List all Devices](src/img/CP05-Sensor-ListAllDevices-Postman.png)
Screen Shot exibinding o dispositivo que foi nomeado SensorVinheria001 ao invez de Lamp001

#### Comet Health Check
![Comet Health Check](src/img/CP05-HealthCheck-Comet.png)

#### Orion Health Check
![Orion Health Check](src/img/CP05-HealthCheck-Orion.png)

---

### Extra: Node-Red

![Screenshot do Node-Red](src/img/CP05-NodeRed.png)

![Flow utilizado no Node-Red](src/img/CP05-NodeRed-Flow.png)
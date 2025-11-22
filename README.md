# Projeto - Monitoramento de Saúde e Bem-Estar com ESP32

Este projeto utiliza um ESP32 integrado a um sensor DHT11 para monitorar temperatura e umidade, enviando os dados para um servidor HTTP. Além disso, o sistema utiliza LEDs para indicar condições específicas de temperatura, promovendo bem-estar no ambiente de trabalho.

## 📌 Funcionalidades
- Leitura de temperatura e umidade via sensor **DHT11**
- Envio de dados para endpoint HTTP
- Indicação luminosa com LEDs conforme a temperatura:

- Conexão Wi-Fi com confirmação de status
- Tratamento de erros de sensor e conexão

## 📹 Vídeo da Apresentação
YouTube: https://youtu.be/MexI8lYOrVM?si=6fPAcBFG9vOZVT5C

## 🧪 Simulação no Wokwi
Wokwi: https://wokwi.com/projects/448157981742895105

## 📡 Requisitos do Sistema
- ESP32  
- Sensor DHT11  
- 3 LEDs (Azul, Verde, Vermelho)  
- Resistores 220Ω  
- Protoboard e jumpers  
- Rede Wi-Fi disponível  

## 🛠️ Bibliotecas Utilizadas
- `WiFi.h`
- `HTTPClient.h`
- `DHT.h`

## ⚙️ Configuração do Hardware
- DHT11 → Pino GPIO 4  
- LED Azul → GPIO 13  
- LED Verde → GPIO 12  
- LED Vermelho → GPIO 14  

## 🚀 Como Usar
1. Conecte todos os componentes conforme a seção de hardware.  
2. Instale as bibliotecas necessárias no Arduino IDE.  
3. Configure seu SSID e senha Wi-Fi no código.  
4. Faça upload para o ESP32.  
5. Abra o Serial Monitor para acompanhar os dados.  

## 🌡️ Funcionamento da Lógica
- A cada 2 segundos:  
  - O ESP32 lê temperatura e umidade.  
  - Envia ao servidor HTTP configurado.  
  - Acende o LED correspondente.  
  - Exibe no monitor serial.  

Caso o sensor apresente erro, o sistema não trava — apenas avisa e tenta novamente.

## 📄 Integrantes
- João Gabriel C. M. Santos (RM563953)  
- Leonardo Vinicius de Souza (RM562299)  
- Miguel dos Santos de Andrade (RM563516)  


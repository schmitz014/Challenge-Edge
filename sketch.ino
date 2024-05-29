#include <LiquidCrystal_I2C.h>  // Inclui a biblioteca para o display LCD I2C
#include <DHT.h>               // Inclui a biblioteca para o sensor de temperatura e umidade DHT22

// Configura o sensor DHT22 no pino 13 e especifica o modelo DHT22
DHT dht(13, DHT22);

// Variáveis para armazenar os valores de umidade e temperatura lidos do sensor
float umid, temp;

// Inicializa o display LCD I2C no endereço 0x27, com 16 colunas e 2 linhas
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Variável booleana para controlar o piscar das mensagens e valores
bool mostrarValores = true; 

// Array para definir o desenho de um caractere personalizado para o símbolo de grau
byte grau[8] = {
    B00001100, B00010010, B00010010, B00001100,
    B00000000, B00000000, B00000000, B00000000
};

void setup() {
    // Inicializa o LCD
    lcd.init();
    // Liga a luz de fundo do LCD para melhor visualização
    lcd.backlight();
    // Inicializa o sensor DHT22 para começar a fazer leituras
    dht.begin();
    // Cria o caractere personalizado do símbolo de grau no LCD, associado ao número 0
    lcd.createChar(0, grau);
}

void loop() {
    // Lê os valores de umidade e temperatura do sensor DHT22
    umid = dht.readHumidity();
    temp = dht.readTemperature();

    // Lógica para determinar a recomendação de pneu com base nas condições
    String pneuRecomendado = "Geral"; // Valor padrão: pneus para todos os climas
    if (temp < 10 && umid > 60) {
        pneuRecomendado = "Chuva Ext"; // Chuva extrema: pneus de chuva com maior aderência
    } else if (temp < 15 && umid > 50) {
        pneuRecomendado = "Chuva";    // Chuva moderada: pneus de chuva
    } else if (temp > 25 && umid < 30) {
        pneuRecomendado = "Duro";     // Pista seca e quente: pneus duros para maior durabilidade
    } else if (temp > 18 && temp <= 25) {
        pneuRecomendado = "Macio";    // Pista seca e com temperatura amena: pneus macios para melhor aderência
    }

    // Exibe informações na primeira linha do LCD
    lcd.setCursor(0, 0); // Posiciona o cursor no início da primeira linha
    if (mostrarValores) {
        if (temp > 40 || temp > 35 || umid > 70 || temp < 5 || umid > 80) {
            lcd.print("Cond.: ALERTA!"); // Condições extremas que impedem a corrida
        } else {
            lcd.print("Condicao: OK");  // Condições adequadas para a corrida
        }
    } else {
        lcd.print("                "); // Limpa a primeira linha
    }

    // Exibe informações na segunda linha do LCD, alternando entre temperatura e umidade
    lcd.setCursor(0, 1); // Posiciona o cursor no início da segunda linha
    if (mostrarValores) {
        if (temp > 40) {
            lcd.print("Temp. Alta");
        } else if (temp < 5) {
            lcd.print("Temp. Baixa");
        } else if (umid > 80) {
            lcd.print("Umid. Alta");
        } else if (umid < 20) {
            lcd.print("Umid. Baixa"); 
        } else {
            lcd.print("Pneu: " + pneuRecomendado); // Mostra a recomendação de pneu
        }
    } else { // Pisca o valor da temperatura ou umidade
        if (temp > 40 || temp < 5) {
          lcd.print("Temp.: ");
            lcd.print(temp);
            lcd.write(byte(0)); // Exibe o símbolo de grau
            lcd.print("C");
        } else if (umid > 80 || umid < 20) {
            lcd.print("Umidade: ");
            lcd.print(umid);
            lcd.print("%");
        }
    }

    // Inverte o valor da variável mostrarValores para fazer as mensagens e valores piscarem
    mostrarValores = !mostrarValores;
    delay(1000); // Aguarda 1 segundo
    lcd.clear(); // Limpa o LCD para a próxima atualização
}

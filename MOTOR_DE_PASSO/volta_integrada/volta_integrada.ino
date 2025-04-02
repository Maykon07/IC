const int stepPin = 5;
const int dirPin  = 2;
const int enPin   = 8;
const int ledPin  = 13;

const int stepsPerRevolution = 1600;
const float grausPorRevolucao = 360.0;
const int delayPasso = 500;  // Reduzi o delay para aumentar a velocidade
const int delayEntreMovimentos = 15000; // 15 segundos entre movimentos

float grausAcumulados = 0.0;
float grausParaGirar = 0.0;
bool recebeuComando = false;

void setup() {
    pinMode(stepPin, OUTPUT);
    pinMode(dirPin, OUTPUT);
    pinMode(enPin, OUTPUT);
    pinMode(ledPin, OUTPUT);
    
    digitalWrite(enPin, LOW);
    digitalWrite(ledPin, LOW);
    Serial.begin(9600);
    Serial.println("Aguardando comando de graus via Serial...");
}

void girarGraus(float graus) {
    if (graus <= 0 || (grausAcumulados + graus) > grausPorRevolucao) {
        Serial.println("Movimento inválido, excede 360° ou é zero.");
        return;
    }

    int passos = (int)((graus / grausPorRevolucao) * stepsPerRevolution);
    Serial.print("Girando ");
    Serial.print(graus, 2);
    Serial.println(" graus.");

    digitalWrite(dirPin, HIGH);
    for (int x = 0; x < passos; x++) {
        digitalWrite(stepPin, HIGH);
        delayMicroseconds(delayPasso);
        digitalWrite(stepPin, LOW);
        delayMicroseconds(delayPasso);
    }

    grausAcumulados += graus;
    Serial.print("Total girado: ");
    Serial.print(grausAcumulados, 2);
    Serial.println(" graus.");
    delay(1000);
}

void loop() {
    if (Serial.available() > 0) {
        grausParaGirar = Serial.parseFloat();
        Serial.print("Recebido: ");
        Serial.print(grausParaGirar, 2);
        Serial.println(" graus.");
        grausAcumulados = 0.0;
        recebeuComando = true;
    }

    if (recebeuComando && grausParaGirar > 0) {
        if (grausAcumulados < grausPorRevolucao) {
            girarGraus(grausParaGirar);
            delay(delayEntreMovimentos);
        } else {
            Serial.println("Rotação completa de 360° atingida. Aguardando novo comando...");
            recebeuComando = false;
        }
    }
}

#include <iostream>  // Biblioteca para entrada e saída de dados (cout, cin)
#include <windows.h>  // Biblioteca para manipulação da porta serial no Windows
#include <string>  // Biblioteca para manipulação de strings

// Define a porta serial que será utilizada. Deve ser alterada conforme a porta correta no seu sistema.
const char* portaSerial = "COM3";  

// Define o intervalo de tempo (em segundos) entre os comandos enviados ao Arduino.
const int intervaloSegundos = 15;  

// Função para abrir e configurar a porta serial.
HANDLE abrirPortaSerial(const char* porta) {
    // Abre a porta serial para escrita
    HANDLE hSerial = CreateFileA(porta, GENERIC_WRITE, 0, 0, OPEN_EXISTING, 0, 0);
    
    // Verifica se a porta foi aberta corretamente
    if (hSerial == INVALID_HANDLE_VALUE) {
        std::cerr << "Erro ao abrir a porta serial!" << std::endl;
        return INVALID_HANDLE_VALUE; // Retorna um valor inválido indicando erro
    }

    // Estrutura para armazenar configurações da porta serial
    DCB dcbSerialParams = {0};
    dcbSerialParams.DCBlength = sizeof(dcbSerialParams); // Define o tamanho da estrutura

    // Obtém as configurações atuais da porta serial
    if (!GetCommState(hSerial, &dcbSerialParams)) {
        std::cerr << "Erro ao obter configurações da porta serial!" << std::endl;
        CloseHandle(hSerial); // Fecha a porta serial antes de sair
        return INVALID_HANDLE_VALUE;
    }

    // Define os parâmetros da comunicação serial
    dcbSerialParams.BaudRate = CBR_9600; // Taxa de transmissão: 9600 baud
    dcbSerialParams.ByteSize = 8; // 8 bits de dados
    dcbSerialParams.StopBits = ONESTOPBIT; // 1 bit de parada
    dcbSerialParams.Parity = NOPARITY; // Sem bit de paridade

    // Aplica as configurações na porta serial
    if (!SetCommState(hSerial, &dcbSerialParams)) {
        std::cerr << "Erro ao configurar a porta serial!" << std::endl;
        CloseHandle(hSerial); // Fecha a porta serial antes de sair
        return INVALID_HANDLE_VALUE;
    }

    return hSerial; // Retorna o manipulador da porta serial configurada
}

// Função para enviar um comando (número de graus) via porta serial
void enviarComando(HANDLE hSerial, float graus) {
    if (hSerial == INVALID_HANDLE_VALUE) return; // Se a porta for inválida, sai da função

    // Converte o valor de graus para uma string e adiciona um "\n" para o Arduino reconhecer o comando
    std::string comando = std::to_string(graus) + "\n";
    DWORD bytesEnviados;

    // Envia os dados para a porta serial
    WriteFile(hSerial, comando.c_str(), comando.length(), &bytesEnviados, NULL);

    // Exibe no console o comando enviado
    std::cout << "Enviado: " << graus << " graus para " << portaSerial << std::endl;
}

// Função principal do programa
int main() {
    // Abre a porta serial
    HANDLE hSerial = abrirPortaSerial(portaSerial);
    if (hSerial == INVALID_HANDLE_VALUE) return 1; // Se não conseguiu abrir a porta, encerra o programa

    float graus; // Variável para armazenar o valor dos graus
    std::cout << "Digite quantos graus o motor deve girar por vez: ";
    std::cin >> graus; // Solicita ao usuário o valor de graus para movimento do motor

    // Envia o primeiro comando para o Arduino
    enviarComando(hSerial, graus);

    // Loop infinito para continuar enviando comandos periodicamente
    while (true) {
        Sleep(intervaloSegundos * 1000); // Aguarda o tempo definido antes de enviar o próximo comando
        enviarComando(hSerial, graus); // Envia novamente o mesmo comando
    }

    CloseHandle(hSerial); // Fecha a conexão da porta serial (nunca será executado devido ao loop infinito)
    return 0;
}

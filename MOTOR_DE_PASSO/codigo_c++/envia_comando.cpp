#include <iostream>
#include <windows.h>
#include <string>

const char* portaSerial = "COM3";

HANDLE abrirPortaSerial(const char* porta) {
    HANDLE hSerial = CreateFileA(porta, GENERIC_WRITE, 0, 0, OPEN_EXISTING, 0, 0);
    if (hSerial == INVALID_HANDLE_VALUE) {
        std::cerr << "Erro ao abrir a porta serial!" << std::endl;
        return INVALID_HANDLE_VALUE;
    }

    DCB dcbSerialParams = {0};
    dcbSerialParams.DCBlength = sizeof(dcbSerialParams);

    if (!GetCommState(hSerial, &dcbSerialParams)) {
        std::cerr << "Erro ao obter configurações da porta serial!" << std::endl;
        CloseHandle(hSerial);
        return INVALID_HANDLE_VALUE;
    }

    dcbSerialParams.BaudRate = CBR_9600;
    dcbSerialParams.ByteSize = 8;
    dcbSerialParams.StopBits = ONESTOPBIT;
    dcbSerialParams.Parity = NOPARITY;

    if (!SetCommState(hSerial, &dcbSerialParams)) {
        std::cerr << "Erro ao configurar a porta serial!" << std::endl;
        CloseHandle(hSerial);
        return INVALID_HANDLE_VALUE;
    }

    return hSerial;
}

void enviarComando(HANDLE hSerial, const std::string& comando) {
    if (hSerial == INVALID_HANDLE_VALUE) return;

    std::string comandoFinal = comando + "\n";
    DWORD bytesEnviados;
    WriteFile(hSerial, comandoFinal.c_str(), comandoFinal.length(), &bytesEnviados, NULL);
    std::cout << "Comando enviado: " << comando << std::endl;
}

int main() {
    HANDLE hSerial = abrirPortaSerial(portaSerial);
    if (hSerial == INVALID_HANDLE_VALUE) return 1;

    while (true) {
        std::cout << "\nDigite o comando (1 = girar motor, 2 = ligar LED): ";
        std::string comando;
        std::getline(std::cin, comando);

        if (comando == "1") {
            enviarComando(hSerial, comando); // envia o comando 1 primeiro
            std::cout << "Digite o valor em graus (float): ";
            std::string graus;
            std::getline(std::cin, graus);
            
            Sleep(100); // pequeno atraso para garantir separação dos comandos
            enviarComando(hSerial, graus); // envia o valor de graus depois
        } else if (comando == "2") {
            enviarComando(hSerial, comando);
        } else {
            std::cout << "Comando inválido. Use 1 (girar) ou 2 (ligar LED)." << std::endl;
        }
    }

    CloseHandle(hSerial);
    return 0;
}

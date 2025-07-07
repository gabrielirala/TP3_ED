#include "Sistema.h"
#include <iostream>
#include <fstream> // Necessário para manipulação de arquivos

int main(int argc, char* argv[]) {
    // Define o locale para evitar problemas com parsing
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    // Verifica se o nome do arquivo foi fornecido na linha de comando
    if (argc < 2) {
        std::cerr << "Erro: Nenhum arquivo de entrada especificado." << std::endl;
        std::cerr << "Uso: " << argv[0] << " <nome_do_arquivo>" << std::endl;
        return 1; // Retorna um código de erro
    }

    // Tenta abrir o arquivo
    std::ifstream arquivo_entrada(argv[1]);
    if (!arquivo_entrada.is_open()) {
        std::cerr << "Erro: Nao foi possivel abrir o arquivo '" << argv[1] << "'" << std::endl;
        return 1; // Retorna um código de erro
    }

    Sistema sistemaLogistico;
    // Passa o fluxo do arquivo para a função executar
    sistemaLogistico.executar(arquivo_entrada);

    arquivo_entrada.close(); // Fecha o arquivo ao final

    return 0;
}

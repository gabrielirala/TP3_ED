#include "Sistema.h"

int main() {
    // Define o locale para evitar problemas com parsing de números e strings
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);
    
    Sistema sistemaLogistico;
    sistemaLogistico.executar();
    
    return 0;
}
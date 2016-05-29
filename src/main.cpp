#include <iostream>

#include "mempool_common.h" // para sobrecarregar os operadores new e delete
#include "MemoryPool.h" // para sobrecarregar os operadores new e delete

int main() {

 SLPool p(666); // criando o gerenciador de memória
 p.Mapear(); // exibindo o estado da memória

 int *teste = new (p)int; // um teste simples debug

 *teste = 42;
 std::cout << *teste << std::endl;

 p.Mapear(); // exibindo o estado da memória

std::cout << "\n>>>>> Saida normal\n";
	return 0;
}

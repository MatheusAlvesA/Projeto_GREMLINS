#include <iostream>

#include "mempool_common.h" // para sobrecarregar os operadores new e delete
#include "MemoryPool.h" // para sobrecarregar os operadores new e delete

struct objeto_pequeno
{
	int x[2]; // dois inteiros
};
struct objeto_medio
{
	int x[8]; // oito inteiros
};
struct objeto_grande
{
	int x[50]; // cinquenta inteiros
};

int main() {

std::cout << "INICIANDO TESTES DA CLASSE SLPool COM ESTRATÈGIA FIRST-FIT\n\n";
 SLPool p(666); // criando o gerenciador de memória

 std::cout << "#################################################################\n";
 std::cout << "Primeiro nível de testes apenas 2 variaveis alocadas e liberadas\n";
 std::cout << "#################################################################\n";

 p.Mapear(); // exibindo o estado da memória

 int *a = new (p)int; // um teste simples debug
 int *b = new (p)int;

 *a = 42;
 *b = 69;
 std::cout << "a = " << *a << std::endl;
 std::cout << "b = " << *b << std::endl;

 p.Mapear(); // exibindo o estado da memória

std::cout << "Liberando: a";
 delete a;

 p.Mapear();

 std::cout << "Liberando: b";
 delete b;
 
 p.Mapear();

 std::cout << "#################################################################\n";
 std::cout << "Segundo nível de testes ainda em desenvolvimento\n";
 std::cout << "#################################################################\n";

// coming son

std::cout << "\n>>>>> Saida normal\n";
	return 0;
}

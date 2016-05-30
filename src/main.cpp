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
 SLPool p(1024*1000); // criando o gerenciador de memória com 1 MegaByte

 std::cout << "#################################################################\n";
 std::cout << "Primeiro nível de testes, apenas 2 variaveis alocadas e liberadas\n";
 std::cout << "#################################################################\n";

 p.Mapear(); // exibindo o estado da memória

 int *a = new (p)int; // um teste simples debug
 int *b = new (p)int;

 *a = 42;
 *b = 10;
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
 std::cout << "Segundo nível de testes, verificar gerenciamento da fragmentação\n";
 std::cout << "#################################################################\n";

objeto_pequeno *x = new (p) objeto_pequeno; // alocando o objeto pequeno
x->x[0] = 1; // setando valor para o objeto pequeno
std::cout << "Valor do objeto pequeno: " << x->x[0];

objeto_medio *y = new (p) objeto_medio; // alocando o objeto medio
y->x[3] = 2; // setando valor para o objeto medio
std::cout << "\nValor do objeto médio: " << y->x[3];

objeto_grande *z = new (p) objeto_grande; // alocando o objeto grande
z->x[40] = 3; // setando valor para o objeto grande
std::cout << "\nValor do objeto grande: " << z->x[40];

p.Mapear();

std::cout << "\n\nLiberando o objeto médio\n";
delete y;
p.Mapear();

std::cout << "Alocando um valor inteiro\n";
a = new (p)int;
p.Mapear();

std::cout << "\nRealocando o objeto médio\n";
y = new (p) objeto_medio;
p.Mapear();

std::cout << "\n\nColocando uma novo espaço de tamanho médio e liberando o medio anterior\n";
objeto_medio *s = new (p)objeto_medio;
s->x[3] = 4;
std::cout << "Valor no novo objeto médio: " << s->x[3];
delete y;
p.Mapear();

std::cout << "Nesse momento a memória já se fragmentou em \"três pedaços\" alocando um inteiro, não deve ocorrer erro\n";
b = new (p)int;
*b = 5;
p.Mapear();

std::cout << "Nesse momento vou deletar alguns objetos e alocar um novo int ele será gravado no espaço vazio maior ou no espaço que melhor se encaixe dependendo apenas da estratégia usada, Best-Fit ou First-Fit\n";
 delete x;
 delete a;
p.Mapear();

a  = new (p)int;
*a = 20;
std::cout << "Agora alocado o valor int com o valor: " << *a;
p.Mapear();

std::cout << "\n\nLIMPANDO TUDO\n";
delete z;
delete s;
delete b;
delete a;
p.Mapear();

 std::cout << "#################################################################\n";
 std::cout << "Terceiro nível de testes, teste de estress\n";
 std::cout << "#################################################################\n";

std::cout << "\nIniciando teste pesado... SLPool";
objeto_grande *data[100] = {nullptr}; // inicialmente nada alocado
unsigned int indice = 0; // indice que se atualiza a cada volta do laço
size_t alocs = 0;
size_t delocs = 0;
for(time_t tempo = time(nullptr); tempo >= time(nullptr)-2;/*vazio*/) {
  indice = std::rand()%100;
  if(data[indice] != nullptr) { // se aquele indice estiver alocado, ele será deletado
   delete data[indice];
   data[indice] = nullptr; // registrando que foi deletado
   delocs++;
  }
  else { // caso contrario será alocado
   data[indice] = new (p)objeto_grande;
   alocs++;
  }
}
p.Mapear();
for(int x = 0; x < 100; x++) {if(data[x] != nullptr) {delete data[x];data[x] = nullptr;}}

std::cout << "\nIniciando teste pesado... Sistema Operacional";
indice = 0; // indice que se atualiza a cada volta do laço
size_t alocs_so = 0;
size_t delocs_so = 0;
for(time_t tempo = time(nullptr); tempo >= time(nullptr)-2;/*vazio*/) {
  indice = std::rand()%100;
  if(data[indice] != nullptr) { // se aquele indice estiver alocado, ele será deletado
   delete data[indice];
   data[indice] = nullptr; // registrando que foi deletado
   delocs_so++;
  }
  else { // caso contrario será alocado
   data[indice] = new objeto_grande; // alocando pelo sistema operacional
   alocs_so++;
  }
}
for(int x = 0; x < 100; x++) if(data[x] != nullptr) delete data[x]; // deletando qualquer coisa que tenha sido deixada para traz

std::cout << "\n\n+++++++++++++++++RESULTADO DO TESTE MASSIVO++++++++++++++++++++++\n"
          << "A classe de gerenciamento desenvolvida alocou " << alocs << " espaços de memória, e desalocou " << delocs << " espaços.\n"
          << "O sistema operacional alocou " << alocs_so << " e, desalocou " << delocs_so << " espaços.\n Ambos executaram por dois segundos.\n";
if(alocs > alocs_so) { // a classe ganhou em alocações
	std::cout << "\nA classe StoragePool venceu em numero de alocações, alocando " << alocs - alocs_so << " espaços a mais que o sistema operacional.\n";
}
else std::cout << "\nO sistema operacional venceu em numero de alocações, alocando " << alocs_so - alocs << " espaços a mais que a classe StoragePool.\n";
if(delocs > delocs_so) { // a classe ganhou em desalocações
	std::cout << "\nA classe StoragePool venceu em numero de desalocações, desalocando " << delocs - delocs_so << " espaços a mais que o sistema operacional.\n";
}
else std::cout << "\nO sistema operacional venceu em numero de desalocações, desalocando " << delocs_so - delocs << " espaços a mais que a classe StoragePool.\n";


std::cout << "\n>>>>>>>>>>> Saida normal\n";
	return 0;
}

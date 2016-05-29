#include "MemoryPool.h"

SLPool::SLPool(size_t tamanho) {
	this->mui_NumberOfBlocks = (tamanho/Block::BlockSize) + 1; // o numero de blocos é a quantidadse de bites dividida pelo tamanho de blocos
     /*note que os blocos devem estar todos alihados consevutivamente na memória, assim será possivel tratar
     mais de um bloco consecutivo como um unico espaço de memória*/
     this->mp_Pool = new Block[this->mui_NumberOfBlocks]; // alocando os blocos concatenados
     this->mp_Pool->mp_Next = nullptr; // o primeiro bloco aponta para nada, posi tudo está livre
     this->mp_Pool->mui_Length = this->mui_NumberOfBlocks; // o tamanho desse unico espaço livre é todo tamanho
     this->mr_Sentinel.mp_Next = this->mp_Pool; // configurando os primeiro elemento no sentinela
}

SLPool::~SLPool() {
  delete [] this->mp_Pool; // apagando toda a memória alocada
}

void *SLPool::Allocate(size_t tamanho) {
  size_t Nblocks = (tamanho/Block::BlockSize) + 1; // Um bloco a mais para garantir que não ocorrerá erro de arredondamento
  Block *trabalhador = this->mr_Sentinel.mp_Next; // inicia o loop com a primeira zona vazia
  Block *anterior = &this->mr_Sentinel; // guardar o anterior é importante para saber qual a zona livre anterior a zona livre solicitada
  while(trabalhador->mui_Length < Nblocks && trabalhador != nullptr) { // percorre até achar uma zona que caiba ou até acabar as zonas vazias
    trabalhador = trabalhador->mp_Next; // preparando-se para checar a próxima zona
    anterior = anterior->mp_Next; // atualizando o anterior para sempre permanecer um passo atrás
  }
  if(trabalhador == nullptr) throw(std::bad_alloc()); // gerando exeção pois não tem memória suficiente
 // nesse ponto da execução da função foi encontrado o espaço nescesário para alocar o objeto
  if(Nblocks == trabalhador->mui_Length) { // situação ideal onde a zona tem ezatamente o numero de blocos nescesários
    anterior->mp_Next = trabalhador->mp_Next; // A zona livre anterior agora salta essa zona que será ocupada, apontando para a zona imediatamente apos a solicitada
  } else { // o numero de blocos da zona livre encontrada é maior que o solicitado
    anterior->mp_Next = trabalhador + Nblocks; // a nova zona vazia é a anterior pulando a parte alocada
    anterior->mp_Next->mui_Length = trabalhador->mui_Length - Nblocks; // configurando a zona nova com seu tamanho reduzido
    anterior->mp_Next->mp_Next = trabalhador->mp_Next; // a nova zona reduzida aponta para a próxima zona sem alteração
  }
  return reinterpret_cast<byte*> (trabalhador + sizeof(Header)); // retornando a zona livre "disfarçada" de void santando o local onde está gravado o tamanho da zona
}

void SLPool::Free(void *entrada) {
  Block *zona = reinterpret_cast<Block*>(entrada); // convertendo de volta em zona de memoria
}

void SLPool::Mapear(void) {
  Block *auxiliar = this->mr_Sentinel.mp_Next; // configurando uma variável auxiliar
  int x = 0;
  std::cout << "\n\n___________________________MAPA DA MEMÓRIA_________________________________\n";
  while(auxiliar != nullptr) { // percorre até achar o fim das zonas vazias
    std::cout << "Zona livre " << auxiliar << ": com " << auxiliar->mui_Length << " blocos\n";
    auxiliar = auxiliar->mp_Next; // indo até a proxima area livre
    x++;
  }
  std::cout << ">>>>>>>>>>> No total " << x << " zonas livres\n";
  std::cout << "___________________________________________________________________________\n\n";
}

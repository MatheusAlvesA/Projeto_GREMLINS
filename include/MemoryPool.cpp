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
    trabalhador->mui_Length = Nblocks; // a zona alocada tem esse tamanho
  }
  return reinterpret_cast<Header*>(trabalhador) + 1U; // retornando a zona livre "disfarçada" de void sautando o local onde está gravado o tamanho da zona
}

void SLPool::Free(void *entrada) {
  Block *zona = reinterpret_cast<Block*>(reinterpret_cast<Header*>(entrada) - 1U); // convertendo de volta em zona de memoria e recuperando o valor header
  Block *seguinte = this->mr_Sentinel.mp_Next; // inicia o loop com a primeira zona vazia
  Block *anterior = &this->mr_Sentinel; // guardar o anterior é importante para saber qual a zona livre anterior a zona livre solicitada
  while(seguinte < zona && seguinte != nullptr) { // percorre até achar uma zona que seja posterior a essa, ou não encontrar
    seguinte = seguinte->mp_Next; // preparando-se para checar a próxima zona
    anterior = anterior->mp_Next; // atualizando o anterior para sempre permanecer um passo atrás
  }
  bool Imediato_anterior = false; // essa variavel guarda se a zona livre anterior é imediatamente antererior a essa sendo liberda
  bool Imediato_seguinte = false; // enquanto essa guarda se o proximo é imediatamente consecutivo
  if(zona == anterior + zona->mui_Length) Imediato_anterior = true; // caso seja vizinho anterior
  if(zona + zona->mui_Length == seguinte) Imediato_seguinte = true; // caso seja vizinho seguinte
  // existem diversas situações que devem ser tratadas de acordo
  // caso a zona a ser liberada seja vizinha a duas outra zonas já livres
  if(Imediato_anterior && Imediato_seguinte) { // as duas variaveis indicam que é todos são vizinhos
    anterior->mp_Next = seguinte->mp_Next; // aponta para a proxima zona livre depois dos três
    anterior->mui_Length += zona->mui_Length + seguinte->mui_Length; // o tamanho é equivalente a soma deles
  }
  // caso a zona a ser liberada seja vizinha a duas outra zonas ocupadas
  if(!Imediato_anterior && !Imediato_seguinte) { // as duas variaveis indicam que ninguém está livre vizinho a essa zona
    anterior->mp_Next = zona; // a zona anterior aponta para essa
    zona = seguinte; // essa aponta para a zona livre seguinte
  }
  // caso a zona a ser liberada seja imediatamente anterior a uma livre porem não seja consecutiva a uma
  if(!Imediato_anterior && Imediato_seguinte) { // a anterior está ocupada e a seguinte não, de acordo com as booleanas
    anterior->mp_Next = zona; // a zona anterior aponta para essa
    zona->mp_Next = seguinte->mp_Next; // essa zona aponta para a proxima livre
    zona->mui_Length += seguinte->mui_Length; // combinando as duas em uma unica zona livre
  }
  // caso a zona a ser liberada seja imediatamente seguinte a uma livre porem não seja anterior a uma
  if(Imediato_anterior && !Imediato_seguinte) { // a anterior está livre e a seguinte não, de acordo com as booleanas
    anterior->mui_Length += zona->mui_Length; // apenas almentar o tamanho da zona imediatamente anterior já é suficiente
  }
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

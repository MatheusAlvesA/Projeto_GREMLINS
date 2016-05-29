#ifndef _MEMORYPOOL_
#define _MEMORYPOOL_

typedef char byte; // usando char pois esse tipo só possui um byte
class StoragePool { // essa classe apenas força as classes de gerenciamento de memória a usar funções basicas de alocar e liberar memória

 public:

  virtual ~StoragePool() {/*vazio*/};
  virtual void *Allocate(size_t) = 0;
  virtual void Free(void *) = 0;
};

class SLPool : public StoragePool { // essa é a classe principal que gerencia a memória

public:
struct Header { // cabeçalho que guarda o tamanho da zona livre em termos de numero de blocos
 unsigned int mui_Length;
 Header () : mui_Length(0u) { /* Empty */ };
};

struct Block : public Header { // o principal componente que representa o espaço alocado

enum { BlockSize = 16 }; // Each block has 16 bytes.

union {
 Block *mp_Next; // um ponteiro para o próximo bloco caracterizando dessa forma uma lista encadeada
 byte mc_RawArea [BlockSize + sizeof(Header)];
};

Block() : Header (), mp_Next (nullptr) { /* Empty */ };

};
// zona de funções
 explicit SLPool (size_t); // construtor que aloca os blocos preparando o gerenciador
~SLPool(); // destrutor que libera a memória usada
void *Allocate (size_t); // função para alocar memória usando a estratégia first-fit
void Free (void *); // função para liberar memória

void Mapear(void); // função de debug que mostra o estado do gerenciador

protected:
unsigned int mui_NumberOfBlocks;

Block *mp_Pool;
// ! < Head of list .
Block mr_Sentinel;
// ! < End of the list 
};

class SLPoolBest : public SLPool { // essa classe apenas se diferencia da anterior pelo fato de uzar a estratégia best-fit
public:
	SLPoolBest(size_t x) : SLPool(x) {/*vazio*/};
	void *Allocate(size_t); // Aloca memoria usano a estratégia best-fit
};

#include "MemoryPool.cpp"

#endif
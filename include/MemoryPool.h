#ifndef _MEMORYPOOL_
#define _MEMORYPOOL_

typedef char byte; // usando char pois esse tipo só possui um byte
class StoragePool {

 public:

  virtual ~StoragePool() {/*vazio*/};
  virtual void *Allocate(size_t) = 0;
  virtual void Free(void *) = 0;
};

class SLPool : public StoragePool {

public:
struct Header {
 unsigned int mui_Length;
 Header () : mui_Length(0u) { /* Empty */ };
};

struct Block : public Header {

enum { BlockSize = 16 }; // Each block has 16 bytes.

union {
 Block *mp_Next;
 byte mc_RawArea [BlockSize + sizeof(Header)];
};

Block() : Header (), mp_Next (nullptr) { /* Empty */ };

};
// zona de funções
 explicit SLPool (size_t); // construtor que aloca os blocos preparando o gerenciador
~SLPool(); // destrutor que libera a memória usada
void *Allocate (size_t); // função para alocar memória
void Free (void *); // função para liberar memória

void Mapear(void); // função de debug que mostra o estado do gerenciador

private:
unsigned int mui_NumberOfBlocks;

Block *mp_Pool;
// ! < Head of list .
Block mr_Sentinel;
// ! < End of the list 
};

#include "MemoryPool.cpp"

#endif
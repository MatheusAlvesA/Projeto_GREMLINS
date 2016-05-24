#include "MemoryPool.h"

SLPool::SLPool(size_t tamanho) {
	this->mui_NumberOfBlocks = (tamanho/BlockSize) + 1; // o numero de blocos é a quantidadse de bites dividida pelo tamanho de blocos
     Block *auxiliar = new Block(); // criando o primeiro bloco
     this->mp_Pool.mp_Next = auxiliar; // configirando o inicio da lista de blocos como o primeiro bloco
	for(unsigned int x = 1; x < this->mui_NumberOfBlocks; x++) { // começa no segundo pois o primeiro foi alocado obrigatoriamente
      auxiliar->mp_Next = new Block(); // criando o novo bloco
      auxiliar = auxiliar->mp_Next; // caminhando até esse novo bloco
	}
	auxiliar->mp_Next = &this->mp_Sentinel; // configurando o final da lista como o sentinela
}

SLPool::~SLPool() {
     Block *auxiliar = this->mp_Pool.mp_Next; // configurando o primeiro bloco para iniciar a deleção
     Block *temp;
	for(auxiliar != &this->mp_Sentinel) { // percorre todos os blocos
      temp = auxiliar->next; // guardando ponteiro para não perder no proximo comando
      delete auxiliar; // deletando o elemento
      auxiliar = temp; // caminhando até esse novo bloco
	}
}
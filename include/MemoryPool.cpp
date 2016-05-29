#include "MemoryPool.h"

/*
Construtor da classe, recebe tamanho em bytes que a aplicação vai necessitar, calcula o numero de blocos nescesários e aloca.
this->mr_Sentinel é o sentinela que aponta para o inicio das zonas vazias sempre, graças a ela será possivel identificar o ponto
de partida para percorrer as zonas vazias, essas zonas terminam em um nullptr.
this->mp_Pool guarda o primeiro bloco de todas as zonas independente de ser ou não um bloco de zona vazia
*/
SLPool::SLPool(size_t tamanho) {
	this->mui_NumberOfBlocks = (tamanho/Block::BlockSize) + 1; // o numero de blocos é a quantidadse de bites dividida pelo tamanho de blocos
     /*note que os blocos devem estar todos alihados consevutivamente na memória, assim será possivel tratar
     mais de um bloco consecutivo como um unico espaço de memória*/
     this->mp_Pool = new Block[this->mui_NumberOfBlocks]; // alocando os blocos concatenados
     this->mp_Pool->mp_Next = nullptr; // o primeiro bloco aponta para nada, posi tudo está livre
     this->mp_Pool->mui_Length = this->mui_NumberOfBlocks; // o tamanho desse unico espaço livre é todo tamanho
     this->mr_Sentinel.mp_Next = this->mp_Pool; // configurando os primeiro elemento no sentinela
}
/*
O destrutor é simples pois apenas apaga o bloco de memória anteriormente alocado
*/
SLPool::~SLPool() {
  delete [] this->mp_Pool; // apagando toda a memória alocada
}
/*
Essa função aloca e retona um ponteiro para a memoria solicitada, 
tamanho contem o numero de bytes requisitado e atravez dele é calculado quantos blocos serão nescessários para suprir a demanda de memória solicitada.
o ponteiro para bloco rabalhador é responsável por percorrer a lista encadeada de zonas vazias e encontrar uma delas que seja grande
o bastante para suprir a demanda.
se for encontrada uma zona igual:
O bloco anterior será configurado para pular essa zona assim excluindo ela da lista de zonas vazias
em caso de encontrar uma zona maior que a nescesária:
O bloco anterior vai apontar para uma versão reduzida da zona a qual ele apontava antes
essa versão reduzida será configurada para se comportar como uma zona vazia nova
*/
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
/*
A função free libera a memória alocada anteriormente,
a entrada é um ponteiro que será convertido de volta em um ponteiro para zona livre composta por um certo numero de blocos
esse numero de blocos foi salvo graças a um calculo de aritimetica de ponteiros realizada na função de alocação,
uma vez que a função tem o ponteiro para a zona e o tamanho da mesma, é possivel calcular onde essa zona se encaixa,
isso devesse ao fato de todos os blocos serem mutuamente vizinhos, assim, apos descobrir onde se encaixa aquela zona ela será
encaixada de volta, note que nesse momento existem situações a serem consideradas onde existem quantro possibilidades:
A zona devolvida não é vizinha de nenhuma outra livre
A zona devolvida é vizinha a outras duas zonas que estão livres
A zona devolvida é imediatamente apos a uma zona livre vizinha
A zona devolvida é imediatamente anterior a uma zona livre vizinha
E cada caso é nescesário tratar de forma especial para garantir que tudo se manterá em seu lugar e dessa forma não averá fragmentação
ou corrupção no espaço de memória geral. Todos os casos estão sendo tratados de acordo.
Existe alguns casos onde não é feita uma interpretação incorreta dos dados, isso de deve ao fato do sentinea não fazer parte de nenhua zona
então quando ela é intepretada como parte da zona graças ao ponteiro "anterior" que aponta para a zona livre imediatamente anterior a zona devolvida.
Entretanto esse erro não acarreta erro na execução do algoritmo que acaba executando a instrução correta mesmo assim, isso pode ser verificado no código a baixo.
*/
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
    zona->mp_Next = seguinte; // essa aponta para a zona livre seguinte
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
/*
Essa função é apenas ilistrativa ela não recebe qualquer parâmetro e serve para exibir uma lista de zonas livre da memória naquele momento
*/
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

// Função da classe SLPoolBest que aloca a memoria usando a estratégia best-fit

void *SLPoolBest::Allocate(size_t tamanho) {
  size_t Nblocks = (tamanho/Block::BlockSize) + 1; // Um bloco a mais para garantir que não ocorrerá erro de arredondamento
  Block *trabalhador = this->mr_Sentinel.mp_Next; // inicia o loop com a primeira zona vazia
  Block *melhor = trabalhador; // este ponteiro aponta para a melhor zona vazia para encaixar a memoria solicitada
  Block *anterior = &this->mr_Sentinel; // guardar o anterior é importante para saber qual a zona livre anterior a zona livre solicitada
  Block *melhor_anterior = anterior; // guarda quem é a zona imediatamente anterior a melhor zona solicitada
  if(trabalhador == nullptr) throw(std::bad_alloc()); // não existe zona livre
  while(trabalhador != nullptr) { // percorre todas as zonas livres em busca da que melhor se encaixa
    if(trabalhador->mui_Length - Nblocks < melhor->mui_Length - Nblocks && trabalhador->mui_Length - Nblocks >= 0) {
     melhor = trabalhador; // se encontrar alguem que se encaixa melhor então atualiza o melhor
     melhor_anterior = anterior;
    }
    trabalhador = trabalhador->mp_Next; // preparando-se para checar a próxima zona
    anterior = anterior->mp_Next; // atualizando o anterior para sempre permanecer um passo atrás
  }
  if(melhor == nullptr || Nblocks > melhor->mui_Length) throw(std::bad_alloc()); // gerando exeção pois não tem memória suficiente
 // nesse ponto da execução da função foi encontrado o espaço nescesário para alocar o objeto
  if(Nblocks == melhor->mui_Length) { // situação ideal onde a zona tem ezatamente o numero de blocos nescesários
    melhor_anterior->mp_Next = melhor->mp_Next; // A zona livre melhor_anterior agora salta essa zona que será ocupada, apontando para a zona imediatamente apos a solicitada
  } else { // o numero de blocos da zona livre encontrada é maior que o solicitado
    melhor_anterior->mp_Next = melhor + Nblocks; // a nova zona vazia é a melhor_anterior pulando a parte alocada
    melhor_anterior->mp_Next->mui_Length = melhor->mui_Length - Nblocks; // configurando a zona nova com seu tamanho reduzido
    melhor_anterior->mp_Next->mp_Next = melhor->mp_Next; // a nova zona reduzida aponta para a próxima zona sem alteração
    melhor->mui_Length = Nblocks; // a zona alocada tem esse tamanho
  }
  return reinterpret_cast<Header*>(melhor) + 1U; // retornando a zona livre "disfarçada" de void sautando o local onde está gravado o tamanho da zona
}
#Projeto GREMLINS

 - [Matheus Alves de Andrade](https://github.com/MatheusAlvesA)

matheuseejam@hotmail.com

Link do projeto no GitHub: https://github.com/MatheusAlvesA/Projeto_GREMLINS.git

#Introdução
Esse projeto implementa um algoritmo responsável por realizar o trabalho de
gerenciamento de memória. Quando sua classe é criada uma determinada quantidade de memória RAM
é alocada, essa memória alocada então passa a ser gerenciada pela classe do algoritmo,
os operadores new e delete são sobrecarregados, dessa forma toda nova requisição
de memória será tratada por essa classe, isso dá ao programador mais controle
e velocidade sobre seus objetos alocados dinamicamente.

#Implementação
Dois arquivos .h foram criados, o MemoryPool.h guarda os prótotipos e as classes
que de fato alocam mémoria e gerenciam a mesma, já o arquivo mempool_common.h sobrecarrega
os operadores new e delete para que, assim, o programador não precisa se preocupar
com a alocação que será feita da mesma forma que antes. Um arquivo main.cpp está
disponível no diretório src/ ele realiza testes na classe MemoryPool buscando demonstrar seu funcionamento.

- void *SLPool::Allocate(size_t tamanho)
  Essa função realiza a alocação de memória equivalente a quantidade de bytes solicitados pela varável tamanho. Um laço percorre a lista encadeada de elementos livres em busca de primeira zona livre grande o bastante para conter a zona solicitada, essa estratégia simples é conhecida como First-Fit.
- void SLPool::Free(void *entrada)
  Essa função é responsável por recolocar a zona de memória usada pelo usuário de volta na lista de zonas livre, dversos casos especiasis são testados apenas para garantir que a zona devolvida será recolocada na lista exatamente na posição a qual ela pertencia antes.
- void *SLPoolBest::Allocate(size_t tamanho)
  Essa função se assemelha bastante ao funcionamento da função Allocate da classe SLPool, sua principal diferença é que ao invez de First-Fit como estratégia de alocação de memória ela usa a Best-Fit, nessa estratégia o laço não vai encontrar a primeira zona que caiba a quantidade de bytes solicitada, mas sim vai encontrar a melhor zona onde encaixar esses bytes, isso diminui relativamente a quantidade partes fragmentadas da memória.

#Até agora
Nessa versão todas as funções estão prontas, entretanto é nescesário realizar uma grande bateria de testes para garantir sua eficiencia e sua corretude.
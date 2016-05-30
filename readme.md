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
Dois arquivos .h foram criados, o MemoryPool.h guarda os protótipos e as classes
que de fato alocam memória e gerenciam a mesma, já o arquivo mempool_common.h sobrecarrega
os operadores new e delete para que, assim, o programador não precise se preocupar
com a alocação que será feita da mesma forma que antes. Um arquivo main.cpp está
disponível no diretório src/ ele realiza testes na classe MemoryPool buscando demonstrar seu funcionamento.

- void *SLPool::Allocate(size_t tamanho)
  Essa função realiza a alocação de memória equivalente a quantidade de bytes solicitados pela varável tamanho. Um laço percorre a lista encadeada de elementos livres em busca de primeira zona livre grande o bastante para conter a zona solicitada, essa estratégia simples é conhecida como First-Fit.
- void SLPool::Free(void *entrada)
  Essa função é responsável por recolocar a zona de memória usada pelo usuário de volta na lista de zonas livre, diversos casos especiais são testados apenas para garantir que a zona devolvida será recolocada na lista exatamente na posição a qual ela pertencia antes.
- void *SLPoolBest::Allocate(size_t tamanho)
  Essa função se assemelha bastante ao funcionamento da função Allocate da classe SLPool, sua principal diferença é que ao invés de First-Fit como estratégia de alocação de memória ela usa a Best-Fit, nessa estratégia o laço não vai encontrar a primeira zona que caiba a quantidade de bytes solicitada, mas sim vai encontrar a melhor zona onde encaixar esses bytes, isso diminui relativamente a quantidade partes fragmentadas da memória.

#Main() de testes
Esse projeto include um main() que demonstra o funcionamento da classe SLPool assim como também demonstra sua corretude.
  Buscando demonstrar o funcionamento na prática dessa classe eu desenvolvi uma bateria de testes dividida em três níveis:
- Primeiro Nível
Nesse nível é realizado um teste simples da classe, esse teste mostra apenas seu funcionamento básico. Duas variáveis são alocadas em seguida a primeira variável alocada é liberada, isso acarreta uma fragmentação da memória que é demonstrada através da função mapear que mostra a memória dividida em duas partes, sendo duas zonas livre separadas pela zona ocupada pela segunda variável que ainda não foi liberada. A segunda variável é então liberada e a memória volta ao seu estado vazio.
- Segundo Nível
Esse nível dedica-se a mostrar como o gerenciamento lida com a fragmentação da memória. Através de diversas alocações de objetos de tamanhos diferentes é possível notar a memoria se fragmentando, entretanto, a medida que esses mesmos objetos vão sendo liberados o gerenciador consegue detectar por meio de aritmética de ponteiros que as partes sendo deslocadas são vizinhas umas as outras, assim o gerenciador une as partes vizinhas em uma só diminuindo consideravelmente o estado da fragmentação.
O teste também demonstra a diferença entre a classe SLPool e a SLPoolBest, onde uma usa a estratégia Firs-fit e a outra usa a estratégia best-fit respectivamente. Durante o teste é possível perceber que a estratégia best-fit encontra sempre a melhor zona para encaixar o espaço solicitado, enquanto a first-fit apenas encontra a primeira zona em que esse objeto caiba.
- Terseiro Nível
Esse é o nível onde o gerenciador é testado através de uma massiva quantidade de alocações e desalocações, um laço foi usado para esse teste, esse laço foi programado para rodar durante dois segundos exatos, em cada volta do laço uma alocação ou desalocação é executada no seguinte critério:
  Um vetor de ponteiros com 100 elementos é inicialmente totalmente preenchido com nullptr, a cada volta do laço um índice é gerado aleatória mente através da função std::rand(), esse índice é usado para indicar a posição no vetor onde o elemento alocado será colocado, se aquela posição já estiver alocada então ela será deletada, se possuir nullptr então é alocada, dessa forma um conjunto massivo de alocações e desalocações aleatórias são executadas, esses números são então registrados para mais tarde comparar com o desempenho do sistema operacional.
  Um segundo loop idêntico é executado em seguida, mas, dessa vez o sistema operacional é o responsável por realizar as alocações de desalocações. Por fim um relatório mostra quantas operações de cada tipo cada gerenciador conseguiu executar no mesmo tempo(2 segundos) e o resultado indica quem foi mais rápido.
  Durante todos os testes que realizei o sistema operacional (Ubuntu) se mostrou melhor alocando e desalocando mais memória, quando o Best-Fit é usado essa diferença aumenta, mostrando que o Best-Fit, apesar de ser melhor para manter a memória pouco fragmentada, é mais lento que o First-Fit.

#Como compilar
Uma arquivo makefile está disponível no repositório desse projeto, sendo assim apenas executar o comando 'make' será suficiente para compilar, também foi configurado um target 'clean' que apaga o binário do projéto, basicamente com esse makefile é possível importar facilmente o projeto para uma IDE como netbeans.
  O binário gerado se encontra na pasta bin/ com o nome de GREMLINS, se a pasta bin não estiver presente ela deve ser criada antes de executar a compilação do projeto.

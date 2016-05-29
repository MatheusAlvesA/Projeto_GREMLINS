#Projeto GREMLINS

 - [Matheus Alves de Andrade] (https://github.com/MatheusAlvesA)

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

#Até agora
Nessa versão Alocar e liberar memória estão concluidos, mas não testados.
depois de realizar testes de extres algumas melhorias serão feitas

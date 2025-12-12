## **Melhor Personagem - Algoritmos Genéticos**
### SSC0713 - Sistemas Evolutivos Aplicados à Robótica
---
* **Docente:** Eduardo do Valle Simões
* **Aluno:** Lucas Issao Omati - 13673090
---
##  Introdução
Projeto desenvolvido para a matéria Sistemas Evolutivos Aplicados à Robótica que utiliza algoritmos genéticos para encontrar o melhor personagem de um jogo para enfrentar dado inimigo aleatório.

O algoritmo evolui através de mutações, crossovers e predação que afetam caracterísca(genes) dos personagens que afetam em como eles desempenham sobre o inimigo
---
## Funcionalidades Principais
* **Criar uma nova geração de Personagens
* **Criar um inimigo aleatório
* **Realizar as batalhas
* **Ativar predação
* **Visualização dos dados de cada personagem a cada interação
---
## Funcionamento
**1.** O sistema carrega inicialmente uma geração inicial e um inimigo em que são totalmente aleatórios
**2.** A partir do menu pode-se alterar essa geração inicial
**3.** Ao iniciar as evoluções, ocorrerão batalhar e serão selecionados os que melhores desempenharam

* ** Cada personagem tem um fitness proveniente de seus genes/status base
* ** Ao fim de cada batalha é redifinido um acréscimo ou decréscimo ao fitness dependo do desempenho do personagem
* ** Por fim ocorre uma seleção entre os melhores, um crossover entre 2 pais selecionados de bom desempenho, mutação e se habilitato no menu a predação
* ** Ao final de tudo será gerada uma nova geração onde a tendencia é encontrar um personagem cada vez melhor decorrente de suas características
---
Código feito em c++

Para executar o projeto basta executar o executável.

Link vídeo: 

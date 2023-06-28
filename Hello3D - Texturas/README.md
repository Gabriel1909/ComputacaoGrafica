# **Trabalho Grau B**

## Teclas para transformações:

- Setas do teclado alteram o objeto selecionado para as transformações 

- X, Y e Z fazem a rotação dos objetos
- 1, 2, 3, 4, 5 e 6 fazem a translação dos objetos nas 3 dimensões
- \- e \+ alteram a escala dos objetos
- Espaço pausa as transformações no objeto
- W, A, S e D alteram a posição da camera
- Mouse altera a orientação da camera

## Arquivo de parametrizações:

O arquivo *parametros.json* contém as parametrizações para configurar o inicio da cena

O campo *objetos* define a lista de objetos presentes, com o caminho do arquivo .obj, transformações iniciais e animação com curva paramétrica. Para as curvas, é possível selecionar a quantidade de pontos por segmento, lista de pontos de controle e o tipo de curva baseada nos valores:
- b: *Bezier*
- h: *Hermite*
- c: *CatmullRom*

O campo *iluminacao* define a posição e cor da fonte de luz

O campo *camera* define a posição e orientação da camera, assim como os valores do *frustum*
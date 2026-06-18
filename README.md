# study_exam04
Estudando para o exame 04 da 42


Converter IP de Rede

NÚMERO para "/X":
Pegar o número "mágico" (256) e subtrair pelo número da máscara
agora dobrar o número 1 até virar o resultado da subtração
pegar o limite máximo de bits (32) e subtrair por quantas vezes o 1 foi multiplicado

"/X" para NÚMERO:
Quantos bits faltam para chegar no número "32" (subtração)
pegar o número 1 e dobra X vezes pelo resultado da subtração = Tamanho do bloco
fazer 256 menos o tamanho do bloco = o número da máscara

Descobrir tamanho do bloco:
Cenário A: Você tem o número da máscara (ex: 224)
Descubra o tamanho do bloco: 256 - 224 = 32
Divida 256 pelo tamanho deo bloco: 256 / 32 = 8

Cenário B: Você tem o /X (ex: /30)
Veja quantos bits faltam para 32: 32 - 30 = 2
Dobre o número 1 aquela quantidade de vezes (2 vezes): 1 -> 2 -> 4 (tamanho do bloco).
Divida 256 pelo tamanho do bloco: 256 / 4 = 64

QUERO DIVIDIR EM X VEZES
ex: 4
256 / 4 = 64
256 - 64 = (192)

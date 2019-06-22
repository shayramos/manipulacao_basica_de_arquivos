import string
import random

def generatorRegistros(numero):
    registro = ""
    for _ in range(numero):
        alfabeto = string.ascii_letters + string.digits
        tamanhoChave, tamanhoConteudo = 20, 50
        chave = random.choice(string.ascii_letters) + "".join(random.choice(alfabeto) for _ in range(random.choice(range(tamanhoChave - 1))))
        conteudo = random.choice(string.ascii_letters) + "".join(random.choice(alfabeto) for _ in range(random.choice(range(tamanhoConteudo - 1))))
        registro = ''.join((registro, ','.join((chave, conteudo,)),))
        yield registro
        registro = '\n'

def main():
    numero = 10000
    with open("base.txt", "w") as file:
        regs = generatorRegistros(numero)
        for registro in regs:
            file.write(registro)


if __name__ == "__main__":
    main()
# Ponderada_CCP
 
# 🔌 Comunicação com Sockets em C++

No meu projeto implementa a comunicação entre dois processos utilizando **sockets TCP em C++**. Um cliente lê o conteúdo de um arquivo e o envia para o servidor, que o exibe ao receber.

---

## 📋 Estrutura do Projeto

```
.
├── client.cpp     # Cliente: lê um arquivo e envia o conteúdo via socket
├── server.cpp     # Servidor: escuta, aceita conexões e imprime os dados recebidos
├── LL.txt   
└── README.md
```

---

## ⚙️ Como Executar

### 1. Compile os programas

```bash
g++ -o servidor server.cpp
g++ -o cliente client.cpp
```

### 2. Execute o servidor

```bash
./servidor
```

> O servidor ficará escutando na porta `7878`.

### 3. Execute o cliente, enviando um arquivo

```bash
./cliente 127.0.0.1 7878 arquivo.txt
```

> O cliente se conecta ao servidor e envia o conteúdo do `arquivo.txt`.

---

## ✉️ Exemplo de Funcionamento

### Conteúdo do arquivo `arquivo.txt`:

```
Palmeiras Primeiro Campeão Mundial - 1951 🏆
```

### Saída do servidor:

```
Servidor escutando em 127.0.0.1:7878
Cliente conectado.
Conteúdo recebido:
Palmeiras Primeiro Campeão Mundial - 1951 🏆
```

![verde](./assets/image.png)

---

## 🔧 Detalhes Técnicos

### Cliente:
- Lê o arquivo como binário
- Conecta via socket TCP
- Envia o conteúdo pelo socket

### Servidor:
- Cria socket TCP na porta 7878
- Aceita conexões
- Lê dados do socket até o fim
- Exibe o conteúdo recebido

---

## 🎥 Demonstração em Vídeo

📹 [Link para o vídeo ](https://youtu.be/seu-link-aqui)



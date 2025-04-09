#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <unistd.h>     
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <cstring>      

class ConnectionFactory {
public:
    virtual int create_connection(const std::string &address, int port) = 0;
    virtual ~ConnectionFactory() {}
};

class TcpConnectionFactory : public ConnectionFactory {
public:
    int create_connection(const std::string &address, int port) override {
        int sock_fd = socket(AF_INET, SOCK_STREAM, 0);
        if (sock_fd == -1) {
            std::cerr << "Erro ao criar socket." << std::endl;
            return -1;
        }
        sockaddr_in server_address;
        server_address.sin_family = AF_INET;
        server_address.sin_port = htons(port);
        if (inet_pton(AF_INET, address.c_str(), &server_address.sin_addr) <= 0) {
            std::cerr << "Endereço inválido." << std::endl;
            close(sock_fd);
            return -1;
        }
        if (connect(sock_fd, (struct sockaddr *)&server_address, sizeof(server_address)) < 0) {
            std::cerr << "Falha na conexão." << std::endl;
            close(sock_fd);
            return -1;
        }
        return sock_fd;
    }
};

class Client {
public:
    Client(const std::string &server_address, int port, const std::string &file_path, ConnectionFactory *factory)
        : server_address_(server_address), port_(port), file_path_(file_path), factory_(factory) {}

    bool send_file() {
        std::ifstream file(file_path_, std::ios::in | std::ios::binary);
        if (!file) {
            std::cerr << "Erro ao abrir o arquivo: " << file_path_ << std::endl;
            return false;
        }
        std::stringstream buffer;
        buffer << file.rdbuf();
        std::string content = buffer.str();
        file.close();

        int sock_fd = factory_->create_connection(server_address_, port_);
        if (sock_fd < 0) {
            std::cerr << "Não foi possível criar a conexão." << std::endl;
            return false;
        }

        ssize_t sent = send(sock_fd, content.c_str(), content.size(), 0);
        if (sent < 0) {
            std::cerr << "Erro ao enviar dados." << std::endl;
            close(sock_fd);
            return false;
        }
        std::cout << "Conteúdo do arquivo enviado com sucesso." << std::endl;
        close(sock_fd);
        return true;
    }

private:
    std::string server_address_;
    int port_;
    std::string file_path_;
    ConnectionFactory *factory_;
};

int main(int argc, char *argv[]) {
    if (argc != 4) {
        std::cerr << "Uso: " << argv[0] << " <endereço_servidor> <porta> <arquivo>" << std::endl;
        return 1;
    }
    std::string server_address = argv[1];
    int port = std::stoi(argv[2]);
    std::string file_path = argv[3];

    TcpConnectionFactory factory;
    Client client(server_address, port, file_path, &factory);
    if (!client.send_file()) {
        return 1;
    }
    return 0;
}

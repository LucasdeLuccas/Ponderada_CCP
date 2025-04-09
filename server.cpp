#include <iostream>
#include <cstring>   // memset
#include <unistd.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

class MessageHandler {
public:
    virtual void handle_message(const std::string &message) = 0;
    virtual ~MessageHandler() {}
};

class PrintMessageHandler : public MessageHandler {
public:
    void handle_message(const std::string &message) override {
        std::cout << "Conteúdo recebido:\n" << message << std::endl;
    }
};

class Server {
public:
    Server(const std::string &address, int port, MessageHandler *handler)
        : address_(address), port_(port), handler_(handler) {}

    bool run() {
        int server_fd = socket(AF_INET, SOCK_STREAM, 0);
        if (server_fd == -1) {
            std::cerr << "Erro ao criar socket." << std::endl;
            return false;
        }

        int opt = 1;
        if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
            std::cerr << "Erro em setsockopt." << std::endl;
            close(server_fd);
            return false;
        }

        sockaddr_in address;
        address.sin_family = AF_INET;
        address.sin_addr.s_addr = inet_addr(address_.c_str());
        address.sin_port = htons(port_);
        if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
            std::cerr << "Erro ao bindar o socket." << std::endl;
            close(server_fd);
            return false;
        }

        if (listen(server_fd, 3) < 0) {
            std::cerr << "Erro ao escutar conexões." << std::endl;
            close(server_fd);
            return false;
        }

        std::cout << "Servidor escutando em " << address_ << ":" << port_ << std::endl;

        while (true) {
            sockaddr_in client_address;
            socklen_t client_addrlen = sizeof(client_address);
            int client_fd = accept(server_fd, (struct sockaddr *)&client_address, &client_addrlen);
            if (client_fd < 0) {
                std::cerr << "Erro ao aceitar conexão." << std::endl;
                continue;
            }
            std::cout << "Cliente conectado." << std::endl;

            std::string data;
            char buffer[1024];
            ssize_t bytes_read;
            while ((bytes_read = read(client_fd, buffer, sizeof(buffer))) > 0) {
                data.append(buffer, bytes_read);
            }

            handler_->handle_message(data);

            close(client_fd);
        }

        close(server_fd);
        return true;
    }

private:
    std::string address_;
    int port_;
    MessageHandler *handler_; 
};

int main() {
    PrintMessageHandler handler;
    Server server("127.0.0.1", 7878, &handler);
    server.run();
    return 0;
}

#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <string.h>

void error(const char *msg)
{
    perror(msg);
    exit(-1);
}

int main(int argc, char *argv[])
{
    // проверка вводимого порта
    if (argc < 2)
    {
        std::cout << "No port provided." << std::endl;
        exit(-1);
    }

    int port = atoi(argv[1]);

    // дескриптор сокета
    int sockfd = socket(        
        AF_INET,        // домен и семейство адресов
        SOCK_STREAM,    // тип сокета
        0               // протокол (по умолчанию)
    );

    if (sockfd == -1){
        error("ERROR on opening socket.");
    }

    // структура описания адреса сокета
    struct sockaddr_in serv_addr;
    bzero(&serv_addr, sizeof(serv_addr));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    serv_addr.sin_port = htons(port);
    
    // установление соединения клиента с сервером
    if (connect(sockfd, (struct sockaddr*) &serv_addr, sizeof(serv_addr)) == -1){
        error("ERROR on connecting.");
    };
    
    char buffer[256];

    int msg_to, msg_from;
    
    while (true)
    {
    
        std::cout << "Enter a message: ";

        bzero(buffer, sizeof(buffer));
        fgets(buffer, sizeof(buffer), stdin);

        // отправляем на сервер сообщение
        msg_to = send(
            sockfd,
            &buffer,
            sizeof(buffer),
            0
        );

        if (msg_to == -1)
        {
            error("ERROR on writing to socket.");
        }

        // получаем сообщение с сервера
        msg_from = recv(
            sockfd,
            &buffer,
            sizeof(buffer),
            0
        );

        if (msg_from == -1)
        {
            error("ERROR on reading from socket.");
        };
    
        std::cout << "Message from server: " + std::string(buffer) << std::endl;

        if (strncmp("Server exit...", buffer, 15) == 0){

            std::cout << "Connection is being closed..." << std::endl;
            break;
        }

    }

    // закрытие сокета
    close(sockfd);

    return 0;
}
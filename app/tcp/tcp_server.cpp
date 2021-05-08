#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netdb.h>
#include <string.h>
#include <stdlib.h>

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
        std::cout << "No port provided" << std::endl;
        exit(-1);
    }

    int port = atoi(argv[1]); // номер порта

    int sockfd, // серверный сокет
        connfd; // дескриптор клиента

    // дескриптор сокета
    sockfd = socket(        
        AF_INET,        // домен и семейство адресов
        SOCK_STREAM,    // тип сокета
        0               // протокол (по умолчанию)
    );

    if (sockfd == -1){
        error("ERROR on opening socket");
    }

    // структура описания адреса сокета
    struct sockaddr_in serv_addr;
    bzero((char*)&serv_addr, sizeof(serv_addr));
    
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(port);

    // связывание сокета с адресом и портом
    if (bind(sockfd, (struct sockaddr*) &serv_addr, sizeof(serv_addr)) == -1){
        error("ERROR on binding");
    }
    
    // перевод сокета в режим ожидания
    if (listen(sockfd, 5) == -1){
        error("ERROR on listen");
    }

    // описание клиентского соединения
    struct sockaddr_in cli_addr;
    socklen_t cli_len = sizeof(cli_addr);

    char buffer[256];

    int msg_from; // полученное сообщение от клиента

    std::cout << "Waiting for a client...\n" << std::endl;

    // готовность принять запрос
    connfd = accept(
        sockfd, // дескриптор слушающего сокета
        (sockaddr*) &cli_addr, // адрес сокета клиента
        &cli_len // длина адреса клиента
    );

    if (connfd == -1)
    {
        error("ERROR on accept request");
    }

    while (true)
    {

        bzero(buffer, sizeof(buffer)); // обнуляем буфер

        // получение данных от клиента (чтение данных из сокета)
        msg_from = recv(
            connfd,         // дескриптор клиентского соединения
            buffer,         // указатель на буфер
            sizeof(buffer), // длина буфера данных
            0               // набор битовых флагов
        );

        if (msg_from == -1)
        {
            error("ERROR on recieve message");
        }

        std::cout << "Message from client: " + std::string(buffer) << std::endl; 

        if (strncmp("exit", buffer, 4) == 0){
            std::string exit_msg = "Server exit...";
            std::cout << exit_msg << std::endl;

            if (send(connfd, exit_msg.c_str(), sizeof(exit_msg), 0) == -1){
                error("ERROR on sending message");
            }
            break;
        }

        // отправка данных
        if (send(
            connfd,                 // дескриптор сокета, через который отправляются данные
            "message received.\n",  // указатель на буфер
            19,                     // длина буфера данных
            0                       // набор битовых флагов
        ) == -1) error("ERROR on sending message");
        
    }
    
    // закрытие клиентского сокета
    close(connfd);

    // закрытие серверного сокета
    close(sockfd);

    return 0;
}
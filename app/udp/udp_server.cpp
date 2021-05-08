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

    // дескриптор сокета
    int sockfd = socket(        
        AF_INET,        // домен и семейство адресов
        SOCK_DGRAM,     // тип сокета
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

    char buffer[256];

    int msg_from; // полученное сообщение от клиента

    // описание клиентского соединения
    struct sockaddr_in cli_addr;
    bzero((char*)&cli_addr, sizeof(cli_addr));
    socklen_t cli_len = sizeof(cli_addr);

    std::cout << "Waiting for a client...\n" << std::endl;

    while (true)
    {
        bzero(buffer, sizeof(buffer));

        // получение данных от клиента (чтение данных из сокета)
        msg_from = recvfrom(
            sockfd,         
            buffer,         
            sizeof(buffer), 
            0,
            (struct sockaddr*)&cli_addr,
            &cli_len              
        );

        if (msg_from == -1)
        {
            error("ERROR on recieve message");
        }

        std::cout << "Message from client: " + std::string(buffer) << std::endl; 
	
	    if (strncmp("exit", buffer, 4) == 0){
            std::string exit_msg = "Server exit...";
            std::cout << exit_msg << std::endl;

            if (sendto(sockfd, exit_msg.c_str(), sizeof(exit_msg), 0, (const struct sockaddr*)&cli_addr, cli_len) == -1){
                error("ERROR on sending message");
            }
            break;
        }
	
        // отправка данных
        if (sendto(
            sockfd, 
            "message received.\n", 
            19, 
            0,
            (const struct sockaddr*)&cli_addr,
            cli_len              
        ) == -1) error("ERROR on send message");
        
    }

    // закрытие сокета
    close(sockfd);

    return 0;
}

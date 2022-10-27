#include <chrono>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <errno.h>
#include <iostream>

#define SIZE 1024

//! Edited by Skynet Cyberdyne 2022.
//! Ported to c++

int send_text(int socket)
{
    int n;
    char buffer[SIZE] = "Test sending from Client!\n";

    n = send(socket, buffer, sizeof(buffer), 0);
    if (n <= 0){
        perror("[-]Error in sending text.");
        return -1;
    }

    bzero(buffer, sizeof(buffer));
    return 0;
}

int receive_text(int socket)
{
    int n;
    char buffer[SIZE];

    n = recv(socket, buffer, SIZE, 0);
    if (n <= 0)
    {
        return -1;
    }
    std::cout<<"Received Text:"<<buffer<<std::endl;

    bzero(buffer, SIZE);
    return 0;
}

void send_image(int socket)
{
    FILE *picture;
    int size, read_size, stat, packet_index;
    char send_buffer[10240], read_buffer[256];
    packet_index = 1;

    picture = fopen("picture.jpeg", "r");
    std::cout<<"Getting Picture Size"<<std::endl;

    if(picture == NULL)
    {
        std::cout<<"Error Opening Image File"<<std::endl;
    }

    fseek(picture, 0, SEEK_END);
    size = ftell(picture);
    fseek(picture, 0, SEEK_SET);
    std::cout<<"Total Picture size:"<<size<<std::endl;

    //! Send Picture Size
    std::cout<<("Sending Picture Size\n");
    send(socket, (void *)&size, sizeof(int), 0);

    //! Send Picture as Byte Array
    std::cout<<("Sending Picture as Byte Array\n");

    do { //! Read while we get errors that are due to signals.
        stat = recv(socket, &read_buffer , 255, 0);
        std::cout<<"Bytes read:"<<stat<<std::endl;
    } while (stat < 0);

    std::cout<<"Received data in socket"<<std::endl;
    std::cout<<"Socket data:"<<read_buffer<<std::endl;

    while(!feof(picture))
    {
        //while(packet_index = 1){
        //! Read from the file into our send buffer
        read_size = fread(send_buffer, 1, sizeof(send_buffer)-1, picture);

        //! Send data through our socket
        do{
            stat = send(socket, send_buffer, read_size, 0);
        }while (stat < 0);

        std::cout<<"Packet Number:"<<packet_index<<std::endl;
        std::cout<<"Packet Size Sent"<<read_size<<std::endl;
        std::cout<<std::endl;
        std::cout<<std::endl;

        packet_index++;

        //! Zero out our send buffer
        bzero(send_buffer, sizeof(send_buffer));
    }
    fclose(picture);
}

int receive_image(int socket)
{ //! Start function

    int recv_size = 0,size = 0, read_size, write_size, packet_index =1,stat;

    char imagearray[10241];
    FILE *image;

    //! Find the size of the image
    do{ stat = recv(socket, &size, sizeof(int), 0);
    } while(stat<0);

    std::cout<<"Packet received."<<std::endl;
    std::cout<<"Packet size:"<<stat<<std::endl;
    std::cout<<"Image size:"<<size<<std::endl;
    std::cout<<std::endl;

    char buffer[] = "Got it";

    //! Send our verification signal
    do{ stat = write(socket, &buffer, sizeof(int));
    } while(stat<0);

    std::cout<<"Reply sent"<<std::endl;
    std::cout<<std::endl;

    image = fopen("recieved.jpeg", "w");

    if(image == NULL) {
        printf("Error has occurred. Image file could not be opened\n");
        return -1; }

    //! Loop while we have not received the entire file yet

    struct timeval timeout = {10,0};

    fd_set fds;
    int buffer_fd;

    while(recv_size < size) {
        //! while(packet_index < 2){

        FD_ZERO(&fds);
        FD_SET(socket, &fds);

        buffer_fd = select(FD_SETSIZE,&fds,NULL,NULL,&timeout);

        if(buffer_fd < 0){
            std::cout<<"error: bad file descriptor set."<<std::endl;
        }

        if(buffer_fd == 0){
            std::cout<<"error: buffer read timeout expired."<<std::endl;
        }

        if(buffer_fd > 0){
            do{ read_size = recv(socket,imagearray, 10241, 0);
            } while(read_size <0);

            std::cout<<"Packet number received:"<<packet_index<<std::endl;
            std::cout<<"Packet size:"<<read_size<<std::endl;

            //! Write the currently read data into our image file
            write_size = fwrite(imagearray,1,read_size, image);
            std::cout<<"Written image size:"<<write_size<<std::endl;

            if(read_size !=write_size)
            {
                printf("error in read write\n");
            }

            //Increment the total number of bytes read
            recv_size += read_size;
            packet_index++;
            std::cout<<"Total received image size:"<<recv_size<<std::endl;
            std::cout<<std::endl;
            std::cout<<std::endl;
        }
    }

    fclose(image);
    std::cout<<"Image successfully Received!"<<std::endl;
    return 1;
}

int socket_init_client()
{
    int socket_desc;
    struct sockaddr_in server;

    //! Create socket
    socket_desc = socket(AF_INET , SOCK_STREAM , 0);

    if (socket_desc == -1)
    {
        std::cout<<"Could not create socket"<<std::endl;
    }

    memset(&server, 0,sizeof(server));
    server.sin_addr.s_addr = inet_addr("127.0.0.1");
    server.sin_family = AF_INET;
    server.sin_port = htons( 8889 );

    //! Connect to remote server
    if (connect(socket_desc , (struct sockaddr *)&server , sizeof(server)) < 0)
    {
        //! cout<<strerror(errno);
        close(socket_desc);
        puts("Connect Error");
        return 1;
    }

    puts("Connected\n");

    std::cout<<"Debug: socket_desc:"<<socket_desc<<std::endl;
    return socket_desc;
}

void delay(int number_of_seconds)
{
    //! Converting time into milli_seconds
    int milli_seconds = 1000 * number_of_seconds;

    //! Storing start time
    clock_t start_time = clock();

    //! looping till required time is not achieved
    while (clock() < start_time + milli_seconds);
}

int main(int argc , char *argv[])
{
    argc=0;
    argv=0;

    int socket_desc_main;

    socket_desc_main = socket_init_client();
    if (socket_desc_main == 1)
    {
        std::cout<<"Could not init socket client!"<<std::endl;
        return 1;
    }

    // receive_text(socket_desc_main);
    // receive_text(socket_desc_main);
    // send_text(socket_desc_main);

    //! Client is in endless loop sending images.
    //! The server will stop after received 1000 images.
    while(1){
        send_image(socket_desc_main);
        receive_text(socket_desc_main);
    }

    close(socket_desc_main);
    std::cout<<"Client Socket Closed!!!"<<std::endl;

    //! This part can be used to recieve images.
    /*
    delay(100);

    socket_desc_main = socket_init_client();
    if (socket_desc_main == 1)
    {
        printf("Could not init socket client!\n");
        return 1;
    }
    receive_image(socket_desc_main);
    close(socket_desc_main);
    printf("Client Socket Closed!!!\n");
    */

    return 0;
}

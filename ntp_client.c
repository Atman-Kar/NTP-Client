#include<stdio.h>
#include<stdlib.h>
#include<stdint.h>
#include<string.h>
#include<sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include<time.h>


#define NTP_DELTA 2208988800ull

typedef struct {

    uint8_t li_vn_mode; // li, vn and mode combined into one 8 bit number

    uint8_t stratum;
    uint8_t poll;
    uint8_t precision;


    uint32_t root_delay;
    uint32_t root_dispersion;
    uint32_t refId;


    uint32_t refTs_s;
    uint32_t refTs_fs;


    uint32_t orgTs_s;
    uint32_t orgTs_fs;


    uint32_t rxTs_s;
    uint32_t rxTs_fs;


    uint32_t txTs_s;
    uint32_t txTs_fs;

} ntp_packet;

void error(char * err);

/* 
NTP Message consists of 48 bytes of data with 17 fields.
define a structure as NTP packet.

Diagram of NTP Packet can be found in the README of this project.


Fields:

li : Leap indicator - Indicates whether last minute of the day needs a leap second to be added.
     Values can range from 0-3.

vn : Version Number - NTP version number (currently 4)

mode : NTP Packet Mode (Client mode in our case)

*/
int main(){

    int port_no = 123; // NTP works on UDP port 123
    char * hostname = "in.pool.ntp.org"; // NTP Indian server


    ntp_packet packet;

    memset( (char *) &packet, 0 , sizeof(ntp_packet));

    /*  
      Set 
      li = 0 (no leap second)
      vn = 4 (current version number)
      mode = 3 (client)

      li_vn_mode = 00100011 = 0x23 (Hex)
   */

    *( (char * ) &packet + 0) = 0x23; 
    
    struct sockaddr_in address;
    struct hostent * server;


    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    if(sockfd <= 0)
        error("Error: opening socket");


    server = gethostbyname(hostname);

    if(server == NULL)
        error("Error: No such host");


    memset( (char *) &address, 0, sizeof(address)); // zero out the contents of address

    // set some values of address
    address.sin_family = AF_INET;
    address.sin_port = htons( port_no );

    bcopy( (char *) server->h_addr, (char *) &address.sin_addr.s_addr, server->h_length);

    // try to connect to the server

    if(connect(sockfd , (struct sockaddr *) &address , sizeof(address)) < 0)
        error("Error: Cannot connect to server");
    else
        printf("Connection successful!\n");


    // once connection is successful, let us write the NTP packet to the server

    if(write(sockfd, (char *) &packet, sizeof(ntp_packet) ) < 0)
        error("Error: Cannot write data to socket");
    else
        printf("Write successful!\n");

    sleep(2); // sleep between write and read

    // once write is successful
    if(read(sockfd, (char *) &packet, sizeof(ntp_packet) ) < 0)
        error("Error: Cannot write data to socket");
    else
        printf("Read successful!\n");


    packet.txTs_s = ntohl( packet.txTs_s );
    packet.txTs_fs = ntohl( packet.txTs_fs );

    time_t txTm = (time_t) (packet.txTs_s - NTP_DELTA);

    printf( "Time: %s", ctime( ( const time_t* ) &txTm ) );
    
    

   
    return 0;
}


void error(char * err){
    perror(err);
    exit(0);
}

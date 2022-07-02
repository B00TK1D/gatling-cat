/*
 *  Print ICMP packets that a raw socket receives on MacOS. 
 *
 *  Note: this does NOT work for echo, timestamps, mask requests!
 *    http://stackoverflow.com/questions/6878603/strange-raw-socket-on-mac-os-x
 *  
 *  This will NOT work for any TCP or UDP either. From http://sock-raw.org/papers/sock_raw:

FreeBSD takes another approach. It *never* passes TCP or UDP packets to raw
sockets. Such packets need to be read directly at the datalink layer by using
libraries like libpcap or the bpf API. It also *never* passes any fragmented 
datagram. Each datagram has to be completeley reassembled before it is passed
to a raw socket.
FreeBSD passes to a raw socket:
        a) every IP datagram with a protocol field that is not registered in
        the kernel
        b) all IGMP packets after kernel finishes processing them
        c) all ICMP packets (except echo request, timestamp request and address
        mask request) after kernel finishes processes them

Note that if your computer is *sending* echo requests, then the incoming echo _replies_
will show on the raw socket. Echo requests from other machines would not, as per above,
although these machines will see echo replies from yours.

 */

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#include <netinet/ip.h>
#include <netinet/ip_icmp.h>

#include <arpa/inet.h>  // inet_ntoa

int main(){
     int sockfd,retval,n;
     socklen_t clilen;
     struct sockaddr_in cliaddr, servaddr;
     char buf[10000]; 
     int i;
     
     // 0 serves as a catch-all for any protocol, including ICMP and those
     //  not handled (gets, e.g., protocol 10)
     // This will allegedly not work on Linux. 
     sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP); // 0 for any proto
     if (sockfd < 0){
          perror("sock:");
          exit(1);
     }
     clilen = sizeof(struct sockaddr_in);    
     while(1){
          printf(" before recvfrom\n");   
          n = recvfrom(sockfd, buf, 10000, 0, (struct sockaddr *)&cliaddr, &clilen);
          printf(" rec'd %d bytes\n",n);
          
          struct ip *ip_hdr = (struct ip *)buf;
          
          printf("IP header is %d bytes.\n", ip_hdr->ip_hl*4);
          
          for (i = 0; i < n; i++) {
               printf("%02X%s", (uint8_t)buf[i], (i + 1)%16 ? " " : "\n");
          }
          printf("\n");
          
          struct icmp *icmp_hdr = (struct icmp *)((char *)ip_hdr + (4 * ip_hdr->ip_hl));

          printf("ICMP msgtype=%d, code=%d ", icmp_hdr->icmp_type, icmp_hdr->icmp_code);
          printf("%s -> ", inet_ntoa(ip_hdr->ip_src));  // Doing this print in one go may surprise you:
          printf("%s\n",   inet_ntoa(ip_hdr->ip_dst));  //  remember that ntoa's return buffer is reused!

          //int ret = sendto(sockfd, ip_hdr, sizeof(struct ip), 0, (struct sockaddr *)&cliaddr, clilen);

          //int ret = send(sockfd, ip_hdr, sizeof(struct ip), 0);


          printf("Returned %d\n", ret);
     }
}
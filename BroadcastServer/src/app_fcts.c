#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <net/if.h>
#include <arpa/inet.h>
#include <netdb.h>

#include "app_fcts.h"

/* read from file filename into array array[]
   return number of elements in array */
int read_file(char *filename, char *array[])
{
  /*  array == char * [MAX_FILE_LINES] */
  struct stat statbuf;
  FILE *fp;
  char buf[MAX_LINE_CHARS];

  /* no. of strings in array */
  int n = 0;

  /* newline found flag */
  int is_0a = 0; 
  int i;

  if (stat(filename, &statbuf) == -1) {
    perror("stat error\n");
    return(-1);
  }
  if (statbuf.st_size < 7) {
    printf("file size too small\n");
    return(-2);
  }
  if (!(S_ISREG(statbuf.st_mode))) {
    printf("%s is not a regular file!\n", filename);
    return(-3);
  }
  if (!(statbuf.st_mode & S_IRUSR)) {
    printf("check file owner read permission\n");
    return(-4);
  }
  if ((fp = fopen(filename, "r")) == NULL) {
    perror("file open error");
    return(-5);
  }
#ifdef DEBUG
    else {
    printf("opened %s\n", filename);
  }
#endif

  /* memory should be zeroed to avoid strlen(array[0]) returning garbage */
  for (i = 0; i < MAX_FILE_LINES; i++)
    if ((array[i] = calloc(MAX_LINE_CHARS, sizeof(char))) == NULL) {
      printf("not enough heap memory");
      return(-6);
    }

  while (!feof(fp)) {
    fgets(buf, MAX_LINE_CHARS, fp);
    if (buf[0] == '\n') {  
      is_0a = 1;
    } else {
      if (is_0a) {
	/* found newline */
	n++;
	is_0a = 0;
	memcpy(array[n], buf, strlen(buf) + 1);
      } else {
	/* line is appended to crt. string if possible */
	i = strlen(array[n]);
	if (i == MAX_LINE_CHARS) {
	  printf("subtitle %d error: too many chars\n", n);
	} else {
	  if ((i + strlen(buf) + 1) > MAX_LINE_CHARS)
	    printf("subtitle %d error: 2nd line is too long\n", n);
	  else
	    memcpy(array[n] + i, buf, strlen(buf) + 1);
	}
      }
    }
  }
  fclose(fp);

  return(n+1);
}

/* free previously allocated memory for the n-lines array */
void free_array(char *array[], int n)
{
  int i;

  for(i = 0; i < n; i++)
    free(array[i]);
}


/* set up UDP socket for sending broadcast messages
   return socket descriptor, negative value on error */
int make_socket()
{
    int sockfd;
    int broadcast = 1;
    struct ifreq ifr;
    //char broadcast = '1'; // if int doesn't work, try this

    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
        perror("socket creation error");
        exit(-1);
    }
    snprintf(ifr.ifr_name,sizeof(ifr.ifr_name),"eth0");
    /* allow broadcast packets to be sent */
    if (setsockopt(sockfd, SOL_SOCKET, SO_BROADCAST, &broadcast,sizeof (broadcast)) == -1) {
    //if (setsockopt(sockfd, SOL_SOCKET,SO_BINDTODEVICE,(void *)&ifr,sizeof(ifr))<0){
        perror("can't set up socket for broadcast or Bind to eth0");
        exit(-1);
    }
    return(sockfd);
}

/* send UDP messages using host name and previously set-up socket
   return number of bytes sent, negative value on error */
int send_msg(char *hostname, int sockfd, char *msg)
{
  struct hostent *hent;
  struct sockaddr_in connector; // connector's address information
  int no_bytes;
  
  /*check valid socket descriptor */
  if (sockfd < 0) {
    printf("invalid socket");
    return(-1);
  }
  
  /* get the host info */
  if ((hent = gethostbyname(hostname)) == NULL) { 
    perror("can't find host name");
    return(-2);
  }
  
  connector.sin_family = AF_INET;
  connector.sin_port = htons(SERVERPORT);
  connector.sin_addr = *((struct in_addr *)hent->h_addr);
  memset(connector.sin_zero, '\0', sizeof connector.sin_zero);
  
  if ((no_bytes=sendto(sockfd, msg, strlen(msg), 0, (struct sockaddr *) &connector, sizeof connector)) == -1) {
    perror("sendto failed");
    return(-3);
  } 
#ifdef DEBUG
  else {
    printf("sent %d bytes to %s\n", numbytes,
	   inet_ntoa(their_addr.sin_addr));
  }
#endif
  return(no_bytes);
}

/* close socket given by socket descriptor
   return 0 on success, negative value on error*/
int close_socket(int sockfd)
{
  if (close(sockfd) != 0) {
    perror("error closing socket");
    return(-1);
  }
  return(0);
}


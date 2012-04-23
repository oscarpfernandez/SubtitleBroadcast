#ifdef __cplusplus
extern "C" {
#endif

#ifndef APP_FCTS_H
#define APP_FCTS_H

#define MAX_FILE_NAME 128  
#define MAX_FILE_LINES 1600
#define MAX_LINE_CHARS 25002
#define SERVERPORT 4950    // the port users will connect to
#define CLIENTIP "192.168.0.255"
  
  /* read lines from "formatted" file and store them in an array 
     params: filename = file name
     array = array to store file lines in
     return: no. of strings in array on success, negative value on error 
  */
  int read_file(char *filename, char *array[]);
  void free_array(char *array[], int n);
  int make_socket();
  int send_msg(char *hostname, int socket, char *msg);
  int close_socket(int socket);
#endif
  
#ifdef __cplusplus
}
#endif

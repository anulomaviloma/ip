#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

struct ip_addr
{
  unsigned char ip[4];
  unsigned short port;
  struct ip_addr *next;
};

void 
append_address(struct ip_addr **head, int ip[4], unsigned short port)
{
  struct ip_addr *curr = (struct ip_addr *)malloc(sizeof(struct ip_addr));

  int i;
  for (i = 0; i < 4; i++)
    curr->ip[i] = ip[i];

  curr->port = port;
  curr->next = *head;
  *head = curr;
}

void
print_address(struct ip_addr *head)
{
  struct ip_addr *curr = head;
  while (curr != NULL)
  {
    printf("IPv4 address: %d.%d.%d.%d:%d\n", 
	   curr->ip[0], curr->ip[1], curr->ip[2], curr->ip[3],  
           curr->port);

    curr = curr->next;
  }
}

int 
is_valid_ip(int ip_address[4])
{
  return ((ip_address[0] >  0 && ip_address[0] <= 255) &&
   	  (ip_address[1] >= 0 && ip_address[1] <= 255) &&
          (ip_address[2] >= 0 && ip_address[2] <= 255) &&
          (ip_address[3] >  0 && ip_address[3] <= 255)); 
}

int
read_number(const char *number_string, int *number)
{
  int nchar = 0;

  *number = 0;
  while (*number_string >= '0' && *number_string <= '9')
  {
    *number = *number * 10 + (*number_string - '0');
    number_string++;
    nchar++;
  }
  return nchar;
}

void 
convert_to_number(const char *address)
{
  struct ip_addr *addr = NULL;
  int temp_addr[4] = {-1, -1, -1, -1};
  int counter = 0;
  int is_port = 0, port = -1;
  char prev = *address;

  while (*address)
  {
    switch (*address)
    {
      case '.': 
        if (prev == ',' || prev == '.' || prev == ':')
        {
          printf("Invalid address (.)\n");
          return;
        }
        counter++; 
        break;
      case ',': 
        if (prev == ',' || prev == '.' || prev == ':')
        {
          printf("Invalid address (,)\n");
          return;
        }

        if (is_valid_ip(temp_addr) && port > 0)
        {
	  int i;
          
          append_address(&addr, temp_addr, port);
          for (i = 0; i < 4; i++)
            temp_addr[i] = -1;
          port = -1;
          counter = 0;	  
          is_port = 0;
        }
        break;
     case '0':
     case '1':
     case '2':
     case '3':
     case '4':
     case '5':
     case '6':
     case '7':
     case '8':
     case '9':
     {
       int nchar;
       if (is_port)
         nchar = read_number(address, &port);
       else
         nchar = read_number(address, &temp_addr[counter]);
       address += nchar - 1;
       printf("%d\n", temp_addr[counter]);
       break;
     }
    case ':': 
      if (prev == '.' || prev == ',' || prev == ':' || is_port == 1)
      {
        printf("Invalid port number\n");
        return;
      }
      is_port = 1;
      break;
    default: 
      printf("Invalid characters detected\n");
      return;
    }   
    prev = *address;
    address++;
  }  

  if (is_valid_ip(temp_addr) && port > 0)
    append_address(&addr, temp_addr, port);

  print_address(addr);
}


int 
main(int argc, char *argv[])
{
  int opt;

  while ((opt = getopt(argc, argv, "a:")) != -1)
  {
    switch (opt)
    {
      case 'a': convert_to_number(optarg); break;
      default: printf("Usage: ip_addr -a <address range>\n"); 
    }
  }
}

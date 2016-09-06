#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

struct ip_addr
{
  unsigned char ip[4];
  struct ip_addr *next;
};

void 
append_address(struct ip_addr **head, int ip[4])
{
  struct ip_addr *curr = (struct ip_addr *)malloc(sizeof(struct ip_addr));

  int i;
  for (i = 0; i < 4; i++)
    curr->ip[i] = ip[i];
  curr->next = *head;
  *head = curr;
}

void
print_address(struct ip_addr *head)
{
  struct ip_addr *curr = head;
  while (curr != NULL)
  {
    printf("IPv4 address: %d.%d.%d.%d\n", 
	   curr->ip[0], curr->ip[1], curr->ip[2], curr->ip[3]);
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
void 
convert_to_number(const char *address)
{
  struct ip_addr *addr = NULL;
  int temp_addr[4] = {-1, -1, -1, -1};
  int counter = 0;
  
  while (*address)
  {
    if (*address == '.')
      counter++;
    else if (*address == ',')
    {
      if (is_valid_ip(temp_addr))
      {
        int i;
        append_address(&addr, temp_addr);
        for (i = 0; i < 4; i++)
          temp_addr[i] = -1;
        counter = 0;
      }
      else 
      {
        printf("Invalid IP address\n");
        return;
      }
    }
    else if (*address >= '0' && *address <= '9')
    {
      if (temp_addr[counter] == -1)
        temp_addr[counter] = *address - '0';
      else temp_addr[counter] = temp_addr[counter] * 10 + (*address - '0');
    }
    address++;
  }

  if (is_valid_ip(temp_addr))
    append_address(&addr, temp_addr);
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

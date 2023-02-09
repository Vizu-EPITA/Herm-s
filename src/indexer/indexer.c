#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//void parser(the document, the link list);
//void lexer(document, start, the forward index);
struct list 
{
	struct list *next;
	char* data;
};

struct list *new_list()
{
	struct list *head = NULL; 
	head = malloc(sizeof(struct list));
	head->next = NULL;
	head->data = NULL;
	return head;
}

struct list *new_element(char *data)
{
	struct list *head = NULL;
	head = malloc(sizeof(struct list));
	head->next = NULL;
	head->data = data;
	return head;
}

void add_top(struct list *list, struct list *element)
{
	if(list->next == NULL)
	{
		list->next = element;
		return;
	}
	element->next = list->next;
	list->next = element;
}

struct list *parser(char* file)
{
	FILE* doc = fopen(file, "r");
	char c;
	
	char* htag = "<a href=\"";
	
	struct list *links = new_list();	

	if (doc == NULL)
	{
		printf("html file can't be opened\n");
		return links;
	}

	int i;
	int pos;
	while((c = fgetc(doc)) != EOF)
	{
		if (c == '<' && !feof(doc))
		{
			i = 1;
			while(i < 9 && !feof(doc))
			{
				c = fgetc(doc);
				if (c != htag[i])
					break;
				i++;
			}
			if(i == 9)
			{
				pos = 0;
				char *link = malloc(sizeof(char)* 256);
				while((c = fgetc(doc)) != EOF && pos < 256)
				{
					if(c == '\"')
					{
						if ((c = fgetc(doc)) == '>')
						{
							link[pos] = '\0';
							break;
						}
						else
						{
							link[pos] = '\"';
						}
					}
					link[pos] = c;
					pos++;
				}
				if(pos != 256)
				{
					link[pos] = '\0';
					link = realloc(link, pos);
					add_top(links, new_element(link));
				}
			}
		}
	}

	fclose(doc);
	return links;
}


int main()
{
	
	struct list *res = parser("stuff.html");
	
	res = res->next;
	while(res != NULL)
	{
		printf("%s\n",res->data);
		res = res->next;
	}
	
	return 0;
}

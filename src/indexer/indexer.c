#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char htag[9] = "a href=\"";

//linked list for the forward index
//info points to word information
struct list 
{
	struct list *next;
	char* data;
	void* info;
};

//hit list, stores hit postions
struct hit
{
	int position;
	struct hit *next;
};

//type : fancy or simple hit
//nhits : number of occurences
//hit : hit list with hit positions
struct info 
{
	char type;
	int nhits;
	struct hit *hits;
};

struct list *new_list()
{
	struct list *head = NULL; 
	head = malloc(sizeof(struct list));
	head->next = NULL;
	head->data = NULL;
	head->info = NULL;
	return head;
}

struct list *new_element(char *data)
{
	struct list *head = NULL;
	head = malloc(sizeof(struct list));
	head->next = NULL;
	head->data = data;
	head->info = NULL;
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

//checks after the offset if tag is a prefix
//length of the tag is given
int check_tag(FILE *doc, char *tag, size_t len)
{
	int i = 1;
	char c;

	while(!feof(doc) && i < len)
	{
		c = fgetc(doc);
		if(c != tag[i])
			break;
		i++;
	}
	return (i == len);
}

//retrieves simple tag keywords and stores them in tag
//c represents the first keyword character already encountered after <
void get_tag(FILE *doc, char c, char **tag)
{
	(*tag)[0] = c;
	c = fgetc(doc);
	int pos = 1;

	while(c != EOF && c != ' ')
	{
		if(c == '>')
			break;
		(*tag)[pos] = c;
		pos++;
		c = fgetc(doc);
	}

	(*tag)[pos] = '\0';
	(*tag) = realloc((*tag), pos);

	while(c != EOF && c != '>')
	{
		c = fgetc(doc);
	}
}

//add the link conrained between the a tags to links
void add_link(struct list *links, FILE * doc)
{
	char c;
	int pos = 0;
	char *link = malloc(sizeof(char)*256);
	
	while((c = fgetc(doc)) != EOF && pos < 256)
	{
		if(c == '\"')
		{
			c = fgetc(doc);
			//it is the end of the link or the a tag
			if (c == '>' || c == ' ')
			{
				link[pos] = '\0';
				break;
			}
			//it is part of the link
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
		//in case there's still space in link
		link = realloc(link, pos);
		add_top(links, new_element(link));
	}

	//moving out of the tag : add attribute processing later
	if(c == ' ')
	{
		while(c != '>')
		{
			c = fgetc(doc);
			//ATTRIBUTE PROCESSING GOES HERE
		}
	}
	c = fgetc(doc);

	//move the offset to the end of the closing tag
	while(c != '>')
	{
		c = fgetc(doc);
	}
}

//skips uninteresting tags, stops at >
void skip_tag(FILE *doc, char *tag, struct list *links)
{
	printf("\n|skipping, tag = %s| --------------------\n", tag);
	char c;

	//skip or process tag content
	while((c = fgetc(doc)) != EOF)
	{
check_again:
		if(c == '<')
		{
			//check for closing tag
			if ((c = fgetc(doc)) == '/')
			{
				if((c = fgetc(doc)) == tag[0])
				{
					if(check_tag(doc, tag, strlen(tag)))
						break;
				}
			}
			//check for link
			if(c == 'a')
			{
				if(check_tag(doc, htag, 8))
				{
					add_link(links, doc);
					if((c = fgetc(doc)) == EOF)
						return;
					//to solve for successive tags
					if(c == '<')
						goto check_again;
				}

			}
		}
		printf("%c",c);
		//CONTENT PROCESSING GOES HERE

	}

	//skip the last tag character
	c = fgetc(doc);
	printf("\n---------------------------------------\n");
}

//parses found paragraph and extracts links
void add_words(struct list *links, FILE *doc)
{
	char c = fgetc(doc);
	printf("\nparagraph========================\n");

	//skip over paragraph opening tag
	while(c != '>')
	{
		c = fgetc(doc);
	}

	int pos = 0;
	char *word = malloc(sizeof(char)*64);
	char *tag = malloc(sizeof(char)*16);
	
	while((c = getc(doc)) != EOF && pos < 64)
	{
		//treat tags
		if(c == '<')
		{
			if(pos != 0)
			{
				word[pos] = '\0';
				printf("%s ", word);
				pos = 0;
			}
tag_processing:
			switch(c = getc(doc))
			{
				//extract link from paragraph
				//LINK KEYWORD EXTACTION GOES HERE
				case 'a':
					if(check_tag(doc, htag, 8))
					{
						add_link(links, doc);
						if((c = fgetc(doc)) == EOF)
							return;
						//to solve for successive tags
						if(c == '<')
							goto tag_processing;
						break;
					}
					else
					{
						skip_tag(doc, "a", links);
					}
				//end of paragraph
				case '/':
					//skip over paragraph closing tag
					c = fgetc(doc);
					c = fgetc(doc);
					printf("\n==================================\n");
					return;	
				//skip uninteresting tag
				default:
					get_tag(doc, c, &tag);
					skip_tag(doc, tag, links);
					if((c = fgetc(doc)) == EOF)
						return;
					//to solve for successive tags
					if(c == '<')
						goto tag_processing;
			}
		}

		//save word
		if(c == ' ')
		{
			if(pos != 0)
			{
				word[pos] = '\0';
				printf("%s ", word);
				pos = 0;
			}
		}
		//construct word
		else
		{
			word[pos] = c;
			pos++;
		}

	}

	//free
	if(word != NULL)
		free(word);

}

//parses the whole file
struct list *parser(char* file)
{
	FILE* doc = fopen(file, "r");
	char c;

	struct list *links = new_list();

	if (doc == NULL)
	{
		printf("html file can't be opened\n");
		return links;
	}

	int i;

	while((c = fgetc(doc)) != EOF)
	{
		if (c == '<' && !feof(doc))
		{
			c = fgetc(doc);
			switch(c)
			{
				//extract link
				case 'a':
					if(check_tag(doc, htag, 8))
					{
						add_link(links, doc);
					}
					break;
				//extract paragraph
				case 'p':
					add_words(links, doc);
					if((c = fgetc(doc)) == EOF)
						return links;
					break;
			}
		}
	}
	fclose(doc);
	return links;
}


int main(int argc, char *argv[])
{
	if(argc != 3)
	{
		printf("please specify the correct option : |0| without links, |1| with links\n");
		printf("|test| text.txt, |real| stuff.html\n");
		return 1;
	}

	struct list *res;

	if(!strcmp(argv[2], "test"))
	{
		printf("parsing test file\n");
		res = parser("text.txt");
	}else
	{
		if(!strcmp(argv[2], "real"))
		{
			printf("parsing example html file\n");
			res = parser("stuff.html");
		}
		else
		{
			printf("wrong file parameter\n");
		}
	}


	if(!strcmp(argv[1],"1") && res != NULL)
	{
		res = res->next;
		while(res != NULL)
		{
			printf("%s\n",res->data);
			res = res->next;
		}
	}

	if(res != NULL)
		free(res);

	return 0;
}

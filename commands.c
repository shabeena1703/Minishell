#include<stdio.h>
#include "shell.h"
#include<unistd.h>

char *external_cmd[152];
char *builtins[] = {"echo", "printf", "read", "cd", "pwd", "pushd", "popd", "dirs", "let", "eval",
					"set", "unset", "export", "declare", "typeset", "readonly", "getopts", "source",
					"exit", "exec", "shopt", "caller", "true", "type", "hash", "bind", "help","fg","bg","jobs", NULL};



struct node *head = NULL;
void extract_external_commands(char **external)
{
	//step1: open a file and read the commands till \n
	//step2: take a length of a command (to allocate the memory for column)..row is fixed(152)
	//step3: allocate the memory based on lenght
	//step4: store the cmd in external_cmd[][]
	char buffer[100];
	int n;
	int i = 0,j = 0;
	int length =0;
	char ch;
	int fd = open("external_commands.txt",O_RDONLY);
	if(fd == -1)
	{
		printf("Error: File can't be opened\n");
		return;
	}
	while((n=read(fd,&ch,1)) > 0)
	{
		//if(ch != '\n')
		if(ch != '\n' && ch != '\r')
		{
			buffer[j++] =ch;
			
		}

		//if it equals to \n
		else if(ch =='\n')
		{
			length =j+1;
			external[i] = malloc(length);
			//store the command in external[i]..then add '\0'
			buffer[j] = '\0';

			strcpy(external[i],buffer);
			i++;
			j=0;

		}
	}
	external[i] = NULL;
	close(fd);

}




char *get_command(char *input_string)
{
	//fetch only the first word -> if input_string containd ls -l, then fetch only ls,store in buffer and return it
	int i=0;
	char *buffer = malloc(20);
	for(i=0;input_string[i] != ' ' && input_string[i] != '\0';i++)
	{
		buffer[i] = input_string[i];
	}
	buffer[i] = '\0';
	//printf("get_cmd is successful");
	return buffer;
}



int check_command_type(char *cmd)
{
	//check if cmd is internal or external
	
	//compare input_string(cmd[])(first word got using get_command function) with all the elements in array using loop...
	for(int i=0; builtins[i]!=NULL; i++)
	{
  		//if matches,then it is internal..
		if(strcmp(cmd,builtins[i])==0)
		{
				//so return BUILTIN
			return BUILTIN;
		}
	}

	//compare the cmd[] with the external_cmd[][]
	for(int i=0;external_cmd[i]!=NULL;i++)
	{
		//else if it is matching 
		if(strcmp(cmd,external_cmd[i]) == 0)
		{
			//return EXTERNAL
			return EXTERNAL;
		}
			
	}

	return NO_COMMAND;

}



void execute_internal_commands(char *input_string)
{
	/*1.exit  2.cd  3.pwd  4.echo $$ 5.echo $?    6.echo $SHELL*/
	//check the input_string contains exit or not...if it is matching
	if(strcmp(input_string,"exit")==0)
	{
		exit(0);
	}

	//check the input_string contains cd or not..if it is matching
	else if(strncmp(input_string,"cd",2)==0)
	{
		//use chdir() system call to change the directory
		// chdir(input_string + 3);   //3 for space

		if(chdir(input_string + 3) == -1)
		{
				printf("cd failed\n");
		}
		else
		{
			printf("cd successful\n");
		}
			
	}

	//check the input_string contains pwd or not..if it is matching
	else if(strcmp(input_string,"pwd") == 0)
	{
		// for this we need to implement one library function called getcwd()
		char buffer[250];
		printf("%s\n",getcwd(buffer,250));  
		//print the buffer also 

	}


	//check the input_string contains echo $$ or not
	else if(strcmp(input_string,"echo $$") == 0)
	{
		printf("%d\n",getpid());
	}

	//check the input_string contains echo $$ or not
	else if(strcmp(input_string,"echo $?") == 0)
	{
		if(WIFEXITED(status))
		{
			printf("%d\n",WEXITSTATUS(status));    //declare globally
		}
		
	}

	//check the inpout contains echo $SHELL or not
	//if it contains
	else if(strcmp(input_string,"echo $SHELL")==0)
	{
		printf("%s",getenv("SHELL"));  
	}

	//check if inout string is having jobs command or not
	//if it is job command
	else if(strcmp(input_string,"jobs")==0)
	{
		print_list(head);
	}


	//check if inout string is having fg command or not
	else if(strcmp(input_string,"fg")==0)
	//if it contains
	{
		if(head == NULL)
		{
			printf("bash: fg: Current: no such job\n");
			return;
		}
		 printf("%s\n", head->cmd_name);

		 
		kill(head->pid,SIGCONT);
		if(waitpid(head->pid,&status,WUNTRACED)>0)   //pid means child pid
		{
			if(!WIFSTOPPED(status))
			{	
				pid = head->pid;
				delete_first(&head);
			}
			else
			{
				head->status = 0;
			}
		}
	}


	//check if inpout string contains bg or not
	else if(strcmp(input_string, "bg") == 0)
	{
		struct node *temp = head;

		while(temp != NULL && temp->status == 1)
		{
			temp = temp->link;
		}

		if(temp == NULL)
		{
			if(head == NULL)
				printf("bash: bg: current: no such job\n");
			else
				printf("bash: bg: job already in background\n");

			return;
		}

		signal(SIGCHLD, signal_handler);

		if(kill(temp->pid, SIGCONT) == 0)
		{
			temp->status = 1;
			printf("[%d]+ %s &\n",temp->job_no, temp->cmd_name);
		}
		else
		{
			 printf("bash: bg: job has terminated\n");
		}
		
	}

	
}




void execute_external_commands(char *input_string)
{
    // input_string is 1D, so convert it to 2D array cmd[][]
    // whenever we get space, increment the row

    char *argv[20];
    int i = 0;

    argv[i] = strtok(input_string, " ");

    while(argv[i] != NULL)
    {
        i++;
        argv[i] = strtok(NULL, " ");
    }

    // check the pipe is present or not

    int cmd_pos[20];
    cmd_pos[0] = 0;
    int cmd_count = 1;

    int pipe_index = 0;

    while(argv[pipe_index] != NULL)
    {
        if(strcmp(argv[pipe_index], "|") == 0)
        {
            argv[pipe_index] = NULL;
            cmd_pos[cmd_count] = pipe_index + 1;
            cmd_count++;
        }
        pipe_index++;
    }

    // if pipe is not present

    if(cmd_count == 1)
    {
        execvp(argv[0], argv);
    }

    // if pipe is present

    else
    {
        int pipe_fd[2];

        for(int i = 0; i < cmd_count; i++)
        {
            if(i < cmd_count - 1)
            {
                pipe(pipe_fd);

                int pid = fork();

                if(pid > 0)
                {
                    close(pipe_fd[1]);
                    dup2(pipe_fd[0], 0);
                    close(pipe_fd[0]);
                }
                else if(pid == 0)
                {
                    close(pipe_fd[0]);
                    dup2(pipe_fd[1], 1);
                    execvp(argv[cmd_pos[i]], argv + cmd_pos[i]);
					exit(1);
                }
            }
            else
            {
                int pid = fork();

                if(pid == 0)
                {
                    execvp(argv[cmd_pos[i]], argv + cmd_pos[i]);
                }
                else if(pid > 0)
                {
                    wait(NULL);
                }
            }
        }
		exit(0);
    }
}





void insert_first(struct node **head)
{
	struct node *new = malloc(sizeof(struct node));
	if(new == NULL)
	{
		printf("ERROR: Creation of node failure\n");
		return ;
	}

	new->pid = pid;
	new->status =0;
	strcpy(new->cmd_name,input_string);
	new->link = NULL;

	if(*head ==NULL)
	{
		new->job_no = 1;
		*head = new;
	}
	else
	{
		new->job_no = (*head)->job_no + 1;
		new->link = *head;
		*head=new;
	}
}




void delete_first(struct node **head)
{
	struct node *temp = *head;
	if(*head == NULL)
	{
		printf("No node to delete\n");
		return ;
	}
	else
	{
		*head = temp->link;
		free(temp);
	}
}




void print_list(struct node *head)
{
	struct node *temp = head;
	int i = 1;
	if(head == NULL)
	{
		return ;
	}
	else
	{
		while(temp != NULL)
		{
			if(temp->status == 0)
			{
				printf("[%d]+  Stopped                    %s\n",temp->job_no, temp->cmd_name);
			}

			temp = temp->link;
		}
	}
}
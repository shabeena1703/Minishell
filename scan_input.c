#include<stdio.h>
#include "shell.h"
#include<string.h>
#include<signal.h>

char prompt[50] = "shabeena-minishell$:";
//int flag;
int pid;
int status;

void signal_handler(int signum)
{
   if(signum == SIGINT)
    {
        if(pid == 0)
        {
             printf("\n%s",prompt);    //we need to declare it globally
             fflush(stdout);
        }
    }

    else if(signum == SIGTSTP)
    {
        if(pid == 0)
        {
            
            printf("\n%s",prompt);
            fflush(stdout);
        }
    }

    else if(signum == SIGCHLD)
    {
        //clear the resources
        while(waitpid(-1,&status,WNOHANG)>0)
        {
            if(WIFEXITED(status) || WIFSIGNALED(status))
            {
                if(head != NULL)
                {
                    printf("\n[%d]+ Done                    %s\n",head->job_no, head->cmd_name);
                    fflush(stdout);
                    delete_first(&head);
                }
            }
            
        }
    }
   
}




void scan_input(char *prompt,char *input_string)
{

    extract_external_commands(external_cmd);

    //register the signal (for controlling ctrl+c and ctrl+z)
    signal(SIGINT,signal_handler);      //ctrl+c
    signal(SIGTSTP,signal_handler);     //ctrl+z

    while(1)
    {
        //printf("%s",prompt);
       printf(ANSI_COLOR_GREEN "shabeena" ANSI_COLOR_BLUE "/minishell" ANSI_COLOR_RESET "$ ");
       fflush(stdout);
        scanf(" %[^\n]", input_string);

        //check whether the input string contains PS1 or not
        //if present
        if((strncmp(input_string, "PS1=",4)==0))
        {
            //check if input_string[4] is space or not...if not space change the prompt
            if(input_string[4] != ' ')
            {
                //change /update the prompt
                strcpy(prompt,input_string + 4);
            }
            else
            {
                printf("Error..Cant change the prompt\n");
            
            }
        }

        //if input string contains commands
        //check if it is internal or external
        else
        {
            //get first word from input_string
            char *ret = get_command(input_string);

            //using this ret only check if it is internal or external
            int res = check_command_type(ret);

            //if it is internal 
            if(res == BUILTIN)
            {
                //logic for executing internal command (using system calls and library functions)
                execute_internal_commands(input_string);
            }

            else if(res == EXTERNAL)
            {
                //logic for executing external commands(using execvp())
                //for executing execvp() we need to create fork

                pid =fork();
                if(pid > 0)
                {
                    //parent
                   
                    //if child is changing its state also then parent should know it...thats why we use waitpid() instead of wait()..
                    //WUNTRACED : to return even if the child is stopped
                   
                    waitpid(pid,&status,WUNTRACED);
                    if(WIFSTOPPED(status))
                    {
                        insert_first(&head);
                         printf("[%d]+  Stopped                    %s\n",head->job_no, head->cmd_name);
                    }
                    pid = 0;
                    
                }

                else if(pid == 0)
                {
                    //child

                    //if we are executing commands,at time time if we give ctrl+z or ctrl+c , we need to print its default behaviour ie terminate(sleep 20)
                    signal(SIGINT,SIG_DFL);
                    signal(SIGTSTP,SIG_DFL);
                    execute_external_commands(input_string);
                }

            }

            else if(res == NO_COMMAND)
            {
                //print error msg(neither external nor internal)
                printf("ERROR..Command is neither internal nor external\n");
                
            }
        }
        
    }
}

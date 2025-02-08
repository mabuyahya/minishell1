#include "minishell.h"

int  handle_input(t_main *main)
{
   if (!lexer(main))
      return(0);
   return (1);
}
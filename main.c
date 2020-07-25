#include "libUseful-4/libUseful.h"


#define TERMCTRL_PRINT 0
#define TERMCTRL_TITLE 1
#define TERMCTRL_GETCH 2
#define TERMCTRL_PROMPT 3
#define TERMCTRL_PASSWD 4
#define TERMCTRL_ECHO   5

#define TERMCTRL_CLEAR 1
#define TERMCTRL_AT 2
#define TERMCTRL_UNICODE 4
#define TERMCTRL_NONEWLINE 8
#define TERMCTRL_NOOPT   16
#define TERMCTRL_STDERR  32
#define TERMCTRL_STDOUT  64
#define TERMCTRL_NOSLASH 128

typedef struct
{
int cmd;
int flags;
int x;
int y;
int timeout;
char *OutputStr;
} TCmd;

TCmd *ParseCommandLine(int argc, char *argv[])
{
TCmd *Cmd;
CMDLINE *Args;
const char *p_arg;
int i;

if (argc > 0)
{
	Cmd=(TCmd *) calloc(1, sizeof(TCmd));
	Cmd->OutputStr=CopyStr(Cmd->OutputStr, "");
	
	p_arg=GetBasename(argv[0]);
	if (strcmp(p_arg, "echo")==0) 
	{
		Cmd->cmd=TERMCTRL_ECHO;
		Cmd->flags |= TERMCTRL_UNICODE | TERMCTRL_NOSLASH;
	}
	else if (strcmp(p_arg, "uecho")==0) 
	{
		Cmd->cmd=TERMCTRL_ECHO;
		Cmd->flags |= TERMCTRL_UNICODE | TERMCTRL_NOSLASH;
	}
	else if (strcmp(p_arg, "print")==0) 
	{
		Cmd->cmd=TERMCTRL_ECHO;
		Cmd->flags |= TERMCTRL_UNICODE;
	}
	else if (strcmp(p_arg, "getch")==0) Cmd->cmd=TERMCTRL_GETCH;
	else if (strcmp(p_arg, "ask")==0) Cmd->cmd=TERMCTRL_PROMPT;
	else if (strcmp(p_arg, "ask-password")==0) Cmd->cmd=TERMCTRL_PASSWD;
	else if (strcmp(p_arg, "xterm-title")==0) Cmd->cmd=TERMCTRL_TITLE;
	else if (strcmp(p_arg, "xtitle")==0) Cmd->cmd=TERMCTRL_TITLE;
	

	Args=CommandLineParserCreate(argc, argv);
	p_arg=CommandLineNext(Args);
	while (p_arg != NULL)
	{
		if (Cmd->flags & TERMCTRL_NOOPT) Cmd->OutputStr=MCatStr(Cmd->OutputStr, p_arg, " ", NULL);
		else if (strcmp(p_arg, "--")==0) Cmd->flags |= TERMCTRL_NOOPT;
		else if (strcmp(p_arg, "-at")==0)
		{
			Cmd->flags |= TERMCTRL_AT;
			Cmd->x=atoi(CommandLineNext(Args));
			Cmd->y=atoi(CommandLineNext(Args));
		}
		else if (strcmp(p_arg, "-u")==0) Cmd->flags |= TERMCTRL_UNICODE;
		else if (strcmp(p_arg, "-U")==0) Cmd->flags &= ~TERMCTRL_UNICODE;
		else if (strcmp(p_arg, "-e")==0) Cmd->flags &= ~TERMCTRL_NOSLASH;
		else if (strcmp(p_arg, "-E")==0) Cmd->flags |= TERMCTRL_NOSLASH;
		else if (strcmp(p_arg, "-n")==0) Cmd->flags |= TERMCTRL_NONEWLINE;
		else if (strcmp(p_arg, "-stderr")==0) Cmd->flags |= TERMCTRL_STDERR;
		else if (strcmp(p_arg, "-stdout")==0) Cmd->flags |= TERMCTRL_STDOUT;
		else if (strcmp(p_arg, "-clear")==0) Cmd->flags |= TERMCTRL_CLEAR;
		else if (strcmp(p_arg, "-title")==0) Cmd->cmd |= TERMCTRL_TITLE;
		else if (strcmp(p_arg, "-getch")==0) Cmd->cmd |= TERMCTRL_GETCH;
		else if (strcmp(p_arg, "-ask")==0) Cmd->cmd |= TERMCTRL_PROMPT;
		else if (strcmp(p_arg, "-prompt")==0) Cmd->cmd |= TERMCTRL_PROMPT;
		else if (strcmp(p_arg, "-password")==0) Cmd->cmd |= TERMCTRL_PASSWD;
		else if (strcmp(p_arg, "-t")==0)
		{
			p_arg=CommandLineNext(Args);
			Cmd->timeout=atoi(p_arg);
		}
		else if (strcmp(p_arg, "-timeout")==0)
		{
			p_arg=CommandLineNext(Args);
			Cmd->timeout=atoi(p_arg);
		}
		else Cmd->OutputStr=MCatStr(Cmd->OutputStr, p_arg, " ", NULL);

		p_arg=CommandLineNext(Args);
	}
}

Destroy(Args);

return(Cmd);
}




int main(int argc, char *argv[])
{
TCmd *Cmd;
STREAM *Term=NULL;
char *Tempstr=NULL;
int ch;


Cmd=ParseCommandLine(argc, argv);

if (Cmd->flags & (TERMCTRL_STDERR)) Term=STREAMFromDualFD(0,2);
if ((! Term) && (Cmd->flags & (TERMCTRL_STDOUT))) Term=STREAMFromDualFD(0,1);
if (! Term) Term=STREAMOpen("/dev/tty", "rw");
if (! Term) Term=STREAMFromDualFD(0,2);

if (Cmd->timeout > 0) STREAMSetTimeout(Term, Cmd->timeout);
TerminalInit(Term, TERM_RAWKEYS | TERM_SAVE_ATTRIBS);
if (Cmd->flags & TERMCTRL_CLEAR) TerminalClear(Term);
if (Cmd->flags & TERMCTRL_UNICODE) UnicodeSetUTF8(3);

if (Cmd->flags & TERMCTRL_NOSLASH)
{
Tempstr=QuoteCharsInStr(Tempstr, Cmd->OutputStr, "\\");
Cmd->OutputStr=CopyStr(Cmd->OutputStr, Tempstr);
}

switch (Cmd->cmd)
{
case TERMCTRL_TITLE:
	XtermSetTitle(Term, Cmd->OutputStr);
break;

case TERMCTRL_GETCH:
	ch=TerminalReadChar(Term);
	printf("%s\n", TerminalTranslateKeyCode(ch));
break;

case TERMCTRL_PROMPT:
	Tempstr=TerminalReadPrompt(Tempstr, Cmd->OutputStr, 0, Term);
	printf("%s\n", Tempstr);
break;

case TERMCTRL_PASSWD:
	Tempstr=TerminalReadPrompt(Tempstr, Cmd->OutputStr, TERM_SHOWTEXTSTARS, Term);
	printf("%s\n", Tempstr);
break;

default:
if (Cmd->flags & TERMCTRL_AT) TerminalCursorMove(Term, Cmd->x, Cmd->y);
if (StrValid(Cmd->OutputStr)) TerminalPutStr(Cmd->OutputStr, Term);
if (! (Cmd->flags & TERMCTRL_NONEWLINE)) TerminalPutStr("\n", Term);
break;
}

TerminalReset(Term);

Destroy(Tempstr);
}

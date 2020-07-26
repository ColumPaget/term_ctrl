#include "libUseful-4/libUseful.h"

#define VERSION "1.0"

#define TERMCTRL_PRINT 0
#define TERMCTRL_TITLE 1
#define TERMCTRL_GETCH 2
#define TERMCTRL_PROMPT 3
#define TERMCTRL_PASSWD 4
#define TERMCTRL_ECHO   5
#define TERMCTRL_HELP   6
#define TERMCTRL_VERSION 7

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
		else if (strcmp(p_arg, "-version")==0) Cmd->cmd |= TERMCTRL_VERSION;
		else if (strcmp(p_arg, "--version")==0) Cmd->cmd |= TERMCTRL_VERSION;
		else if (strcmp(p_arg, "-help")==0) Cmd->cmd |= TERMCTRL_HELP;
		else if (strcmp(p_arg, "--help")==0) Cmd->cmd |= TERMCTRL_HELP;
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


void PrintHelp()
{
printf("\n  term_ctrl [options] [text]\n");
printf("\ndefault action is to print out [text] with support for both slash-sequences and libUseful tilde-sequences\n");
printf("\n");
printf("-clear         Clear screen before doing anything else\n");
printf("-title         Set xterm-title instead of outputting text\n");
printf("-getch         Read a single keypress and return it on stdout\n");
printf("-ask           Write a prompt or message and read the user's reply, returning it on stdout\n");
printf("-prompt        Write a prompt or message and read the user's reply, returning it on stdout\n");
printf("-password      Write a prompt or message and read the user's reply without echoing that to the screen. Return the reply on stdout\n");
printf("-timeout <cs>  Wait 'cs' centi-seconds for a reply (mostly useful with 'getch' to await a keypress.\n");
printf("-t <cs>        Wait 'cs' centi-seconds for a reply (mostly useful with 'getch' to await a keypress.\n");
printf("-u             Enable unicode support\n");
printf("-U             Disable unicode support\n");
printf("-e             Enable support for '\' style escape sequences\n");
printf("-E             Disable support for '\' style escape sequences\n");
printf("-at <x> <y>    When outputing text, put it at screen co-ords x,y.\n");
printf("-n             Suppress trailing '\n'\n");
printf("-stderr        Write output to stderr rather than to the terminal\n");
printf("-stdout        Write output to stdout rather than to the terminal\n");

printf("\n\nSymlink term_ctrl to these names to get the following functions\n");
printf("echo            emulate the 'echo' command (no unicode or slash-sequences by default).\n");
printf("uecho           emulate the 'echo' command but with unicode support (no slash-sequences by default).\n");
printf("print           output text with support for unicode output, and tilde-style and slash-style escape sequences.\n");
printf("getch           read keypresses from the terminal and return their names on stdout.\n");
printf("ask             print query string and read an input string in response.\n");
printf("ask-password    print a query string and read an input string in response, but echo stars instead of the typed characters.\n");
printf("xtitle          set title of xterm compatible terminal emulators.\n");
printf("\n\nterm_ctrl recognizes the following libuseful tilde-escape-sequences:\n\n");
printf("~~        output the tilde character '~'\n");
printf("~r        switch color to red\n");
printf("~R        switch background to red\n");
printf("~g        switch color to green\n");
printf("~G        switch background to green\n");
printf("~b        switch color to blue\n");
printf("~B        switch background to blue\n");
printf("~n        switch color to black ('night' or 'noir')\n");
printf("~N        switch background to black ('night' or 'noir')\n");
printf("~y        switch color to yellow\n");
printf("~Y        switch background to yellow\n");
printf("~m        switch color to magenta\n");
printf("~M        switch background to magenta\n");
printf("~c        switch color to cyan\n");
printf("~C        switch background to cyan\n");
printf("~e        switch to bold text\n");
printf("~i        switch to inverse text\n");
printf("~u        switch to underlined text\n");
printf("~<        clear to start of line\n");
printf("~>        clear to end of line\n");
printf("~0        reset all attributes (return to normal text)\n");
printf("~Uxxxx    output a unicode character (xxxx is the 4-digit unicode name)\n");
printf("~:name:   output a unicode character using a name from the unicode names file \n");
printf("\n\nterm_ctrl recognizes the following slash-escape-sequences\n");
printf("\n");
printf("  \\\\      backslash\n");
printf("  \\a      alert (BEL)\n");
printf("  \\b      backspace\n");
printf("  \\e      escape\n");
printf("  \\f      form feed\n");
printf("  \\n      new line\n");
printf("  \\r      carriage return\n");
printf("  \\t      horizontal tab\n");
printf("  \\0NNN   byte with octal value NNN (1 to 3 digits)\n");
printf("  \\xHH    byte with hexadecimal value HH (1 to 2 digits)\n");
printf("\n");
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

case TERMCTRL_VERSION:
	printf("term_ctrl version %s\n", VERSION);
break;

case TERMCTRL_HELP:
	PrintHelp();
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

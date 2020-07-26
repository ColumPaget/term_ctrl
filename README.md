SYNOPSIS
========

term_ctrl is a program that provides various functions for an xterm or other ansi compatible terminal. These include:

 * enhanced 'echo' with color commands and unicode support
 * xterm set title command with unicode support
 * get character command that supports many non-character keys like the arrow keys and page up/down.
 * 'prompt' command that reads a string in response to a prompt
 * 'password' command for reading secrets from the terminal

AUTHOR
======

term_ctrl is copyright 2020 Colum Paget.

LICENCE
=======

term_ctrl is released under the GNU Public Licence Version 3.

USAGE
=====

```
	term_ctrl [options] [text]
```

OPTIONS
=======

```
-clear         Clear screen before doing anything else
-title         Set xterm-title instead of outputting text
-getch         Read a single keypress and return it on stdout
-ask           Write a prompt or message and read the user's reply, returning it on stdout
-prompt        Write a prompt or message and read the user's reply, returning it on stdout
-password      Write a prompt or message and read the user's reply without echoing that to the screen. Return the reply on stdout
-timeout <cs>  Wait 'cs' centi-seconds for a reply (mostly useful with 'getch' to await a keypress.
-t <cs>        Wait 'cs' centi-seconds for a reply (mostly useful with 'getch' to await a keypress.
-u             Enable unicode support
-U             Disable unicode support
-e             Enable support for '\' style escape sequences
-E             Disable support for '\' style escape sequences
-at <x> <y>    When outputing text, put it at screen co-ords x,y.
-n             Suppress trailing '\n'
-stderr        Write output to stderr rather than to the terminal
-stdout        Write output to stdout rather than to the terminal
```

The default behavior of term_ctrl is to output text to the screen. This text can include libUseful '~' escape sequences to set color and do other fuctions, like so:

```
term_ctrl "~r red ~0\n~g green ~0"
```
By default the output includes a terminating '\n' character. This can be suppressed with the '-n' option.

Text can be positioned with the `-at` option, like so:

```
term_ctrl -at 10 5 "This text at col 10 and row 5"
```

By default text is written to the terminal by opening `/dev/tty` and writing to it. If '/dev/tty' can't be opened, then standard-error is used instead. This keeps standard-out available to return data to the calling shell/process. Output to standard-error can be forced using the '-stderr' option.

```
term_ctrl -stderr "this text goes to standard-error"
```

Similarly output to standard out is requested with the `-stdout` option.

```
term_ctrl -stdout "this text goes to standard-out"
```

The `-title` option switches the behavior to updating the title-bar of xterm or any other compatible terminal emulator.

```
term_ctrl -title "This text goes in the title bar"
```

The `-ask` or `-prompt` options change the behavior to asking the user a question. The text of the question is written to standard-out. For example:

```
FRUIT=`term_ctrl -ask "What is your favorite fruit? "`
```

The `-password` option asks a question like `-ask` or `-prompt`, but when the user types it echoes asterisk characters instead of their keypresses.

The '-getch' option changes the behavior to reading a keypress and returning it's name on standard-out. Keypresses that map to english letters and numbers are returned as single characters. Keypresses that map to 'unprintable' keys, like page-up or the arrow or function keys, are returned as a multi character name like 'PGUP', 'LEFT' or 'F5'.

```
KEY=`term_ctrl -getch`
```

PROGRAM NAMES
=============

term_ctrl can be linked to other names, using either hard-links or symbolic links, like so:

```
ln term_ctrl echo
ln -s term_ctrl getch
```

When it is started up under one of these alternative names it will behave differently.

```
echo            emulate the 'echo' command (no unicode or slash-sequences by default).
uecho           emulate the 'echo' command but with unicode support (no slash-sequences by default).
print           output text with support for unicode output, and tilde-style and slash-style escape sequences.
getch           read keypresses from the terminal and return their names on stdout.
ask             print query string and read an input string in response.
ask-password    print a query string and read an input string in response, but echo stars instead of the typed characters.
xtitle          set title of xterm compatible terminal emulators.
```

TILDE SEQUENCES
===============

term_ctrl recognizes the following libuseful tilde-escape-sequences:

```
~~        output the tilde character '~'
~r        switch color to red
~R        switch background to red
~g        switch color to green
~G        switch background to green
~b        switch color to blue
~B        switch background to blue
~n        switch color to black ('night' or 'noir')
~N        switch background to black ('night' or 'noir')
~y        switch color to yellow
~Y        switch background to yellow
~m        switch color to magenta
~M        switch background to magenta
~c        switch color to cyan
~C        switch background to cyan
~e        switch to bold text
~i        switch to inverse text
~u        switch to underlined text
~<        clear to start of line
~>        clear to end of line
~0        reset all attributes (return to normal text)
~Uxxxx    output a unicode character (xxxx is the 4-digit unicode name)
~:name:   output a unicode character using a name from the unicode names file 
```

so, for example

```
term_ctrl "~y~RYellow text on a red background~0"
```

SLASH SEQUENCES
===============

term_ctrl recognizes the following slash-escape-sequences

```
  \\      backslash
  \a      alert (BEL)
  \b      backspace
  \e      escape
  \f      form feed
  \n      new line
  \r      carriage return
  \t      horizontal tab
  \0NNN   byte with octal value NNN (1 to 3 digits)
  \xHH    byte with hexadecimal value HH (1 to 2 digits)
```



<div align="center">
    <img src="img/COG-BANNER.png" alt="cog preview" width="550"/>
</div>
    
cog is a minimalist line-based text editor implemented in C, inspired by classic editors like ed. It enables basic text file editing in a terminal environment using simple commands.


# Features:

- Load existing text files or create new files by default.

- Append, delete, and navigate text lines interactively.

- Save changes back to disk.

- Switch between command mode and insert mode.

- Execute shell commands prefixed with bsh$.

- Basic terminal control with ANSI escape codes.


# Build:

Use the provided Makefile or compile directly with gcc:

```bash
$ make all
```

```bash
$ gcc main.c -o cog
```

To install system-wide (optional):

```bash
$ sudo make install
```

To uninstall:

```bash
$ sudo make uninstall
```

#Usage:

Run the editor optionally with a filename argument:

```bash 
$ cog <filename.txt>
```

If no filename is provided, the editor uses/creates a default file named foo._.


#Commands:

``a`` — Enter insert mode to append new lines. End insert mode by typing a single dot ``.`` on a new line.

``p`` — Print the current line.

``n`` — Print the current line with its line number.

``P`` — Print all lines without line numbers; current line prefixed with *.

``N`` — Print all lines with line numbers; current line prefixed with *.

``+`` — Move cursor to the next line.

``-`` — Move cursor to the previous line.

``l.<NUMBER>`` — Move cursor to the specified line number (e.g., l.5).

``d`` — Delete the current line.

``w`` — Save the buffer to the current file.

``clear`` — Clear the terminal screen.

``bsh$ <COMMAND>`` — Execute a shell command from within the editor.

``q`` — Quit the editor.


# Technical Details:

- The editor uses an array of dynamically allocated strings to store text lines.

- Limits: up to 1000 lines, each line up to 1024 characters (adjustable).

- Terminal cursor and screen management via ANSI escape sequences.

- Simple command-line interface for interaction.

- No external dependencies besides standard C library.


# Compatibility:

cog is compatible with Windows, Linux, MacOS and general Unix systems. Android systems are included to full compatibility on version 0.3.


# Screenshot:

<a href="https://ibb.co/xqPrgNKH"><img src="https://i.ibb.co/Jj0Sp9WQ/Shot-2025-07-09-200041.png" alt="cog-screenshot" border="0"></a>

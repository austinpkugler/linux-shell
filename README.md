# Linux Shell

A Simplified Implementation of the Linux shell using C system calls like
`fork()` and `execvp()`. Processes are executed concurrently through
forking.

## Using This Shell

Clone the shell with:
```
$ git clone https://github.com/austinpkugler/linux-shell.git
```
Compile using the included makefile:
```
$ make
```
Run the shell with
```
./shell
```
View implemented commands with:
```
shell> help
```

## Running Processes in the Background

Precede a process with `bg` to run it as a background process. Output for this
process is redirected to `/dev/null`.

Display all background processes with:
```
shell> bglist
```
Kill a background process using the ID displayed in `bglist`:
```
shell> bgkill 1
```

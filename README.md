# PA3: Shells Part II (Background Process)

**Purpose:** To reinforce the `fork()` and `execvp()` commands introduced in the last assignment.

## Introduction
In this assignment you extend the simple shell program written for PA2 to include background execution.

## Requirements for Background Execution
Many shells allow programs to be started in the background --- that is, the program is run but the shell continues to accept input from the user.

You will **implement a simplified version of background processing** that supports a **fixed number (in this case 5) of processes executing in the background**.

If the user types:
<pre>
gdshell> <strong>bg cat foo.txt</strong>
</pre>
your shell will start the command `cat` with the argument `foo.txt` in the background. That is, the program will execute and the shell will also continue to execute. Be sure to redirect your output to `/dev/null`.

For example if the user inputs
<pre>
gdshell> <strong>bg ls</strong>
</pre>
Run the command
```
ls > /dev/null
```
> Note: Recall the metacharacter `>` is a redirection operator that redirects the standard output of a program to a file. In this case above, the standard output of the program `ls` is redirected to the file `/dev/null`.

The command `bglist` will display a listing of all the commands currently executing in the background, similar to:
```
gdshell> bg sleep 10
gdshell> bg sleep 20
gdshell> bglist
0: sleep
1: sleep
Total Background Jobs: 2
```

In this case, there are 2 background jobs, both running the program `sleep`.

The command
```
gdshell> bgkill 1
```
will send the **TERM** signal to **job 1** to terminate that job. See the `man` page for the `kill()` system call for details.

Your shell must remove jobs from `bglist` when they have terminated. Read the `man` page for the `waitpid()` system call. I suggest using the `WNOHANG` option. Remember that a process can terminate either by completing or by being killed. Be sure to allow a new process to take the place of the old one if this occurs.

### Instructor will be Looking for the Following
* Commands beginning with `bg` run properly 
   * `bg` is removed from the command before it is run
   * no output is displayed, and
   * the shell does not wait
* List of active background processes shown when the user enters `bglist`
* Process that have ended on their own are automatically removed from `bglist`
* `bgkill` removes the process from `bglist` and actually kills the process.

## Submit
Make sure code is committed and pushed to Github. Submit GitHub URL in Canvas.

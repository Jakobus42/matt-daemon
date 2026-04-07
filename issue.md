Hello,

Jakob and I are currently working on the [matt-daemon](https://projects.intra.42.fr/projects/42cursus-matt-daemon) project, and we have found two requirements, we believe are outdated when compared to the modern implementations.

1. Only one daemon instance
Subject requirements:
> Only one daemon instance should be able to run at once.

> When attempting to run a second daemon whereas one instance is already running,
> an error message indicating a creation/file opening on `matt_daemon.lock` attempt
> must pop.

Subject example:
```
# ./Matt_daemon
# ./Matt_daemon
Can't open :/var/lock/matt_daemon.lock
# ps aux | grep Matt
root 8062 98.7 0.0 15172 2364 ? Rs 15:00 0:46 ./Matt_daemon
# tail -n 8 /var/log/matt_daemon/matt_daemon.log
[11/01/2016-15:00:25] [ INFO ] - Matt_daemon: Started.
[11/01/2016-15:00:25] [ INFO ] - Matt_daemon: Creating server.
[11/01/2016-15:00:25] [ INFO ] - Matt_daemon: Server created.
[11/01/2016-15:00:25] [ INFO ] - Matt_daemon: Entering Daemon mode.
[11/01/2016-15:00:25] [ INFO ] - Matt_daemon: started. PID: 8062.
[11/01/2016-15:00:26] [ INFO ] - Matt_daemon: Started.
[11/01/2016-15:00:26] [ ERROR ] - Matt_daemon: Error file locked.
[11/01/2016-15:00:26] [ INFO ] - Matt_daemon: Quitting
```

The issue is that while a `Matt_daemon` instance is running, the log file (`/var/log/matt_daemon/matt_daemon.log`) is actively being used by that process.
If we attempt to start a second process, it writes some messages into the log file. This is generally not considered safe, and is a race condition. In order to do this properly there would need to be some kind of communication to happen between the two processes, which does not seem to be the goal of the subject.
We would suggest, that this error should be printed to stderr instead, so that the user can see the error directly after executing the command.

Small side note: The subject asks the executeable to be named `MattDaemon` but in the examples it's `Matt_daemon`.

2. Lock File
Subject requirements:

> A `matt_daemon.lock` file must be created in /var/lock/ when the daemon starts.

> When the daemon shuts down, the matt_daemon.lock file must be deleted.

> [...] The correct use of
> fork, chdir, flock (not LOCK_SH), and signal will be checked during
> the evaluation.

Those requirements are a little bit contradictory.

We are asked to 1. create a lock file 2. lock it with flock 3. delete the file after use.

This is generally considered very bad practice, as it allows for the following to happen:

1. Process A starts and locks the file
2. Process A stops and unlocks the file
3. Process B starts, sees the existing lock file and locks it
4. Process A deletes the lock file
5. Process C starts, sees that there is no existing lock file, therefore creates a new one and locks it

=> Process B and C are running and both think they have a valid lock.

Due to the possibility of this race condition it is generally discouraged to clean up the lock file after use. Normally it should just reside in the file system, and be ready for future use.
Therefore we would like to suggest removing the deletion requirement.

3. Coplien Form
Subject requirements:
> Your classes must respect the **Coplien form**.

> You must code in C++ (any version) and turn-in a Makefile (respecting the regular
> rules).

Whilst in the past it encouraged to use the Coplien form for C++ classes, in modern C++ it is not recommended.
In modern C++ it is suggested to design classes with the rule of three/five/zero, depending on the purpose of the class. (Ref: https://en.cppreference.com/w/cpp/language/rule_of_three.html)

We would suggest either removing this requirement, or rewording it to something like: Your classes must respect the Coplien form, or equivalent depending on the version of C++ you have chosen for the project.

Best,
Jakob & Simon

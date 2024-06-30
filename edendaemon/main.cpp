#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <syslog.h>
#include <signal.h>

// Signal handler for SIGTERM
void signal_handler(int signum) {
    if (signum == SIGTERM) {
        syslog(LOG_INFO, "Daemon edend terminated.");
        closelog();
        exit(EXIT_SUCCESS);
    }
}

void daemonize() {
    pid_t pid, sid;

    // Fork off the parent process
    pid = fork();
    if (pid < 0) {
        exit(EXIT_FAILURE);
    }

    // If we got a good PID, then we can exit the parent process.
    if (pid > 0) {
        exit(EXIT_SUCCESS);
    }

    // Change the file mode mask
    umask(0);

    // Open a log for the daemon
    openlog("edend_daemon", LOG_NOWAIT | LOG_PID, LOG_USER);
    syslog(LOG_INFO, "Daemon edend started.");

    // Create a new SID for the child process
    sid = setsid();
    if (sid < 0) {
        syslog(LOG_ERR, "Could not create process group leader.");
        exit(EXIT_FAILURE);
    }

    // Change the current working directory
    if ((chdir("/")) < 0) {
        syslog(LOG_ERR, "Could not change working directory to root.");
        exit(EXIT_FAILURE);
    }

    // Close out the standard file descriptors
    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);

    // Signal handling
    signal(SIGTERM, signal_handler);
}

int main() {
    daemonize();

    // Main loop for the daemon
    while (true) {
        // Daemon code here (e.g., monitoring tasks, handling requests, etc.)
        syslog(LOG_INFO, "Daemon edend is running.");
        sleep(60); // Sleep for 60 seconds
    }

    return EXIT_SUCCESS;
}

#include <stdio.h>
#include <fcntl.h>
#include <sys/file.h>

#define LOCKFILE "lockfile.lock"

int main() {
	// Process A starts and locks file
    printf("[A] Locking file\n");
    int a_fd = open(LOCKFILE, O_CREAT | O_RDWR | O_CLOEXEC, 0644);
    if (a_fd == -1)
    {
    	printf("[A] Failed to open lockfile\n");
    	return 1;
    }

    int a_lock = flock(a_fd, LOCK_EX | LOCK_NB);
    if (a_lock == -1)
    {
    	printf("[A] Failed to lock lockfile\n");
    	return 1;
    }

    // Process A starts quit
	flock(a_fd, LOCK_UN);

    printf("[B] Locking file\n");
	// Process B starts before A is able to delete the lock file
	int b_fd = open(LOCKFILE, O_CREAT | O_RDWR | O_CLOEXEC, 0644);
	if (b_fd == -1)
	{
		printf("[B] Failed to open lockfile\n");
		return 1;
	}

	int b_lock = flock(b_fd, LOCK_EX | LOCK_NB);
    if (b_lock == -1)
    {
    	printf("[B] Failed to lock lockfile\n");
    	return 1;
    }

	
}

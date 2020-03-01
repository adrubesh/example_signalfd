#include <signal.h>
#include <sys/signalfd.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int
main (int argc, char **argv)
{
        /* man signalfd */
        sigset_t mask;

        /* create our signal mask for signals we want to catch in polling loop */
        sigemptyset(&mask);
        sigaddset(&mask, SIGINT);
        sigaddset(&mask, SIGQUIT);

        /* block the default action for signals we want to catch */
        assert(sigprocmask(SIG_BLOCK, &mask, NULL) != -1);

        int sfd;
        assert((sfd = signalfd(-1, &mask, 0)) != -1);

        struct signalfd_siginfo fdsi;
        size_t s;

        while(1) {
                /* read the signalfd and process the results */
                s = read(sfd, &fdsi, sizeof(struct signalfd_siginfo));
                assert(s == sizeof(struct signalfd_siginfo));

                if (fdsi.ssi_signo == SIGINT) {
                        printf("INT!\n");
                        break;
                } else if (fdsi.ssi_signo == SIGQUIT) {
                        printf("QUIT!\n");
                        break;
                }
        }

        exit(0);
}



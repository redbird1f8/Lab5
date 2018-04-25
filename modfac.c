#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include <stdbool.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/wait.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <signal.h>
#include <fcntl.h>
#include <errno.h>

#include <pthread.h>
#include <getopt.h>




pthread_mutex_t mut = PTHREAD_MUTEX_INITIALIZER;

int total_fact = 1;
struct FactArgs {
    int begin;
    int end;
    int mod;
};

int Fact(const struct FactArgs *args) {
    int fact = 1;
    printf("%d %d\n", args->begin, args->end);
    // TODO: your code here 
    for(int i = args->begin; i <= args->end; ++i)
    {
        pthread_mutex_lock(&mut);
        total_fact = (total_fact * (i%(args->mod)))%(args->mod);
        pthread_mutex_unlock(&mut);
    }
    return total_fact;
}

void *ThreadFact(void *args) {
    struct FactArgs *fact_args = (struct FactArgs *)args;
    return (void *)(size_t)Fact(fact_args);
}

int main(int argc, char **argv) {
    /*
     *  TODO:
     *  pnum by command line arguments
     *  mod by command line arguments
     *	k by command line arguments
     */

    int pnum = -1;
    int mod = -1;
    int k = -1;
    while (true) {
        int current_optind = optind ? optind : 1;

        static struct option options[] = {
            {"k", required_argument, 0, 0},
            {"pnum", required_argument, 0, 0},
            {"mod", required_argument, 0, 0},
            {0, 0, 0, 0}};

        int option_index = 0;
        int c = getopt_long(argc, argv, "", options, &option_index);

        if (c == -1) break;

        switch (c) {
            case 0:
                switch (option_index) {
                    case 0:
                        {
                            k = atoi(optarg);
                            if(k <= 0)
                            {
                                return 1;
                            }
                            // your code here
                            // error handling
                            break;
                        }
                    case 1:
                        {
                            pnum = atoi(optarg);
                            // your code here
                            // error handling
                            if(pnum <= 0)
                            {
                                return 1;
                            }
                            break;
                        }
                    case 2:
                        {
                            mod = atoi(optarg);
                            // your code here
                            // error handling
                            if(mod <= 0)
                            {
                                return 1;
                            }
                            break;
                        }
                    defalut:
                        printf("Index %d is out of options\n", option_index);
                }
                break;
//            case 'k':
//                {
//                    k = atoi(optarg);
//                    if(k <= 0)
//                    {
//                        return 1;
//                    }
//                    break;
//                }
                
            case '?':
                break;

            default:
                printf("getopt returned character code 0%o?\n", c);
        }
    }

    if (optind < argc) {
        printf("Has at least one no option argument\n");
        return 1;
    }

    if (k == -1 || mod == -1 || pnum == -1 || optind == 1) {
        printf("Usage: %s --k \"num\" --pnum \"num\" --mod \"num\" \n",
                argv[0]);
        return 1;
    }

    pthread_t *threads = malloc(sizeof(pthread_t) * pnum);

    /*
     * TODO:
     * your code here
     * Generate array here
     */

    struct FactArgs *args = malloc(sizeof(struct FactArgs) * pnum);
    int tc = k/pnum;
    for(int i = 0; i < pnum; ++i)
    {
        args[i].mod = mod;
        args[i].begin = tc*i + 1;
        if(i == pnum - 1)
        {
            args[i].end = tc*(i+1) + k%pnum;
        }
        else
        {
            args[i].end = tc*(i+1);
        }
    }

    struct timeval start_time;
    gettimeofday(&start_time, NULL);

    for (uint32_t i = 0; i < pnum; ++i) {
        if (pthread_create(&threads[i], NULL, ThreadFact, (void *)&(args[i]))) {
            printf("Error: pthread_create failed!\n");
            return 1;
        }
    }

    //int total_fact = 1;
    int tnn = pnum;
    for (uint32_t i = 0; i < tnn; ++i) {
        int fact = 1;
        pthread_join(threads[i], (void **)&fact);
        printf("Fact: %d\n", fact);
//        total_fact = (total_fact * fact)%mod;
    }
    struct timeval finish_time;
    gettimeofday(&finish_time, NULL);

    double elapsed_time = (finish_time.tv_sec - start_time.tv_sec) * 1000.0;
    elapsed_time += (finish_time.tv_usec - start_time.tv_usec) / 1000.0;




    free(args);
    free(threads);
    printf("Total: %d\n", total_fact);
    printf("Elapsed time: %fms\n", elapsed_time);
    return 0;
}

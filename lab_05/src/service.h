#ifndef _SERVICE_H_
#define _SERVICE_H_

#define T1_MIN 1
#define T1_MAX 5
#define T2_MIN 0
#define T2_MAX 3
#define T3_MIN 0
#define T3_MAX 4
#define T4_MIN 0
#define T4_MAX 1

#define Q1_PASSED 1000

typedef struct
{
    int status;
    int len_1;
    int len_2;
    int count_len;
    int passed;
    int passed_1;
    int passed_2;
    float timer;
    float total;
    float process_time;
} service_t;

void print_menu(void);
void print_addresses(void);
int change_time(int *t1_min, int *t1_max, int *t2_min, int *t2_max,
                int *t3_min, int *t3_max, int *t4_min, int *t4_max);
int process(void *data_1, void *data_2, int mode, int adresses_print_flag,
			int t1_min, int t1_max, int t2_min, int t2_max,
			int t3_min, int t3_max, int t4_min, int t4_max);

#endif // _SERVICE_H_

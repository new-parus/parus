#include "my_time.h"
#include "network_test.h"
extern int comm_rank;
extern int comm_size;

Test_time_result_type* test(int mes_length,int num_repeats);
Test_time_result_type* test_noise(int mes_length, int num_repeats, int num_noise_repeats, int loading, int num_processors);
Test_time_result_type* test_noise_blocking(int mes_length, int num_repeats, int num_noise_repeats, int loading, int num_processors);

int random_choice(int proc1, int proc2, int num_processors, int* processors);
int is_rand(int rank, int *rand_processors, int num_processors);

/*
 * Structure to keep most of test data together
 */
typedef struct tag_test_data
{
	Test_time_result_type *times;
	px_my_time_type **tmp_results;
	char **send_data;
	char **recv_data;
	char **send_data_noise;
	char **recv_data_noise;
	int *processors;
} test_data;

void init_test_data( test_data* td );
void clear_test_data( test_data* td );
void clear_more_test_data( test_data* td, int i );
bool alloc_test_data( test_data* td, int mes_length, int num_repeats, int loading, int num_processors );
void free_test_data( test_data* td );
int random_choice( int proc1, int proc2, int num_processors, int* processors );
int is_rand(int rank, int *rand_processors, int num_processors);

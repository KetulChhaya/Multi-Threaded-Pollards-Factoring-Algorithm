#include <gmp.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

void pollards_rho(mpz_t n, mpz_t factor) {
    if (mpz_cmp_ui(n, 1) == 0) return;
    if (mpz_probab_prime_p(n, 25)) {
        mpz_set(factor, n);
        return;
    }

    mpz_t x, y, d, one, c, temp;
    mpz_inits(x, y, d, one, c, temp, NULL);

    mpz_set_ui(one, 1);
    mpz_set_ui(c, rand() % 10 + 1); // Random constant
    mpz_set_ui(x, rand() % 10 + 2); // Random start
    mpz_set(y, x);
    mpz_set_ui(d, 1);

    while (mpz_cmp(d, one) == 0) {
        mpz_mul(x, x, x);
        mpz_add(x, x, c);
        mpz_mod(x, x, n);

        mpz_mul(y, y, y);
        mpz_add(y, y, c);
        mpz_mod(y, y, n);
        mpz_mul(y, y, y);
        mpz_add(y, y, c);
        mpz_mod(y, y, n);

        mpz_sub(temp, x, y);
        mpz_abs(temp, temp);

        mpz_gcd(d, temp, n);
    }

    if (mpz_cmp(d, n) != 0) {
        mpz_set(factor, d);
    } else {
        mpz_set_ui(factor, 0);
    }

    mpz_clears(x, y, d, one, c, temp, NULL);
}

void pollards_p_minus_one(mpz_t n, mpz_t factor) {
    if (mpz_cmp_ui(n, 1) == 0) return;
    if (mpz_probab_prime_p(n, 25)) {
        mpz_set(factor, n);
        return;
    }

    mpz_t a, d, one, temp;
    mpz_inits(a, d, one, temp, NULL);

    mpz_set_ui(one, 1);
    mpz_set_ui(a, 2);
    mpz_set_ui(d, 1);

    unsigned long B = 100000; //Max Bound
    for (unsigned long i = 2; i <= B; i++) {
        mpz_set_ui(temp, i);
        mpz_powm(a, a, temp, n);
        mpz_sub_ui(temp, a, 1);
        mpz_gcd(d, temp, n);

        if (mpz_cmp(d, one) > 0 && mpz_cmp(d, n) < 0) {
            mpz_set(factor, d);
            break;
        }
    }

    if (mpz_cmp(d, one) <= 0 || mpz_cmp(d, n) >= 0) {
        mpz_set_ui(factor, 0);
    }

    mpz_clears(a, d, one, temp, NULL);
}

// Shared variables
int factor_found = 0; // Flag to indicate if a factor has been found
pthread_mutex_t lock; 

// Threads Data Struct 
typedef struct {
    mpz_t n;
    mpz_t factor;
    int method; // 1: Pollard's Rho, 2: Pollard's P-1
} thread_data;

// Thread function
void *run_pollard_method(void *arg) {
    thread_data *data = (thread_data *)arg;

    if (data->method == 1) {
        pollards_rho(data->n, data->factor);
    } else if (data->method == 2) {
        pollards_p_minus_one(data->n, data->factor);
    }

    pthread_mutex_lock(&lock);
    if (!factor_found && mpz_cmp_ui(data->factor, 0) != 0) {
        factor_found = 1;
        gmp_printf("Factor found using Method %d : %Zd\n\n\n", data->method, data->factor);
    }
    pthread_mutex_unlock(&lock);

    return NULL;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <modulus_value>\n", argv[0]);
        return 1;
    }

    mpz_t n;
    mpz_init(n);

    if (mpz_set_str(n, argv[1], 10) != 0) {
        fprintf(stderr, "Invalid modulus input.\n");
        mpz_clear(n);
        return 1;
    }

    gmp_printf("Current Processing Modulus: %Zd\n", n);

    pthread_t threads[2];
    thread_data data[2];
    pthread_mutex_init(&lock, NULL);

    mpz_inits(data[0].factor, data[1].factor, NULL);

    // Initialize thread data for Pollard's Rho
    mpz_init_set(data[0].n, n);
    data[0].method = 1;

    // Initialize thread data for Pollard's P-1
    mpz_init_set(data[1].n, n);
    data[1].method = 2;

    // 2 separate threads creation
    pthread_create(&threads[0], NULL, run_pollard_method, &data[0]);
    pthread_create(&threads[1], NULL, run_pollard_method, &data[1]);

    // Joining and Waiting for Threads to complete
    pthread_join(threads[0], NULL);
    pthread_join(threads[1], NULL);

    if (!factor_found) {
        printf("No factors found by any of the methods\n");
    }

    mpz_clears(n, data[0].n, data[1].n, data[0].factor, data[1].factor, NULL);
    pthread_mutex_destroy(&lock);

    return 0;
}

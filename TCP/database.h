#include<json-c/json.h>

typedef struct person_data {
    char name[32];
    char surname[32];
    char email[128];
    char photo[256];
    char address[64];
    char education[64];
} person_data;

int test_database();

int find_user_by_education(char* education, person_data persons[]);
int find_user_by_email(char* email, person_data* person);
#include<json-c/json.h>

int test_database();

// Listar todas as pessoas formadas em um determinado curso
int list_user_by_education(char* education, char* res);

// Listar as habilidades dos perfis que moram em uma determinada cidade
int list_skills_by_city(char* city, char* res);

// Acrescentar uma nova experiência em um perfil
int add_skill(int pk, char* experience, char* res);

// Dado o email do perfil,retornar sua experiência
int list_experience_by_email(char* email, char* res);

// Listar todas as informações de todos os perfis
int list_all(char* res);

// Dado o email de um perfil, retornar suas informações
int get_user_by_email(char* email, char* res);
#include<stdio.h>
#include<string.h>
#include"database.h"

#define BUFFER_SIZE 2048

// Auxiliar function used only inside database.c
void read_database_file(struct json_object **database){
	char buffer[BUFFER_SIZE];
	struct json_object *parsed_json;
	FILE *fp;

	fp = fopen("database.json","r");
	fread(buffer, 2048, 1, fp);
	fclose(fp);
	parsed_json = json_tokener_parse(buffer);
	json_object_object_get_ex(parsed_json, "database", database);
}

int test_database() {
	struct json_object *database;
	int n_total;

	printf("Testing database\n");
	read_database_file(&database);
	
	printf("%s\n",  json_object_to_json_string_ext(database, JSON_C_TO_STRING_PRETTY));
	n_total = json_object_array_length(database);
	printf("Found %d persons in database\n", n_total);
	
	return 1;
}

int my_strcmp(const void * a, const void * b){
	json_object * const *pa, * const *pb;
	pa = (json_object* const*)a;
	pb = (json_object* const*)b;
	return strcmp(json_object_get_string(*pa),json_object_get_string(*pb));
}

// --------------------------------------------------------------------------- //

// Listar todas as pessoas formadas em um determinado curso
int list_user_by_education(char* education, char* res) {
	struct json_object *database;
	struct json_object *persons;
	int n_persons;

	read_database_file(&database);
	n_persons = json_object_array_length(database);
	persons = json_object_new_array();

	for(int i = 0; i < n_persons; i++) {
		struct json_object *dbperson;
		struct json_object *dbedcuation;

		dbperson = json_object_array_get_idx(database, i);
		json_object_object_get_ex(dbperson, "education", &dbedcuation);
		if (strcmp(json_object_get_string(dbedcuation), education) == 0){
			json_object_array_add(persons, dbperson);
		}
	}
	strcpy(res, json_object_to_json_string_ext(persons, JSON_C_TO_STRING_PRETTY));
	return json_object_array_length(persons);
}

// Listar as habilidades dos perfis que moram em uma determinada cidade
int list_skills_by_city(char* city, char* res){
	struct json_object *database;
	struct json_object *skills;
	int n_persons;

	read_database_file(&database);
	n_persons = json_object_array_length(database);
	skills = json_object_new_array();

	for(int i = 0; i < n_persons; i++) {
		struct json_object *dbperson;
		struct json_object *dbcity;

		dbperson = json_object_array_get_idx(database, i);
		json_object_object_get_ex(dbperson, "city", &dbcity);

		if (strcmp(json_object_get_string(dbcity), city) == 0){
			struct json_object *dbskills;
			int n_skills;

			json_object_object_get_ex(dbperson, "skills", &dbskills);
			n_skills = json_object_array_length(dbskills);

			for(int j = 0; j < n_skills; j++) {
				struct json_object *dbskill;

				dbskill = json_object_array_get_idx(dbskills, j);
				if (json_object_array_bsearch(dbskill, skills, my_strcmp) == NULL){
					json_object_array_add(skills, dbskill);
					json_object_array_sort(skills, my_strcmp);
					strcpy(res, json_object_to_json_string_ext(skills, JSON_C_TO_STRING_PRETTY));
					printf("%s\n", res);
				}
			}
		}
	}
	
	strcpy(res, json_object_to_json_string_ext(skills, JSON_C_TO_STRING_PRETTY));
	return json_object_array_length(skills);
}

// Acrescentar uma nova experiência em um perfil
int add_skill(int pk, char* res);

// Dado o email do perfil,retornar sua experiência
int find_user_by_education(char* email, char* res);

// Listar todas as informações de todos os perfis
int list_all(char* res);

// Dado o email de um perfil, retornar suas informações
int get_user_by_email(char* email, char* res){
	struct json_object *database;
	int n_persons;

	read_database_file(&database);
	n_persons = json_object_array_length(database);

	for(int i = 0; i < n_persons; i++) {
		struct json_object *dbperson;
		struct json_object *dbemail;

		dbperson = json_object_array_get_idx(database, i);
		json_object_object_get_ex(dbperson, "email", &dbemail);
		if (strcmp(json_object_get_string(dbemail), email) == 0){
			strcpy(res, json_object_to_json_string_ext(dbperson, JSON_C_TO_STRING_PRETTY));
			return 1;
		}
	}
	return 0;
}
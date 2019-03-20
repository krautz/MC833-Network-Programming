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

void construct_person_data(struct json_object *dbperson, person_data* person){
	struct json_object *data;

	json_object_object_get_ex(dbperson, "name", &data);
	strcpy(person->name, json_object_get_string(data));

	json_object_object_get_ex(dbperson, "surname", &data);
	strcpy(person->surname, json_object_get_string(data));

	json_object_object_get_ex(dbperson, "email", &data);
	strcpy(person->email, json_object_get_string(data));

	json_object_object_get_ex(dbperson, "address", &data);
	strcpy(person->address, json_object_get_string(data));

	json_object_object_get_ex(dbperson, "photo", &data);
	strcpy(person->photo, json_object_get_string(data));

	json_object_object_get_ex(dbperson, "education", &data);
	strcpy(person->education, json_object_get_string(data));
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


int find_user_by_education(char* education, person_data persons[]) {
	struct json_object *database;
	struct json_object *dbperson;
	struct json_object *dbedcuation;
	int n_total;
	int n_persons = 0;

	read_database_file(&database);
	n_total = json_object_array_length(database);

	for(int i = 0; i < n_total; i++) {
		dbperson = json_object_array_get_idx(database, i);
		json_object_object_get_ex(dbperson, "education", &dbedcuation);
		if (strcmp(json_object_get_string(dbedcuation), education) == 0){
			construct_person_data(dbperson, persons + n_persons);
			n_persons++;
		}
	}
	return n_persons;
}


int find_user_by_email(char* email, person_data* person) {
	struct json_object *database;
	struct json_object *dbperson;
	struct json_object *dbemail;
	int n_total;

	read_database_file(&database);
	n_total = json_object_array_length(database);

	for(int i = 0; i < n_total; i++) {
		dbperson = json_object_array_get_idx(database, i);
		json_object_object_get_ex(dbperson, "email", &dbemail);
		if (strcmp(json_object_get_string(dbemail), email) == 0){
			construct_person_data(dbperson, person);
			return 1;
		}
	}
	return 0;
}
#include<stdio.h>
#include<string.h>
#include"database.h"

#define BUFFER_SIZE 1048576

// Auxiliar function used only inside database.c
void read_database_file(struct json_object **database){
	char buffer[BUFFER_SIZE];
	struct json_object *parsed_json;
	FILE *fp;

	fp = fopen("database.json","r");
	fread(buffer, BUFFER_SIZE, 1, fp);
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

// --------------------------------------------------------------------------- //

// Dado o email do perfil,retornar sua nome, sobrenome e foto
int list_person_info_by_email(char* email, char* res){
	struct json_object *database;
	int n_persons;
    struct json_object *personInfo;

    personInfo = json_object_new_object();

	read_database_file(&database);
	n_persons = json_object_array_length(database);

	for(int i = 0; i < n_persons; i++) {
		struct json_object *dbperson;
		struct json_object *dbemail;

		dbperson = json_object_array_get_idx(database, i);
		json_object_object_get_ex(dbperson, "email", &dbemail);

		if (strcmp(json_object_get_string(dbemail), email) == 0){
			struct json_object *dbname;
            struct json_object *dbsurname;
            struct json_object *dbphoto;

			json_object_object_get_ex(dbperson, "name", &dbname);
            json_object_object_add(personInfo, "name", dbname);

            json_object_object_get_ex(dbperson, "surname", &dbsurname);
            json_object_object_add(personInfo, "surname", dbsurname);

            json_object_object_get_ex(dbperson, "photo", &dbphoto);
            json_object_object_add(personInfo, "photo", dbphoto);

			strcpy(res, json_object_to_json_string_ext(personInfo, JSON_C_TO_STRING_PRETTY));
			return 1;
		}
	}
	return 0;
}

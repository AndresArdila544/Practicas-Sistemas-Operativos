#include <stdio.h> 
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
//#include <stdio_ext.h>










#define CAPACITY  5000

int registros = 0;

struct dogType{
   char nombre[32];
   char tipo[32];
   int edad;
   char raza[16];
   int estatura;
   float peso;
   char sexo;
   int next;
};

int menu(int opcion){
	printf("                                                                   \n");
	printf("                       __                                          \n");
	printf("                     .'  '.                                        \n");
	printf("                 _.-'/  | o .                                      \n");
	printf("      ,      _.-'  ,|   /    `-.                                   \n");
	printf("     /'   .-''       `--''-.__.'=========================          \n");
	printf("    ( '-'`        .___.--._)                             |         \n");
	printf("     '            .'                                     |         \n");
	printf("      |     /,_.-'       Veterinaria Patas               |         \n");
	printf("    _/   _.'(                                            |         \n");
	printf("   /  ,-'|(  '                                           |         \n");
	printf("  (  (   |  `-'        1. Ingresar registro           /`-._        \n");
	printf("   `-'   |             2. Ver registro              _/,.._/        \n");
	printf("         |             3. Borrar registro        ,-'   ,  `-:,.-') \n");
	printf("         |             4. Buscar registro       : o ):';     _  {  \n");
	printf("         |             5. Salir                  `-.  `' _,.-(`-.) \n");
	printf("         |                                           `((`|`-,.-'   \n");
	printf("          ===============================================          \n");
	do{
		printf("\n          Opcion: ");
		scanf("%i", &opcion);
	}while(opcion < 1 || opcion > 5);
	return opcion;
}

//Hash Table

unsigned long hash_function(char* str) {
    unsigned long i = 0;
    int j;
    for (j = 0; str[j]; j++)
        i += str[j];
    return i % CAPACITY;
}
 
typedef struct Ht_item Ht_item;
 
// Define the Hash Table Item here
struct Ht_item {
    int head;
    int tail;
};
 
typedef struct HashTable HashTable;
 
// Define the Hash Table here
struct HashTable {
    // Contains an array of pointers
    // to items
    Ht_item** items;
    int size;
    int count;
};
 
Ht_item* create_item(int key, int value) {
    // Creates a pointer to a new hash table item
    Ht_item* item = (Ht_item*) malloc (sizeof(Ht_item));
    item->head = key;
    item->tail = value;
    return item;
}
 
HashTable* create_table(int size) {
    // Creates a new HashTable
    int i;
    HashTable* table = (HashTable*) malloc (sizeof(HashTable));
    table->size = size;
    table->count = 0;
    table->items = (Ht_item**) calloc (table->size, sizeof(Ht_item*));
    for (i = 0; i < table->size; i++)
        table->items[i] = NULL;
    return table;
}
 
void free_item(Ht_item* item) {
    // Frees an item
    free(&item->head);
    free(&item->tail);
    free(item);
}
 
void free_table(HashTable* table) {
    // Frees the table
    int i;
    for (i = 0; i < table->size; i++) {
        Ht_item* item = table->items[i];
        if (item != NULL)
            free_item(item);
    }
    free(table->items);
    free(table);
}

void print_table(HashTable* table) {
	int i;
    printf("\nHash Table\n-------------------\n");
    for (i = 0; i < table->size; i++)
        if (table->items[i])
            printf("Index:%d, Key:%d, Value:%d\n", i, table->items[i]->head, table->items[i]->tail);
    printf("-------------------\n\n");
}
 
HashTable* ht_insert(HashTable* table,char* key, int value) {
    registros++;
    unsigned long index = hash_function(key);
    Ht_item* current_item = table->items[index];
    if (current_item == NULL) {
        // Key does not exist.
        if (table->count == table->size) {
            // Hash Table Full
            printf("Insert Error: Hash Table is full\n");
            return NULL;
        }
        // Insert directly
        Ht_item* item = create_item(value, value);
        table->items[index] = item; 
		//print_table(table);
        table->count++;
		return table;
    }else {
        // Scenario 1: We only need to update value
        int posicion = current_item->tail, r;
	    FILE* archivo;
	    archivo = fopen("dataDogs.dat", "rb+");
	    if (archivo == NULL){
		perror("error fopen");
		exit(-1);
   	}
	    struct dogType mascota;
	    r = fseek(archivo, posicion, SEEK_SET);
	    if(r != 0){
			perror("Error fseek");
			exit(-1);
		}
	    r = fread(&mascota, sizeof(struct dogType), 1, archivo);
	    if(r = 0){
			perror("Error fread");
			exit(-1);
		}
        mascota.next = value;
	    current_item->tail = value;
	    r = fseek(archivo, posicion, SEEK_SET);
	    if(r != 0){
			perror("Error fseek");
			exit(-1);
		}
	    r = fwrite(&mascota, sizeof(struct dogType), 1, archivo);
	    if(r = 0){
			perror("Error fwrite");
			exit(-1);
		}
 	    r = fclose(archivo);
 	    if(r < 0){
			perror("Error fclose");
			exit(-1);
		}
	    return table;
    }
}
 
Ht_item* ht_search(HashTable* table, char* key) {
    // Searches the key in the hashtable
    // and returns NULL if it doesn't exist
    int index = hash_function(key);
    Ht_item* item = table->items[index];
    // Ensure that we move to a non NULL item
    if (item != NULL)
        return item;
    return NULL;
}

int verMascota(struct dogType mascota){
	if(mascota.nombre[0] == '*')
		return 1;
	else{
		printf("          Nombre: %s\n", mascota.nombre);
		printf("          Tipo: %s\n", mascota.tipo);
		printf("          Edad: %i\n", mascota.edad);
		printf("          Raza: %s\n", mascota.raza);
		printf("          Estatura: %i\n", mascota.estatura);
		printf("          Peso: %f\n", mascota.peso);
		printf("          Sexo: %c\n", mascota.sexo);
		return 0;
	}
}
 
void print_search(HashTable* table, char* key) {
    Ht_item* val = ht_search(table, key);
    if (val == NULL) {
        printf("          La mascota con el ID: %s no esta registrado en la base de datos\n", key);
        return;
    }
    else {
		FILE* archivo;
		archivo = fopen("dataDogs.dat", "ab+");
		if(archivo == NULL){
			perror("Error fopen");
			exit(-1);
		}
		struct dogType mascota;
		int pos = val->head, r;
		r = fseek(archivo, val->head, SEEK_SET);
		if(r != 0){
			perror("Error fseek");
			exit(-1);
		}
		r = fread(&mascota, sizeof(struct dogType), 1, archivo);
		if(r = 0){
			perror("Error fread");
			exit(-1);
		}
	
		while(mascota.next != -1){
			if(strcmp(mascota.nombre, key) == 0){
				printf("\n          ID: %d\n", pos);
				verMascota(mascota);
			}
			r = fseek(archivo,mascota.next, SEEK_SET);
			if(r != 0){
				perror("Error fseek");
				exit(-1);
			}
			pos = ftell(archivo);
			r = fread(&mascota, sizeof(struct dogType), 1, archivo);
			if(r = 0){
				perror("Error fread");
				exit(-1);
			}
			
		}
		if(strcmp(mascota.nombre, key) == 0){
			printf("\n          ID: %d\n", pos);
			verMascota(mascota);
		}
		r = fclose(archivo);
		if(r < 0){
			perror("Error fclose");
			exit(-1);
		}
    }
}

HashTable* guardarRegistro(HashTable* table, void *puntero){
	FILE* archivo;
	struct dogType* mascota;
	mascota = puntero;
	int r;
	
	archivo = fopen("dataDogs.dat", "ab+");
	if(archivo == NULL){
		perror("Error fopen");
		exit(-1);
	}
	r = fseek(archivo, 0, SEEK_END);
	if(r != 0){
		perror("Error fseek");
		exit(-1);
	}
	r = fwrite(puntero, sizeof(struct dogType), 1, archivo);
	if(r = 0){
		perror("Error fwrite");
		exit(-1);
	}
	int posicion = ftell(archivo)- sizeof(struct dogType);
	table = ht_insert(table, mascota->nombre, posicion);
	r = fclose(archivo);
	if(r < 0){
		perror("Error fclose");
		exit(-1);
	}
	return table;
}

HashTable* delete_item(HashTable* table, char* key, int code) {
	registros--;
    Ht_item* val = ht_search(table, key);
    if (val == NULL) {
        printf("          La mascota con el ID: %s no esta registrado en la base de datos\n", key);
        return NULL;
    }
    else {
		FILE* archivo;
		struct dogType mascota, eliminar;
		int current = val->head, r;
		
		archivo = fopen("dataDogs.dat", "rb+");
		if(archivo == NULL){
			perror("Error fopen");
			exit(-1);
		}
		r = fseek(archivo, val->head, SEEK_SET);
		if(r != 0){
			perror("Error fseek");
			exit(-1);
		}
		r = fread(&mascota, sizeof(struct dogType), 1, archivo);
		if(r = 0){
			perror("Error fread");
			exit(-1);
		}
		if(code == current){
			val->head = mascota.next;
			return table;
		}
		while(mascota.next != -1){
			if(mascota.next == code){
				r = fseek(archivo, mascota.next, SEEK_SET);
				if(r != 0){
					perror("Error fseek");
					exit(-1);
				}
				r = fread(&eliminar, sizeof(struct dogType), 1, archivo);
				if(r = 0){
					perror("Error fread");
					exit(-1);
				}
				int sig = eliminar.next;
				mascota.next = sig;
				r = fseek(archivo, current, SEEK_SET);
				if(r != 0){
					perror("Error fseek");
					exit(-1);
				}
				r = fwrite(&mascota, sizeof(struct dogType), 1, archivo);
				if(r = 0){
					perror("Error fwrite");
					exit(-1);
				}
				fclose(archivo);
				return table;
			}
			if(mascota.next == val->tail){
				r = fread(&mascota, sizeof(struct dogType), 1, archivo);
				if(r = 0){
					perror("Error fread");
					exit(-1);
				}
				mascota.next = -1;
				r = fseek(archivo, current, SEEK_SET);
				if(r != 0){
					perror("Error fseek");
					exit(-1);
				}
				r = fwrite(&mascota, sizeof(struct dogType), 1, archivo);
				if(r = 0){
					perror("Error fwrite");
					exit(-1);
				}
				val->tail = current;
				fclose(archivo);
				return table;
			}
			r = fseek(archivo, mascota.next, SEEK_SET);
			if(r != 0){
				perror("Error fseek");
				exit(-1);
			}
			current = ftell(archivo);
			r = fread(&mascota, sizeof(struct dogType), 1, archivo);
			if(r = 0){
				perror("Error fread");
				exit(-1);
			}
		}
		printf("          La mascota con el ID: %s no esta registrado en la base de datos\n", key);
		r = fclose(archivo);
		if(r < 0){
			perror("Error fclose");
			exit(-1);
		}
    }
}

int registrarMascota(void *puntero){
	int i;
	struct dogType *mascota;
	mascota = puntero;
	printf("\n          Nombre: ");
	scanf("%s", mascota->nombre);
	for(i = 0; mascota->nombre[i]; i++)
		if(i == 0)
			mascota->nombre[i] = toupper(mascota->nombre[i]);
		else
			mascota->nombre[i] = tolower(mascota->nombre[i]);
	printf("          Tipo: ");
	scanf("%s", mascota->tipo);
	printf("          Edad:");
	scanf("%i", &mascota->edad);
	printf("          Raza: ");
	//gets(mascota->raza);
	scanf("%s", mascota->raza);
	printf("          Estatura: ");
	scanf(" %i", &mascota->estatura);
	printf("          Peso: ");
	scanf(" %f", &mascota->peso);
	printf("          Sexo: ");
	scanf(" %c", &mascota->sexo);
	mascota->next = -1;
	return 0;
}

int mostrarHistoria(char nombre[], char id[]) {

	FILE *historia;
	int r, i;
	char dir[500] = "cd ", cwd[500], cwdCopia[500];
	
	if (getcwd(cwd, sizeof(cwd)) == NULL) {
       	perror("error getcwd()");
       	exit(-1);
    }
   	for(i = 0; cwd[i]; i++){
   		if(cwd[i] == 92) cwd [i] = 47;
   		cwdCopia[i] = cwd[i];
	}
	
	strcat(cwd, "/historias_clinicas/");
	strcat(cwd, id);
	strcat(cwd, "_");
	strcat(cwd, nombre);
	strcat(cwd, ".txt");

	historia = fopen(cwd, "a+");
	if(historia == NULL){
		perror("Error fopen");
		exit(-1);
	}
	
	r = fclose(historia);
	if(r < 0){
		perror("Error fclose");
		exit(-1);
	}
	
	strcat(dir, cwdCopia);
	strcat(dir, "/historias_clinicas/");
	strcat(dir, " && nano ");
	strcat(dir, id);
	strcat(dir, "_");
	strcat(dir, nombre);
	strcat(dir, ".txt");

	system(dir);
	return 0;
}

struct HashTable* hash_db(){
	FILE *ptr;
	HashTable* ht = create_table(CAPACITY);
	struct dogType dog; 
	int r;
	int count=0;
    ptr = fopen("dataDogs.dat","rb");
	if (ptr == NULL){
		perror("error fopen");
		return NULL;
   	}
	while (fread(&dog, sizeof(struct dogType), 1 ,ptr) != 0){
		if(dog.nombre[0] != '*'){
			printf("%d\n",count);
			
			int posicion = ftell(ptr)- sizeof(struct dogType);
			ht_insert(ht, dog.nombre , posicion);
			count++;
		}
	}
	r = fclose(ptr);
	if(r < 0){
		perror("Error fclose");
		exit(-1);
	}
	return ht; 
}

struct dogType leerEsctructura(int id, struct dogType dog){
	int r;
	FILE *ptr;
	ptr = fopen("dataDogs.dat", "rb");
	if (ptr == NULL){
		perror("error fopen");
		exit(-1);
   	}
	r = fseek(ptr, id, SEEK_SET);
	if(r != 0){
		perror("Error fseek");
		exit(-1);
	}
	r = fread(&dog, sizeof(struct dogType), 1, ptr);
	if(r = 0){
		perror("Error fread");
		exit(-1);
	}
	r = fclose(ptr);
	if(r < 0){
		perror("Error fclose");
		exit(-1);
	}
	return dog;
}

int borrarRegistro(int posicion){
	FILE* archivo;
	struct dogType dog;
	int r, i;
	archivo = fopen("dataDogs.dat", "r+b");
	if(archivo == NULL){
		perror("Error fopen");
				exit(-1);
	}
	r = fseek(archivo,posicion,SEEK_SET);
	if(r < 0){
		perror("Error fseek");
		exit(-1);
	}

	r = fread(&dog, sizeof(struct dogType), 1 ,archivo);
	if(r = 0){
		perror("Error fread");
		exit(-1);
	}
	
	for(i = 0; dog.nombre[i]; i++)
		dog.nombre[i] = ' ';
	dog.nombre[0] = '*';
	dog.edad = 0;
	dog.estatura = 0;
	dog.peso = 0;
	dog.sexo = ' ';
	for(i = 0; dog.tipo[i]; i++)
		dog.tipo[i] = ' ';
	for(i = 0; dog.raza[i]; i++)
		dog.raza[i] = ' ';
		
	r = fseek(archivo, posicion, SEEK_SET);
	if(r != 0){
		perror("Error fseek");
		exit(-1);
	}
	r = fwrite(&dog, sizeof(struct dogType), 1, archivo);
	if(r = 0){
		perror("Error fwrite");
		exit(-1);
	}
	r = fclose(archivo);
	if(r < 0){
		perror("Error fclose");
		exit(-1);
	}
	return 0;
}

int main(int argc, char** argv) {

	FILE *ptr;
	HashTable* ht = create_table(CAPACITY);
	printf("\n          Welcome\n");
	ht = hash_db();
	int opcion, validacion, id, i, h2;
	struct dogType *mascota, dog;
	char nombre[32], historia, charId[12];
	mascota = malloc(sizeof(struct dogType));
	if(mascota == NULL){
		perror("Error malloc");
		exit(-1);
	}
	do{
		opcion = menu(opcion);
	
		switch(opcion){
			case 1:
				validacion = registrarMascota((void *)mascota);
				if(validacion != 0){
					perror("Error registrando");
					exit(-1);
				}
				ht = guardarRegistro(ht,(void *)mascota);
				//__fpurge(stdin);
				printf("\n          Presiona enter para regresar al menu.");
				getchar();	
				break;
			case 2:
				printf("\n          Numero de registros: %i", registros);
				printf("\n          ID de la mascota que deseas ver: ");
				scanf("%i", &id);
				if(id % 100 != 0){
					//__fpurge(stdin);
					printf("          ID invalido.\n");
					printf("\n          Presiona enter para regresar al menu.\n");
					getchar();
					break;
				}
				dog = leerEsctructura(id, dog);
				validacion = verMascota(dog);
				if(validacion == 1){
					printf("          ID invalido. La mascota con ese ID ha sido eliminada del sistema.\n");
					printf("\n          Presiona enter para regresar al menu.\n");
					//__fpurge(stdin);
					getchar();
					break;
				}
				//__fpurge(stdin);
				printf("          Abrir historia clinica? (S: si | N: no): ");
				scanf("%c", &historia);
				if(historia == 's' || historia == 'S'){
					sprintf(charId, "%d", id);
					
					validacion = mostrarHistoria((char*)dog.nombre, charId);
					if(validacion != 0){
						perror("          Error abriendo historia");
						exit(-1);
					}
					//__fpurge(stdin);
					printf("\n          Presiona enter para regresar al menu.\n");
					getchar();
				}else{
					//__fpurge(stdin);
					printf("\n          Presiona enter para regresar al menu.\n");
					getchar();
				}
				break;
			case 3:
				printf("\n          Numero de registros: %i\n", registros);
				printf("\n          ID de la mascota que deseas borrar: ");
				scanf("%i", &id);
				if(id % 100 != 0){
					//__fpurge(stdin);
					printf("          ID invalido.\n");
					printf("\n          Presiona enter para regresar al menu.\n");
					getchar();
					break;
				}
				dog = leerEsctructura(id, dog);
				if(dog.nombre[0] != '*'){
					ht = delete_item(ht, (char *) dog.nombre, id);
					borrarRegistro(id);
					//__fpurge(stdin);
					printf("\n          Presiona enter para regresar al menu.\n");
					getchar();
					break;
				}
				else{
					//__fpurge(stdin);
					printf("          ID invalido. El ID ingresado ya ha sido eliminado del sistema.\n");
					printf("\n          Presiona enter para regresar al menu.\n");
					getchar();
					break;
				}
			case 4:
				printf("          Ingresa el nombre de la mascota: ");	
				scanf("%s", nombre);
				for(i = 0; nombre[i]; i++)
					if(i == 0)
						nombre[i] = toupper(nombre[i]);
					else
						nombre[i] = tolower(nombre[i]);
				print_search(ht, (char *)nombre);
				//__fpurge(stdin);
				printf("\n          Presiona enter para regresar al menu.\n");
				getchar();
				break;
			case 5:
				//__fpurge(stdin);
				printf("\n          Presiona enter para confirmar la salida.");
				getchar();
				free(mascota);
				break;	
		}
	}while(opcion != 5);

	return 0;
}
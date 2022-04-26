/*---- CONSTANTES, DECLARAÇÃO DE
PROTÓTIPOS DE FUNÇÕES E STRUCTS ------*/
/*Sara Pinheiro ist102507*/

/*for command 'a'*/
#define ID_M 4
/*para ids de airports */
#define PAIS_M 31
#define CIDADE_M 51
#define AERO_M 40

#define RES_ID 11

/*errors*/
#define ERR_A1 "duplicate airport\n"
#define ERR_A2 "invalid airport ID\n"
#define ERR_A3 "too many airports\n"
#define ERR_L1 "no such airport ID\n"
/* also 'p' command error */
#define ERR_V1 "invalid flight code\n"
#define ERR_V2 "flight already exists\n"
#define ERR_V3 ": no such airport ID\n"
#define ERR_V4 "too many flights\n"
#define ERR_V5 "invalid date\n"
/*also 't' command error*/
#define ERR_V6 "invalid duration\n"
#define ERR_V7 "invalid capacity\n"
#define ERR_E1 "not found\n"
#define OUTP_CP "%s %s %02d-%02d-%d %02d:%02d\n"
/* formato do output do comando 'c' e 'p'*/
#define OUTP_V "%s %s %s %02d-%02d-%d %02d:%02d\n"
/*formato do output do comando 'v'*/
#define OUTP_T "%02d-%02d-%d\n"
/* formato do output do commando 't'*/
#define OUT_ERRL1 "%s: "
#define OUT_A "airport %s\n"

#define OUTP_ERRV3 "%s"
#define DATA "%d-%d-%d"
#define INPUT_R1 "%s %d-%d-%d"

#define RES_ERR1 "invalid reservation code\n"
#define RES_ERR2 ": flight does not exist\n"
#define RES_ERR3 ": flight reservation already used\n"
#define RES_ERR4 "too many reservations\n"
#define RES_ERR6 "invalid passenger number\n"

#define HORA "%d:%d"
#define IDS "%s%s%s"
#define OUT_L "%s %s %s %d\n"

/* for command 'v'; voos*/
#define MAX_PASS 100
#define MIN_PASS 10
#define MAX_ID 7
#define MAX_DUR 12
#define MAX_VOO 30000
#define MAX_CHAR 65535
/* flags */
#define CHE 0
#define PAR 1

#define HT_SIZE 30271

typedef struct {
	char id[ID_M];
	char pais[PAIS_M];
	char cidade[CIDADE_M];
} aero;

typedef struct {
	int horas;
	int min;
} hora;

typedef struct {
	int dia;
	int mes;
	int ano;
} data;

typedef struct node_r {
	char *id_reserva;
	char *id_voo;
	int pass;
	struct node_r *next_voo;
} node;

typedef struct node_str {
	char *id_reserva;
	struct node_str *next;
} nodestr;

typedef struct {
	char id[MAX_ID];

	char id_og[ID_M];

	char id_new[ID_M];

	hora hora_og;

	data data_og;

	hora duracao;

	int pass;

	hora hora_chegada;

	data data_chegada;

	node *lista_reservas;

	int nr_reservas;
} voo;

/* FUNCTIONS DEFINITIONS*/
int c_hegadas();
int p_artidas();
int conta_voos(char id_aero[]);
void print_p_ou_c();
int error_1();

void copy_a_1(int c);

void copy_a_2(int c, char pais[], char cidade[], char id[]);
void copy_l_1(char id[], char id_new[], char id_og[]);
void aumenta_data_if(data data_og);
void copy_l_2(int pass, data data_og, hora hora_og, hora duracao);
int error_v3(int mark_og, int mark_new, char id_og[], char id_new[]);
void aumenta_data_if(data data_og);
int print_error(char error[]);

node *init_node();
void add_node(node *head, node *og, node *new_node);
int hash_func(char *v);

voo *get_reservation(char id_reserva[]);
void free_lista(node *header);

voo *get_voo(char *id_voo, data *date);
int error_reservas_total(char id_voo[], data data);
void add_hash(char *id_reserva);
void delete_res(char *id_reserva);
int reserva_finder(char *id_reserva);
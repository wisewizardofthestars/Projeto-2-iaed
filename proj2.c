/*---- FUNCTION FILE ------*/
/* Sara Pinheiro ist102507*/
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "structs.h"

aero aeros[AERO_M];
int nr_aero, nr_voos, nr_reservations;
voo voos[MAX_VOO], sort[MAX_VOO];
data data_h;
nodestr *hashtable[HT_SIZE];

/* SMALL FUNCTIONS :*/

/*to check if a data is in the past or if its more than an year
in the future*/
/*if it returns 0 it means that its a date in the future
or in the past*/

int data_checker(int ano, int mes, int dia, int flag) {
	/* if flag == 0 it means new alterations therefore it doesnt need to be
	 * checked in the future*/
	if ((ano < data_h.ano)) {
		if (flag == 1) {
			printf(ERR_V5);
		}
		return 0;
	} else if (ano == data_h.ano) {
		if (mes < data_h.mes || ((mes == data_h.mes) && (dia < data_h.dia))) {
			if (flag == 1) {
				printf(ERR_V5);
			}
			return 0;
		}
	} else if (ano > data_h.ano && flag == 1) {
		if (mes > data_h.mes || ((mes == data_h.mes) && (dia > data_h.dia))) {
			printf(ERR_V5);
			return 0;
		}
	}
	return 1;
}

/* função que vê se existe um código igual ao do input
para exatemente para o mesmo dia*/
int error_v2(char id[], int c, data data_og, int flag) {
	if ((!strcmp(voos[c].id, id)) && (data_og.ano == voos[c].data_og.ano) &&
		(data_og.mes == voos[c].data_og.mes) &&
		(data_og.dia == voos[c].data_og.dia)) {
		if (!flag) {
			return print_error(ERR_V2);
		}
		if (flag) {
			return 2;
		}
	}
	return 1;
}

/* copies the flights from the global variable
to the new vector, vetor*/
void copy(int ind_l, int ind_r, voo vetor[]) {
	vetor[ind_l] = voos[ind_r];
	return;
}

/* it swaps elements of the vector voo to sort*/
void swap(int ind_l, int ind_r, voo vetor[]) {
	voo temp_v;
	temp_v = vetor[ind_l];
	vetor[ind_l] = vetor[ind_r];
	vetor[ind_r] = temp_v;
	return;
}

/* compares two int variables;*/
int compare(int left, int right) {
	if (left < right) {
		return 1;
	} else if (left == right) {
		return 2;
	}
	return 0;
}

/* dependendo do valor da flag a função devolve o parametros
que será utilizado noutra função*/
int element(int flag, data vetor_data, hora vetor_hora) {
	switch (flag) {
		case 0:
			return vetor_data.ano;
		case 1:
			return vetor_data.mes;
		case 2:
			return vetor_data.dia;
		case 3:
			return vetor_hora.horas;
		case 4:
			return vetor_hora.min;
		default:
			return 0;
	}
}

/*function that checks if a date is the same as the other*/
int equal_date(data *d1, data *d2) {
	return d1->ano == d2->ano && d1->mes == d2->mes && d1->dia == d2->dia;
}

/* função que compara duas posições adjacentes
		mark == 1 simboliza partidas, mode 0 = chegadas
		when flag_part 0 year,1 month, 2 day, 3 hour, 4 min*/
void ordenar(int ind_l, int ind_r, voo vetor[], int mark) {
	static int flag_data = 0;
	/* partidas */
	if (mark) {
		switch (compare(
			element(flag_data, vetor[ind_l].data_og, vetor[ind_l].hora_og),
			element(flag_data, vetor[ind_r].data_og, vetor[ind_r].hora_og))) {
			case 1:
				swap(ind_l, ind_r, vetor);
				flag_data = 0;
				break;
			case 2:
				flag_data++;
				ordenar(ind_l, ind_r, vetor, mark);
				break;
			default:
				flag_data = 0;
				return;
		}
	} else {
		switch (compare(element(flag_data, vetor[ind_l].data_chegada,
								vetor[ind_l].hora_chegada),
						element(flag_data, vetor[ind_r].data_chegada,
								vetor[ind_r].hora_chegada))) {
			case 1:
				swap(ind_l, ind_r, vetor);
				flag_data = 0;
				break;
			case 2:
				flag_data++;
				ordenar(ind_l, ind_r, vetor, mark);
				break;
			default:
				flag_data = 0;
				return;
		}
	}
}

/* função que dá print do erro no such airport id
		mark_og indica se exite o airport de partida
		mark_new indica se existe o airport de chegada*/
int error_v3(int mark_og, int mark_new, char id_og[], char id_new[]) {
	if ((!mark_og) || (!mark_new)) {
		if (!mark_og) printf(OUTP_ERRV3 ERR_V3, id_og);

		if (!mark_new) printf(OUTP_ERRV3 ERR_V3, id_new);
		return 0;
	}
	return 1;
}
/* funcao que passa a data, hora duracao e nr de passageiros
para o vetor global voos*/
void copy_l_2(int pass, data data_og, hora hora_og, hora duracao) {
	voos[nr_voos].pass = pass;
	voos[nr_voos].data_og = data_og;
	voos[nr_voos].hora_og = hora_og;
	voos[nr_voos].duracao = duracao;
	return;
}

/*copies the scaned id and the airports id to use in command 'l'*/
void copy_l_1(char id[], char id_new[], char id_og[]) {
	strcpy(voos[nr_voos].id, id);
	strcpy(voos[nr_voos].id_new, id_new);
	strcpy(voos[nr_voos].id_og, id_og);
	return;
}

/* função que aumenta o mes se a data passar dos 28/30/31
dias dependendo do mês; aumenta o ano se passar dos 12 meses*/
data aumenta_data(data data1) {
	data nova_data = data1;
	nova_data.dia++;
	if (nova_data.mes == 1 || nova_data.mes == 3 || nova_data.mes == 5 ||
		nova_data.mes == 7 || nova_data.mes == 8 || nova_data.mes == 10 ||
		nova_data.mes == 12) {
		if (nova_data.dia > 31) {
			nova_data.mes++;
			nova_data.dia -= 31;
		}
	} else if (nova_data.mes == 2) {
		if (nova_data.dia > 28) {
			nova_data.mes++;
			nova_data.dia -= 28;
		}
	} else {
		if (nova_data.dia > 30) {
			nova_data.mes++;
			nova_data.dia -= 30;
		}
	}
	if (nova_data.mes > 12) {
		nova_data.mes -= 12;
		nova_data.ano++;
	}
	return nova_data;
}
/* funcao que avalia se é necessário usar a função aumenta_data
se as horas passarem de 24, acerta as horas*/
void aumenta_data_if(data data_og) {
	if (voos[nr_voos].hora_chegada.horas >= 24) {
		voos[nr_voos].hora_chegada.horas -= 24;
		voos[nr_voos].data_chegada = aumenta_data(data_og);
		return;
	}
}

/*checks if nr of flights will suprasse the max nr*/
int error_v4() {
	/* too many flights error*/
	if (nr_voos >= MAX_VOO) return print_error(ERR_V4);
	return 1;
}

/* função que dá print do erro 1 do comando 'v'
se tal for necessário*/
int error_1(int len, char id[]) {
	int c;
	if (len >= 9 || len < 3) {
		return print_error(ERR_V1);
	}
	
	for (c = 0; c < len; c++) {
		
		if (isdigit(id[c]) && (c == 0 || c == 1)) {

			return print_error(ERR_V1);
		}
		if (c >= 4 && ((id[c] < '0') || (id[c] > '9'))) {

			return print_error(ERR_V1);
		}
	}
	return 1;
}

/* função que conta o nr de voos de um airport*/
int conta_voos(char id_aero[]) {
	int c, cont = 0;
	for (c = 0; c < nr_voos; c++) {
		if (!strcmp(voos[c].id_og, id_aero)) {
			cont++;
		}
	}
	return cont;
}

/* função que dá print ao erro de no such airport id*/
int error(char id[]) {
	int c;
	for (c = 0; c < nr_aero; c++) {
		if (!strcmp(aeros[c].id, id)) {
			return 1;
		}
	}
	printf(OUT_ERRL1 ERR_L1, id);
	return 0;
}

/*função que muda o valor da mark se houver um aeroporto
com o mesmo id*/
int mark_checker(char id_in[], int mark) {
	int c;
	for (c = 0; c < nr_aero; c++) {
		if (!strcmp(aeros[c].id, id_in)) mark++;
	}
	return mark;
}

/* função que soma a hora de partida com a duração do
voo para determinar a hora de chegada
e acerta a hora se esta passar dos 60 min*/
hora sum_horas(hora hora1, hora hora2) {
	hora soma;
	soma.horas = hora1.horas + hora2.horas;
	soma.min = hora1.min + hora2.min;
	if (soma.min >= 60) {
		soma.horas++;
		soma.min -= 60;
	}
	return soma;
}

/* função que dá print do formato de output da função c e p*/
void print_p_ou_c(data data_in, hora hora_in, char id[], char id_aero[]) {
	printf(OUTP_CP, id, id_aero, data_in.dia, data_in.mes, data_in.ano,
		   hora_in.horas, hora_in.min);
}

/* função que atribui o valor do elemento anterior
do vetor ao do elemento seguinte*/
void copy_a_1(int c) {
	strcpy(aeros[c + 1].id, aeros[c].id);
	strcpy(aeros[c + 1].pais, aeros[c].pais);
	strcpy(aeros[c + 1].cidade, aeros[c].cidade);
	return;
}

/* função que atribui o valor das variáveis scaned
ao vetor mais à direita da lista*/
void copy_a_2(int c, char pais[], char cidade[], char id[]) {
	strcpy(aeros[c + 1].id, id);
	strcpy(aeros[c + 1].pais, pais);
	strcpy(aeros[c + 1].cidade, cidade);
	return;
}

/* checks error 6 and 7 from command 'v'*/
int error6_7_v(hora duracao, int pass) {
	if ((duracao.horas > MAX_DUR) ||
		((duracao.horas == MAX_DUR) && (duracao.min)))
		return print_error(ERR_V6);

	if ((pass < MIN_PASS)) return print_error(ERR_V7);

	return 1;
}

/* função que conta todos os voos do airport*/
int for_conta_voos(int c) {
	int i, cont = 0;
	for (i = 0; i < nr_voos; i++) {
		cont = conta_voos(aeros[c].id);
	}
	return cont;
}

/* error a1 checker*/
int err_aa1(char id[]) {
	int c;
	for (c = 0; c < nr_aero; c++) {
		if (!strcmp(aeros[c].id, id)) return print_error(ERR_A1);
	}
	return 1;
}

/* prints all the flights*/
void for_print() {
	int c;
	for (c = 0; c < nr_voos; c++) {
		printf(OUTP_V, voos[c].id, voos[c].id_og, voos[c].id_new,
			   voos[c].data_og.dia, voos[c].data_og.mes, voos[c].data_og.ano,
			   voos[c].hora_og.horas, voos[c].hora_og.min);
	}
	return;
}

/* ------------ MAIN FUNCTIONS-----------------*/

/* comando de chegadas irá apenas correr o comando partidas
		mas com uma flag CHE que indica que será organizado por
		chegadas*/
int c_hegadas() { return (p_artidas(CHE)); }

/* função que dá print às partidas de um certo aeroporto
		ou às chegadas dependendo do valor da flag
		if flag == 1 => partidas  if flag = 0 => chegadas*/
int p_artidas(int flag) {
	char id[ID_M];
	int c, j, i = 0;

	scanf(OUTP_ERRV3, id);
	/* error handler */
	if (!error(id)) return 0;
	if (flag) {
		/* to discover which flights come FROM the airport */
		for (c = 0; c < nr_voos; c++)
			if (!strcmp(voos[c].id_og, id)) {
				copy(i, c, sort);
				/* copys the flight to vector sort */
				i++;
			}
	} else
		/* to discover which flights come TO the airport */
		for (c = 0; c < nr_voos; c++)
			if (!strcmp(voos[c].id_new, id)) {
				copy(i, c, sort);
				i++;
			}

	/*implementação de bubble sort dos voos*/
	for (j = 0; j < i - 1; j++)
		for (c = 0; c < i - j - 1; c++) ordenar(c, c + 1, sort, flag);
	/* partidas flag == 1; chegadas == 0*/

	for (c = i - 1; c >= 0; c--) {
		if (flag)
			print_p_ou_c(sort[c].data_og, sort[c].hora_og, sort[c].id,
						 sort[c].id_new);
		else {
			if (data_checker(sort[c].data_chegada.ano, sort[c].data_chegada.mes,
							 sort[c].data_chegada.dia, 0)) {
				/*fkag == 0 bc aletration*/
				/*if the date isnt in the past data_checker also checks in the
				 * future*/
				print_p_ou_c(sort[c].data_chegada, sort[c].hora_chegada,
							 sort[c].id, sort[c].id_og);
			}
		}
	}
	return 1;
}

/*command 't' avança a data do sistema*/
int t_ime() {
	int ano, mes, dia;
	scanf(DATA, &dia, &mes, &ano);

	if (!data_checker(ano, mes, dia, 1)) {
		return 0;
	}
	data_h.dia = dia;
	data_h.mes = mes;
	data_h.ano = ano;

	printf(OUTP_T, dia, mes, ano);
	return 1;
}

/*command 'v' adiciona ou lista todos os voos*/
int v_oo() {
	char id[MAX_ID], id_og[ID_M], id_new[ID_M];
	int pass, mark_og = 0, mark_new = 0, ch, c, len;
	hora hora_og, duracao;
	data data_og;

	ch = getchar();

	/* to create a flight*/
	if (ch != '\n') {
		scanf(IDS, id, id_og, id_new);
		scanf(DATA, &data_og.dia, &data_og.mes, &data_og.ano);
		scanf(HORA HORA, &hora_og.horas, &hora_og.min, &duracao.horas,
			  &duracao.min);
		scanf("%d", &pass);

		/*id error*/
		len = strlen(id);
		if (!error_1(len, id)) return 0;

		/* ERR_3 */
		for (c = 0; c < nr_voos; c++) {
			if (!error_v2(id, c, data_og, 0)) return 0;
		}

		mark_og = mark_checker(id_og, mark_og);
		mark_new = mark_checker(id_new, mark_new);

		if (!error_v3(mark_og, mark_new, id_og, id_new) || !error_v4()) {
			return 0;
		}

		if (!data_checker(data_og.ano, data_og.mes, data_og.dia, 1) ||
			!error6_7_v(duracao, pass))
			return 0;

		copy_l_1(id, id_new, id_og);
		copy_l_2(pass, data_og, hora_og, duracao);

		voos[nr_voos].hora_chegada = sum_horas(hora_og, duracao);
		voos[nr_voos].data_chegada = voos[nr_voos].data_og;

		aumenta_data_if(data_og);
		voos[nr_voos].lista_reservas = NULL;
		voos[nr_voos].nr_reservas = 0;
		nr_voos++;

	} else if (nr_voos > 0) {
		for_print();
	}
	return 1;
}

/*'l' command: listar os aeroportos*/
int l_istar() {
	char id_aero[ID_M];
	int mark = 0, c, ch, cont = 0;
	/* the mark marks if theres something after the
	'l' command, ou seja se entra no loop,
	nest caso também vai marcar se o id é valido*/
	ch = getchar();
	while (ch != '\n') {
		scanf(OUT_ERRL1, id_aero);
		for (c = 0; c <= nr_aero; c++) {
			if (!strcmp(aeros[c].id, id_aero)) {
				cont = for_conta_voos(c);
				printf(OUT_L, aeros[c].id, aeros[c].cidade, aeros[c].pais,
					   cont);

				mark = 1;
				break;
			}
		}
		if (!mark) printf(OUT_ERRL1 ERR_L1, id_aero);

		mark = 0;
		ch = getchar();
		if (ch == '\n') {
			return 1;
		}
	}
	/* quando não há nada depois do l, ou seja listar todos os airports*/
	if ((!mark) && (nr_aero > 0)) {
		for (c = 0; c <= nr_aero - 1; c++) {
			printf(OUT_L, aeros[c].id, aeros[c].cidade, aeros[c].pais,
				   conta_voos(aeros[c].id));
		}
	}
	return 1;
}

/*command 'a', adiciona um aeroporto*/
int a_dicionar() {
	char id[ID_M], pais[PAIS_M], cidade[CIDADE_M];
	/* declarar uma string de ids, cidade e pais*/

	int c = 0, i, mark = 0, len;

	scanf("%s %s%*c", id, pais);

	fgets(cidade, CIDADE_M, stdin);
	len = strlen(cidade);

	/* to clean the string*/
	if ((len > 0) && (cidade[len - 1]) == '\n') cidade[--len] = '\0';

	/* erros handler*/
	for (c = 0; c <= 2; c++)
		if islower (id[c]) {
			printf(ERR_A2);
			return 0;
		}

	/* too many aerports handler*/
	if (nr_aero >= AERO_M) {
		printf(ERR_A3);
		return 0;
	}

	else {
		if (!err_aa1(id)) return 0;
		/* se o vetor ainda não tem outros aeros*/
		if (!nr_aero) copy_a_2(nr_aero - 1, pais, cidade, id);

		/* caso tenha outros comparar para ficar já ordenado*/
		else {
			for (c = nr_aero - 1; ((c >= 0) && (mark != 2)); c--) {
				for (i = 0; i <= 2; i++) {
					if (id[i] > aeros[c].id[i]) {
						copy_a_2(c, pais, cidade, id);
						mark = 2;
						break;
					}
					if (id[i] < aeros[c].id[i]) {
						copy_a_1(c);
						copy_a_2(c - 1, pais, cidade, id);

						mark = 1;
						break;
					}
				}
			}
		}
		nr_aero++;
	}
	printf(OUT_A, id);
	return 1;
}
/* ------------------PROJECT 2---------------------- */

/*--------------------funções de errors--------------*/
/*checks that the reservtion error is only
made of upper letters or digits; checks if the length of
the reservation error is bigger than 10*/
int valid_reserva_id(char *id_reserva) {
	int i, len;
	len = strlen(id_reserva);
	if (len < 10) {
		return 0;
	}
	for (i = 0; i < len; i++) {
		if ((!isupper(id_reserva[i]) && (!isdigit(id_reserva[i])))) {
			return 0;
		}
	}
	return 1;
}

/*checks if the passangers number of a reservation is correct*/
int pass_error(int pass) {
	if (sizeof(pass) != sizeof(int)) {
		return 0;
	}
	if (pass <= 0) {
		return 0;
	}
	return 1;
}

/* error2: erro de não haver voo para a data */
int voo_exists(char voo[], data data) {
	int i;
	for (i = 0; i < nr_voos; i++) {
		if (error_v2(voo, i, data, 1) == 2) {
			return 1;
		}
	}
	return 0;
}

/*to print an error*/
int print_error(char error[]) {
	printf("%s", error);
	return 0;
}

/*error function that checks if adding a reservation will
surprass the maximum passengers allowed*/
int too_many_reservas(int pass, char id_voo[], data *date) {
	voo *v = get_voo(id_voo, date);

	if ((v->nr_reservas + pass) > v->pass) {
		printf(RES_ERR4);
		return 0;
	}
	return 1;
}

/*error function that checks the overall of errors; only used
in r_ervas() when it has the objective of adding a reservatio*/
int error_reservas(char id_reserva[], int pass, char id_voo[], data data) {
	if (!error_reservas_total(id_voo, data)) return 0;

	if (!valid_reserva_id(id_reserva)) {
		printf(RES_ERR1);
		return 0;
	}
	if (reserva_finder(id_reserva) == 1) {
		printf("%s" RES_ERR3, id_reserva);
		return 0;
	}
	if (!too_many_reservas(pass, id_voo, &data)) return 0;

	if (!data_checker(data.ano, data.mes, data.dia, 1)) return 0;
	if (!pass_error(pass)) return print_error(RES_ERR6);
	return 1;
}

/*error function that tests in both options of the r_eserva() input;
it tests the date and also if the flight exists*/
int error_reservas_total(char id_voo[], data data) {
	if (!voo_exists(id_voo, data)) {
		printf("%s%s", id_voo, RES_ERR2);
		return 0;
	}
	if (!data_checker(data.ano, data.mes, data.dia, 1)) {
		return 0;
	}
	return 1;
}
/*------------------------------------------*/

/*function that goes throught the flight array and checks if
theres a flight with the same id and date*/
voo *get_voo(char *id_voo, data *date) {
	int c;
	for (c = 0; c < nr_voos; c++) {
		if (!(strcmp(id_voo, voos[c].id)) && equal_date(date, &voos[c].data_og))
			return &voos[c];
	}
	return NULL;
}

/*function that prints all reservations*/
void lista_reservas(char id_voo[], data *date) {
	node *x;
	voo *v = get_voo(id_voo, date);

	for (x = v->lista_reservas; x != NULL; x = x->next_voo) {
		printf("%s %d\n", x->id_reserva, x->pass);
	}
}

/*function that adds a reservation to the flight structure
in the form of a linked list*/
void voo_adder(node *reserva_, char *id_voo, data *date) {
	voo *v = get_voo(id_voo, date);
	node *n = v->lista_reservas, *prev = NULL;

	v->nr_reservas += reserva_->pass;

	if (n == NULL) {
		reserva_->next_voo = v->lista_reservas;
		v->lista_reservas = reserva_;
		return;
	}
	while (n != NULL) {
		if (strcmp(n->id_reserva, reserva_->id_reserva) > 0) {
			if (prev != NULL) {
				prev->next_voo = reserva_;
				reserva_->next_voo = n;
			} else {
				reserva_->next_voo = v->lista_reservas;
				v->lista_reservas = reserva_;
			}
			return;
		}
		prev = n;
		n = n->next_voo;
	}
	prev->next_voo = reserva_;
	reserva_->next_voo = NULL;
}

/*function for the command 'r'*/
int r_eserva() {
	char id_reserva[MAX_CHAR], id_voo[MAX_ID], c;
	data date;
	node *reserva = malloc(sizeof(node));
	scanf(INPUT_R1, id_voo, &date.dia, &date.mes, &date.ano);
	c = getchar();

	if (c != '\n') {
		scanf("%s %d", id_reserva, &reserva->pass);
		if (!error_reservas(id_reserva, reserva->pass, id_voo, date)) {
			free(reserva);
			return 0;
		}
		reserva->id_reserva = malloc(sizeof(char) * (strlen(id_reserva) + 1));
		strcpy(reserva->id_reserva, id_reserva);

		add_hash(reserva->id_reserva);
		voo_adder(reserva, id_voo, &date);
	} else {
		if (!error_reservas_total(id_voo, date)) {
			free(reserva);
			return 0;
		}

		lista_reservas(id_voo, &date);
		free(reserva);
	}
	return 1;
}

/*function to free the memory allocated*/
void free_lista(node *header) {
	node *temp, *x;

	for (x = header; x != NULL; x = x->next_voo) {
		temp = x;
		delete_res(x->id_reserva);

		free(temp->id_reserva);
		free(temp);
	}
}

/*----------e_liminar() functions--------------*/

/*deletes a flight from the array*/
void delete_voo(int ind) {
	int i;
	for (i = ind; i < nr_voos - 1; i++) {
		voos[i] = voos[i + 1];
	}
}

/*adds a reservation to the hahstable*/
void add_hash(char *id_reserva) {
	int ind;
	nodestr *reserva;
	ind = hash_func(id_reserva);
	reserva = malloc(sizeof(nodestr));
	reserva->next = hashtable[ind];
	reserva->id_reserva = id_reserva;
	hashtable[ind] = reserva;
}

/*finds the reservation in the hashtable*/
int reserva_finder(char *id_reserva) {
	int ind;
	nodestr *x;
	ind = hash_func(id_reserva);
	for (x = hashtable[ind]; x != NULL; x = x->next)
		if (!strcmp(id_reserva, x->id_reserva)) return 1;
	return 0;
}

/*delete the reservation from the hahstable*/
void delete_res(char *id_reserva) {
	int ind;
	nodestr *x, *prev;
	ind = hash_func(id_reserva);
	for (x = hashtable[ind], prev = NULL; x != NULL; prev = x, x = x->next) {
		if (!strcmp(id_reserva, x->id_reserva)) {
			if (prev != NULL) {
				prev->next = x->next;
			} else {
				hashtable[ind] = x->next;
			}
			free(x);
			break;
		}
	}
}

/*function that gets the reservation from the
flight struct*/
voo *get_reservation(char id_reserva[]) {
	int ind;
	node *x;
	for (ind = 0; ind < nr_voos; ind++) {
		for (x = voos[ind].lista_reservas; x != NULL; x = x->next_voo) {
			if (!strcmp(x->id_reserva, id_reserva)) {
				return &voos[ind];
			}
		}
	}
	return NULL;
}

/* returns the index*/
int hash_func(char *v) {
	int h, a = 31415, b = 27183;
	for (h = 0; *v != '\0'; v++, a = a * b % (HT_SIZE - 1))
		h = (a * h + *v) % HT_SIZE;
	return h;
}

/*function that deletes a reservation*/
void delete_reservation(voo *v, char *id) {
	node *x, *prev;
	for (x = v->lista_reservas, prev = NULL; x != NULL;
		 prev = x, x = x->next_voo) {
		if (!strcmp(id, x->id_reserva)) {
			v->nr_reservas -= x->pass;
			if (prev != NULL)
				prev->next_voo = x->next_voo;
			else
				v->lista_reservas = x->next_voo;
			free(x->id_reserva);
			free(x);
			break;
		}
	}
}

/*function that hadles the 'e' command*/
int e_limina() {
	/*quando é seguido pwor um codigo de im voo, remover o voo apenas
	o qie tem data de chegada posterios*/
	char id[MAX_CHAR];
	int len, i, found_flag = 0, temp = nr_voos, n = 0;
	voo *v;

	scanf("%s", id);
	len = strlen(id);
	if (len <= MAX_ID) {
		/*aqui apaga os voos and aslso reservas*/
		/*se datata checker tiver uma flag de 1 means que é só para afzer check
		das datas anteriores neste caso se for anterior vai retornar 0 e não
		pode fazer print, logo flag de e será == 2*/
		for (i = 0; i < temp; i++) {
			if (!strcmp(voos[i - n].id, id) &&
				!data_checker(voos[i - n].data_chegada.ano,
							  voos[i - n].data_chegada.mes,
							  voos[i - n].data_chegada.dia, 2)) {
				found_flag = 1;
				free_lista(voos[i - n].lista_reservas);
				delete_voo(i - n);
				nr_voos--;
				n++;
			}
		}
		if (!found_flag) {
			printf(ERR_E1);
		}
	}

	else {
		/*aqui apaga as reservas*/
		if (reserva_finder(id) == 0) {
			printf(ERR_E1);
			return 0;
		}
		v = get_reservation(id);
		delete_res(id);
		delete_reservation(v, id);
	}
	return 0;
}
/*frees the linked list*/
void free_ll() {
	int c;
	for (c = 0; c < nr_voos; c++) {
		free_lista(voos[c].lista_reservas);
	}
}
/* função que lê e seleciona os comandos */
int commands() {
	char com;
	com = getchar();
	switch (com) {
		case 'a':
			a_dicionar();
			return 1;
		case 'l':
			l_istar();
			return 1;

		case 'v':
			v_oo();
			return 1;

		case 'p':
			p_artidas(PAR);
			return 1;
		case 'c':
			c_hegadas();
			return 1;

		case 't':
			t_ime();
			return 1;
		case 'q':
			free_ll();

			return 0;
		case 'r':
			r_eserva();
			return 1;
		case 'e':
			e_limina();
			return 1;
		default:
			return 1;
	}
}

/* main irá inicalizar as variáveis globais e
irá correr a função de selecionar os comandos*/
int main() {
	int c;
	nr_aero = 0;
	nr_voos = 0;
	nr_reservations = 0;
	data_h.dia = 1;
	data_h.mes = 1;
	data_h.ano = 2022;

	for (c = 0; c < HT_SIZE; c++) {
		hashtable[c] = NULL;
	}
	while (commands()) {
	}
	return 0;
}

/*
 *   Ce fichier fait partie d'un projet de programmation donné en Licence 3 
 *   à l'Université de Bordeaux
 *
 *   Copyright (C) 2014, 2015 Adrien Boussicault
 *
 *    This Library is free software: you can redistribute it and/or modify
 *    it under the terms of the GNU General Public License as published by
 *    the Free Software Foundation, either version 2 of the License, or
 *    (at your option) any later version.
 *
 *    This Library is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU General Public License for more details.
 *
 *    You should have received a copy of the GNU General Public License
 *    along with this Library.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "automate.h"
#include "table.h"
#include "ensemble.h"
#include "outils.h"
#include "fifo.h"

#include <search.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h> 

#include <assert.h>

#include <math.h>


void action_get_max_etat(const intptr_t element, void* data){
  int* max = (int*) data;
  if( *max < element ) *max = element;
}

int get_max_etat( const Automate* automate ){
	int max = INT_MIN;

	pour_tout_element(automate->etats, action_get_max_etat, &max);

	return max;
	
}

void action_get_min_etat( const intptr_t element, void* data ){
	int * min = (int*) data;
	if( *min > element ) *min = element;
}

int get_min_etat( const Automate* automate ){
	int min = INT_MAX;

	pour_tout_element( automate->etats, action_get_min_etat, &min );

	return min;
}


int comparer_cle(const Cle *a, const Cle *b) {
	if( a->origine < b->origine )
		return -1;
	if( a->origine > b->origine )
		return 1;
	if( a->lettre < b->lettre )
		return -1;
	if( a->lettre > b->lettre )
		return 1;
	return 0;
}

void print_cle( const Cle * a){
	printf( "(%d, %c)" , a->origine, (char) (a->lettre) );
}

void supprimer_cle( Cle* cle ){
	xfree( cle );
}

void initialiser_cle( Cle* cle, int origine, char lettre ){
	cle->origine = origine;
	cle->lettre = (int) lettre;
}

Cle * creer_cle( int origine, char lettre ){
	Cle * result = xmalloc( sizeof(Cle) );
	initialiser_cle( result, origine, lettre );
	return result;
}

Cle * copier_cle( const Cle* cle ){
	return creer_cle( cle->origine, cle->lettre );
}

Automate * creer_automate(){
	Automate * automate = xmalloc( sizeof(Automate) );
	automate->etats = creer_ensemble( NULL, NULL, NULL );
	automate->alphabet = creer_ensemble( NULL, NULL, NULL );
	automate->transitions = creer_table(
		( int(*)(const intptr_t, const intptr_t) ) comparer_cle , 
		( intptr_t (*)( const intptr_t ) ) copier_cle,
		( void(*)(intptr_t) ) supprimer_cle
	);
	automate->initiaux = creer_ensemble( NULL, NULL, NULL );
	automate->finaux = creer_ensemble( NULL, NULL, NULL );
	automate->vide = creer_ensemble( NULL, NULL, NULL ); 
	return automate;
}

Automate * translater_automate_entier( const Automate* automate, int translation ){
	Automate * res = creer_automate();

	Ensemble_iterateur it;
	for( 
		it = premier_iterateur_ensemble( get_etats( automate ) );
		! iterateur_ensemble_est_vide( it );
		it = iterateur_suivant_ensemble( it )
	){
		ajouter_etat( res, get_element( it ) + translation );
	}

	for( 
		it = premier_iterateur_ensemble( get_initiaux( automate ) );
		! iterateur_ensemble_est_vide( it );
		it = iterateur_suivant_ensemble( it )
	){
		ajouter_etat_initial( res, get_element( it ) + translation );
	}

	for( 
		it = premier_iterateur_ensemble( get_finaux( automate ) );
		! iterateur_ensemble_est_vide( it );
		it = iterateur_suivant_ensemble( it )
	){
		ajouter_etat_final( res, get_element( it ) + translation );
	}

	// On ajoute les lettres
	for(
		it = premier_iterateur_ensemble( get_alphabet( automate ) );
		! iterateur_ensemble_est_vide( it );
		it = iterateur_suivant_ensemble( it )
	){
		ajouter_lettre( res, (char) get_element( it ) );
	}

	Table_iterateur it1;
	Ensemble_iterateur it2;
	for(
		it1 = premier_iterateur_table( automate->transitions );
		! iterateur_est_vide( it1 );
		it1 = iterateur_suivant_table( it1 )
	){
		Cle * cle = (Cle*) get_cle( it1 );
		Ensemble * fins = (Ensemble*) get_valeur( it1 );
		for(
			it2 = premier_iterateur_ensemble( fins );
			! iterateur_ensemble_est_vide( it2 );
			it2 = iterateur_suivant_ensemble( it2 )
		){
			int fin = get_element( it2 );
			ajouter_transition(
				res, cle->origine + translation, cle->lettre, fin + translation
			);
		}
	};

	return res;
}


void liberer_automate( Automate * automate ){
	assert( automate );
	liberer_ensemble( automate->vide );
	liberer_ensemble( automate->finaux );
	liberer_ensemble( automate->initiaux );
	pour_toute_valeur_table(
		automate->transitions, ( void(*)(intptr_t) ) liberer_ensemble
	);
	liberer_table( automate->transitions );
	liberer_ensemble( automate->alphabet );
	liberer_ensemble( automate->etats );
	xfree(automate);
}

const Ensemble * get_etats( const Automate* automate ){
	return automate->etats;
}

const Ensemble * get_initiaux( const Automate* automate ){
	return automate->initiaux;
}

const Ensemble * get_finaux( const Automate* automate ){
	return automate->finaux;
}

const Ensemble * get_alphabet( const Automate* automate ){
	return automate->alphabet;
}

void ajouter_etat( Automate * automate, int etat ){
	ajouter_element( automate->etats, etat );
}

void ajouter_lettre( Automate * automate, char lettre ){
	ajouter_element( automate->alphabet, lettre );
}

void ajouter_transition(
	Automate * automate, int origine, char lettre, int fin
){
	ajouter_etat( automate, origine );
	ajouter_etat( automate, fin );
	ajouter_lettre( automate, lettre );

	Cle cle;
	initialiser_cle( &cle, origine, lettre );
	Table_iterateur it = trouver_table( automate->transitions, (intptr_t) &cle );
	Ensemble * ens;
	if( iterateur_est_vide( it ) ){
		ens = creer_ensemble( NULL, NULL, NULL );
		add_table( automate->transitions, (intptr_t) &cle, (intptr_t) ens );
	}else{
		ens = (Ensemble*) get_valeur( it );
	}
	ajouter_element( ens, fin );
}

void ajouter_etat_final(
	Automate * automate, int etat_final
){
	ajouter_etat( automate, etat_final );
	ajouter_element( automate->finaux, etat_final );
}

void ajouter_etat_initial(
	Automate * automate, int etat_initial
){
	ajouter_etat( automate, etat_initial );
	ajouter_element( automate->initiaux, etat_initial );
}

const Ensemble * voisins( const Automate* automate, int origine, char lettre ){
	Cle cle;
	initialiser_cle( &cle, origine, lettre );
	Table_iterateur it = trouver_table( automate->transitions, (intptr_t) &cle );
	if( ! iterateur_est_vide( it ) ){
		return (Ensemble*) get_valeur( it );
	}else{
		return automate->vide;
	}
}

Ensemble * delta1(
	const Automate* automate, int origine, char lettre
){
	Ensemble * res = creer_ensemble( NULL, NULL, NULL );
	ajouter_elements( res, voisins( automate, origine, lettre ) );
	return res; 
}

Ensemble * delta(
	const Automate* automate, const Ensemble * etats_courants, char lettre
){
	Ensemble * res = creer_ensemble( NULL, NULL, NULL );

	Ensemble_iterateur it;
	for( 
		it = premier_iterateur_ensemble( etats_courants );
		! iterateur_ensemble_est_vide( it );
		it = iterateur_suivant_ensemble( it )
	){
		const Ensemble * fins = voisins(
			automate, get_element( it ), lettre
		);
		ajouter_elements( res, fins );
	}

	return res;
}

Ensemble * delta_star(
	const Automate* automate, const Ensemble * etats_courants, const char* mot
){
	int len = strlen( mot );
	int i;
	Ensemble * old = copier_ensemble( etats_courants );
	Ensemble * new = old;
	for( i=0; i<len; i++ ){
		new = delta( automate, old, *(mot+i) );
		liberer_ensemble( old );
		old = new;
	}
	return new;
}

void pour_toute_transition(
	const Automate* automate,
	void (* action )( int origine, char lettre, int fin, void* data ),
	void* data
){
	Table_iterateur it1;
	Ensemble_iterateur it2;
	for(
		it1 = premier_iterateur_table( automate->transitions );
		! iterateur_est_vide( it1 );
		it1 = iterateur_suivant_table( it1 )
	){
		Cle * cle = (Cle*) get_cle( it1 );
		Ensemble * fins = (Ensemble*) get_valeur( it1 );
		for(
			it2 = premier_iterateur_ensemble( fins );
			! iterateur_ensemble_est_vide( it2 );
			it2 = iterateur_suivant_ensemble( it2 )
		){
			int fin = get_element( it2 );
			action( cle->origine, cle->lettre, fin, data );
		}
	};
}

Automate* copier_automate( const Automate* automate ){
	Automate * res = creer_automate();
	Ensemble_iterateur it1;
	// On ajoute les états de l'automate
	for(
		it1 = premier_iterateur_ensemble( get_etats( automate ) );
		! iterateur_ensemble_est_vide( it1 );
		it1 = iterateur_suivant_ensemble( it1 )
	){
		ajouter_etat( res, get_element( it1 ) );
	}
	// On ajoute les états initiaux
	for(
		it1 = premier_iterateur_ensemble( get_initiaux( automate ) );
		! iterateur_ensemble_est_vide( it1 );
		it1 = iterateur_suivant_ensemble( it1 )
	){
		ajouter_etat_initial( res, get_element( it1 ) );
	}
	// On ajoute les états finaux
	for(
		it1 = premier_iterateur_ensemble( get_finaux( automate ) );
		! iterateur_ensemble_est_vide( it1 );
		it1 = iterateur_suivant_ensemble( it1 )
	){
		ajouter_etat_final( res, get_element( it1 ) );
	}
	// On ajoute les lettres
	for(
		it1 = premier_iterateur_ensemble( get_alphabet( automate ) );
		! iterateur_ensemble_est_vide( it1 );
		it1 = iterateur_suivant_ensemble( it1 )
	){
		ajouter_lettre( res, (char) get_element( it1 ) );
	}
	// On ajoute les transitions
	Table_iterateur it2;
	for(
		it2 = premier_iterateur_table( automate->transitions );
		! iterateur_est_vide( it2 );
		it2 = iterateur_suivant_table( it2 )
	){
		Cle * cle = (Cle*) get_cle( it2 );
		Ensemble * fins = (Ensemble*) get_valeur( it2 );
		for(
			it1 = premier_iterateur_ensemble( fins );
			! iterateur_ensemble_est_vide( it1 );
			it1 = iterateur_suivant_ensemble( it1 )
		){
			int fin = get_element( it1 );
			ajouter_transition( res, cle->origine, cle->lettre, fin );
		}
	}
	return res;
}

Automate * translater_automate(
	const Automate * automate, const Automate * automate_a_eviter
){
	if(
		taille_ensemble( get_etats(automate) ) == 0 ||
		taille_ensemble( get_etats(automate_a_eviter) ) == 0
	){
		return copier_automate( automate );
	}
	
	int translation = 
		get_max_etat( automate_a_eviter ) - get_min_etat( automate ) + 1; 

	return translater_automate_entier( automate, translation );
	
}

int est_une_transition_de_l_automate(
	const Automate* automate,
	int origine, char lettre, int fin
){
	return est_dans_l_ensemble( voisins( automate, origine, lettre ), fin );
}

int est_un_etat_de_l_automate( const Automate* automate, int etat ){
	return est_dans_l_ensemble( get_etats( automate ), etat );
}

int est_un_etat_initial_de_l_automate( const Automate* automate, int etat ){
	return est_dans_l_ensemble( get_initiaux( automate ), etat );
}

int est_un_etat_final_de_l_automate( const Automate* automate, int etat ){
	return est_dans_l_ensemble( get_finaux( automate ), etat );
}

int est_une_lettre_de_l_automate( const Automate* automate, char lettre ){
	return est_dans_l_ensemble( get_alphabet( automate ), lettre );
}

void print_ensemble_2( const intptr_t ens ){
	print_ensemble( (Ensemble*) ens, NULL );
}

void print_lettre( intptr_t c ){
	printf("%c", (char) c );
}

void print_automate( const Automate * automate ){
	printf("- Etats : ");
	print_ensemble( get_etats( automate ), NULL );
	printf("\n- Initiaux : ");
	print_ensemble( get_initiaux( automate ), NULL );
	printf("\n- Finaux : ");
	print_ensemble( get_finaux( automate ), NULL );
	printf("\n- Alphabet : ");
	print_ensemble( get_alphabet( automate ), print_lettre );
	printf("\n- Transitions : ");
	print_table( 
		automate->transitions,
		( void (*)( const intptr_t ) ) print_cle, 
		( void (*)( const intptr_t ) ) print_ensemble_2,
		""
	);
	printf("\n");
}

int le_mot_est_reconnu( const Automate* automate, const char* mot ){
	Ensemble * arrivee = delta_star( automate, get_initiaux(automate) , mot ); 
	
	int result = 0;

	Ensemble_iterateur it;
	for(
		it = premier_iterateur_ensemble( arrivee );
		! iterateur_ensemble_est_vide( it );
		it = iterateur_suivant_ensemble( it )
	){
		if( est_un_etat_final_de_l_automate( automate, get_element(it) ) ){
			result = 1;
			break;
		}
	}
	liberer_ensemble( arrivee );
	return result;
}

Automate * mot_to_automate( const char * mot ){
	Automate * automate = creer_automate();
	int i = 0;
	int size = strlen( mot );
	for( i=0; i < size; i++ ){
		ajouter_transition( automate, i, mot[i], i+1 );
	}
	ajouter_etat_initial( automate, 0 );
	ajouter_etat_final( automate, size );
	return automate;
}

//Ajoute une simple transition (origine,lettre)->fin
//Cette fonction peut être passée en paramètre à "pour_toute_transition(...)"
void ajouter_transition_pour_toute( int origine, char lettre, int fin, void* data ){
  ajouter_transition(data, origine, lettre, fin);
}
	
//Copie chaque transition partant d'un état initial de l'Automate a
//En la faisant partir de l'unique état initial
void union_nouvelles_transitions_depuis_initial(Automate *res, const Automate *a){
	Ensemble_iterateur it_alpha, it;
	
	//Parcours de l'alphabet
	for(it_alpha = premier_iterateur_ensemble( a->alphabet );
		! iterateur_ensemble_est_vide( it_alpha );
		it_alpha = iterateur_suivant_ensemble( it_alpha )
	){
		Ensemble * initiaux_alpha= delta(a, get_initiaux(a), get_element(it_alpha));
		//Parcours des états initiaux
		for(it = premier_iterateur_ensemble( initiaux_alpha );
			! iterateur_ensemble_est_vide( it );
			it = iterateur_suivant_ensemble( it )
		){
			ajouter_transition(res, INT_MIN, get_element(it_alpha), get_element(it));
		}
		
		liberer_ensemble(initiaux_alpha);
	}
}

Automate * creer_union_des_automates(
	const Automate * automate_1, const Automate * automate_2
){
	//Incorporation du premier automate
	Automate *res= copier_automate(automate_1);
	vider_ensemble(res->initiaux);
	
	//Incorporation du second automate
	Automate *a2= translater_automate(automate_2, automate_1); //Copie de automate_2
  	pour_toute_transition(a2, ajouter_transition_pour_toute, res);
	ajouter_elements(res->finaux, a2->finaux);

	//Création de l'unique état initial
	ajouter_etat(res, INT_MIN);
	ajouter_etat_initial(res, INT_MIN);
	
	//Ajout des transitions partant de l'unique état initial
	union_nouvelles_transitions_depuis_initial(res, automate_1);
	union_nouvelles_transitions_depuis_initial(res, a2);
	
	liberer_automate(a2);
	return res;
}


Ensemble* etats_accessibles( const Automate * automate, int etat ){

	//Si l'état n'existe pas
	if(! est_un_etat_de_l_automate(automate, etat) )
		return NULL;
    
	Ensemble* res = creer_ensemble(NULL, NULL, NULL);
	const Ensemble* alphabet = get_alphabet(automate);
	Ensemble_iterateur itRes;
	Ensemble_iterateur itAlph;
	
	//Ajout des états empruntés par les chemins commençant à l'état donné
	ajouter_element(res, etat);
	itRes = premier_iterateur_ensemble( res );
	while(! iterateur_ensemble_est_vide( itRes )){
		for(itAlph = premier_iterateur_ensemble( alphabet );
			! iterateur_ensemble_est_vide( itAlph );
			itAlph = iterateur_suivant_ensemble( itAlph ))
		{
			Ensemble * ens= delta1(automate, get_element(itRes), get_element(itAlph));
			ajouter_elements(res, ens);
			liberer_ensemble(ens);
		}

		itRes = iterateur_suivant_ensemble( itRes );
	}

    return res;
}

Ensemble* accessibles( const Automate * automate ){

	Ensemble* res = creer_ensemble( NULL, NULL, NULL );
	Ensemble_iterateur it_ens;

	//Ajout des états accessibles depuis chaque état initial
	for(it_ens = premier_iterateur_ensemble( get_initiaux(automate) );
		! iterateur_ensemble_est_vide( it_ens );
		it_ens  = iterateur_suivant_ensemble( it_ens ))
	{
		Ensemble * ens = etats_accessibles( automate, get_element(it_ens) );
		ajouter_elements( res, ens );
		liberer_ensemble( ens );
	}

	return res;  
}

Automate *automate_accessible( const Automate * automate ){
	Automate * res = creer_automate();
	Ensemble_iterateur it_ens;
	int etat, origine;
	char lettre;
	Cle * c;
	Ensemble * fins;

	Ensemble * access = accessibles( automate );

	// On ajoute les transitions, ainsi que les lettres et états
	Table_iterateur it_tab;
	for(
		it_tab = premier_iterateur_table( automate->transitions );
		! iterateur_est_vide( it_tab );
		it_tab = iterateur_suivant_table( it_tab )
	){
		c = (Cle*) get_cle( it_tab );
		origine = c->origine; 
		lettre = c->lettre;
		//Si l'origine des transitions est accessible, alors ajout des transitions
		if( est_dans_l_ensemble( access, origine ) ){ 
			fins = (Ensemble*) get_valeur( it_tab );
			for(
				it_ens = premier_iterateur_ensemble( fins );
				! iterateur_ensemble_est_vide( it_ens );
				it_ens = iterateur_suivant_ensemble( it_ens )
			){
				ajouter_transition( res, origine, lettre, get_element( it_ens ));
			}
		}
	}
	
	// On ajoute les états initiaux (forcément accessibles)
	for(
		it_ens = premier_iterateur_ensemble( get_initiaux( automate ) );
		! iterateur_ensemble_est_vide( it_ens );
		it_ens = iterateur_suivant_ensemble( it_ens )
	){
		ajouter_etat_initial( res, get_element( it_ens ) );
	}
	
	// On ajoute les états finaux
	for(
		it_ens = premier_iterateur_ensemble( access );
		! iterateur_ensemble_est_vide( it_ens );
		it_ens = iterateur_suivant_ensemble( it_ens )
	){
		etat = get_element( it_ens );
		if( est_un_etat_final_de_l_automate( automate, etat ) ){
			ajouter_etat_final( res, etat );
		}
	}
	
	liberer_ensemble( access );
	return res; 
}

void ajouter_transistion_inverse( int origine, char lettre, int fin, void* data ){
  ajouter_transition(data, fin, lettre, origine);
}

Automate *miroir( const Automate * automate){
	Automate * res = creer_automate();
	Ensemble_iterateur it_ens;
	
	// On ajoute les états initiaux depuis les finaux originaux
	for(
		it_ens = premier_iterateur_ensemble( get_finaux( automate ) );
		! iterateur_ensemble_est_vide( it_ens );
		it_ens = iterateur_suivant_ensemble( it_ens )
	){
		ajouter_etat_initial( res, get_element( it_ens ) );
	}
	
	// On ajoute les états finaux depuis les initiaux originaux
	for(
		it_ens = premier_iterateur_ensemble( get_initiaux( automate ) );
		! iterateur_ensemble_est_vide( it_ens );
		it_ens = iterateur_suivant_ensemble( it_ens )
	){
		ajouter_etat_final( res, get_element( it_ens ) );
	}
	
	// On ajoute les transitions inverses, ainsi que les états et les lettres
	pour_toute_transition(automate, ajouter_transistion_inverse, res);
  
	return res; 
}

Automate * creer_automate_du_melange(
	const Automate* automate_1,  const Automate* automate_2
){
	Automate *res= creer_automate();
	Ensemble *nouv_alphabet= creer_union_ensemble( get_alphabet(automate_1)
													,get_alphabet(automate_2));
	
	typedef struct{
		int q1;
		int q2;
	} couple_etat;
	
	int max=  taille_ensemble( get_etats(automate_1) )
		 	* taille_ensemble( get_etats(automate_2) );
	couple_etat etats[max];
	
	Ensemble_iterateur it_ens1, it_ens2, it_alpha, it_access;
	
	//Création des couples d'états
	//Ajout de ces couples dans "res"
	//Et ajout des états initiaux et finaux
	int n= 0;
	for(it_ens1 = premier_iterateur_ensemble( get_etats(automate_1) );
		! iterateur_ensemble_est_vide( it_ens1 );
		it_ens1 = iterateur_suivant_ensemble( it_ens1 ))
	{
		for(it_ens2 = premier_iterateur_ensemble( get_etats(automate_2) );
			! iterateur_ensemble_est_vide( it_ens2 );
			it_ens2 = iterateur_suivant_ensemble( it_ens2 ))
		{
			etats[n].q1= get_element(it_ens1);
			etats[n].q2= get_element(it_ens2);
			ajouter_etat(res, n);
			if(est_un_etat_initial_de_l_automate(automate_1, get_element(it_ens1))
			&& est_un_etat_initial_de_l_automate(automate_2, get_element(it_ens2)))
				ajouter_etat_initial(res, n);
			if(est_un_etat_final_de_l_automate(automate_1, get_element(it_ens1))
			&& est_un_etat_final_de_l_automate(automate_2, get_element(it_ens2)))
				ajouter_etat_final(res, n);
			n++;
		}
	}
	
	//Ajout des transitions
	int i, j;
	for(i= 0; i<n; i++){
		//Parcours de l'alphabet à la recherche de transitions partant d'etats[i]
		for(it_alpha = premier_iterateur_ensemble( nouv_alphabet );
			! iterateur_ensemble_est_vide( it_alpha );
			it_alpha = iterateur_suivant_ensemble( it_alpha ))
		{
			//Récupération des états accessibles par la lettre dans les 2 automates
			Ensemble *access_q1= delta1(automate_1, etats[i].q1, (char)get_element(it_alpha));
			Ensemble *access_q2= delta1(automate_2, etats[i].q2, (char)get_element(it_alpha));
			//Création des transitions pour automate_1
			for(it_access = premier_iterateur_ensemble( access_q1 );
				! iterateur_ensemble_est_vide( it_access );
				it_access = iterateur_suivant_ensemble( it_access ))
			{
				//Recherche du couple (nouveauQ1, q2)
				for(j= 0; j<n; j++){
					if(etats[j].q1 == get_element(it_access)
					&& etats[j].q2 == etats[i].q2){
						ajouter_transition(res, i, get_element(it_alpha), j);
						break;
					}
				}
			}
			//Création des transitions pour automate_2
			for(it_access = premier_iterateur_ensemble( access_q2 );
				! iterateur_ensemble_est_vide( it_access );
				it_access = iterateur_suivant_ensemble( it_access ))
			{
				//Recherche du couple (q1, nouveauQ2)
				for(j= 0; j<n; j++){
					if(etats[j].q1 == etats[i].q1
					&& etats[j].q2 == get_element(it_access)){
						ajouter_transition(res, i, get_element(it_alpha), j);
						break;
					}
				}
			}
			liberer_ensemble(access_q1);
			liberer_ensemble(access_q2);
		}
	}

	return res;
}
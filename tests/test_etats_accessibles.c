/*
 *   Ce fichier fait partie d'un projet de programmation donné en Licence 3 
 *   à l'Université de Bordeaux
 *
 *   Copyright (C) 2015 Adrien Boussicault
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
#include "outils.h"


int test_etats_accessibles(){

	int result = 1;

	Automate* automate = creer_automate();


	/* ajouter_etat( automate, 3 ); */
	/* ajouter_etat( automate, 5 ); */
	/* ajouter_transition( automate, 3, 'a', 5 ); */
	/* ajouter_transition( automate, 5, 'b', 3 ); */
	/* ajouter_transition( automate, 5, 'a', 5 ); */
	/* ajouter_transition( automate, 5, 'c', 6 ); */
	/* ajouter_etat_initial( automate, 3 ); */
	/* ajouter_etat_final( automate, 6 ); */

	ajouter_etat( automate, 0);
	ajouter_etat( automate, 1);
	ajouter_etat( automate, 2);
	ajouter_etat( automate, 3);
	ajouter_etat( automate, 4);
	
	ajouter_transition( automate, 0, 'a', 1);
	ajouter_transition( automate, 0, 'b', 2);
	ajouter_transition( automate, 1, 'b', 3);
	ajouter_transition( automate, 2, 'c', 4);
	ajouter_transition( automate, 3, 'a', 1);

	Ensemble* ens1 = creer_ensemble( NULL, NULL, NULL);
	ajouter_element(ens1, 1);
	ajouter_element(ens1, 3);//L'etat est considéré comme accessible depuis lui même

	Ensemble* ens2 = creer_ensemble( NULL, NULL, NULL);
	ajouter_element(ens2, 4);
	ajouter_element(ens2, 2);//L'etat est considéré comme accessible depuis lui même

	TEST(
	     1
	     && automate
	     && etats_accessibles( automate, 5 ) == NULL
	     && comparer_ensemble( etats_accessibles( automate, 3), ens1) == 0 
      	     && comparer_ensemble( etats_accessibles( automate, 2), ens2) == 0
	     && comparer_ensemble( etats_accessibles( automate, 1), ens2) != 0
	     , result
	     );

	liberer_ensemble(ens1);
	libere_ensemble(ens2);
	liberer_automate( automate );

	return result;
}


int main(){

	if( ! test_etats_accessibles() ){ return 1; }

	return 0;
}

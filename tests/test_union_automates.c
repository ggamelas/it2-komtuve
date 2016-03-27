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

void wrap_liberer_automate( Automate * aut ){
	if( aut ){
		liberer_automate(aut);
	}
}

int test_automate_du_unionge(){

	int result = 1;

	{
		Automate * aut1 = mot_to_automate("a");
		Automate * aut2 = mot_to_automate("b");

		Automate * unio = creer_union_des_automates( aut1, aut2 );
		
		TEST(
			1
			&& unio
			&& le_mot_est_reconnu( unio, "a" )
			&& le_mot_est_reconnu( unio, "b" )
			&& ! le_mot_est_reconnu( unio, "" )
			&& ! le_mot_est_reconnu( unio, "aa" )
			&& ! le_mot_est_reconnu( unio, "bb" )
			&& ! le_mot_est_reconnu( unio, "aaa" )
			&& ! le_mot_est_reconnu( unio, "aab" )
			&& ! le_mot_est_reconnu( unio, "aba" )
			&& ! le_mot_est_reconnu( unio, "abb" )
			&& ! le_mot_est_reconnu( unio, "baa" )
			&& ! le_mot_est_reconnu( unio, "bab" )
			&& ! le_mot_est_reconnu( unio, "bba" )
			&& ! le_mot_est_reconnu( unio, "bbb" )
			, result
		);
		wrap_liberer_automate( aut1 );
		wrap_liberer_automate( aut2 );
		wrap_liberer_automate( unio );
	}


	{
		Automate * aut1 = creer_automate();

		ajouter_transition( aut1, 0, 'a', 1 );
		ajouter_transition( aut1, 1, 'b', 2 );
		ajouter_transition( aut1, 2, 'a', 2 );
		ajouter_transition( aut1, 2, 'b', 1 );
		ajouter_etat_initial( aut1, 0 );
		ajouter_etat_final( aut1, 1 );
		

		Automate * aut2 = creer_automate();

		ajouter_transition( aut2, 0, 'b', 1 );
		ajouter_transition( aut2, 1, 'a', 1 );
		ajouter_transition( aut2, 0, 'a', 0 );
		ajouter_transition( aut2, 1, 'b', 0 );
		ajouter_etat_initial( aut2, 0 );
		ajouter_etat_final( aut2, 1 );

		Automate * unio = creer_union_des_automates( aut1, aut2 );

		TEST(
			1
			&& unio
			&& ! le_mot_est_reconnu( unio, "" )
			&& le_mot_est_reconnu( unio, "a" )
			&& le_mot_est_reconnu( unio, "b" )

			&& ! le_mot_est_reconnu( unio, "aa" )
			&& ! le_mot_est_reconnu( unio, "ab" )
			&& le_mot_est_reconnu( unio, "ba" )
			&& ! le_mot_est_reconnu( unio, "bb" )

			&& ! le_mot_est_reconnu( unio, "aaa" )
			&& ! le_mot_est_reconnu( unio, "aab" )
			&& ! le_mot_est_reconnu( unio, "aba" )
			&& le_mot_est_reconnu( unio, "abb" )
			&& le_mot_est_reconnu( unio, "baa" )
			&& ! le_mot_est_reconnu( unio, "bab" )
			&& ! le_mot_est_reconnu( unio, "bba" )
			&& le_mot_est_reconnu( unio, "bbb" )

			&& ! le_mot_est_reconnu( unio, "aaaa" )
			&& ! le_mot_est_reconnu( unio, "aaab" )
			&& ! le_mot_est_reconnu( unio, "aaba" )
			&& ! le_mot_est_reconnu( unio, "aabb" )
			&& ! le_mot_est_reconnu( unio, "abaa" )
			&& le_mot_est_reconnu( unio, "abab" )
			&& ! le_mot_est_reconnu( unio, "abba" )
			&& ! le_mot_est_reconnu( unio, "abbb" )
			&& le_mot_est_reconnu( unio, "baaa" )
			&& ! le_mot_est_reconnu( unio, "baab" )
			&& ! le_mot_est_reconnu( unio, "baba" )
			&& le_mot_est_reconnu( unio, "babb" )
			&& ! le_mot_est_reconnu( unio, "bbaa" )
			&& le_mot_est_reconnu( unio, "bbab" )
			&& le_mot_est_reconnu( unio, "bbba" )
			&& ! le_mot_est_reconnu( unio, "bbbb" )
			, result
		);
		wrap_liberer_automate( aut1 );
		wrap_liberer_automate( aut2 );
		wrap_liberer_automate( unio );
	}

	return result;
}


int main(){

	if( ! test_automate_du_unionge() ){ return 1; }

	return 0;
}

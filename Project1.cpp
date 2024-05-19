/*
 * Course: CS216-002
 * Project: Project 1
 * Purpose: it reads data from an input file which passes as command line argument
 *          then stores in an object of IMDB class (to represent imdb database)
 *          it then allows user to search for all actors in either one of two movies
 *          two movies are from the user-input, then performs case insensitive matching
 *          and match the movie titles from the database
 * Author: Rylee Hobbs
 */

#include <iostream>
#include <limits>
#include <fstream>
#include <string>
#include <cctype>
#include <set>
#include <map>
#include <sstream>
#include "imdb.h"

using namespace std;

void analyzeMovies(const IMDB& imdb);
void analyzeCoActors(const IMDB& imdb);

int main(int argc, char* argv[])
{
    // Check whether the number of command line arguments is exactly one 
    if (argc != 2)
    {
        cout << "Warning: need exactly one command line argument." << endl;
        cout << "Usage: " << argv[0] << " inputfile_name" << endl;
        return 1;
    }

    ifstream in_file;
    in_file.open(argv[1]);

    // Check whether the input file can be open successfully or not
    if (!in_file.good())
    {
        cout << "Warning: cannot open file named " << argv[1] << "!" << endl;
        return 2;
    }

    // Read data from the input file, assume "good data" from the input file
    // each line of the input file: actor name; movie1; movie2; ...; movieN;
    // read one line at a time then tokenize it:
    // create a pair <actor_name, the set of movieTitles which the actor was in>
    //        and insert to IMDB object
    IMDB cs216_imdb;

    while (!in_file.eof())
    {
        string line;
        getline(in_file, line);
        string actorName, movieTitle;
        // check if the text line is not empty then tokenize it
        if (line.length() != 0)
        {
            istringstream iss(line);
            getline(iss, actorName,';');

            // Extract extra white space
            iss>>ws;

            // Create a new set of movies associated with name
            set<string> movies;

            while (getline(iss, movieTitle, ';'))
            {
                movies.insert(movieTitle);
                // extract white space 
                iss>>ws;
            }
            // insert the pair to IMDB object: cs216_imdb
            cs216_imdb.insert_an_actor(actorName, movies);
        }
        
    }
    // close the input file
    in_file.close();

    string userinput;
    while (true)
    {
        cout << "This application stores information about Actors and their Movies, please choose your option (Enter Q or q to quit):" << endl;
        cout << "1. Actors in Movies" << endl;
        cout << "2. Actors and co-actors" << endl;
        cin >> userinput;
        if (userinput.size() == 1)
        {
            char option = userinput[0];
            switch (option)
            {
                case '1': analyzeMovies(cs216_imdb);
                        break;
                case '2': analyzeCoActors(cs216_imdb);
                        break;
                case 'Q':
                case 'q':
                        cout << "Thank you for using my program, bye..." << endl;
                        return 0;
                default:
                        cout << "Invalid option!" << endl;
            }
        }
        else cout << "Invalid option!" << endl;
    }
}


// this function asks the user to input two movie titles
// then finds the matched two movies from the database
// and displays all the actors in either of the two movies
// if no matched movie then display "Invalid movie title" and do nothing
// note that using const call by reference is for efficiency purpose:
//           no copy of IMDB object is made and the argument has been protected
void analyzeMovies(const IMDB& imdb)
{
    // Ask the user to input two movie titles to search for
    string movie1, movie2;
    cout << "Please input the first movie title: ";
    cin.ignore(256, '\n');
    getline(cin,movie1);
    cout << "Please input the second movie title: ";
    getline(cin,movie2);

    // Check if both movie1 and movie2 can be matched with movies in the database
    // If either one is not, quit the program
    // Otherwise display all the actors in either of two movies
    //                   which are actually the union of two sets
    string matchedMovie1, matchedMovie2;
    matchedMovie1 = imdb.matchExistingMovie(movie1);
    matchedMovie2 = imdb.matchExistingMovie(movie2);
    if ((matchedMovie1.length() > 0) && (matchedMovie2.length() > 0))
    {
        cout << "Your input matches the following two movies: " <<endl;
        cout << matchedMovie1 << endl;
        cout << matchedMovie2 << endl;
        cout << "Both movies are in the database, please continue..." << endl;
        
        set<string> actors_in_movie1 = imdb.find_actors_in_a_movie(matchedMovie1);
        set<string> actors_in_movie2 = imdb.find_actors_in_a_movie(matchedMovie2);
       
        string input;
        char choice = 't';

     while (choice != 'Q' && choice != 'q')
     {
        cout << "Please input your menu option (enter Q or q to quit)" << endl;
        cout << "A -- to print all of the actors in either of the two movies." << endl;
        cout << "C -- to print all the common actors in both of the movies." << endl;
        cout << "O -- to print all the actors who are in only one movies, but not in both." << endl;
        cin >> input;
        if (input.size() == 1)
        {
            choice = input[0];
            switch (choice)
            {
                case 'q':
                case 'Q':
                    {
                        return;
                    }
                case 'A':
                case 'a':
                    {
                        set<string> all;   // to store the union of actors_in_movie1 and actors_in_movie2
                        all = actors_in_movie1;
                        for (auto i = actors_in_movie2.begin(); i != actors_in_movie2.end(); i++)
                            all.insert(*i);
                        // Display all the actors in either one movie
                        cout << "All the actors in either of the two movies:" << endl;
                        for (auto i = all.begin(); i != all.end(); i++)
                        cout << *i << endl;
                        break;
                    }
                case 'C':
                case 'c':
                    {
                        cout << "All actors in both of the movies: " << endl;
                        for (auto i = actors_in_movie1.begin(); i != actors_in_movie1.end(); i++)
                        {
                            for (auto it = actors_in_movie2.begin(); it != actors_in_movie2.end(); it++)
                            {
                                    if (*it == *i)
                                        cout << *it << endl;
                            }
                        }
                        break;
                    }
                case 'O':
                case 'o':
                    {
                        set<string> symDif; // to store the symmetric difference of actors
                        cout << "All actors that are not in both movies" << endl;
                
                        for (auto im = actors_in_movie1.begin(); im != actors_in_movie1.end(); im++)
                            {
                                bool isinboth = false;
                                for (auto is = actors_in_movie2.begin(); is != actors_in_movie2.end(); is++)
                                {
                                    if (*is == *im)
                                        isinboth = true;
                                }
                                if (isinboth == false)
                                    symDif.insert(*im);
                            }
                        for (auto sm = actors_in_movie2.begin(); sm != actors_in_movie2.end(); sm++)
                            {
                                bool isinboth = false;
                                for (auto fm = actors_in_movie1.begin(); fm != actors_in_movie1.end(); fm++)
                                    {
                                        if (*fm == *sm)
                                            isinboth = true;
                                    }
                                if (isinboth == false)
                                    symDif.insert(*sm);

                            }
                        for (auto s = symDif.begin(); s != symDif.end(); s++) //display all elements of symDif
                            {
                                cout << *s << endl;
                            }
                            break;
                    }
                default: 
                        cout << "Invalid input!" << endl;
            }
        }
            else cout << "Invalid input!" << endl;
     }
     return;
    }
    else
    {
        cout << "Invalid movie title." << endl;
        return;
    }
}
// This function asks the user to type in an actor's name
// It then checks if the user's input is a valid name within the imdb database
// If it is valid, display each movie the actor is in as well as the coactors within the same movie
void analyzeCoActors(const IMDB& imdb)
{
    string actor_name;
    cout << "Find co-actors of the actor by typing his/her name: ";
    cin.ignore(256, '\n');
    getline(cin, actor_name);
    if(!imdb.isExistingActor(actor_name))
    {
        cout << "The actor name you entered is not in the database." << endl;
        return;
    }

    set<string> movies_of_actor;
    movies_of_actor = imdb.find_movies_for_an_actor(actor_name);
    for (set<string>::iterator im = movies_of_actor.begin(); im != movies_of_actor.end(); im++)
    {
        cout << "The co-actors of " << actor_name << " in the movie \"" << *im << "\" are: " << endl;
        //Display all the coactors in one movie
        set<string> coactors = imdb.find_actors_in_a_movie(*im);
        for (set<string>::iterator ia = coactors.begin(); ia != coactors.end(); ia++)
        {
            if(*ia != actor_name)
            {
                cout << *ia << endl;
            }
        }
        cout << "***********************************" << endl;
    }
}

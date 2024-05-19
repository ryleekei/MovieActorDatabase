/*
 * File: imdb.cpp
 * Course: CS216-00x
 * Project: Project 1
 * Purpose: Provide the implementation of the class named IMDB
 *          This class represents a movie-actor database
 */
#include <regex>
#include "imdb.h"

using namespace std;

// default constructor
IMDB::IMDB()
{
    //do nothing
    //since map class contains a default contructor
    //      which constructs an empty container, with no elements
}

// insert a pair<actorName, the set of movieTitle of this actor>
void IMDB::insert_an_actor(string actorName, set<string> movieTitles)
{
    // First upgrade actors_db
    // if the key actorName does not exist
    //        add the key to the map actors_db
    if(!isExistingActor(actorName))
        actors_db[actorName] = movieTitles;
    else //if actor does not exist
    {
        actors_db[actorName].insert(movieTitles.begin(), movieTitles.end());
    }

    // Second upgrade movies_db
    // for each movieTitle in the set of movieTitles,
    // insert actorName to its actors set
    for (auto it = movieTitles.begin(); it != movieTitles.end(); it++)
    {
        if (movies_db.find(*it) == movies_db.end())
        {
            set<string> actors;
            actors.insert(actorName);
            movies_db[*it] = actors;
        }
        else
        {
            movies_db[*it].insert(actorName);
        }
    }
    
}

// insert a pair <movieTitle, the set of actor names in this movie>
// this function is optional to this project
void IMDB::insert_a_movie(string movieTitle, set<string> actorNames) 
{
    // First upgrade movies_db
    // if the key moiveTitle does not exist
    //        add the key to the map movies_db
    if (!isExistingMovie(movieTitle))
        movies_db[movieTitle] = actorNames;
    else // if the key movieTitle does exist
    {
        movies_db[movieTitle].insert(actorNames.begin(), actorNames.end());
    }

    // Second upgrade actors_db
    // for each actor name in the set of actorNames,
    // insert movieTitle to its movie set
    for (auto it = actorNames.begin(); it != actorNames.end(); it++)
    {
        if (actors_db.find(*it) == actors_db.end())
        {
            set<string> movies;
            movies.insert(movieTitle);
            actors_db[*it] = movies;
        }
        else
        {
            actors_db[*it].insert(movieTitle);
        }
    }
}

// use passing in parameter, movieTitle, as the pattern to match 
// one existing movie in the map
// return the matched movie title from the map
// otherwise return empty string
string IMDB::matchExistingMovie(string movieTitle) const
{
    string matchedMovie;
    bool found = false;
    // generate the regular expression to match 
    // which contains movieTitle as a substring
    // note that it performs case insensitive matching
    regex reg(movieTitle, regex_constants::icase);
    auto it = movies_db.begin();
    while (!found && it != movies_db.end())
    {
        if (regex_search(it->first, reg))
        {
            found = true;
            matchedMovie = it->first;
        }
        else
            it++;
    }
    return matchedMovie;
}

// check if a movieTitle does exist in the map
// return true if it does; otherwise return false
bool IMDB::isExistingMovie(string movieTitle) const
{
    auto it = movies_db.find(movieTitle); 
    if (it == movies_db.end()) //The iterator goes out of range of the set
        return false;
    else
        return true;
        
}

// check if an actorName does exist in the map
// return true if it does; otherwise return false
bool IMDB::isExistingActor(string actorName) const
{
    auto it = actors_db.find(actorName);
    if(it == actors_db.end())
        return false; //actor name does not exist
    else
        return true; 
}

// return a set of movie titles which actorName is in
// if the passing in parameter: actorName, is not in the database, 
//                   display message and return an empty set
set<string> IMDB::find_movies_for_an_actor(string actorName) const
{
    set<string> actorMovies;
    auto it = actors_db.find(actorName);
    if(it == actors_db.end()) //out of range
    {
        cout<<"Actor" <<actorName<<" not found"<< endl;
        return actorMovies;
    }
    else
    {
        for(auto i = actors_db.begin(); i != actors_db.end(); i++)
        {
            if (i->first == actorName)
                actorMovies = i->second; //insert value for every i into actorMovies
        }
        return actorMovies;
    }
}

// return a set of actor names which are all in the movieTitle
// if the passing in parameter: movieTitle is not in the database, 
//                   display message and return an empty set
set<string> IMDB::find_actors_in_a_movie(string movieTitle) const
{
    set<string> movieActors;//new set to input actor names
    auto it = movies_db.find(movieTitle);
    if(it == movies_db.end())
    {
        cout<<"movie" <<movieTitle<<" not found"<< endl;
        return movieActors; //at this point, movieActors is empty
    }
    else
    {
        for (auto i = movies_db.begin(); i != movies_db.end(); i++)
        {
            if (i->first == movieTitle)
                movieActors = i->second;
        }
        return movieActors;
    }
}

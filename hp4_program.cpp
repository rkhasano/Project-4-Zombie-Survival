/*
MODIFY THIS FILE
*/

#include "hp4_program.hpp"

#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <ctime>
using namespace std;


//! Initializes the program state.
/**
    - Seed the random number generator:       srand( time( NULL ) );
    - Set the two pointer variables to nullptr; m_locationList and m_team.
    - Set the m_week variable to 0.
    - Call the Setup() function.
*/
Program::Program()
{
	srand(time(NULL));
	m_locationList = nullptr;
	m_team = nullptr;
	m_week = 0;
	Setup();
}

//! Cleans up the program.
/**
    - Call the Cleanup() function.
*/
Program::~Program()
{
	Cleanup();
}

//! Calls functions needed for program setup, and then calls Main().
/**
    - Call SetupDynamicArrays()
    - Call LoadLocations()
    - Call LoadPeople()
    - Call Main()


*/
void Program::Setup()
{
	SetupDynamicArrays();
	LoadLocations();
	LoadPeople();
	Main();
}

//! Cleans up the dynamic memory.
/**
    Safely free the memory allocated by m_team and m_locationList.


*/
void Program::Cleanup()
{
	if(m_team !=nullptr)
	{
		delete[]m_team;
		m_team = nullptr;
	}
	
	if (m_locationList != nullptr)
	{
		delete[]m_locationList;
		m_locationList = nullptr;
	}
}

//! Main game loop.
/**
    With the game loop...
        - Increment the week by 1
        - Call DisplayWeekStats()
        - Call UpdateTeam()
        - Call ScavengePhase()
        - Call ZombiePhase()
        - if everyone is dead (call IsEveryoneDead())...
            - done with game loop
        - Call RoundPause()

    - Once the game is over, call DisplaySummary().
*/
void Program::Main()
{
	bool gameover = false;
	while (!gameover)
	{
		m_week++;
		DisplayWeekStats();
		UpdateTeam();
		ScavengePhase();
		ZombiePhase();
		
		if (IsEveryoneDead())
		{
			gameover = true;
		}
		
		RoundPause();
	}
	DisplaySummary();
}

//! Display the base's stats and the current week.
/**
    Display the current week, as well as the current ammo and food held by the base.
*/
void Program::DisplayWeekStats()
{
	cout << "-----------------------------" << endl;
	cout << "------------WEEK " << m_week << "-----------" << endl;
	cout << "BASE STATS" << endl;
	cout << m_base.GetFood() << " unit(s) food \t" << m_base.GetAmmo() << " unit(s) ammo" << endl;
}

//! Update the team's hunger and health.
/**
    For each team member:
        - Call their Update() function
        - Look at the person's GetHunger() value, and request that amount of food from the base via TakeFood().
        - Have person Eat() that food.
        - Display that person's stats afterwards.
*/
void Program::UpdateTeam()
{
	cout << "\nTEAM STATS " << endl;
	for (int i = 0; i < m_teamCount; i++)
	{
		//Integer hunger to store person's hunger value
		int hunger=0;

		//Update person's stats
		m_team[i].Update();
		
		//Display eating person on new line
		cout << "\n";

		//If a person is not dead check his hunger and feed him
		if (!m_team[i].IsDead())
		{
			
			//Make hunger equal to GetHunger
			hunger = m_team[i].GetHunger();
			
			//Take the food from base
			m_team[i].Eat(m_base.TakeFood(hunger));
		}
		//Display stats of a person
		m_team[i].DisplayStats();
	}
}

//! Get player to choose scavenging locations and update base and team stats.
/**
    - Display a list of all the available locations.
    - Ask the user which 3 locations to scavenge at, get their input.
    - Do input validation to make sure they have selected 3 valid indices.
    - Calculate the total amount of ammo salvaged from all 3 locations.
    - Calculate the total amount of food salvaged from all 3 locations.
    - Display the results - amount of food and ammo scavenged (total of each.)
    - Add that amount of ammo and food back to the base.
*/
void Program::ScavengePhase()
{
	//Create the array of size 3, to store 3 locations indexes from the user
	int locations[3];
	//create integer variables ammo and food and initilaze them to 0
	int ammo = 0, food = 0;
	
	
	cout << "\nSELECT 3 LOCATIONS TO SCAVENGE " << endl;

	for(int i = 0; i < m_locationCount;i++)
	{
		cout << i << ". " << m_locationList[i].GetName() << endl;
	}
	
	
	int index = 0;
	while(index < 3)
	{
		//Check if index is valid
		cin >> locations[index];
		if (locations[index] >= 0 && locations[index] < m_locationCount)
		{
			ammo = +m_locationList[locations[index]].GetAmmo();
			food = +m_locationList[locations[index]].GetFood();
			index++;
		}
		else 
		{
			cout << "Invalid location" << endl;
			
		}
	}
	
	cout << ammo << " unit(s) of ammo scavenged" << endl;
	cout << food << " unit(s) of food scavenged" << endl;

	m_base.AddAmmo(ammo);
	m_base.AddFood(food);
}

//! Check for zombie attack, and adjust team health and base stats.
/**
    - Generate a random number between 0 and 3.
    - If the random number is 0, then zombie attack:
        - Randomly select the zombie amount, between 1 and 3.
        - Calculate the total zombie health: 10 x the zombie count.
        - Display a message about the zombie attack, and how many zombies there are.
        - Determine if the base has enough ammo to fend off the zombies.
        - If there is not enough ammo:
            - Display a message that the base is out of ammo.
            - Subtract the amount of ammo the base has from the total zombie health.
            - Base ammo is now 0.
            - Zombies get a chance to attack a person. Randomly select a team member to attack.
            - Display a message of who the zombies attacked.
            - Call that team member's Hurt() function, with a random amount of damage between 10 and 20.
        - If there is enough ammo:
            - Subtract the total zombie health from the ammo count.
            - Display a message that the zombies have been killed, and how much ammo was used (equivalent to total zombie health).
*/
void Program::ZombiePhase()
{
	int randomNumber;
	randomNumber = rand() % 3;

	if(randomNumber==0)
	{
		int zombiesHealth;
		int numberOfZombies;
		
		numberOfZombies = rand() % 3 + 1;
		zombiesHealth = numberOfZombies * 10;

		cout << "\nZOMBIE ATTACK!" << endl;
		cout << numberOfZombies << " zombies attack!" << endl;

		if(zombiesHealth > m_base.GetAmmo())
		{
			//Create a variable for random person to be hurt.
			int randomPerson;
			randomPerson = rand() % m_teamCount;

			//Create random damage variable.
			int damage = rand() % 11 + 10;
			
			cout << "Ran out of Ammo!" << endl;
			zombiesHealth = zombiesHealth - m_base.GetAmmo();
			
			//Set base's ammo to 0.
			m_base.SetAmmo(0);

			cout << "Zombie attacks " << m_team[randomPerson].GetName() << "!" << endl;
			m_team[randomPerson].Hurt(damage);
			
		}
		else 
		{
			m_base.SubtractAmmo(zombiesHealth);
			cout << "Zombie(s) were killed. Used " << zombiesHealth << " unit(s) of ammo." << endl;
		}
	}
}

//! Check if the entire team is dead. If everyone is dead, return true. Otherwise, return false.
/**
    Determine if everybody on the team is dead. If at least one person lives, then this will return false.
    If everybody is dead, return true.

    If everyone is dead, display a message that everyone is dead.

    @return <bool>      true if everyone is dead; false if at least one person is alive.
*/
bool Program::IsEveryoneDead()
{
	int deadsCount=0;
	
	for(int i=0;i< m_teamCount;i++)
	{
		if(m_team[i].IsDead())
		{
			deadsCount++;
		}
	}

	if (deadsCount==m_teamCount)
	{
		return true;
	}
	else
	{
		return false; 
	}
}

//! Display the game summary.
/**
    Display the amount of weeks that the team survived.
*/
void Program::DisplaySummary()
{
	cout << "\nTeam survived " << m_week << " weeks" << endl;
	char q = 'a';
	while (q != 'q') 
	{
		cout << "Press q to quit the game.";
		cin >> q;
	}
}

//! Ask the user for input before continuing the program execution.
/**
    Ask the user to enter a value before continuing.
    (Can just be a dummy cin into a variable that doesn't do anything.)
*/
void Program::RoundPause()
{
	char q='q';
	cout << "Do you want to continue? (y/n): " << endl;

	while(q!='y')
	{
		cin >> q;
	}
}

//! Load location information from the locations.txt file.
/**
    Create an ifstream object and open the locations.txt file.

    Create three temporary variables: name (string), ammo (int), food (int).
    Also create a counter variable i (int) and initialize it to 0.

    Create a while loop, while the counter i is less than the location count m_locationCount...
        - Retrieve name, ammo, and food from the input file
        - Setup location #i via its Setup() function.
        - Increment the counter variable i.

    Close the input file when done.
*/
void Program::LoadLocations()
{
	string name;
	int ammo, food;
	int i = 0;
	ifstream input;
	
	input.open("locations.txt");

	while (i < m_locationCount)
	{
		input >> name >> ammo >> food;
		m_locationList[i].Setup(name, ammo, food);
		i++;
	}
	
	input.close();

}

//! Load a list of names from the names.txt file and randomly assign names to the team.
/**
    Load names:
        - Create an array of strings that will store names that are loaded in. The array size should be 207.
        - Create a counter variable i, initialize it to 0.
        - Create an input file stream, and open names.txt
        - Use a while loop to load data from the input file to the names array
            e.g., while ( input >> names[i] )
        - Within the while loop, increment i by 1.
        - Close the input file once done.

    Assign names to team:
        - For each team member, use the Setup() function.
        - Pass in a random name to the Setup function to assign the person a random name.
*/
void Program::LoadPeople()
{
	string names[207];
	int i = 0;
	ifstream input;
	input.open("names.txt");

	while (input >> names[i])
	{
		i++;
	}
	input.close();

	
	for (int j = 0; j<m_teamCount; j++)
	{
		int b = rand() % 207;
		m_team[j].Setup(names[b]);
	}
}

//! Ask the user how many locations and team members are available, and allocate memory for the dynamic arrays.
/**
    Ask the player how many locations and how many team members to have.

    Max location size is 10. Utilize a while loop to validate that the user has entered
    an amount between 0 and 10.

    Team count has to be 1 or more, use a while loop to validate that the user has entered
    an amount greater than 1.

    Afterward, use the m_locationList and m_team pointers to create
    dynamic arrays with the sizes given.
*/
void Program::SetupDynamicArrays()
{
	cout << "How many locations (maximum 10)?";
	cin >> m_locationCount;
	while (m_locationCount <1 || m_locationCount >10)
	{
		cout << "The number of locations cannot be greater than 10 or less than 1" << endl;
		cin >> m_locationCount;
	}

	cout << "How many people on the team (maximum 207)?";
	cin >> m_teamCount;
	while (m_teamCount<1 || m_teamCount >207)
	{
		cout << "The number of people cannot be greater than 207 or less than 1" << endl;
		cin >> m_teamCount;
	}
	m_locationList = new Location[m_locationCount];
	m_team = new Person[m_teamCount];
}

#include "Person.hpp"

#include <iostream>
#include <iomanip>
#include <cstdlib>
using namespace std;

//! Initializes health to 0 and hunger to 100, and sets the private member variable m_name to the name parameter passed in.
/**
    Initialize hunger to 0 and hunger to 100. Set the person's name to whatever the parameter passed in is.

    @param name <const string&>     The value to assign to the m_name variable
    @return <void>
*/
void Person::Setup( const string& name )
{
	m_name = name;
	m_hunger = 0;
	m_health = 100;
}

//! Update the user's hunger stat, adjusting health if needed.
/**
    If the person is alive (if their health is above 0):
        - Add a random amount to their hunger, between 1 and 10. Use the rand() function.
        - If their hunger is above 100, then:
            - Display a message that this person is starving.
            - Subtract 10 from their health.
        - If their health drops to or below 0:
            - Reset health to 0 (don't let it be negative).
            - Display a message that they have died.

    If hunger goes above 100, then reset it to 100 (don't go above 100).

    @return <void>
*/
void Person::Update()
{
	//Random number to determine how hungry a person is
	int randomNumber;
	randomNumber = rand() % 10;
	
	if (m_health > 0)
	{
		m_hunger += randomNumber;


		if (m_hunger >= 100)
		{
			m_hunger = 100;
			m_health -= 10;
			
		}
	}
	if(m_health < 0)
	{
		m_health = 0;
	
	}
}

//! Display person's name, as well as health and hunger, and whether person is dead.
/**
    Display the person's name, their health, and hunger. If this person is dead,
    add a little "(dead)" flag to the end of their stat.

    @return <void>
*/
void Person::DisplayStats()
{
	cout << m_name << ":  Health:  " << m_health << "  Hunger:  " << m_hunger;
	//If person is alive, but his hunger equals to 100, inform user that the person is starving
	if(!IsDead() && m_hunger==100)
	{
		cout << "(starving)" ;
	}
	//If a person is dead, display a message that he is dead
	if(IsDead())
	{
		cout << "(dead)" ;
	}
	cout << endl;
}

//! Returns true if health is less than or equal to 0, and false otherwise.
/**
    If the person's health is above 0, then they are not dead.
    If their health is 0 or less, then they are dead.

    @return <bool>      Whether or not this person is dead.
*/
bool Person::IsDead()
{
	if (m_health > 0)
	{
		return false;
	}
	else
	{
		return true;
	}
}

//! Returns the person's name.
/**
    @return <string>        The person's name.
*/
string Person::GetName()
{
    return m_name; 
}

//! Adjusts the person's health. If their health drops to or below 0, they will die.
/**
    If the player is dead, then just display the message, "(person) is already dead!"

    If they're not dead, then:
        - Subtract the amount from their health.
        - Display a message of how much damage they took.
        - If their health has dropped to or below 0, then:
            - Reset health to 0 (don't let it be negative.)
            - Display a message that this person has died.

    @param amount <int>     The amount of damage that the player is taking.
    @return <void>
*/
void Person::Hurt( int amount )
{
	if(!IsDead())
	{
		m_health -= amount;
		cout << m_name << " receives " << amount << " damage " << endl;
		
		if(m_health<=0)
		{
			cout << m_name << " died" << endl;
			m_health = 0;
		}
	}
}

//! Return the person's hunger stat.
/**
    @return <int>       The amount of hunger for this person.
*/
int Person::GetHunger()
{
    return m_hunger; 
}

//! Adjust the person's hunger stat based on the amount of food received.
/**
    Display a message that this person is eating <amount> units of food.
    Subtract the amount from the person's hunger.

    If hunger is less than 0, reset it to 0 (don't let hunger be negative.)

    @param amount <int>     The amount of food this person is eating.
*/
void Person::Eat( int amount )
{
	m_hunger = m_hunger - amount;
	cout << m_name << " eats " << amount << " of food." << endl;
	
	if (m_hunger < 0) 
	{
		m_hunger = 0;
	}
}

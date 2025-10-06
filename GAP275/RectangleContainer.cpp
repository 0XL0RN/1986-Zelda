#include "RectangleContainer.h"
#include <iostream>

RectangleContainer::RectangleContainer()
	: m_pRectangles(nullptr)
	, m_length(0)
	, m_occupied(0)
{
}

//////////////////////////////////////////
// Destructor: Cleans up allocated memory
//////////////////////////////////////////
RectangleContainer::~RectangleContainer()
{
	// Delete all rectangle objects
	for (size_t i = 0; i < m_length; ++i)
	{
		delete m_pRectangles[i];
	}
	// Delete the array of pointers
	delete[] m_pRectangles;
	// Print the number of deleted rectangles
	std::cout << "Deleted: " << m_occupied << " Rectangles";
}

/////////////////////////////////////////////
// Initialize the container with a given size
/////////////////////////////////////////////
void RectangleContainer::Init(const size_t& size)
{
	// Allocate memory for the array of rectangle pointers
	m_pRectangles = new SDL_FRect * [size];
	m_length = size;
}

///////////////////////////////////
// Add a rectangle to the container
///////////////////////////////////
void RectangleContainer::Add(SDL_FRect* pRectangle)
{
	// Store the rectangle pointer and increment the occupied count
	m_pRectangles[m_occupied] = pRectangle;
	++m_occupied;
}

//////////////////////////////////////
// Get a rectangle at a specific index
//////////////////////////////////////
SDL_FRect* RectangleContainer::GetAt(const size_t& index) const
{
	// Return the rectangle if it exists, otherwise return nullptr
	if (m_pRectangles[index] != nullptr)
		return m_pRectangles[index];
	return nullptr;
}
#pragma once
#include <math.h>
#include <cassert>

template<class T>
class Array 
{

public:
	//Constructor
	Array(int size, int growBy = 1) :
		m_array(NULL), m_maxSize(0), m_growSize(0), m_numElements(0)
	{
		if (size)	// Is this a legal size for an array?
		{
			m_maxSize = size;
			m_array = new T[m_maxSize];		// Dynamically allocating an array to m_maxSize
			memset(m_array, 0, sizeof(T) * m_maxSize);	// Explicitly set 0 to all elements in the array

			m_growSize = ((growBy > 0) ? growBy : 0);
		}
	}
	// Destructor
	~Array()
	{
		if (m_array != nullptr)
		{
			delete[] m_array;
			m_array = nullptr;
		}
	}
	void pop()
	{
		if (m_numElements > 0)
		{
			m_numElements--;	// Reduce the total number of elements by 1. Ignoring the last element.
		}
	}
	void remove(int index)
	{
		assert(m_array != nullptr);

		if (index >= m_numElements)
		{
			// I am trying to remove something outside of the bounds of the array
			return;	// <-- Maybe could do some form of exception handling
		}

		for (int i = index; i < m_numElements; i++)
		{
			// Start at the index we want to remove.
			// Shift everything after index back by one.
			if (i + 1 < m_numElements)	// Confines the loop into the array
			{
				m_array[i] = m_array[i + 1];
			}
		}
		m_numElements--;
	}
	// Overloaded [] operator
	T& operator[](int index)
	{
		assert(m_array != nullptr && index < m_numElements);
		return m_array[index];
	}
	void clear()
	{
		m_numElements = 0;	 // Ignore (or forgets) all current items in the array
	}
	// Gets and Sets
	int GetSize()
	{
		return m_numElements;
	}
	int GetMaxSize()
	{
		return m_maxSize;
	}
	int GetGrowSize()
	{
		return m_growSize;
	}
	int SetGrowSize(int val)
	{
		assert(val >= 0);
		m_growSize = val;
	}
	int SetSize(int val)
	{
		assert(val >= 0);
		m_numElements = val;
	}
private:
	bool Expand()
	{
		if (m_growSize <= 0)
		{
			// LEAVE!
			return false;
		}

		// Create the new array
		T* temp = new T[m_maxSize * pow(2, m_growSize)];
		assert(temp != nullptr);

		// Copy the contents of the original array into the new array
		memcpy(temp, m_array, sizeof(T) * m_maxSize);

		// Delete the old array
		delete[] m_array;

		// Clean up variable assignments
		m_array = temp;
		temp = nullptr;

		m_maxSize *= pow(2, m_growSize);

		return true;
	}
private:
	// Private Variables
	T* m_array;			// Pointer to the beginning of the array

	int m_maxSize;		// Maximum size of the array
	int m_growSize;		// Amount the array can grow through expansion
	int m_numElements;	// Number of items currently in my array
};

template <class T>
class OrderedArray : public Array<T>
{
public:
	void push(T val)
	{
		//	checks if val is already in the array 
		//	if not then it pushing it into the array
		if (val != binarySearch(val, 0, Array<int>::GetSize() - 1)) {

			assert(T & operator() != nullptr);

			if (Array<int>::GetSize() >= Array<int>::GetMaxSize())
			{
				Expand();
			}

			int i, k;	// i - Index to be inserted. k - Used for shifting purposes
			// Step 1: Find the index to insert val
			for (i = 0; i < Array<int>::GetSize(); i++)
			{
				if (m_array[i] > val)
				{
					break;
				}
			}

			// Step 2: Shift everything to the right of the index(i) forward by one. Work backwards
			for (k = Array<int>::GetSize(); k > i; k--)
			{
				m_array[k] = m_array[k - 1];
			}

			// Step 3: Insert val into the array at index
			m_array[i] = val;

			Array<int>::SetSize(Array<int>::GetSize()+1);

			// return i;
		}
	}
	// Searching
	// Binary Search
	int search(T searchKey)
	{
		// Call to binary search recursive function
		// Binary Search: searchKey, initial lowerBound, initial upperBound
		return binarySearch(searchKey, 0, Array<int>::GetSize() - 1);
	}
private:
	// Private functions
	// Recursive Binary Search
	int binarySearch(T searchKey, int lowerBound, int upperBound)
	{
		assert(m_array != nullptr);
		assert(lowerBound >= 0);
		assert(upperBound < Array<int>::GetSize());

		// Bitwise divide by 2
		int current = (lowerBound + upperBound) >> 1;

		// Check 1 "Base case": Did we find the searchKey at the current index?
		if (m_array[current] == searchKey)
		{
			// We found it! Return the index
			return current;
		}
		// Check 2 "Base base": Are we done searching? 
		else if (lowerBound > upperBound)
		{
			// Did not find searchKey within m_array
			return -1;
		}
		// Check 3: Which half of the array is searchKey in?
		else
		{
			if (m_array[current] < searchKey)
			{
				// Search the upper half of the array
				return binarySearch(searchKey, current + 1, upperBound);
			}
			else
			{
				// Search the lower half of the array
				return binarySearch(searchKey, lowerBound, current - 1);
			}
		}

		return -1;
	}
	bool Expand()
	{
		if (Array<int>::GetGrowSize() <= 0)
		{
			// LEAVE!
			return false;
		}

		// Create the new array
		T* temp = new T[Array<int>::GetMaxSize() * pow(2, Array<int>::GetGrowSize())];
		assert(temp != nullptr);

		// Copy the contents of the original array into the new array
		memcpy(temp, m_array, sizeof(T) * Array<int>::GetMaxSize());

		// Delete the old array
		delete[] m_array;

		// Clean up variable assignments
		m_array = temp;
		temp = nullptr;

		Array<int>::SetMaxSize(Array<int>::GetMaxSize() * pow(2, Array<int>::GetGrowSize()));

		return true;
	}
	private:
		// Private Variables
		T* m_array;			// Pointer to the beginning of the array

};

template <class T>
class UnorderedArray : public Array <T>
{
public:
	// Searching
	// Linear Search
	int search(T val)
	{
		assert(m_array != nullptr);

		// Brute-force Search
		for (int i = 0; i < Array<int>::GetSize(); i++)
		{
			if (m_array[i] == val)
			{
				return i;	// Return the index of where the item is located in the array
			}
		}

		return -1;
	}
	// ---------------- SORTING ALGORITHMS --------------------------------
	// Bubble Sort -- Big O = O(N^2)
	void BubbleSort()
	{
		assert(m_array != nullptr);

		T temp;

		// Start at the end of the array and move backwards
		for (int k = Array<int>::GetSize() - 1; k > 0; k--)
		{
			for (int i = 0; i < k; i++)	 // Compare elements until k is reached
			{
				// Compare 2 adjacent elements
				if (m_array[i] > m_array[i + 1])
				{
					// Swap the elements
					temp = m_array[i];
					m_array[i] = m_array[i + 1];
					m_array[i + 1] = temp;
				}
			}
		}
	}
	// Selection Sort -- Big O = O(N^2)
	void SelectionSort()
	{
		assert(m_array != nullptr);

		T temp;	// Swapping
		int min = 0;	// Hold the index of the current smallest value

		// The lowest position to swap elements into
		for (int k = 0; k < Array<int>::GetSize(); k++)
		{
			min = k;	// Set the value at index k as a "default" minimum value

			// Iterate through the array to find smallest value, if there is one.
			for (int i = k + 1; i < Array<int>::GetSize(); i++)
			{
				// Compare the value at the current index vs the current "min" index
				if (m_array[i] < m_array[min])
				{
					// Store the index to the smallest element
					min = i;
				}
			}

			// Swap the lowest element with the lowest available index
			if (m_array[k] > m_array[min])
			{
				// Swap
				temp = m_array[k];
				m_array[k] = m_array[min];
				m_array[min] = temp;
			}
		}
	}
	// Insertion Sort -- Big O = O(N^2)
	void InsertionSort()
	{
		assert(m_array != nullptr);
		
		T temp;
		int i = 0;

		// Iterate through all elements to be sorted starting with index of 1, increasing as more of the array is sorted
		for (int k = 1; k < Array<int>::GetSize(); k++)
		{
			temp = m_array[k];
			i = k;

			// Shifting of elements if necessary. Create a space for an elements
			// to be inserted in the correct location.
			while (i > 0 && m_array[i - 1] >= temp) //compare temp value with previous index, if prev is larger then stop
			{
				// Push elements to the right
				m_array[i] = m_array[i - 1];
				i--;
			}

			// Place the item in temp into the correct location found by the last loop (i)
			m_array[i] = temp;
		}
	}
	// Merge Sort -- Big O = O(N logN)
	void MergeSort()
	{
		assert(m_array != nullptr);

		T* tempArray = new T[Array<int>::GetSize()]; //create the temparray needed for mergesort
		assert(tempArray != nullptr);

		MergeSort(tempArray, 0, Array<int>::GetSize() - 1);
		delete[] tempArray;
	}
	private:
		// Private functions
		// Recursive Merge Sort
		void MergeSort(T* tempArray, int lowerBound, int upperBound)
		{
			// Base case, if lowerbound=upperbound then there is only 1 element left
			if (lowerBound == upperBound)
			{
				return;
			}

			// Determine the middle of the array
			int mid = (lowerBound + upperBound) >> 1;

			MergeSort(tempArray, lowerBound, mid);	// Mergesort the lower half of the array recursively
			MergeSort(tempArray, mid + 1, upperBound); // Mergesort the upper half of the array recursively

			// Merge by using the temparray, compare values and place sorted pieces back into m_array
			Merge(tempArray, lowerBound, mid + 1, upperBound);
		}
		void Merge(T* tempArray, int low, int mid, int upper)
		{
			// passed in values: lowerbound, Mid+1, Upperbound
			int tempLow = low, tempMid = mid - 1;
			int index = 0;

			// comparing 2 sections of the split array, which of the 2 values is smaller? move it into the temp array
			while (low <= tempMid && mid <= upper)
			{
				if (m_array[low] < m_array[mid])
				{
					tempArray[index++] = m_array[low++];	// Left half lower value is the lowest, put it in temparray, increment
				}
				else
				{
					tempArray[index++] = m_array[mid++];	// Right half lower value is the lowest, put it in temparray, increment
				}
			}

			//clean up remaining values
			while (low <= tempMid) // Clean up lower half, putting in the remaining values into temparray
			{
				tempArray[index++] = m_array[low++];
			}
			while (mid <= upper) // Clean up upper half, putting in the remaining values into temparray
			{
				tempArray[index++] = m_array[mid++];
			}

			// Place the stored tempArray into the main array in the correct locations
			for (int i = 0; i < upper - tempLow + 1; i++)
			{
				m_array[tempLow + i] = tempArray[i];
			}
		}
		bool Expand()
	{
		if (Array<int>::GetGrowSize() <= 0)
		{
			// LEAVE!
			return false;
		}

		// Create the new array
		T* temp = new T[Array<int>::GetMaxSize() * pow(2, Array<int>::GetGrowSize())];
		assert(temp != nullptr);

		// Copy the contents of the original array into the new array
		memcpy(temp, m_array, sizeof(T) * Array<int>::GetMaxSize());

		// Delete the old array
		delete[] m_array;

		// Clean up variable assignments
		m_array = temp;
		temp = nullptr;

		Array<int>::SetMaxSize(Array<int>::GetMaxSize() * pow(2, Array<int>::GetGrowSize()));

		return true;
	}
		private:
			// Private Variables
			T* m_array;			// Pointer to the beginning of the array
};
#include <map>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <numeric>

template<typename K, typename V>
class interval_map {
	friend void IntervalMapTest();
	V m_valBegin;
	std::map<K,V> m_map;
public:
	void printMap() {
		for(auto it=m_map.begin(); it != m_map.end(); ++it)
			std::cout << it->first << ' ' << it->second << '\n';
	}

	// constructor associates whole range of K with val
	interval_map(V const& val)
	: m_valBegin(val)
	{}

	// Assign value val to interval [keyBegin, keyEnd).
	// Overwrite previous values in this interval.
	// Conforming to the C++ Standard Library conventions, the interval
	// includes keyBegin, but excludes keyEnd.
	// If !( keyBegin < keyEnd ), this designates an empty interval,
	// and assign must do nothing.
	void assign( K const& keyBegin, K const& keyEnd, V const& val ) {

		/// Note: there is a case where one interval may be fully included in another, where there should be three resulting intervals.
		/// But as far as assign function doesn't have the information of where the interval ends, it spans out the intervals up to next element.

        bool erase = true;
		bool insert = true;
		
		if(!(keyBegin < keyEnd)) /// make sure to use the < operator as it is the only one guaranteed to be implemented.
            return;

        auto start_it = m_map.lower_bound(keyBegin); /// gives first element to be deleted
        auto end_it = m_map.lower_bound(keyEnd); /// gives first element to not be deleted, may be m_map.end() if all need to be deleted

		if(start_it == m_map.end())
			erase = false; /// nothing to erase, all keys are on left

		if(erase == true && end_it != m_map.end() && end_it->second == val) /// Making sure to not have double entries by checking if after insertion the next value is the same. Then delete it.
			++end_it;
		
		if(start_it != m_map.begin() && std::prev(start_it)->second == val || start_it == m_map.begin() && m_valBegin == val)
			insert = false; /// There is already the same value before the place where we want to insert, we won't insert it again to make sure we're not doing double entries. Takes m_valBegin into account.

		if(erase)
        	m_map.erase(start_it, end_it); /// will erase [keyBegin, keyEnd).

		if(insert)
        	m_map.emplace(keyBegin, val);
    }

	// look-up of the value associated with key
	V const& operator[]( K const& key ) const {
		auto it=m_map.upper_bound(key);
		if(it==m_map.begin()) {
			return m_valBegin;
		} else {
			return (--it)->second;
		}
	}
};

void update(std::vector<char>& vec, int start, int end, char val) {
	for(int i=start; i<=end; ++i)
		vec[i] = val;
}

void print(std::vector<char>& vec) {
	for(int i=0; i<20; ++i)
		std::cout << i-10 << ' ' << vec[i] << '\n';
}

int main() {
	std::srand(static_cast<unsigned int>(std::time(nullptr))); // Seed the random number generator

    interval_map<int, char> imap('B');
	
	std::vector<char> testvec;

	imap.assign(0,1,'D');
	imap.assign(1,2,'B');
	imap.assign(2,3,'D');
	imap.assign(1,2,'A');
	std::cout<<'\n';
	imap.printMap();
	std::cout<<'\n';
	//print(testvec);
	return 0;	
}

// Many solutions we receive are incorrect. Consider using a randomized test
// to discover the cases that your implementation does not handle correctly.
// We recommend to implement a test function that tests the functionality of
// the interval_map, for example using a map of int intervals to char.
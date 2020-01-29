#ifndef HASHTABLE_H_
#define HASHTABLE_H_

#include "prime.h"
#include <vector>
#include <list>
#include <algorithm>
#include <iostream>
using namespace std;



template <typename valueType, typename keyType>
class HTEntry
{
public:
    HTEntry()
    {}

    HTEntry(
        const valueType& value,
        keyType key
    )
    :m_value(value), m_key(key) {}

    HTEntry(const HTEntry<valueType, keyType>& copySource)
    {
        m_key = copySource.m_key;
        m_value = copySource.m_value;
    }

    HTEntry<valueType, keyType>& operator=(const HTEntry<valueType, keyType>& left)
    {
        m_key = left.m_key;
        m_value = left.m_value;
    }

    keyType GetKey() const
    {
        return m_key;
    }

    const valueType& GetValue() const
    {
        return m_value;
    }

    void SetKey(const keyType& key)
    {
        m_key = key;
    }

    void SetValue(const valueType& value)
    {
        m_value = value;
    }

private:
    keyType m_key;
    valueType m_value;
};

/*
template <typename valueType>
class HTOpenAddressing
{
public:
    //Constructor
    //assumption : dynEntryArray doesn't have a pair of elements whose keys are the same
    HTOpenAddressing(double loadFactor, const vector<HTEntry<valueType, unsigned int>>& dynEntryArray)
    :m_numElements(0)
    {
        if (loadFactor > 0 && loadFactor < 1)
            m_loadFactor = loadFactor;
        else 
            m_loadFactor = 0.1;

        m_numSlot = GetPrimeLargerThan(dynEntryArray.size() / m_loadFactor);
        m_empty = m_numSlot + 1;

        m_hashTable = vector<HTEntry<valueType, unsigned int>>(m_numSlot); 

        //Initialize all elements to have m_empty value 
        for_each (m_hashTable.begin(),
                    m_hashTable.end(),
                    [this] (auto& element) {
                        element.SetKey(this->m_empty);
                    });

        for_each (dynEntryArray.cbegin(),
                    dynEntryArray.cend(),
                    [this](auto& element) {
                        this->Insert(element);
                    });

        m_loadFactor = m_numElements / double(m_numSlot);
    }

    double GetLoadFactor()
    {
        return m_loadFactor;
    }

    void Insert(const HTEntry<valueType, unsigned int>& entry)
    {
        int insertedIndex = 0;
        unsigned int key = entry.GetKey();

        int i = 0;
        for (; i < m_numSlot; i++)
        {
            insertedIndex = doubleHashing(key, i);
            if (m_hashTable[insertedIndex].GetKey() == m_empty)
            {
                m_hashTable[insertedIndex] = entry;
                m_numElements++;
                break;
            }
        }

        if (i == m_numSlot)
            cout << "Insertion Failed" << endl;
    }

    unsigned int Search(unsigned int key)
    {
        unsigned int returnIndex = m_empty;
        unsigned int workingIndex = 0;

        for (int i = 0; i < m_numSlot; i++)
        {
            workingIndex = doubleHashing(key, i);
            unsigned int foundKey = m_hashTable[workingIndex].GetKey();
            if (foundKey == key)
            {
                returnIndex = workingIndex;
                break;
            }
            else if (foundKey == m_empty || i == (m_numSlot - 1))
            {
                cout << "not found" << endl;
                break;
            }
        }
        return returnIndex;
    }

    void Show() const
    {
        cout << "load factor :" << m_loadFactor << endl;
        cout << "number of slot : " << m_numSlot << endl;
        cout << "number of elements :" << m_numElements << endl;
        cout << "EMPTY_VALUE : " << m_empty << endl;

        int index = 0;
        for_each(m_hashTable.cbegin(),
                    m_hashTable.cend(),
                    [this, index] (auto& element) mutable {
                        cout << index++ << " : ";
                        if (element.GetKey() == this->m_empty)
                            cout << "empty" << endl;
                        else
                            cout << element.GetKey() << endl;
                    });
    }

private:
    unsigned int doubleHashing(unsigned int key, unsigned int i)
    {
        return (h1(key) + i * h2(key)) % m_numSlot;
    }

    unsigned int h1(unsigned int key)
    {
        return key % m_numSlot;
    }

    unsigned int h2(unsigned int key)
    {
        return key % (m_numSlot - 1) + 1;
    }

    vector<HTEntry<valueType, unsigned int>> m_hashTable;
    double m_loadFactor;
    unsigned int m_numSlot;
    unsigned int m_numElements;
    unsigned int m_empty;
};
*/

//This version implements deletion
template <typename valueType>
class HTOpenAddressing
{
public:
    HTOpenAddressing()
        :m_loadFactor(0),
         m_numSlot(0),
         m_numElements(0), 
         m_empty(0),
         m_deleted(0)
    {}

    //Constructor
    //assumption : dynEntryArray doesn't have a pair of elements whose keys are the same
    HTOpenAddressing(double loadFactor, const vector<HTEntry<valueType, unsigned int>>& dynEntryArray)
    :m_numElements(0)
    {
        if (loadFactor > 0 && loadFactor < 1)
            m_loadFactor = loadFactor;
        else 
            m_loadFactor = 0.1;

        m_numSlot = GetPrimeLargerThan(dynEntryArray.size() / m_loadFactor);
        m_empty = m_numSlot + 1;
        m_deleted = m_numSlot + 2;

        m_hashTable = vector<HTEntry<valueType, unsigned int>>(m_numSlot); 

        //Initialize all elements to have m_empty value 
        for_each (m_hashTable.begin(),
                    m_hashTable.end(),
                    [this] (auto& element) {
                        element.SetKey(this->m_empty);
                    });

        for_each (dynEntryArray.cbegin(),
                    dynEntryArray.cend(),
                    [this](auto& element) {
                        this->Insert(element);
                    });

        m_loadFactor = m_numElements / double(m_numSlot);
    }

    //Constructor
    HTOpenAddressing(unsigned int numSlot)
    {
        m_numElements = 0;
        m_loadFactor = 0;
        m_numSlot = GetPrimeLargerThan(numSlot);
        m_empty = m_numSlot + 1;
        m_deleted = m_numSlot + 2;

        m_hashTable = vector<HTEntry<valueType, unsigned int>>(m_numSlot); 

        //Initialize all elements to have m_empty value 
        for_each (m_hashTable.begin(),
                    m_hashTable.end(),
                    [this] (auto& element) {
                        element.SetKey(this->m_empty);
                    });
    }

    HTOpenAddressing(const HTOpenAddressing<valueType>& source)
    {
        m_hashTable = source.m_hashTable;
        m_loadFactor = source.m_loadFactor;
        m_numSlot = source.m_numSlot;
        m_numElements = source.m_numElements;
        m_empty = source.m_empty;
        m_deleted = source.m_deleted;
        m_emptyEntry = source.m_emptyEntry;
    }
    

    HTOpenAddressing<valueType>& operator=(const HTOpenAddressing<valueType> rightSource)
    {
        m_hashTable = rightSource.m_hashTable;
        m_loadFactor = rightSource.m_loadFactor;
        m_numSlot = rightSource.m_numSlot;
        m_numElements = rightSource.m_numElements;
        m_empty = rightSource.m_empty;
        m_deleted = rightSource.m_deleted;
        m_emptyEntry = rightSource.m_emptyEntry;
    }

    double GetLoadFactor()
    {
        m_loadFactor = m_numElements / (double)m_numSlot;
        return m_loadFactor;
    }

    void Insert(const HTEntry<valueType, unsigned int>& entry)
    {
        int insertedIndex = 0;
        unsigned int key = entry.GetKey();

        int i = 0;
        for (; i < m_numSlot; i++)
        {
            insertedIndex = doubleHashing(key, i);
            if (m_hashTable[insertedIndex].GetKey() == m_empty || m_hashTable[insertedIndex].GetKey() == m_deleted)
            {
                m_hashTable[insertedIndex] = entry;
                m_numElements++;
                break;
            }
        }

        if (i == m_numSlot)
            cout << "Insertion Failed" << endl;
    }

    unsigned int Search(unsigned int key)
    {
        unsigned int returnIndex = m_empty;
        unsigned int workingIndex = 0;

        for (int i = 0; i < m_numSlot; i++)
        {
            workingIndex = doubleHashing(key, i);
            unsigned int foundKey = m_hashTable[workingIndex].GetKey();
            if (foundKey == key)
            {
                returnIndex = workingIndex;
                break;
            }
            else if (foundKey == m_empty || i == (m_numSlot - 1))
            {
                cout << "not found" << endl;
                break;
            }
        }
        return returnIndex;
    }

    void Delete(unsigned int key)
    {
        unsigned int workingIndex = 0;

        for (int i = 0; i < m_numSlot; i++)
        {
            workingIndex = doubleHashing(key, i);
            unsigned int foundKey = m_hashTable[workingIndex].GetKey();
            if (foundKey == key)
            {
                m_hashTable[workingIndex].SetKey(m_deleted);
                m_numElements--;
                cout << "deleted" << endl;
                break;
            }
            else if (foundKey == m_empty || i == (m_numSlot - 1))
            {
                cout << "not found to be deleted" << endl;
                break;
            }
        }
    }

    HTEntry<valueType, unsigned int>& operator[] (const unsigned int key)
    {
        unsigned int foundIndex = Search(key);
        if (foundIndex == m_empty || foundIndex == m_deleted)
        {
            return m_emptyEntry;
        }
        else
        {
            return m_hashTable[foundIndex];
        }
    }

    void Show() const
    {
        cout << "load factor :" << m_loadFactor << endl;
        cout << "number of slot : " << m_numSlot << endl;
        cout << "number of elements :" << m_numElements << endl;
        cout << "EMPTY_VALUE : " << m_empty << endl;

        int index = 0;
        for_each(m_hashTable.cbegin(),
                    m_hashTable.cend(),
                    [this, index] (auto& element) mutable {
                        cout << index++ << " : ";
                        if (element.GetKey() == this->m_empty)
                            cout << "empty" << endl;
                        else if (element.GetKey() == this->m_deleted)
                            cout << "deleted" << endl;
                        else
                            cout << element.GetKey() << endl;
                    });
    }
    
    unsigned int GetEmptyKey()
    {
        return m_empty;
    }

private:
    unsigned int doubleHashing(unsigned int key, unsigned int i)
    {
        return (h1(key) + i * h2(key)) % m_numSlot;
    }

    unsigned int h1(unsigned int key)
    {
        return key % m_numSlot;
    }

    unsigned int h2(unsigned int key)
    {
        return key % (m_numSlot - 1) + 1;
    }

    vector<HTEntry<valueType, unsigned int>> m_hashTable;
    double m_loadFactor;
    unsigned int m_numSlot;
    unsigned int m_numElements;
    unsigned int m_empty;
    unsigned int m_deleted;
    HTEntry<valueType, unsigned int> m_emptyEntry;
};

struct AB
{
    AB()
        :m_a(0), m_b(0), m_key(0) {}

    AB(unsigned int a, unsigned int b, unsigned int key)
        :m_a(a), m_b(b), m_key(key) {}

    unsigned int m_a;
    unsigned int m_b;
    unsigned int m_key;
};

//valueType must have default constructor
//Universal Hashing and Chaining
template <typename valueType>
class HTUnivChaining
{
public:
    //constructor
    //assumption : dynEntryArray doesn't have a pair of elements whose keys are the same
    //maxKey is a key value which is must be the possible maximum value the keys could be
    //maxNumElements is an expected max number of elements which will be stored in the future
    HTUnivChaining(unsigned int numSlot, 
                   const vector<HTEntry<valueType, unsigned int>>& dynEntryArray,
                   unsigned int maxKey,
                   unsigned int maxNumElements)
        :m_numSlot(numSlot) 
    {
        m_prime = GetPrimeLargerThan(maxKey);

        m_hashTable.resize(numSlot);

        HTOpenAddressing<AB> abHashTable(maxNumElements);
        m_abHashTable = abHashTable;

        for_each(dynEntryArray.cbegin(),
                    dynEntryArray.cend(),
                        [this] (auto& element)
                        {
                            this->Insert(element);
                        });
        
        m_loadFactor = m_numElements / m_numSlot;
    }

    void Insert(const HTEntry<valueType, unsigned int>& entry)
    {
        unsigned int key = entry.GetKey();

        //pick a and b randomly
        unsigned int a = GetRandomValue(m_prime - 1) + 1;
        unsigned int b = GetRandomValue(m_prime);

        //calculate slot number using universal hashing
        unsigned int slot = universalHash(a, b, key);

        //insert entry into m_hashTable
        m_hashTable[slot].push_back(entry);

        //record a and b with key value into m_abHashTable
        HTEntry<AB, unsigned int> abEntry(AB(a, b, key), key);
        m_abHashTable.Insert(abEntry);

        m_numElements++;
    }

    const valueType& Search(unsigned int key)
    {
        //get a & b value from m_abHashTable
        if (m_abHashTable[key].GetKey() == m_abHashTable.GetEmptyKey())
        {
            cout << "Not Found" << endl;
            return m_emptyValue;
        }
        else
        {
            AB ab = m_abHashTable[key].GetValue();

            //calculate slot using a b and key by universalHash
            unsigned int slot = universalHash(ab.m_a, ab.m_b, key);

            //list<HTEntry<valueType, unsigned int>>::iterator ite;
            auto ite = m_hashTable[slot].cbegin();
            for (; ite != m_hashTable[slot].cend(); ite++)
            {
                if ((*ite).GetKey() == key)
                    break;
            }

            if (ite == m_hashTable[slot].cend())
            {
                //not found
                cout << "not found from HTUnivChaining::Search" << endl;
                return m_emptyValue;
            }
            else
            {
                return (*ite).GetValue();
            }
            
        }
    }

    void Delete(unsigned int key)
    {
        if (m_abHashTable[key].GetKey() == m_abHashTable.GetEmptyKey())
        {
            cout << "Not Found" << endl;
        }
        else
        {
            AB ab = m_abHashTable[key].GetValue();

            //calculate slot using a b and key by universalHash
            unsigned int slot = universalHash(ab.m_a, ab.m_b, key);

            auto ite = m_hashTable[slot].begin();
            for (; ite != m_hashTable[slot].end(); ite++)
            {
                if ((*ite).GetKey() == key)
                    break;
            }

            if (ite == m_hashTable[slot].cend())
            {
                //not found
                cout << "not found from HTUnivChaining::Delete" << endl;
            }
            else
            {
                m_hashTable[slot].erase(ite);
            }
            
        }

    }

    void Show()
    {
        int index = 0;
        for_each(m_hashTable.cbegin(),
                    m_hashTable.cend(),
                        [&index] (auto& element) mutable {
                            cout << index++ << " : ";
                            for_each(element.cbegin(),
                                        element.cend(),
                                        [] (auto& e){
                                            cout << " --> " << e.GetKey();
                                        });
                            cout << endl;
                        });
    }


private:
    unsigned int universalHash(unsigned int a, unsigned int b, unsigned int key)
    {
        return ((a * key + b) % m_prime) % m_numSlot;
    }

    unsigned int m_numSlot;
    unsigned int m_prime;
    vector<list<HTEntry<valueType, unsigned int>>> m_hashTable;
    HTOpenAddressing<AB> m_abHashTable;
    unsigned int m_numElements;
    unsigned int m_loadFactor;
    valueType m_emptyValue;
};

#endif
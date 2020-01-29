Implemented 2 types of Hash Table.
1. HTOpenAddressing
   is a class of hash table using an open addressing method to solve collisions.
   It uses double hashing as its hash function.
2. HTUnivChaining
  is a class of hash table using a chaining method to solve collisions.
  It uses universal hashing as its hash function.

usage
```C++
HTOpenAddressing htoa(100); //hash table with 100 empty slots
HTEntry<YourClass, unsigned int> entry(YourClass( ... ), 2020);
htoa.Insert(entry);

```

#include <cmath>
#include <cstdio>
#include <vector>
#include <iostream>
#include <algorithm>

using namespace std;

class IntCell 
{
    
  private:
    int * storedValue;
  public:
    IntCell(int initialValue=0) {
        storedValue = new int;
        *storedValue = initialValue;
    }
    
    // Copy Constructor
    IntCell(const IntCell & rhs)
    {
        storedValue = new int;
        *storedValue = *(rhs.storedValue);
    }
    
    // Copy assignment
    IntCell & operator=(const IntCell & rhs)
    {
        *storedValue = *(rhs.storedValue);
        return *this;
    }
    
    // Move Constructor
    IntCell (IntCell && rhs)
    {
        storedValue = new int;
        storedValue = rhs.storedValue;
        rhs.storedValue = nullptr;
    }
    
    //Move Assignment
    IntCell & operator=(IntCell && rhs)
    {
        swap(storedValue, rhs.storedValue);
        return *this;
    }
    
    /* You may need to define copy/move constructors and assignment operators*/
    int read() const {return *storedValue;}
    void write(int x) {*storedValue = x;}
    
};

class Collection 
{
    
  private:
  	IntCell * arr;
  	IntCell size;
    int count;
    
    void copy(IntCell* arr1, const IntCell* arr2, int n);

  	/* You may need to define more members */
  
  public:
    // Constuctor
  	Collection(int inpSize=10) : size(inpSize) 
    {
        arr = new IntCell[size.read()];
        count = 0;
    }
    
    // Copy constructor
    Collection (const Collection & rhs) 
    {
        size = rhs.size;
        arr = new IntCell[size.read()];
        // for(int i=0; i<size.read(); i++)
        // {
        //     arr[i] = rhs.arr[i];
        // }
        copy(arr, rhs.arr, size.read());
        count = rhs.count;
    }
    
    // Copy assignment
    Collection & operator=(const Collection & rhs) 
    {
        size = rhs.size;
        arr = new IntCell[size.read()];
        // for(int i=0; i<size.read(); i++)
        // {
        //     arr[i] = rhs.arr[i];
        // }
        copy(arr, rhs.arr, size.read());
        count = rhs.count;
        return *this;
    }
    
    // Move constructor
    Collection (Collection && rhs)
    {
        size = rhs.size;
        arr = new IntCell[size.read()];
        arr = rhs.arr;
        rhs.arr = nullptr;
        count = rhs.count;
    }
    
    // Move assignment
    Collection & operator=(Collection && rhs)
    {
        swap(arr, rhs.arr);
        return *this;
        count = rhs.count;
    }
  
  	~Collection()
    {
        delete arr;
    }
    
    bool isEmpty();
    bool contains(const IntCell & x);
    void remove(const IntCell & x);
    void insert(const IntCell & x);
    void makeEmpty();
};

void Collection::copy(IntCell* arr1, const IntCell* arr2, int n)
{
    for(int i=0; i<n; i++)
    {
        arr1[i] = arr2[i];
    }
}

bool Collection::isEmpty()
{
    if(count==0) return true;
    return false;
}

bool Collection::contains(const IntCell & x)
{
    for(int i=0; i<count; i++)
    {
        if(arr[i].read()==x.read()) return true;
    }
    return false;
}

void Collection::remove(const IntCell & x)
{
    for(int i=0; i<count; i++)
    {
        if(arr[i].read()==x.read())
        {
            arr[i] = arr[count-1];
            count--;
            i--;
        }
    }
}

void Collection::insert(const IntCell & x)
{
    if(count==size.read())
    {
        IntCell* new_arr = new IntCell[size.read() + 1];
        copy(new_arr, arr, size.read());
        new_arr[count] = x;
        size.write(size.read() + 1);
        delete arr;
        arr = new_arr;
    }
    else
    {
        arr[count] = x;
    }
    count++;
}

void Collection::makeEmpty()
{
    count = 0;
}

/*DO NOT MAKE ANY CHANGES TO THE CODE BELOW*/

int main() {
  int N;
  cin >> N;
  Collection c;
  int command;
  for (int i = 0; i < N; i++)
  {
    cin >> command;
    switch (command)
    {
      case 1 /*"initialize"*/:
        {
            int inpSize;
            cin >> inpSize;
            Collection c2(inpSize);
            c = c2;
        }
        break;

       case 2 /*"initializeWith"*/:
       {
            int size2;
            cin >> size2;
            Collection c2(size2);
            for (int j = 0; j < size2; j++)
            {
              int elem;
              cin >> elem;
              IntCell elemCell(elem);
              c2.insert(elemCell);
            }
            Collection c3 = c2;
            c = c3;
            break;
       }
      case 3 /*"insert"*/:
        {
            int elem;
            cin >> elem;
            IntCell elemCell(elem);
            c.insert(elemCell);
            break;
        }
      case 4 /*"remove"*/:
       {
            int elem;
            cin >> elem;
            IntCell elemCell(elem);
            c.remove(elemCell);
            break;
       } 
      case 5 /*"copy"*/:
       {
            int size2;
            cin >> size2;
            Collection c2(size2);
            for (int j = 0; j < size2; j++)
            {
              int elem;
              cin >> elem;
              IntCell elemCell(elem);
              c2.insert(elemCell);
            }
            c = c2;
            break;
       }
      case 6 /*"isEmpty"*/:
      {
            if (c.isEmpty())
            cout << "Collection is empty" << endl;
            else
            cout << "Collection is not empty" << endl;
            break;
      }
      case 7 /*"makeEmpty"*/:
            c.makeEmpty();
            break;
      case 8 /*"contains"*/:
      {
            int elem;
            cin >> elem;
            IntCell elemCell(elem);
            if (c.contains(elemCell))
            cout << "Collection contains " << elem << endl;
            else
            cout << "Collection does not contain " << elem << endl;
            break;
      }
      default:
            cout << "Invalid command!" << endl;
    }
  }
  return 0;
}
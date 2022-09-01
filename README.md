# Iterable
Introduction
In this assignment, you will write a series of routines that will perform basic actions on iterables. An iterable
represents a sequence of values. All iterable instances are read-only, you cannot update an iterable. However, it
is possible to create new iterables from existing ones.
An iterable i can be of finite or infinite size. Each value in i is called an element of i. Since i is a sequence, its
elements have an ordering. Although the iterable interface does not allow indexing, for ease of description in
this text, we will use i[x] to refer to the xth element of i where x is a 0-based index. In other words, the iterable
i is a sequence of the form:
i[0] , i[1] , i[2] , . . .
In this text, we will also use len(i) to refer to the number of elements in i. It is possible that len(i) = ∞.
We read the elements of an iterable by creating an iterator, via the iterable’s iter() function. The elements can
then be iterated through by repeatedly calling the next() function on the iterator. To be more precise, a newly
created iterator points to the first element of the iterable. Each next() call returns the pointed element and
advances the iterator to the next element. When all elements of the iterable are exhausted, the iterator throws
a StopIteration exception upon a next() call (and it should continue throwing the same exception for further
next() calls).
It is possible to create multiple iterators on a single iterable. Each iterator iterates through the iterable’s elements independently. In other words, one can iterate through an iterable’s elements more than once and
simultaneously.
The following interface represents an iterable:
  template < typename T >
  class IIterable
  {
  public :
  // Returns a new iterator for this iterable .
  virtual IIterator <T > * iter () const = 0;
  // Destroys the iterable .
  virtual ~ IIterable () {};
  };
  The following interface represents an iterator:
  template < typename T >
  class IIterator
  {
  public :
  // Returns the element pointed by this iterator and advances
  // to the next element in the iterable . If the iteration has
  // already reached the end of the iterable , this call throws
  // a StopIteration exception .
  virtual T next () = 0;
  // Destroys the iterator .
  virtual ~ IIterator () {};
  };
  Finally, the following defines the StopIteration exception:
  class StopIteration : public std :: exception {};
  Task
Iterables are created and manipulated via a set of global function templates which you are going to implement.
Here is the list of the functions you need to implement:
• template < typename T > IIterable <T > * repeat ( T v );
Creates an iterable representing the infinite sequence
v, v, v, . . .
• template < typename T > IIterable <T > * count ( T start , T delta );
Creates an iterable representing the infinite sequence
start , start + delta , start + delta + delta , . . .
• template < typename T > IIterable <T > * skip ( unsigned k , IIterable <T > * i );
Skips the first k elements of i. This would correspond to the sequence
i[k] , i[k + 1] , i[k + 2] , . . .
If k ≥ len(i), then the resulting iterable is empty. The result has infinite length if len(i) = ∞.
• template < typename T > IIterable <T > * take ( unsigned k , IIterable <T > * i );
Creates an iterable representing the first k elements of i. That is,
i[0] , . . . , , i[k − 1]
The resulting iterable is always finite. If len(i) ≤ k elements, then the result contains all elements of i.
• template < typename T > IIterable <T > * cycle ( IIterable <T > * i );
Creates an iterable that cycles through i infinitely many times. That is, it represents the sequence
i[0] , . . . , i[len(i) − 1] , i[0] , . . . , i[len(i) − 1] , i[0] , . . .
If i is empty then so is the resulting iterable. If len(i) = ∞, then the result is equivalent to i.
• template < typename T > IIterable <T > * concat ( IIterable <T > *i ,
IIterable <T > * j );
Concatenates two iterables i and j. That is,
i[0] , . . . , i[len(i) − 1] , j[0] , . . . , j[len(j) − 1]
If len(i) = ∞, then the result is equivalent to i.
• template < typename T > IIterable <T > * alternate ( IIterable <T > *i ,
IIterable <T > * j );
Alternates between two iterables i and j. That is,
i[0] , j[0] , i[1] , j[1] , . . .
If either i or j is exhausted before the other one, the iterable continues only with the elements of the
non-exhausted iterable.
• template < typename T > IIterable <T > * filter ( IIterable <T > *i ,
IIterable <bool > * f );
Filters i with a Boolean iterable f. That is, returns an iterable representing
i[x1] , i[x2] , i[x3] , . . .
where the sequence (xp) is the increasing sequence of indices such that f[xp] = true. If i and f have
different lengths, the iteration finishes when either i or f is exhausted.
To ease your thinking process, we will provide you an implementation of the following function:
• template < typename T > IIterable <T > * accumulate ( IIterable <T > * i );
Creates an iterable representing
i[0] , i[0] + i[1] , i[0] + i[1] + i[2] , . . .
Ownership Semantics
The usage of the iterable and iterator objects is subject to a particular ownership semantics. At any time, an
iterable/iterator object is owned by an owner code. This owner code is the sole code that can work on the
object, but is also responsible for destroying the object when the object is no longer needed.
The following are the rules that describe the ownership:
• When an iterable is created via one of your functions, the caller becomes the owner of the returned
iterable.
• When an iterable is passed to one of your functions as a parameter, your code becomes the new owner
of the passed iterable. Since the caller code is losing ownership, it will guarantee that all iterators on the
iterable have been destroyed at the time of calling. That also means that your code is now responsible for
destroying the iterable. Of course, you cannot destroy the passed iterable right away; it will have to be
used by the iterable that you return. (You cannot copy the elements of that iterable to your new iterable.
Guess why?) The iterable that you return should keep track of the passed iterable and destroy it when it
itself is destroyed.
• For functions that take two iterable parameters, the caller guarantees that they are different instances.
• When an iterator is created via iter(), the owner of the iterator is the caller. The ownership cannot be
passed around, therefore the caller of iter() is responsible ensuring the proper destruction of the iterator.
The main reason to define an ownership semantics is to ensure that all unneeded memory is properly and safely
deallocated. In other words, it prevents memory leaks. Your code is responsible for following the rules above
and deleting all owned objects that are no longer needed. Our grader will follow the rules and appropriately
delete all objects that is owned by our code. If we detect a memory leak in your implementation, you may lose
a portion of the points.


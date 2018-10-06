# Exercises Section 13.4

**Exercise 13.33**: Why is the parameter to the save and remove members of
Message a Folder&? Why didn’t we define that parameter as Folder? Or
const Folder&?

**Exercise 13.34**: Write the Message class as described in this section.

**Exercise 13.35**: What would happen if Message used the synthesized
versions of the copy-control members?

**Exercise 13.36**: Design and implement the corresponding Folder class.
That class should hold a set that points to the Messages in that Folder.

**Exercise 13.37**: Add members to the Message class to insert or remove a
given Folder* into folders. These members are analogous to Folder’s
addMsg and remMsg operations.

**Exercise 13.38**: We did not use copy and swap to define the Message
assignment operator. Why do you suppose this is so?

# Exercises Section 13.5
**Exercise 13.39**: Write your own version of StrVec, including versions of
reserve, capacity (§ 9.4, p. 356), and resize (§ 9.3.5, p. 352).

**Exercise 13.40**: Add a constructor that takes an initializer_list<string>
to your StrVec class.

**Exercise 13.43**: Rewrite the free member to use for_each and a lambda
(§ 10.3.2, p. 388) in place of the for loop to destroy the elements. Which
implementation do you prefer, and why?

**Exercise 13.44**: Write a class named String that is a simplified version of
the library string class. Your class should have at least a default
constructor and a constructor that takes a pointer to a C-style string. Use
an allocator to allocate memory that your String class uses.

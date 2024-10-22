/********************************
The following mySwap works like the swap function in the library. 
********************************/

template <class T> //Parameters, variables, and functions after this line that are type "T" can have/return any data type (int or string in this case). 
void mySwap(T& a, T& b) //The "&" means we are passing "a" and "b" by reference, meaning these are the actual variables themselves that were declared in swapClient.cpp, rather than being copies of them (we actually want to swap the original "a" and "b").  
{
  T temp = a; //Setting a "temporary" variable of any data type (int or string in this case) to "a" so we don't lose the original "a" value when it gets overwritten with "b".      
  a = b; //Overwriting the original "a" variable with the same value as the "b" variable. 
  b = temp; //Overwriting the original "b" variable with our new "temporary" variable, so "b" now stores the original "a" value. 
}

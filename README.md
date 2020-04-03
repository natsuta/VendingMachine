# VendingMachine
For Advanced Programming Techniques Semester 1 2018  
Code checked April 2020

# Background
This assignment involves the implementation of a vending machine application for pastry treats, which involves file handling and command-line arguments. Coin and stock data is tracked as the application is used.

# How to run
Compile the program in GCC using the included Makefile command `make all`.

Execute the program using the command `vm <stock file> <coins file>`.

# Notes
- The files `stock.dat` and `coins.dat` are provided, but you can also use your own stock and coin files.
  - The format of the stock file needs to be as follows:  
  `[ItemID]|[ItemName]|[ItemDesc]|[Price]|[NumberOnHand]`
  - The format of the coin file needs to be as follows:  
  `[denomination],[quantity]`
- Denominations need to be given in cents. The values of all Australian coins along with $5 and $10 notes are accepted.  
Example: $1 = 100 cents, $5 = 500 cents
#A Parallel in-memory database management system
##Features
* This database system is based upon the concept of shared nothing architecture among a cluster of compute nodes.
* It parallelizes the insertion of data and the processing of queries.
* Each compute node consists of two processes (A & B), process A is responsible for receiving user queries and processing final results while process B is responsible for periodically inserting the data into main memory from text file as well as processing the queries received from process A.
* At the moment, the database system is capable of working with only a fixed schema that represents a Sale Database. The schema is as follows:

Entity | Description
-------|------------
sales_id| an auto-incrementing integer
date| yyyy/mm/dd format
company_id| auto-incrementing integer (starting from 1).
company_name| A set of unique strings. The count will match with that of company_id
sales_total| A value between 0.99 to 999.99
* It can only process a predefined set of queries on the above presented Sale Database. These queries are as follows:
  + Company Sale - Returns the total sale for each of the companies in the database.
    Result schema is as follows:
    
    Entity | Description
    -------|------------
    company_name| A unique string representing company name
    Total_Sale | A floating point value representing the total sale for this company
    
  + Sale by Date - Returns the Sale total for each date in the user specified date range.
    Result Schema is as follows:
    
    Entity | Description
    -------|------------
    Date| yyyy/mm/dd format
    Total_Sale| A floating point value representing the total sale on this date
  
  + Delete - Deletes all the sale records with date in the user specified date range. i.e delete by date range.
  
## Compilation and Execution Instructions
* The following are the minimum requirements to compile the program
  + C11 Compiler
  + OpenMP 2.0 Compiler
* In order to compile the program, go to the directory consisting of all the source files and execute the following command
  + mpicc *.c *.h -o exe
* In order to run the program, please use the following command
  + mpirun -np _a_ ./exe _b_
  where a = the number of processes in the MPI environment and b = an integer between 10 and 1000 that will be used to 
  determine the number of records to be read per signal time (which is set to 3 seconds by default).

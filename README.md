# File-Chat

File Chat Program
This is a C or C++ application to read from the screen and write that data with a timestamp to an output file. The application also reads from an input file and prints the data read to the screen. Reads are normally blocking calls. Setting up timer generation in a certain way will allow your program to break out of those calls.

1. The program accepts the name of a file on the command line as the first argument. This is the input file for the application.

2. The program accepts the name of a file on the command line as the second argument. This is the output file for the application.

3. The program reads from standard input, accepting input data from the user.
(a) The program writes all data received from the user to the output file.
(b) The program time stamps all output data written into the output file.
(c) The format of the output file shall be TIMESTAMP,STRING\n. This output file will contain comma separated values with two columns: the time stamp and the input data.

4. The program will read from an input file
(a) The program will write all data read from the input file to the screen. If there is no data to be written, go back to reading input from the screen.

## MICE_DATA

# FINISHED 
    [x] DFA 
    [x] COSIONOR ANALYSIS 
    [x] POLYNOMIAL FIT
    [x] MULTI PARAMETER LINEAR FIT 
    [x] RHYTHM ANALYSIS 
    [x] COSINOR PRECESSION

# Not finished



# Usage 
    $ ./test [path] -- will do analysis on every temp or act file(with two days data)
    $ ./main --help  -- will show you how to use this program 

    ./main [OPTION]... [ARGS]... 
    	-f, --file
    	    Name of the file to be analysised, followed by a file name
    	-T, --filetype
    	    The type of graph. [ MICE_DATA | MICE_DATA_38 | SINGLE_COLUM ]
    	-d, --dfa
    	    Exec dfa, followed by dfa order.
    	-m, --mean
    	    Exec mean analysis
    	-s, --std
    	    Exec standard analysis
    	-t, --graph-type
    	    The type of graph. [ GINDIVIDUAL | GAVERAGE | GDEVIATION]
    	-c, --checkp
    	    Check if the period equals to 24h
    	-D, --duration
    	    Set the duration of a single segment, 1 = 10sec (1080 = 3h)
    	-L, --dlength
    	    Set the whole length of individual dfa, 1 = 10sec
    	-P, --startp
    	    Set the start point of individual dfa, 1 = 10sec
    	-S, --graph-size
    	    Determint the dimensions of the graph
    	-o, --output
    	    Generate a pdf file for the graph, followed by a file name. If 
    	not seted, show a gnuplot window
    

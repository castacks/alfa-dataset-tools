from alfa_python import Sequence
# from alfa_python import Commons

parsed = ParseCommandLine(sys.argc, sys.argv, sequenceDir, sequenceName)




def ParseCommandLine(argc, argv, out_sequence_path, out_sequence_name):

    #  Check the number and the format of the inputs
    if ((argc != 2) or (argv[argc-1] == NULL) or (argv[argc-1][0] == '-') ):
    
        PrintHelpMessage()
        return false
    

    #  Extract the path and the sequence name
    bag_path = argv[1]
    extracted = Commons.ExtractFilenameAndExtension(bag_path, out_sequence_name, extension, out_sequence_path)

    #  Check that the file exists and extension is correct
    if (not extracted or (extension != "bag")):
    
        PrintHelpMessage()
        return false
    

    #  Add the path separator to the path
    if (not out_sequence_path or out_sequence_path[out_sequence_path.length() - 1] != Commons.FilePathSeparator) 
        out_sequence_path += Commons.FilePathSeparator

    return true

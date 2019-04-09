 /* example.i */
 %module example
 %{
 /* Put header files here or function declarations like below */
 extern int main(int argc, char** argv);
 extern bool ParseCommandLine(int argc, char** argv, std::string &out_sequence_path, std::string &out_sequence_name);
 %}
 
 extern int main(int argc, char** argv);
 extern bool ParseCommandLine(int argc, char** argv, std::string &out_sequence_path, std::string &out_sequence_name);
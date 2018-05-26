#include "project01.h"

extern char **environ;
string user = getenv("USER"); // Sets default to current user
deque<string> dlist_storage = {};
vector<string> hlist_storage = {};

bool starts_with(string a, string b){
    //Example usage: starts_with("Apple", "App") --> True
    return a.length() >= b.length() && a.substr(0,b.length()) == b;
}

bool ends_with(string a, string b){
    //Example usage: starts_with("Apple", "ple") --> True
    return a.length() >= b.length() && a.substr(a.length()-b.length(),b.length()) == b;
}

void strip(string& s){
    // Removes leading or trailing whitespace inplace
    const string DELIMITERS = " \f\n\r\t\v";
    s.erase( 0, s.find_first_not_of( DELIMITERS ) );
    s.erase( s.find_last_not_of( DELIMITERS ) + 1 );
}

void write_dlist(string dir){
    if(dlist_storage.size()==10){
        dlist_storage.pop_back();
    }
    dlist_storage.push_front(dir);
}

void cd(string command){
    if(starts_with(command, "cd #")){ // Replaces hashtag with directory from dlist
        int indx = stoi(command.substr(4,command.length()))-1;
        command = "cd "+dlist_storage[indx];
    } else if(starts_with(command, "cd ~")){
        if(command.length()==4){ // Reset to home directory of current user
            command = "cd /user/"+user;
        } else{ // Reset to directory of user
            command = "cd /user/"+command.substr(4,command.length());
        }
    }
    string dir = command.substr(3,command.length()); // Removes cd from command
    if(chdir(dir.c_str())==-1){
        cout << "Not a valid directory" << endl;
    } else{
        char cwd[1024];
        getcwd(cwd, sizeof(cwd));
        write_dlist(cwd);
    }
}

void curr(){
    // Prints current directory
    char cwd[1024];
    if (getcwd(cwd, sizeof(cwd)) != NULL){
        cout << "Current directory is: " << cwd << endl;
    }
}

void date(){
    // Prints current time and date from system
    time_t now = time(0);
    cout << "Current time is: " << ctime(&now);
}

void dlist(){
    // Lists previous ten directories in reverse order
    int i = 1;
    for(string d : dlist_storage) {
        cout << i << " " << d << endl;
        i++;
    }
}

void env(int argc, char **argv){
    // Outputs current environment variables
    for(char **current = environ; *current; current++) {
        cout << *current << endl;
    }
}

void hlist(){
    // Lists up to ten previous commands
    int init = ((hlist_storage.size()>10) ? (hlist_storage.size()-10) : 0);
    for(size_t i=init; i<hlist_storage.size(); i++){
        cout << i+1 << " " << hlist_storage[i] << endl;
    }
}

string get_hlist(string command){
    // Returns command for given hlist index
    int indx = stoi(command.substr(1,command.length()))-1;
    string i = hlist_storage[indx];
    cout << i << endl;
    return i;
}

void execute(int argc, char **argv, bool should_wait)
{
    pid_t pid;
    if ((pid = fork()) < 0) {// fork a child process
        cout << "*** ERROR: forking child process failed" << endl;
        exit(1);
    } else if(pid == 0){ // for the child process
        if(!should_wait){
            argv[argc-1] = NULL;
        }
        if (execvp(*argv, argv)< 0){// execute the command
            cout << "*** ERROR: exec failed" << endl;
        }
        exit(1);
    } else if(should_wait){ // Main process
        cout << "Waiting for child process" << endl;
        while (wait(NULL) != pid); // wait for completion
    } else{
        cout << "Child process running in the background" << endl;
    }
}

int parse(char *line, char **argv){
    int argc=0;
    while (*line != '\0') { // if not the end of line
    while (*line == ' ' || *line == '\t' || *line == '\n')
    *line++ = '\0'; // replace white spaces with 0
    *argv++ = line; // save the argument position
    argc++;
    while (*line != '\0' && *line != ' ' && *line != '\t' && *line != '\n')
        line++; // skip the argument until
    }
    *argv = NULL; // mark the end of argument list
    return argc;
}

int main(int a, char **b){
    int counter = 1;
    int argc;
    char* argv[1024];
    char holder[100];
    char cwd[1024];
    getcwd(cwd, sizeof(cwd));
    write_dlist(cwd);
    while(1){
        string command = "";
        cout << "<" << counter << " "<< user << ">";
        cin.getline(holder,sizeof(holder));
        command = holder;
        strip(command);
        argc = parse(holder,argv);
        if(command!=""){
            hlist_storage.push_back(command);
            counter++;
        }
        if(starts_with(command, "!")){
            command = get_hlist(command);
        } 
        if(command=="quit"){
            return 0;
        } else if(command=="date"){
            date();
        } else if(command=="curr"){
            curr();
        } else if(command=="env"){
            env(a, b);
        } else if(command=="hlist"){
            hlist();
        } else if(command=="dlist"){
            dlist();
        } else if(starts_with(command, "cd")){
            cd(command);
        } else{ // Attempt to exec. an external command
            bool should_wait = strcmp(argv[argc-1],"&");
            //ends_with(command, "&") ? true : false;
            execute(argc, argv, should_wait);
        }        
    }
}
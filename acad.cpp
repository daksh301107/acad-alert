#include<iostream>
#include<vector>
#include<fstream>
#include<sstream>
#include<iomanip>
#include <limits> 
using namespace std;

// for categories of students goal
enum class GoalCategory{PASS,DECENT,TOPPER};

float getThreshold(GoalCategory g){
    if(g==GoalCategory::PASS)return 40;
    if(g==GoalCategory::DECENT)return 60;
    return 80;
}

// SUBJECT CLASS
class Subject{
private:
    string name;
    float marks;

public:
    void input(string n){
        float practical,minor,attendance;
        name=n;
        cout<<"\nName : "<<name<<"\n";

        cout<<"Enter Practical marks (out of 10): ";
        cin>>practical;
        while(practical>10)
         {
            cout<<"Invalid output! Cannot enter value more than 10"<<endl<<"Enter valid output: ";
            cin>>practical;
            if(practical>10)
             continue;
         }

        cout<<"Enter Attendance (out of 10): ";
        while (true) {
    try {
        cin >> attendance;

        if (attendance < 0 || attendance > 10)
            throw attendance;

        break;  
    }
    catch (...) {
        cout << "Invalid! Enter attendance between 0 and 10: ";
        cin.clear();
        cin.ignore(2, '\n');
    }
}
        attendance=((int)(attendance*10+0.5))/10.0f;

        cout<<"Enter Minor Exam marks (out of 30): ";
       while (true) {
    try {
        cin >> minor;

        if (minor < 0 || minor > 30)
            throw minor;   

        break;  
    }
    catch (...) {
        cout << "Invalid! Enter marks between 0 and 30: ";
        cin.clear();
        cin.ignore(100, '\n');
    }
}
        marks=minor+attendance+practical;
    }

    void setfromfile(string n,float m){
        name=n;
        marks=m;
    };

    float getinternalmarks(){
        return marks;
    }

    float getpercent(){
        return getinternalmarks()*2;
    }

    string getName(){return name;}
};

// STUDENT CLASS
class Student{
private:
    string name;
    int id;
    GoalCategory goal;

public:
    

void input() {
    cin.ignore(numeric_limits<streamsize>::max(), '\n');  

   while (true) {
    cout << "Enter Name: ";
    getline(cin >> ws, name);

    bool valid = true;

    for (char c : name) {
        if (!isalpha(c) && c != ' ') {
            valid = false;
            break;
        }
    }

    if (valid && !name.empty()) break;

    cout << "Invalid name! Only alphabets allowed.\n";
}  

    cout << "Enter ID: ";
    cin >> id;

    int g;
    cout << "\n0: PASS | 1: DECENT | 2: TOPPER\nEnter Goal: ";
    

while (true) {
    try {
        cin >> g;

        if (g < 0 || g > 2)
            throw g;

        goal = (GoalCategory)g;
        break;   
    }
    catch (...) {
        cout << "Invalid input! Enter 0 (PASS), 1 (DECENT), or 2 (TOPPER): ";
        cin.clear();
        cin.ignore(100, '\n');
    }
}
}
    int getId(){return id;}
    string getName(){return name;}
    int getGoal(){return (int)goal;}
};

// STUDENT RECORD(STUDENT+SUBJECT AS MEMBERS)
class StudentRecord{
private:
    Student student;
    static vector<string>names;
public:
    vector<Subject> subjects;
    void inputrecord(){
        student.input();
        for(auto &name:names){
            Subject s;
            s.input(name);
            subjects.push_back(s);
        }
    };

    float gettotalinternal(){
        float sum=0;
        for(auto &s:subjects){sum+=s.getinternalmarks();}
        return sum;
    };

    float getoverallpercent(){
        return (gettotalinternal()/names.size())*2.0f;
    }
    int getid(){return student.getId();}
    string getname(){return student.getName();}
    int getgoal(){return student.getGoal();}
};
vector<string>StudentRecord::names={"OOPS","MATHEMATICS","BEEE","APPLIED CHEM"};

//REPORT(BASE CLASS)
class Report{
    public:
    virtual void generate(StudentRecord &r)=0;
    virtual ~Report(){};
};
//BASE REPORT
class BasicReport:public Report{
public:
    void generate(StudentRecord &r){
        cout<<"\n===== BASIC REPORT =====\n";
        cout<<"Name: "<<r.getname()<<endl;
        float internalpercent=r.getoverallpercent();
        float threshold=getThreshold((GoalCategory)r.getgoal());
        cout<<"YOUR GOAL  :"<<getThreshold((GoalCategory)r.getgoal())<<endl;
        cout<<"Internal Performance: "<<r.getoverallpercent()<<"%\n";

        if(internalpercent<threshold)
            cout<<"Status: GOAL NOT ACHIEVED\n";
        else if(internalpercent==threshold)
            cout<<"Status:GOAL ACHIEVED\n";
        else
            cout<<"Status:GOAL OVER ACHIEVED\n";
    }
};

//DETAILED REPORT
class DetailedReport:public Report{
public:
    void generate(StudentRecord &r){

        cout<<"\n===== DETAILED REPORT =====\n";
        cout<<"Name: "<<r.getname()<<endl;

        float internalpercent=r.getoverallpercent();
        float threshold=getThreshold((GoalCategory)r.getgoal());

        cout<<"Internal Performance: "<<r.getoverallpercent()<<"%\n";

        if(internalpercent<threshold)
            cout<<"Status: GOAL NOT ACHIEVED\n";
        else if(internalpercent==threshold)
            cout<<"Status:GOAL ACHIEVED\n";
        else
            cout<<"Status:GOAL OVER ACHIEVED\n";

        float requiredinternalmarks=(threshold)*(r.subjects.size());
        float gapMarks=requiredinternalmarks-(r.gettotalinternal());

        cout<<"Major Exam Requirement (Total): ";

        if(gapMarks>0)
            cout<<gapMarks<<" marks needed in major exam to achieve your goal\n";
        else
            cout<<"No major exam requirement (already safe)\n";

        int flag=0;
        cout<<"-------------ALL SUBJECTS-------------- "<<endl;
        for(auto &s:r.subjects){
            cout<<s.getName()<<" : ("<<s.getpercent()<<"% )"<<endl;
        }
        cout<<"-------------WEAK SUBJECTS-------------- "<<endl;
        for(auto &s:r.subjects){
            float percent=s.getpercent();
            if(percent<threshold){
                flag=1;
                cout<<s.getName()<<" : ("<<percent<<"% )"<<endl;}
            else{
                cout<<endl;
            }
    
            }
        if(!flag){
            cout<<"YOU HAVE NO WEAK SUBJECTS"<<endl;
        }
    }
};

//FILE HANDLING
string file="students.txt";
class StudentPortal{
public:

void run(){
while(true){
int id;
cout<<"\nEnter ID(-1 exit): ";
while (!(cin >> id)) {
    cout << "Invalid input! Enter numeric ID: ";
    cin.clear();
    cin.ignore(100, '\n');
}
if(id==-1)break;

StudentRecord record;

if(!load(id,record)){
cout<<"ID not found. Creating new record...\n";
record.inputrecord();
save(record);
}

menu(record);
}
};

int menu(StudentRecord &record){
while(true){
int choice;
cout<<"\n1.Basic (PRESS 1 )\n 2.Detailed (PRESS 2) \n 3.Exit(press any number except 1 and 2\n";
cin>>choice;

if(choice==1){
Report *r=new BasicReport();
r->generate(record);
delete r;
}
else if(choice==2){
Report *r=new DetailedReport();
r->generate(record);
delete r;
}
else{break;}
}
return 0;
};

bool load(int id,StudentRecord &r){
ifstream f(file);
string line,key="ID="+to_string(id);
bool found=false;

while(getline(f,line)){
if(line.find(key)!=string::npos){
found=true;
continue;
}

if(found){
if(line=="END")break;

string name="",marks="";
int i=0;

while(i<line.size()&&line[i]!=' ')
name+=line[i++];

i++;
while(i<line.size())
marks+=line[i++];

Subject s;
try {
   s.setfromfile(name, stof(marks));

}
catch (...) {
    continue;
}
r.subjects.push_back(s);
}
}
return found;
};

void save(StudentRecord &r){
ofstream f(file,ios::app);

f<<"ID="<<r.getid()<<" NAME="<<r.getname()
<<" GOAL="<<r.getgoal()<<"\n";

for(auto &s:r.subjects)
f<<s.getName()<<" "<<s.getinternalmarks()<<"\n";

f<<"END\n";
}
};
int main(){
    StudentPortal s;
    s.run();
    return 0;
}
#include <iostream>
#include <fstream>
#include <vector>
#include <iomanip>
using namespace std;

// ========== Base Class ==========
class Student {
protected:
    int rollNo;
    string name;
    string branch;
    float marks;

public:
    Student() {}
    Student(int r, string n, string b, float m) : rollNo(r), name(n), branch(b), marks(m) {}

    virtual void input() {
        cout << "Enter Roll Number: "; cin >> rollNo;
        cout << "Enter Name: "; cin.ignore(); getline(cin, name);
        cout << "Enter Branch: "; getline(cin, branch);
        cout << "Enter Marks: "; cin >> marks;
    }

    virtual void display() const {
        cout << left << setw(10) << rollNo
             << setw(20) << name
             << setw(15) << branch
             << setw(10) << marks;
    }

    int getRollNo() const { return rollNo; }

    virtual void update() {
        cout << "Update Name: "; cin.ignore(); getline(cin, name);
        cout << "Update Branch: "; getline(cin, branch);
        cout << "Update Marks: "; cin >> marks;
    }

    virtual ~Student() {}
};

// ========== Derived Classes ==========
class UGStudent : public Student {
private:
    int year;

public:
    UGStudent() : year(0) {}

    void input() override {
        Student::input();
        cout << "Enter Year (1-4): "; cin >> year;
    }

    void display() const override {
        Student::display();
        cout << setw(10) << "UG - Y" + to_string(year) << endl;
    }

    void update() override {
        Student::update();
        cout << "Update Year: "; cin >> year;
    }
};

class PGStudent : public Student {
private:
    string specialization;

public:
    void input() override {
        Student::input();
        cout << "Enter Specialization: "; cin.ignore(); getline(cin, specialization);
    }

    void display() const override {
        Student::display();
        cout << setw(10) << "PG - " + specialization << endl;
    }

    void update() override {
        Student::update();
        cout << "Update Specialization: "; cin.ignore(); getline(cin, specialization);
    }
};

// ========== Manager Class ==========
class StudentManager {
private:
    string filename = "students.dat";

public:
    void addStudent() {
        int type;
        Student* s;

        cout << "Select Student Type:\n1. UG Student\n2. PG Student\nChoice: ";
        cin >> type;

        if (type == 1)
            s = new UGStudent();
        else
            s = new PGStudent();

        s->input();

        ofstream out(filename, ios::app | ios::binary);
        out.write((char*)s, sizeof(*s));
        out.close();

        delete s;
        cout << "Student added successfully.\n";
    }

    void displayAll() {
        ifstream in(filename, ios::binary);
        if (!in) {
            cout << "No records found.\n";
            return;
        }

        Student s;
        cout << left << setw(10) << "RollNo" << setw(20) << "Name" << setw(15)
             << "Branch" << setw(10) << "Marks" << "Type\n";
        cout << "--------------------------------------------------------------\n";

        while (in.read((char*)&s, sizeof(s))) {
            s.display();
        }

        in.close();
    }

    void searchStudent(int roll) {
        ifstream in(filename, ios::binary);
        Student s;
        bool found = false;

        while (in.read((char*)&s, sizeof(s))) {
            if (s.getRollNo() == roll) {
                cout << "Student Found:\n";
                s.display();
                found = true;
                break;
            }
        }

        if (!found)
            cout << "Student with Roll No " << roll << " not found.\n";

        in.close();
    }

    void deleteStudent(int roll) {
        ifstream in(filename, ios::binary);
        ofstream out("temp.dat", ios::binary);

        Student s;
        bool found = false;

        while (in.read((char*)&s, sizeof(s))) {
            if (s.getRollNo() != roll) {
                out.write((char*)&s, sizeof(s));
            } else {
                found = true;
            }
        }

        in.close();
        out.close();

        remove(filename.c_str());
        rename("temp.dat", filename.c_str());

        if (found)
            cout << "Student deleted.\n";
        else
            cout << "Student not found.\n";
    }

    void updateStudent(int roll) {
        fstream file(filename, ios::in | ios::out | ios::binary);
        Student s;
        bool found = false;

        while (file.read((char*)&s, sizeof(s))) {
            if (s.getRollNo() == roll) {
                cout << "Old Details:\n";
                s.display();

                s.update();

                int pos = file.tellg();
                file.seekp(pos - sizeof(s));
                file.write((char*)&s, sizeof(s));

                cout << "Student updated.\n";
                found = true;
                break;
            }
        }

        file.close();
        if (!found)
            cout << "Student not found.\n";
    }
};

// ========== Main ==========
int main() {
    StudentManager sm;
    int choice, roll;

    while (true) {
        cout << "\n===== Student Record Management System =====\n";
        cout << "1. Add Student\n2. Display All Students\n3. Search Student\n4. Update Student\n5. Delete Student\n6. Exit\nChoice: ";
        cin >> choice;

        switch (choice) {
            case 1: sm.addStudent(); break;
            case 2: sm.displayAll(); break;
            case 3:
                cout << "Enter Roll No: "; cin >> roll;
                sm.searchStudent(roll); break;
            case 4:
                cout << "Enter Roll No: "; cin >> roll;
                sm.updateStudent(roll); break;
            case 5:
                cout << "Enter Roll No: "; cin >> roll;
                sm.deleteStudent(roll); break;
            case 6: exit(0);
            default: cout << "Invalid choice.\n";
        }
    }

    return 0;
}

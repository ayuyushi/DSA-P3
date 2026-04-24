#pragma once
#include <string>
#include <unordered_map>
#include <unordered_set>

using namespace std;

class Student
{
    string name;
    int studentID;
    int residenceID;
    unordered_set<string> studentClasses; // to prevent duplicate classes

    public:

    Student() {}

    // initialized constructor
    Student(const string &name, int studentID, int residenceID)
    {
        this->name = name;
        this->studentID = studentID;
        this->residenceID = residenceID;
    }

    // return name
    string getName() const
    {
        return name;
    }

    // return residence ID
    int getResidenceID() const
    {
        return residenceID;
    }

    // is student in the class? return true if yes and false if no
    bool studentInClass(const string &classCode) const
    {
        return (studentClasses.find(classCode) != studentClasses.end());
    }

    // add student to class
    bool addClass(const string &classCode)
    {
        if (studentInClass(classCode))
        {
            return false;
        }
        studentClasses.insert(classCode);
        return true;
    }

    // remove student from class
    bool removeClass(const string &classCode)
    {
        return (studentClasses.erase(classCode) > 0);
    }

    // does the student have no classes? return true if yes and false if no
    bool hasNoClasses() const
    {
        return (studentClasses.empty());
    }

    // helper function to replace classes
    bool replaceClassHelper(const string &oldCode, const string &newCode)
    {
        if (!(studentInClass(oldCode)) || studentInClass(newCode))
        {
            return false;
        }
        studentClasses.erase(oldCode);
        studentClasses.insert(newCode);
        return true;
    }

    // return the set of classes
    const unordered_set<string>& getClasses() const
    {
        return studentClasses;
    }
};

// manager class for more complex methods
class StudentManager
{
    unordered_map<int, Student> studentDatabase;

    public:

    StudentManager() {}

    // return student object if student exists
    Student* getStudent(const int &studentID)
    {
        if (studentDatabase.find(studentID) == studentDatabase.end())
        {
            return nullptr;
        }
        return &(studentDatabase.find(studentID)->second);
    }

    // insert student into database
    bool insert(string& studentName, int& studentID, int& residenceID, unordered_set<string>& classes)
      {
          if (studentDatabase.find(studentID) != studentDatabase.end())
          {
              return false;
          }
          Student newStudent(studentName, studentID, residenceID);
          for (const string& classCode : classes)
          {
              newStudent.addClass(classCode);
          }
          studentDatabase.emplace(studentID, newStudent);
          return true;
      }

    // remove student from database
    bool remove(int& studentID)
    {
        if (studentDatabase.find(studentID) != studentDatabase.end())
        {
            studentDatabase.erase(studentID);
            return true;
        }
        return false;
    }

    // remove a class from a student
    bool dropClass(const int& studentID, const string& classCode)
    {
        if (studentDatabase.find(studentID) == studentDatabase.end())
        {
            return false;
        }

        if (!(studentDatabase.find(studentID)->second.removeClass(classCode)))
        {
            return false;
        }

        if (studentDatabase.find(studentID)->second.hasNoClasses())
        {
            studentDatabase.erase(studentID);
        }
        return true;
    }

    // replace a class from a student
    bool replaceClass(const int& studentID, const string &oldCode, const string &newCode)
    {
        if (studentDatabase.find(studentID) == studentDatabase.end())
        {
            return false;
        }
        return studentDatabase.find(studentID)->second.replaceClassHelper(oldCode, newCode);
    }

    int removeClass(const string& classCode)
    {
        int studentNum = 0;

        for (auto it = studentDatabase.begin(); it != studentDatabase.end();)
        {
            if (it->second.removeClass(classCode))
            {
                studentNum++;
                if (it->second.hasNoClasses())
                {
                    it = studentDatabase.erase(it);
                    continue;
                }
            }
            ++it;
        }
        return studentNum;
    }
};
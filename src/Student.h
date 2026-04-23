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

    Student(const string &name, int studentID, int residenceID)
    {
        this->name = name;
        this->studentID = studentID;
        this->residenceID = residenceID;
    }

    string getName() const
    {
        return name;
    }

    int getResidenceID() const
    {
        return residenceID;
    }

    bool studentInClass(const string &classCode) const
    {
        return (studentClasses.find(classCode) != studentClasses.end());
    }

    bool addClass(const string &classCode)
    {
        if (studentInClass(classCode))
        {
            return false;
        }
        studentClasses.insert(classCode);
        return true;
    }

    bool removeClass(const string &classCode)
    {
        return (studentClasses.erase(classCode) > 0);
    }

    bool hasNoClasses() const
    {
        return (studentClasses.empty());
    }

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

    const unordered_set<string>& getClasses() const
    {
        return studentClasses;
    }
};

class StudentManager
{
    unordered_map<int, Student> studentDatabase;

    public:

    StudentManager() {}

    Student* getStudent(const int &studentID)
    {
        if (studentDatabase.find(studentID) == studentDatabase.end())
        {
            return nullptr;
        }
        return &(studentDatabase.find(studentID)->second);
    }

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

    bool remove(int& studentID)
    {
        if (studentDatabase.find(studentID) != studentDatabase.end())
        {
            studentDatabase.erase(studentID);
            return true;
        }
        return false;
    }

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


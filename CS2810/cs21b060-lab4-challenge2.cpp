#include <iostream>
#include <string>
#include <vector>

using namespace std;

template<class Object>
class DoublyLinkedList
{
    private:
        struct Node
        {
            Object data;
            Node *next;
            Node *prev;
              Node(const Object & d = Object{ }, Node * p = nullptr, Node * n = nullptr) 
              : data(d), prev(p), next(n) {}
        };
          
        Node *head; //sentinel node at the beginning
        Node *rear; //sentinel node at the end
    
    public:
        DoublyLinkedList()
        {
            head = new Node;
            rear = new Node;
            head->next = rear;
            head->prev = NULL;
            rear->next = NULL;
            rear->prev = head;
        }
  
        class Iterator
        {
            private:
                Node *current;        
            public:
                Iterator(Node *inp) : current(inp) {}
                Object & operator*() {return current->data;}
          
            friend class DoublyLinkedList<Object>;
            
            bool operator!= (Iterator itr)
            {
                if(current!=itr.current) return true;
                return false;
            }

            Iterator operator++ (int) 
            {
                Iterator temp(this->current);
                this->current = this->current->next;
                return temp;
            }

            Iterator operator-- ()
            {
                this->current = this->current->prev;
                return *this;
            }
        };

        Iterator begin(){return Iterator(head->next);}    
        Iterator end(){return Iterator(rear);}
    
        Iterator insert(Iterator itr, Object & o)
        {
            Node* cur = itr.current;
            Node* pre = cur->prev;
            Node* new_node;
            new_node = new Node(o);
            new_node->prev = pre;
            new_node->next = cur;
            cur->prev = new_node;
            pre->next = new_node;
            return Iterator(new_node);
        }
    
        Iterator erase(Iterator itr)
        {
            Node* cur = itr.current;
            Node* pre = cur->prev;
            Node* nex = cur->next;
            pre->next = nex;
            nex->prev = pre;
            free(cur);
            return Iterator(nex);
        }
    
        Iterator atIndex(int p)
        {
            Node* temp = head;
            for(int i=0; i<=p; i++)
            {
                if(temp->next!=NULL) temp = temp->next;
                else return Iterator(rear);
            }
            return Iterator(temp);
        }
    
        int indexOf(Iterator itr)
        {
            Node* cur = itr.current;
            Node* temp = head->next;
            int i=0;
            while(temp!=cur)
            {
                if(temp->next!=NULL)
                {
                    temp = temp->next;
                    i++;
                }
            }
            return i;
        }
    
        void display()
        {            
            for (auto it = begin(); it != end(); it++)
                cout << *it << " ";
            cout << endl;
        }        
};

template <class Key, class Value>
class Database
{
    public:
        class Record
        {
            private:
                Key k;
                Value v;

            public:
                Record(const Key& ik = Key(), const Value& iv = Value()) : k(ik), v(iv) {}
                Key& getKey() { return k; }
                Value& getValue() { return v; }
        };

    private:
        DoublyLinkedList<Record> database;
        int num_records;

    public:
        Database() : num_records(0) {}
    
        bool isPresent(Key& k1)
        {
            int start = 0, end = num_records-1;
            int mid;
            while(start<=end)
            {
                mid = (start + end)/2;
                Record r = *(database.atIndex(mid));
                if(r.getKey()==k1) return true;
                else if(r.getKey()<k1) start = mid + 1;
                else end = mid - 1;
            }
            return false;
        }
    
        Record selectRecord(Key& k1)
        {
            int start = 0, end = num_records-1;
            int mid;
            while(start<=end)
            {
                mid = (start + end)/2;
                Record r = *(database.atIndex(mid));
                if(r.getKey()==k1) return r;
                else if(r.getKey()<k1) start = mid + 1;
                else end = mid - 1;
            }
            return Record();
        }
    
        vector<Record> selectRangeRecord(Key & rangeStart, Key & rangeEnd)
        {
            int start = 0, end = num_records-1;
            int mid;
            int range_start, range_end;
            while(start<=end)
            {
                mid = (start + end)/2;
                Record r_mid = *(database.atIndex(mid));
                Record r_prev = *(database.atIndex(mid-1));
                if(r_prev.getKey()<rangeStart && (r_mid.getKey()>rangeStart || r_mid.getKey()==rangeStart))
                {
                    range_start = mid;
                    break;
                }
                else if(r_mid.getKey()<rangeStart) start = mid + 1;
                else end = mid - 1;
            }
            
            start = 0, end = num_records-1;
            while(start<=end)
            {
                mid = (start + end)/2;
                Record r_mid = *(database.atIndex(mid));
                Record r_next = *(database.atIndex(mid+1));
                if(r_next.getKey()>rangeEnd && (r_mid.getKey()<rangeEnd || r_mid.getKey()==rangeEnd))
                {
                    range_end = mid;
                    break;
                }
                else if(r_mid.getKey()>rangeEnd) end = mid - 1;
                else start = mid + 1;
            }
            vector<Record> recs_range;
            for(int i=range_start; i<=range_end; i++) recs_range.push_back(*(database.atIndex(i)));
            return recs_range;
        }
    
        void updateRecord(Key & k1, Value & v1)
        {
            int start = 0, end = num_records-1;
            int mid;
            while(start<=end)
            {
                mid = (start + end)/2;
                Record r = *(database.atIndex(mid));
                if(r.getKey()==k1)
                {
                    Record new_record(k1, v1);
                    database.erase(database.atIndex(mid));
                    database.insert(database.atIndex(mid), new_record);
                    return;
                }
                else if(r.getKey()<k1) start = mid + 1;
                else end = mid - 1;
            }
        }
    
        void updateRangeRecord(Key & rangeStart, Key & rangeEnd, Value & v1)
        {
            int start = 0, end = num_records-1;
            int mid;
            int range_start, range_end;
            while(start<=end)
            {
                mid = (start + end)/2;
                Record r_mid = *(database.atIndex(mid));
                Record r_prev = *(database.atIndex(mid-1));
                if(r_prev.getKey()<rangeStart && (r_mid.getKey()>rangeStart || r_mid.getKey()==rangeStart))
                {
                    range_start = mid;
                    break;
                }
                else if(r_mid.getKey()<rangeStart) start = mid + 1;
                else end = mid - 1;
            }
            
            start = 0, end = num_records-1;
            while(start<=end)
            {
                mid = (start + end)/2;
                Record r_mid = *(database.atIndex(mid));
                Record r_next = *(database.atIndex(mid+1));
                if(r_next.getKey()>rangeEnd && (r_mid.getKey()<rangeEnd || r_mid.getKey()==rangeEnd))
                {
                    range_end = mid;
                    break;
                }
                else if(r_mid.getKey()>rangeEnd) end = mid - 1;
                else start = mid + 1;
            }
            
            for(int i=range_start; i<=range_end; i++)
            {
                Key k1 = ((*database.atIndex(i)).getKey());
                Record new_record(k1, v1);
                database.erase(database.atIndex(i));
                database.insert(database.atIndex(i), new_record);
            }
        }
    
        void insertRecord(Key & k1, Value & v1)
        {
            Record new_record(k1, v1);
            if(num_records==0)
            {
                database.insert(database.end(), new_record);
                num_records++;
                return;
            }
            int start = 0, end = num_records-1;
            int mid;
            while(start<=end)
            {
                mid = (start + end)/2;
                Record r_mid = *(database.atIndex(mid));
                Record r_prev = *(database.atIndex(mid-1));
                if(r_prev.getKey()<k1 && (r_mid.getKey()>k1 || r_mid.getKey()==k1))
                {
                    database.insert(database.atIndex(mid), new_record);
                    num_records++;
                    return;
                }
                else if(r_mid.getKey()<k1) start = mid + 1;
                else end = mid - 1;
            }
            if(mid==num_records-1) 
            {
                database.insert(database.end(), new_record);
            }
            num_records++;
        }
    
        void deleteRecord(Key & k1)
        {
            int start = 0, end = num_records-1;
            int mid;
            while(start<=end)
            {
                mid = (start + end)/2;
                Record r = *(database.atIndex(mid));
                if(r.getKey()==k1)
                {
                    database.erase(database.atIndex(mid));
                    num_records--;
                    return;
                }
                else if(r.getKey()<k1) start = mid + 1;
                else end = mid - 1;
            }
        }
    
        void deleteRangeRecord(Key & rangeStart, Key & rangeEnd)
        {
            int start = 0, end = num_records-1;
            int mid;
            int range_start, range_end;
            while(start<=end)
            {
                mid = (start + end)/2;
                Record r_mid = *(database.atIndex(mid));
                Record r_prev = *(database.atIndex(mid-1));
                if(r_prev.getKey()<rangeStart && (r_mid.getKey()>rangeStart || r_mid.getKey()==rangeStart))
                {
                    range_start = mid;
                    break;
                }
                else if(r_mid.getKey()<rangeStart) start = mid + 1;
                else end = mid - 1;
            }
            
            start = 0, end = num_records-1;
            while(start<=end)
            {
                mid = (start + end)/2;
                Record r_mid = *(database.atIndex(mid));
                Record r_next = *(database.atIndex(mid+1));
                if(r_next.getKey()>rangeEnd && (r_mid.getKey()<rangeEnd || r_mid.getKey()==rangeEnd))
                {
                    range_end = mid;
                    break;
                }
                else if(r_mid.getKey()>rangeEnd) end = mid - 1;
                else start = mid + 1;
            }
    
            for(int i=range_start; i<=range_end; i++) 
            {
                database.erase(database.atIndex(range_start));
                num_records--;
            }
        }
    
        Record getMinRecord() { return *database.begin(); }
    
        Record getMaxRecord() { return *(--database.end()); }
    
        vector<Record> getallRecords()
        {
            vector<Record> all_recs;
            for(int i=0; i<num_records; i++) all_recs.push_back(*database.atIndex(i));
            return all_recs;
        }
    
        
};

class StudentsKey
{
    public:
        string rollNo;
        StudentsKey(const string &inp = "") : rollNo(inp) {}
        bool operator<(StudentsKey &rhs) { return rollNo < rhs.rollNo; }
        bool operator==(StudentsKey& rhs) {return rollNo==rhs.rollNo; }
        bool operator>(StudentsKey &rhs) { return rollNo > rhs.rollNo; }
        friend ostream & operator<<(ostream & out, StudentsKey &k);
};

ostream & operator<<(ostream & out, StudentsKey &k)
{ 
    out << k.rollNo;
    return out;
}

class StudentsValue
{
    public:
        string name;
        string hostel;
        float cgpa;
        StudentsValue(const string &n = "", const string &h = "", float c = 10.0) : name(n), hostel(h), cgpa(c) {}
        friend ostream & operator<<(ostream & out, StudentsValue &v);
};

ostream & operator<<(std::ostream & out, StudentsValue &v)
{
    out << v.name << " " << v.hostel << " " << v.cgpa;
    return out;
}

class FacultyKey
{
    public:
        string empId;
        FacultyKey(const string &inp = "") : empId(inp) {}
        bool operator<(FacultyKey &rhs) { return empId < rhs.empId; }
        bool operator==(FacultyKey& rhs) { return empId==rhs.empId; }
        bool operator>(FacultyKey &rhs) { return empId > rhs.empId; }
        friend ostream & operator<<(ostream & out, FacultyKey &k);
};

ostream & operator<<(ostream & out, FacultyKey &k)
{ 
    out << k.empId;
    return out;
}

class FacultyValue
{
    public:
        string name;
        string designation;
        float salary;
        FacultyValue(const string &n = "", const string &d = "", float s = 10.0) : name(n), designation(d), salary(s) {}
        friend ostream & operator<<(ostream & out, FacultyValue &v);
};

ostream & operator<<(std::ostream & out, FacultyValue &v)
{
    out << v.name << " " << v.designation << " " << v.salary;
    return out;
}

class CourseKey
{
    public:
        string courseId;
        CourseKey(const string &inp = "") : courseId(inp) {}
        bool operator<(CourseKey &rhs) { return courseId < rhs.courseId; }
        bool operator==(CourseKey& rhs) { return courseId==rhs.courseId; }
        bool operator>(CourseKey &rhs) { return courseId > rhs.courseId; }
        friend ostream & operator<<(ostream & out, CourseKey &k);
};

ostream & operator<<(ostream & out, CourseKey &k)
{ 
    out << k.courseId;
    return out;
}

class CourseValue
{
    public:
        string name;
        string semester;
        string facultyName;
        CourseValue(const string &n = "", const string &s = "", const string &f = "") : name(n), semester(s), facultyName(f) {}
        friend ostream & operator<<(ostream & out, FacultyValue &v);
};

ostream & operator<<(std::ostream & out, CourseValue &v)
{
    out << v.name << " " << v.semester << " " << v.facultyName;
    return out;
}

int main() 
{
    int command;
    bool b;
    int BREAKING_COMMAND = 50;

    Database<StudentsKey, StudentsValue> student_db;
    Database<FacultyKey, FacultyValue> faculty_db;
    Database<CourseKey, CourseValue> course_db;

    while (true) {
        cin >> command;
        if (command == BREAKING_COMMAND) {
            break;
        }

        if (command == 1) { /*insert record*/
            int numberOfRecords = 0; /*number of records to be inserted*/
            cin >> numberOfRecords;

            while (numberOfRecords--) {

                string database;
                cin >> database;

                if (database == "STUDENT") {
                    string rollNo, name, hostel;
                    float cgpa;
                    cin >> rollNo >> name >> hostel >> cgpa;

                    StudentsKey k(rollNo);
                    StudentsValue v(name, hostel, cgpa);
                    student_db.insertRecord(k, v);
                } else if (database == "FACULTY") {
                    string empId, name, designation;
                    float salary;
                    cin >> empId >> name >> designation >> salary;

                    FacultyKey k(empId);
                    FacultyValue v(name, designation, salary);
                    faculty_db.insertRecord(k, v);
                } else {
                    string courseId, name, semester, facultyName;
                    cin >> courseId >> name >> semester >> facultyName;

                    CourseKey k(courseId);
                    CourseValue v(name, semester, facultyName);
                    course_db.insertRecord(k, v);
                }
            }

        } 
        else if (command == 2) { /*check whether the key is present*/
            string database;
            cin >> database;

            if (database == "STUDENT") {
                string rollNo;
                cin >> rollNo;

                StudentsKey k(rollNo);
                b = student_db.isPresent(k);
                cout << b << endl;
            } else if (database == "FACULTY") {
                string empId;
                cin >> empId;

                FacultyKey k(empId);
                b = faculty_db.isPresent(k);
                cout << b << endl;
            } else {
                string courseId;
                cin >> courseId;

                CourseKey k(courseId);
                b = course_db.isPresent(k);
                cout << b << endl;
            }

        } 
        else if (command == 3) {
            string database; /*returns the record containing the key*/
            cin >> database;

            if (database == "STUDENT") {
                string rollNo;
                cin >> rollNo;

                StudentsKey k(rollNo);
                auto r = student_db.selectRecord(k);
                cout << r.getKey() << " " << r.getValue() << endl;
            } else if (database == "FACULTY") {
                string empId;
                cin >> empId;

                FacultyKey k(empId);
                auto r = faculty_db.selectRecord(k);
                cout << r.getKey() << " " << r.getValue() << endl;
            } else {
                string courseId;
                cin >> courseId;

                CourseKey k(courseId);
                auto r = course_db.selectRecord(k);
                cout << r.getKey() << " " << r.getValue() << endl;
            }

        } 
        else if (command == 4) { /*returns a vector containing all the records in the database
                                     whose key lies between rangeStart and rangeEnd (inclusive).*/
            string database;
            cin >> database;

            if (database == "STUDENT") {
                string rangeStart, rangeEnd;
                cin >> rangeStart >> rangeEnd;

                StudentsKey rs(rangeStart);
                StudentsKey re(rangeEnd);
                auto lst = student_db.selectRangeRecord(rs, re);
                for (auto r : lst) {
                    cout << r.getKey() << " " << r.getValue() << endl;
                }
            } else if (database == "FACULTY") {
                string rangeStart, rangeEnd;
                cin >> rangeStart >> rangeEnd;

                FacultyKey rs(rangeStart);
                FacultyKey re(rangeEnd);
                auto lst = faculty_db.selectRangeRecord(rs, re);
                for (auto r : lst) {
                    cout << r.getKey() << " " << r.getValue() << endl;
                }
            } else {
                string rangeStart, rangeEnd;
                cin >> rangeStart >> rangeEnd;

                CourseKey rs(rangeStart);
                CourseKey re(rangeEnd);
                auto lst = course_db.selectRangeRecord(rs, re);
                for (auto r : lst) {
                    cout << r.getKey() << " " << r.getValue() << endl;
                }
            }

        } 
        else if (command == 5) { /*updates the record in the database whose key is k with value v.
                                     If k is not present, there is no change in the database.*/
            string database;
            cin >> database;

            if (database == "STUDENT") {
                string rollNo, name, hostel;
                float cgpa;
                cin >> rollNo >> name >> hostel >> cgpa;

                StudentsKey k(rollNo);
                StudentsValue v(name, hostel, cgpa);
                student_db.updateRecord(k, v);
            } else if (database == "FACULTY") {
                string empId, name, designation;
                float salary;
                cin >> empId >> name >> designation >> salary;

                FacultyKey k(empId);
                FacultyValue v(name, designation, salary);
                faculty_db.updateRecord(k, v);
            } else {
                string courseId, name, semester, facultyName;
                cin >> courseId >> name >> semester >> facultyName;

                CourseKey k(courseId);
                CourseValue v(name, semester, facultyName);
                course_db.updateRecord(k, v);
            }

        } 
        else if (command == 6) { /*updates all records in the database whose key lies between
                                     rangeStart and rangeEnd, with value v (inclusive).*/
            string database;
            cin >> database;

            if (database == "STUDENT") {
                string rangeStart, rangeEnd, name, hostel;
                float cgpa;
                cin >> rangeStart >> rangeEnd >> name >> hostel >> cgpa;

                StudentsKey rs(rangeStart);
                StudentsKey re(rangeEnd);
                StudentsValue v(name, hostel, cgpa);
                student_db.updateRangeRecord(rs, re, v);
            } else if (database == "FACULTY") {
                string rangeStart, rangeEnd, name, designation;
                float salary;
                cin >> rangeStart >> rangeEnd >> name >> designation >> salary;

                FacultyKey rs(rangeStart);
                FacultyKey re(rangeEnd);
                FacultyValue v(name, designation, salary);
                faculty_db.updateRangeRecord(rs, re, v);
            } else {
                string rangeStart, rangeEnd, name, semester, facultyName;
                cin >> rangeStart >> rangeEnd >> name >> semester >> facultyName;

                CourseKey rs(rangeStart);
                CourseKey re(rangeEnd);
                CourseValue v(name, semester, facultyName);
                course_db.updateRangeRecord(rs, re, v);
            }

        } 
        else if (command == 7) { /*deletes the record with key k.*/
            string database;
            cin >> database;

            if (database == "STUDENT") {
                string rollNo;
                cin >> rollNo;

                StudentsKey k(rollNo);
                student_db.deleteRecord(k);
            } else if (database == "FACULTY") {
                string empId;
                cin >> empId;

                FacultyKey k(empId);
                faculty_db.deleteRecord(k);
            } else {
                string courseId;
                cin >> courseId;

                CourseKey k(courseId);
                course_db.deleteRecord(k);
            }

        } 
        else if (command == 8) { /*deletes all records with key between rangeStart and rangeEnd (inclusive).*/
            string database;
            cin >> database;

            if (database == "STUDENT") {
                string rangeStart, rangeEnd;
                cin >> rangeStart >> rangeEnd;

                StudentsKey rs(rangeStart);
                StudentsKey re(rangeEnd);
                student_db.deleteRangeRecord(rs, re);
            } else if (database == "FACULTY") {
                string rangeStart, rangeEnd;
                cin >> rangeStart >> rangeEnd;

                FacultyKey rs(rangeStart);
                FacultyKey re(rangeEnd);
                faculty_db.deleteRangeRecord(rs, re);
            } else {
                string rangeStart, rangeEnd;
                cin >> rangeStart >> rangeEnd;

                CourseKey rs(rangeStart);
                CourseKey re(rangeEnd);
                course_db.deleteRangeRecord(rs, re);
            }

        } 
        else if (command == 9) { /*return the minimum record*/
            string database;
            cin >> database;

            if (database == "STUDENT") {
                auto r = student_db.getMinRecord();
                cout << r.getKey() << " " << r.getValue() << endl;
            } else if (database == "FACULTY") {
                auto r = faculty_db.getMinRecord();
                cout << r.getKey() << " " << r.getValue() << endl;
            } else {
                auto r = course_db.getMinRecord();
                cout << r.getKey() << " " << r.getValue() << endl;
            }

        } 
        else if (command == 10) { /*return the maximum record*/
            string database;
            cin >> database;

            if (database == "STUDENT") {
                auto r = student_db.getMaxRecord();
                cout << r.getKey() << " " << r.getValue() << endl;
            } else if (database == "FACULTY") {
                auto r = faculty_db.getMaxRecord();
                cout << r.getKey() << " " << r.getValue() << endl;
            } else {
                auto r = course_db.getMaxRecord();
                cout << r.getKey() << " " << r.getValue() << endl;
            }

        } 
        else if (command == 11) { /*returns a vector containing all the records in the database.*/
            string database;
            cin >> database;

            if (database == "STUDENT") {
                auto lst = student_db.getallRecords();
                for (auto r : lst) {
                    cout << r.getKey() << " " << r.getValue() << endl;
                }
            } else if (database == "FACULTY") {
                auto lst = faculty_db.getallRecords();
                for (auto r : lst) {
                    cout << r.getKey() << " " << r.getValue() << endl;
                }
            } else {
                auto lst = course_db.getallRecords();
                for (auto r : lst) {
                    cout << r.getKey() << " " << r.getValue() << endl;
                }
            }

        } else {
            cout << "INVALID COMMAND!" << endl;
            break;
        }
    }
}

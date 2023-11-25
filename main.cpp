#include <cstdlib>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <vector>
#include <algorithm>
#include <functional>

namespace MyOwnNamespace { //Pr

    // Lambda Function
    auto myCout = [](std::ostream& os, const auto& value) -> std::ostream& //pr
    {
        return os << value;
    };

    class no_of_periods { //pr
    public:
        std::string subject;
        int no_of_ppw;
    };

    class Teacher { //Pr
    public:
        std::string teacherName;
    };

    // vectors
    std::vector<int> random() {//Da
        std::vector<int> mot;
        std::vector<int> ret;
        for (int i = 1; i <= 25; i++) {
            mot.push_back(i);
        }
        int lb = 1;
        int ub = 25;
        while (ret.size() != 25) {
            int k = (rand() % (ub - lb + 1)) + lb;
            if (mot[k] != 0) {
                ret.push_back(k);
                mot[k] = 0;
            }
        }
        return ret;
    };

    class Printable { //De
    public:
        virtual void print(std::ostream& os) const = 0;
        virtual ~Printable() = default;  // Virtual destructor for the abstract class
    };

    class timetable : public Printable { //De
    public:
        std::string subject;
        std::string teacherName;
        int period_no;

        // Parameterized constructor
        timetable(std::string subj, std::string tName, int pNo) 
            : subject(std::move(subj)), teacherName(std::move(tName)), period_no(pNo) {}

        // Delegating constructor
        timetable() : timetable("", "", 0) {}

        void print(std::ostream& os) const override {
            os << std::setw(15) << subject << std::setw(15) << teacherName << std::setw(10) << period_no << std::endl;
        }
    };

    class CompareParola {//Dh
    public:
        bool operator()(const timetable &x, const timetable &y) const {
            return x.period_no < y.period_no;
        }
    };

    // inline functions
    inline void printTable(const std::vector<timetable>& day, std::ofstream& outputFile) {  //De
        outputFile << std::setw(15) << "Subject" << std::setw(15) << "Teacher" << std::setw(10) << "Period" << std::endl;
        for (const auto& entry : day) {
            entry.print(outputFile);
        }
        outputFile << std::endl;
    }

} // namespace MyOwnNamespace

int main() { //Dh
    std::ifstream inputFile("subjects.txt");
    if (!inputFile.is_open()) {
        std::cerr << "Error opening the file." << std::endl;
        return 1;
    }

    int n;
    if (!(inputFile >> n) || n <= 0) {
        std::cerr << "Error: Invalid or missing number of subjects in the input file." << std::endl;
        return 1;
    }

    std::vector<MyOwnNamespace::no_of_periods> alist;
    std::vector<MyOwnNamespace::Teacher> teachers;
    std::vector<MyOwnNamespace::timetable> tt;
    std::vector<int> yaar = MyOwnNamespace::random();

    for (int i = 0; i < n; i++) {
        MyOwnNamespace::no_of_periods s;
        if (!(inputFile >> s.subject >> s.no_of_ppw) || s.no_of_ppw <= 0) {
            std::cerr << "Error: Invalid or missing subject data in the input file." << std::endl;
            return 1;
        }
        alist.push_back(s);

        MyOwnNamespace::Teacher t;
        if (!(inputFile >> t.teacherName)) {
            std::cerr << "Error: Missing teacher name in the input file." << std::endl;
            return 1;
        }
        teachers.push_back(t);
    } //Dh

    int total_i_p = 0;   //Da
    for (int i = 0; i < n; i++) {
        total_i_p += alist[i].no_of_ppw;
    }

    if (total_i_p > 25) {
        std::cerr << "Error: Total periods per week must be less than or equal to 25. Please edit the input file accordingly." << std::endl;
        return 1;
    }

    if (total_i_p <= 25) {
        MyOwnNamespace::no_of_periods s2;
        s2.subject = "Revision";
        s2.no_of_ppw = 25 - total_i_p;
        alist.push_back(s2);

        MyOwnNamespace::Teacher t;
        if (!(inputFile >> t.teacherName)) {
            std::cerr << "Error: Missing teacher name for revision in the input file." << std::endl;
            return 1;
        }
        teachers.push_back(t);
    }

    int sim = 0;
    for (int i = 0; i < alist.size(); i++) {
        for (int j = 0; j < alist[i].no_of_ppw; j++) {
            MyOwnNamespace::timetable deep;
            deep.subject = alist[i].subject;
            deep.teacherName = teachers[i].teacherName;
            deep.period_no = yaar[sim];
            tt.push_back(deep);
            sim += 1;
        }
    }

    std::sort(tt.begin(), tt.end(), MyOwnNamespace::CompareParola());

    std::ofstream outputFile("output.txt");
    if (!outputFile.is_open()) {
        std::cerr << "Error opening the output file." << std::endl;
        return 1;
    }

    std::vector<MyOwnNamespace::timetable> days[5];
    for (int i = 0; i < tt.size(); i++) {
        int day = (tt[i].period_no - 1) / 5;
        days[day].push_back(tt[i]);
    }

    for (int i = 0; i < 5; i++) {
        outputFile << "Day " << i + 1 << " Timetable:" << std::endl;
        MyOwnNamespace::printTable(days[i], outputFile);
    }

    MyOwnNamespace::myCout(std::cout, "Timetable successfully generated in output.txt");

    inputFile.close();
    outputFile.close();

    return 0;
}

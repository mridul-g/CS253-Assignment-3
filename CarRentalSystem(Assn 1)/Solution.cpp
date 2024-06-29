#include <iostream>
#include <fstream>
#include <vector>
#include <cstring>
#include <math.h>
#include <ctime>

using namespace std;

struct request
{
    string user_id;
    string model;
    int condition;
    int rent_cat; // 1 for customer, 2 for employee
    tm return_date;
    int request_cat;  // 1 for rent, 2 for return
};

double customer_rec_avg = 8, employee_rec_avg = 8;
const int seconds_per_day = 60*60*24;

tm default_date = {0, 0, 0, 1, 0, 100};

string Rent_category[] = {"None", "Customer", "Employee"};
string Request_category[] = {"None", "Rent", "Return"};

int time_diff(tm tm1, tm tm2)
{
    time_t t1 = std::mktime(&tm1);
    time_t t2 = std::mktime(&tm2);
    return difftime(t1, t2) / seconds_per_day;
}

tm day_add(tm tm1, int days)
{
    tm1.tm_mday += days;
    std::mktime(&tm1);
    return tm1;
}

tm get_time()
{
    time_t now = time(0);
    return *gmtime(&now);
}

class Car
{
    private:
        string model;
        int condition;
        bool rented;
        double price;
        string rentedTo;
        int rent_cat;  // 1 for customer, 2 for employee
        tm return_date;
    
    public:
        Car(string model, int condition, int price, bool rented = false, string rented_to = "None", int rent_cat = 0, tm date = default_date)
        {
            this-> model = model;
            this-> condition = condition;
            this-> rented = rented;
            this-> price = price;
            this-> rentedTo = rented_to;
            this-> rent_cat = rent_cat;
            this->return_date = date;
            std::mktime(&return_date);
        }

        string getModel() const {return model;}
        int getrent_cat() const {return rent_cat;}
        int getCondition() const {return condition;}
        double getPrice() const {return price;}
        bool isRented() const {return rented;}
        void setRented(bool rented) {this-> rented = rented;}
        void setCondition(const int& condition) {this-> condition = condition;}
        bool operator==(const Car& car) const {return model == car.model && condition == car.condition;}
        void setRentedTo(const string& rentedTo) {this-> rentedTo = rentedTo;}
        string getRentedTo() const {return rentedTo;}
        tm getreturnDate() const {return return_date;}
        void setreturnDate(const tm& return_date) {this-> return_date = return_date;}

        void rent_request(int rent_cat, string rented_to, tm return_date)
        {
            rented = true;
            this->rent_cat = rent_cat;
            this->rentedTo = rented_to;
            this->return_date = return_date;
            std::mktime(&this->return_date);
        }
};

class CarDatabase 
{
    public:
        vector<Car> cars;

        void ReadCar(const Car& car) {
            cars.push_back(car);
        }
        void addCar(const Car& car) {
            cars.push_back(car);
            updateCarDatabase();
        }
        void updateCar(int index, const Car& car) {
            if (index >= 0 && index < cars.size()) {
                cars[index] = car;
                updateCarDatabase();
            }
        }

        void deleteCar(int index) {
            if (index >= 0 && index < cars.size()) {
                cars.erase(cars.begin() + index);
                updateCarDatabase();
            }
        }

        int searchCar(const string& model) const {
            for (int i = 0; i < cars.size(); ++i) {
                if (cars[i].getModel() == model) {
                    return i;
                }
            }
            return -1;
        }

        void showCars(int x) const {
            std::cout << "Model      Condition       Price      is Rented?" << endl;
            for (int i = 0; i < cars.size(); i++) {
                if(cars[i].isRented() == false)
                    std::cout << cars[i].getModel() << " " << cars[i].getCondition() << " " << cars[i].getPrice()*(double)(1 - x * 0.1) << " " << cars[i].isRented() << endl;
            }
        }

        Car *getcar(int index) {
            return &cars[index];
        }

        int carnumber() const {
            return cars.size();
        }

        void updateCarDatabase()
        {
            ofstream fout;
            fout.open("Files/carDatabase.txt", ios::out | ios::trunc);
            for (int i = 0; i < cars.size(); ++i) {
                fout << cars[i].getModel() << " " << cars[i].getCondition() << " " << cars[i].getPrice() << " " << cars[i].isRented() << " " << cars[i].getRentedTo() << " " << cars[i].getrent_cat() << " " << cars[i].getreturnDate().tm_mday << " " << (cars[i].getreturnDate().tm_mon) << " " << (cars[i].getreturnDate().tm_year) << endl;
            }
            fout.close();
        }

        void showRentedCars()
        {
            std::cout << "Model      Condition          Rented To       DueDate" << endl;
            for (int i = 0; i < cars.size(); i++) {
                if(cars[i].isRented())
                    std::cout << cars[i].getModel() << " " << cars[i].getCondition() << " "  << " "  << cars[i].getRentedTo()  << " "<< cars[i].getreturnDate().tm_mday << "/" << (cars[i].getreturnDate().tm_mon + 1) << "/" << (cars[i].getreturnDate().tm_year + 1900) << endl;
            }
        }
};

class User
{
    protected:
        string name;
        string id;
        string pass;
    public:
        User(const string& name, const string& id, const string &pass) : name(name), id(id), pass(pass) {}
        string getID() const {return id;}
        string getName() const {return name;}
        string getPassword() const {return pass;}
};

class RequestsDatabase
{
    protected:
        vector<request> requests;
    
    public:
        void ReadRequest(const request& request) {
            requests.push_back(request);
        }
        void addRequest(const request& request) {
            requests.push_back(request);
            updateRequestDatabase();
        }
        void updateRequest(int index, const request& request) {
            if (index >= 0 && index < requests.size()) {
                requests[index] = request;
                updateRequestDatabase();
            }
        }
        void deleteRequest(CarDatabase &carDatabase, int index){
            if(requests[index].request_cat == 1)
            {
                int car_index = carDatabase.searchCar(requests[index].model);
                carDatabase.cars[car_index].rent_request(requests[index].rent_cat, requests[index].user_id, requests[index].return_date);
                carDatabase.cars[car_index].setRented(true);
            }
            else
            {
                int car_index = carDatabase.searchCar(requests[index].model);
                carDatabase.cars[car_index].rent_request(0, "None", default_date);
                carDatabase.cars[car_index].setRented(false);
            }
            if (index >= 0 && index < requests.size()) {
                requests.erase(requests.begin() + index);
                updateRequestDatabase();
            }
        }
        void removeRequest(int index) {
            if (index >= 0 && index < requests.size()) {
                requests.erase(requests.begin() + index);
                updateRequestDatabase();
            }
        }
        int searchRequest(const string& user_id, const string& model, int rent_cat, int request_cat) const {
            for (int i = 0; i < requests.size(); ++i) {
                if (requests[i].user_id == user_id && requests[i].model == model && requests[i].rent_cat == rent_cat && requests[i].request_cat == request_cat) {
                    return i;
                }
            }
            return -1;
        }
        void showRequests() const {
            std::cout << "Request Index     User ID      Model       Condition       Rent Category       Return Date       Request Category" << endl;
            for (int i = 0; i < requests.size(); ++i) {
                std::cout << i << " " << requests[i].user_id << " " << requests[i].model << " " << requests[i].condition << " " << Rent_category[requests[i].rent_cat] << " " <<  requests[i].return_date.tm_mday << "/" << requests[i].return_date.tm_mon + 1 << "/" << requests[i].return_date.tm_year + 1900 << " " << Request_category[requests[i].request_cat] << endl;
            }
        }

        int requestnumber() const {
            return requests.size();
        }

        request *getrequest(int index) {
            return &requests[index];
        }

        void updateRequestDatabase()
        {
            ofstream fout;
            fout.open("Files/request.txt", ios::out | ios::trunc);
            for (int i = 0; i < requests.size(); ++i) {
                fout << requests[i].user_id << " " << requests[i].model << " " << requests[i].condition << " " << requests[i].rent_cat << " " << requests[i].return_date.tm_mday << " " << requests[i].return_date.tm_mon << " " << requests[i].return_date.tm_year << " " << requests[i].request_cat << endl;
            }
            fout.close();
        }

};

class Customer: public User
{
    protected:
        vector<pair<Car, pair<tm, int > > > rented_cars;
        int fine_due;
        double customer_record;

    public:
        Customer(string id, string name, string pass, int fine_due = 0, double customer_record = customer_rec_avg) : User(name, id, pass)
        {
            this-> fine_due = fine_due;
            this-> customer_record = customer_record;
        }

        void rentCar(Car& car, tm time)
        {

            car.setRented(true);
            rented_cars.push_back(make_pair(car, make_pair(time, car.getCondition())));
        }

        int sumRentedCars()
        {
            int sum = 0;
            for (int i = 0; i < rented_cars.size(); ++i)
                sum += rented_cars[i].first.getCondition();
            return sum;
        }

        void rent_request(Car& car, tm time, RequestsDatabase &requestsDatabase)
        {
            if(car.isRented())
            {
                std::cout << "Car is already rented" << endl;
                return;
            }

            if(sumRentedCars() + car.getCondition() > customer_record)
            {
                std::cout << "You can't rent this car" << endl;
                return;
            }

            request r;
            r.user_id = id;
            r.model = car.getModel();
            r.condition = car.getCondition();
            r.rent_cat = 1;
            r.return_date = time;
            r.request_cat = 1;
            requestsDatabase.addRequest(r);
            requestsDatabase.updateRequestDatabase();
            std::cout << "Request sent to manager" << endl;
        }

        void return_request(Car& car, RequestsDatabase &requestsDatabase)
        {
            request r;
            r.user_id = id;
            r.model = car.getModel();
            r.condition = car.getCondition();
            r.rent_cat = 1;
            r.return_date = car.getreturnDate();
            r.request_cat = 2;
            requestsDatabase.addRequest(r);
            requestsDatabase.updateRequestDatabase();
        }

        void returnCar(Car& car, const tm& return_time, int condition)
        {
            for (int i = 0; i < rented_cars.size(); ++i) {
                if (rented_cars[i].first == car) 
                {   
                    // fine = late charges + damage to car
                    fine_due += (int)max(time_diff(return_time, car.getreturnDate()), 0) * 2500 + (int)pow((rented_cars[i].second.second - condition),2) * 5000;
                    customer_record += time_diff(car.getreturnDate(), return_time) / 2.0 + 0.5 - (rented_cars[i].second.second - condition) * 2.0;
                    customer_record = max(customer_record, 1.0);
                    customer_record = min(customer_record, 15.0);
                    rented_cars.erase(rented_cars.begin() + i);
                    car.setRented(false);
                    break;
                }
            }

            cout << "Kindly ask the user to pay the fine of " << fine_due << endl;
        }

        void payFine(int fine)
        {
            if(fine <= fine_due)
            {
                fine_due -= fine;
                cout << "Fine paid successfully!\n";
            }
            else
                cout << "Cannot pay more than dues!\n";
        }

        int getFine() const {return fine_due;}
        double getRecord() const {return customer_record;}
        bool checkPassword(const string& pass) const {return this-> pass == pass;}
        void showRentedCars() const
        {
            std::cout << "Rented Car Model       Return Date         Condition at time of Rent" << endl;
            for (int i = 0; i < rented_cars.size(); ++i) {
                std::cout << rented_cars[i].first.getModel() << " " << rented_cars[i].second.first.tm_mday << "/" << rented_cars[i].second.first.tm_mon+1<< "/" << rented_cars[i].second.first.tm_year << " " << rented_cars[i].second.second << endl;
            }
        }
};

class Employee: public User
{
    private:
        vector<pair<Car, pair<tm, int> > > rented_cars;
        int fine_due;
        double employee_record;

    public:
        Employee(string id, string name, string pass, int fine_due = 0, double employee_record = employee_rec_avg) : User(name, id, pass)
        {
            this-> fine_due = fine_due;
            this-> employee_record = employee_record;
        }
        int sumRentedCars()
        {
            int sum = 0;
            for (int i = 0; i < rented_cars.size(); ++i)
                sum += rented_cars[i].first.getCondition();
            return sum;
        }
        void rentCar(Car& car, const tm& return_time)
        {
            car.setRented(true);
            rented_cars.push_back(make_pair(car, make_pair(return_time, car.getCondition())));
        }

        void rent_request(Car& car, tm time, RequestsDatabase &requestsDatabase)
        {
            if(car.isRented())
            {
                std::cout << "Car is already rented" << endl;
                return;
            }

            if(sumRentedCars() + car.getCondition() >= employee_record)
            {
                std::cout << "You can't rent this car" << endl;
                return;
            }

            request r;
            r.user_id = id;
            r.model = car.getModel();
            r.condition = car.getCondition();
            r.rent_cat = 2;
            r.return_date = time;
            r.request_cat = 1;
            requestsDatabase.addRequest(r);
            requestsDatabase.updateRequestDatabase();
            std::cout << "Request sent to manager" << endl;
        }

        void return_request(Car& car, RequestsDatabase &requestsDatabase)
        {
            request r;
            r.user_id = id;
            r.model = car.getModel();
            r.condition = car.getCondition();
            r.rent_cat = 2;
            r.return_date = car.getreturnDate();
            r.request_cat = 2;
            requestsDatabase.addRequest(r);
            requestsDatabase.updateRequestDatabase();
        }
    
        void returnCar(Car& car, tm return_time, int condition)
        {
            for (int i = 0; i < rented_cars.size(); ++i) {
                if (rented_cars[i].first == car) {
                    fine_due += (int)max(time_diff(return_time, car.getreturnDate()), 0) * 2500 + (int)pow((car.getCondition() - condition),2) * 5000;
                    employee_record += time_diff(car.getreturnDate(), return_time) / 2.0 + 0.5  - (car.getCondition() - condition) * 2;
                    employee_record = max(employee_record, 1.0);
                    employee_record = min(15.0, employee_record);
                    rented_cars.erase(rented_cars.begin() + i);
                    car.setRented(false);
                    break;
                }
            }
            cout << "Kindly as the user to pay the fine of " << fine_due << endl;
        }

        void payFine(int fine)
        {
                fine_due -= fine;
                cout << "Fine paid successfully!\n";
        }

        int getFine() const {return fine_due;}

        double getRecord() const {return employee_record;}

        bool checkPassword(const string& pass) const {return this-> pass == pass;}

        void showRentedCars() const
        {
            std::cout << "Rented Car Model       Return Date         Condition at time of Rent" << endl;
            for (int i = 0; i < rented_cars.size(); ++i) {
                std::cout << rented_cars[i].first.getModel() << " " << rented_cars[i].second.first.tm_mday << "/" << rented_cars[i].second.first.tm_mon+1<< "/" << rented_cars[i].second.first.tm_year+1900 << " " << rented_cars[i].second.second << endl;
            }
        }

};

class CustomerDatabase 
{
    protected:
        vector<Customer> customers;

    public:
        void ReadCustomer(const Customer& customer) {
            customers.push_back(customer);
        }
        void addCustomer(const Customer& customer) {
            if(searchCustomer(customer.getID()) == -1)
            {
                customers.push_back(customer);
                updateCustomerDatabase();
            }
            else
                std::cout << "Customer already exists" << endl;
        }

        void updateCustomer(int index, const Customer& customer) {
            if (index >= 0 && index < customers.size()) {
                customers[index] = customer;
                updateCustomerDatabase();
            }
        }

        void deleteCustomer(int index) {
            if (index >= 0 && index < customers.size()) {
                customers.erase(customers.begin() + index);
                updateCustomerDatabase();
            }
        }

        int searchCustomer(const string& id) const {
            for (int i = 0; i < customers.size(); ++i) {
                if (customers[i].getID() == id) {
                    return i;
                }
            }
            return -1;
        }

        void rentRequest(int index, Car& car, tm time, RequestsDatabase &requestsDatabase) {
            customers[index].rent_request(car, time, requestsDatabase);
        }

        void returnRequest(int index, Car& car, RequestsDatabase &requestsDatabase) {
            customers[index].return_request(car, requestsDatabase);
        }
        void returnCar(int index, Car& car, int condition) {
            customers[index].returnCar(car, get_time(),  condition);
            double record_i = 0;
            car.setCondition(condition);
            for(int i = 0; i < customers.size(); i++)
            {
                record_i += customers[i].getRecord();
            }
            customer_rec_avg = record_i / customers.size();
        }

        void rentCar(int index, Car &car, tm return_time) {
            customers[index].rentCar(car, return_time);
        }

        void payFine(int index, int &fine) {
            customers[index].payFine(fine);
        }

        int getFine(int index) const {
            return customers[index].getFine();
        }

        double getRecord(int index) const {
            return customers[index].getRecord();
        }

        bool checkPassword(int index, const string& pass) const {
            return customers[index].checkPassword(pass);
        }

        void showRentedCars(int index) const
        {
            customers[index].showRentedCars();
        }

        Customer *getcustomer(int index) {
            return &customers[index];
        }

        int customernumber() const {
            return customers.size();
        }

        void updateCustomerDatabase()
        {
            double record_i = 0;
            ofstream fout;
            fout.open("Files/customerDatabase.txt", ios::out | ios::trunc);
            for (int i = 0; i < customers.size(); ++i) {
                fout << customers[i].getID() << " " << customers[i].getName() << " " << customers[i].getPassword() << " " << customers[i].getFine() << " " << customers[i].getRecord() << endl;
                record_i += customers[i].getRecord();
            }
            fout.close();
            customer_rec_avg = record_i / customers.size();
        }

        void showCustomers() const {
            std::cout << " ID     Name     Fine Due     Record      Type" << endl;
            for (int i = 0; i < customers.size(); ++i) {
                std::cout << customers[i].getID() << " " << customers[i].getName() << " " << customers[i].getFine() << " " << customers[i].getRecord() << " Customer" <<endl;
            }
        }
};

class EmployeeDatabase 
{
    protected:
        vector<Employee> employees;

    public:
        void ReadEmployee(const Employee& employee) {
            employees.push_back(employee);
        }
        void addEmployee(const Employee& employee) {
            if(searchEmployee(employee.getID()) == -1)
            {
                employees.push_back(employee);
                updateEmployeeDatabase();
            }
            else
                std::cout << "Employee already exists" << endl;
        }

        void updateEmployee(int index, const Employee& employee) {
            if (index >= 0 && index < employees.size()) {
                employees[index] = employee;
                updateEmployeeDatabase();
            }
        }

        void deleteEmployee(int index) {
            if (index >= 0 && index < employees.size()) {
                employees.erase(employees.begin() + index);
                updateEmployeeDatabase();
            }
        }

        int searchEmployee(const string& id) const {
            for (int i = 0; i < employees.size(); ++i) {
                if (employees[i].getID() == id) {
                    return i;
                }
            }
            return -1;
        }

        void returnCar(int index, Car& car, int condition) {
            employees[index].returnCar(car, get_time(), condition);
            double record_i = 0;
            car.setCondition(condition);
            for(int i = 0; i < employees.size(); i++)
            {
                record_i += employees[i].getRecord();
            }
            customer_rec_avg = record_i / employees.size();
        }

        void rentCar(int index, Car &car, tm return_time) {
            employees[index].rentCar(car, return_time);
        }

        void rentRequest(int index, Car& car, tm time, RequestsDatabase &requestsDatabase) {
            employees[index].rent_request(car, time, requestsDatabase);
        }

        void returnRequest(int index, Car& car, RequestsDatabase &requestsDatabase) {
            employees[index].return_request(car, requestsDatabase);
        }

        void payFine(int index, int fine) {
            employees[index].payFine(fine);
        }

        int getFine(int index) const {
            return employees[index].getFine();
        }

        double getRecord(int index) const {
            return employees[index].getRecord();
        }

        bool checkPassword(int index, const string& pass) const {
            return employees[index].checkPassword(pass);}
        
        void showRentedCars(int index) const{
            employees[index].showRentedCars();
        }

        Employee *getemployee(int index) {
            return &employees[index];
        }

        int employeenumber() const {
            return employees.size();
        }

        void updateEmployeeDatabase()
        {
            double record_i = 0;
            ofstream fout;
            fout.open("Files/employeeDatabase.txt", ios::out | ios::trunc);
            for (int i = 0; i < employees.size(); ++i) {
                fout << employees[i].getID() << " " << employees[i].getName() << " " << employees[i].getPassword() << " " << employees[i].getFine() << " " << employees[i].getRecord() << endl;
                record_i += employees[i].getRecord();
            }
            fout.close();
            employee_rec_avg = record_i / employees.size();
        }

        void showEmployees() const {
            std::cout << " ID     Name     Fine Due     Record      Type" << endl;
            for (int i = 0; i < employees.size(); ++i) {
                std::cout << employees[i].getID() << " " << employees[i].getName() << " " << employees[i].getFine() << " " << employees[i].getRecord() << " Employee" <<endl;
            }
        }
};

class Manager: public User
{
    private:


    public:
        Manager(string id, string name, string pass) : User(name, id, pass) {}
        // Manager functions
        string getID() const {return id;}
        string getPassword() const {return pass;}
        bool checkPassword(const string& pass) const {return this-> pass == pass;}
};

class ManagerDatabase
{
    protected:
        vector<Manager> managers;
    
    public:
        void ReadManager(const Manager& manager) {
            managers.push_back(manager);
        }
        void addManager(const Manager& manager) {
            managers.push_back(manager);
            updateManagerDatabase();
        }
        void updateManager(int index, const Manager& manager) {
            if (index >= 0 && index < managers.size()) {
                managers[index] = manager;
                updateManagerDatabase();
            }
        }
        void deleteManager(int index) {
            if (index >= 0 && index < managers.size()) {
                managers.erase(managers.begin() + index);
                updateManagerDatabase();
            }
        }
        int searchManager(const string& id) const {
            for (int i = 0; i < managers.size(); ++i) {
                if (managers[i].getID() == id) {
                    return i;
                }
            }
            return -1;
        }
        bool checkPassword(int index, const string& pass) const {
            return managers[index].checkPassword(pass);
        }

        // car functions
        void addCar(CarDatabase &carDatabase, const Car& car)
        {
            carDatabase.addCar(car);
        }
        void updateCar(CarDatabase &carDatabase, int index, const Car& car)
        {
            carDatabase.updateCar(index, car);
        }
        void deleteCar(CarDatabase &carDatabase, int index)
        {
            carDatabase.deleteCar(index);
        }

        // customer functions
        void addCustomer(CustomerDatabase &customerDatabase, const Customer& customer)
        {
            customerDatabase.addCustomer(customer);
        }
        void updateCustomer(CustomerDatabase &customerDatabase, int index, const Customer& customer)
        {
            customerDatabase.updateCustomer(index, customer);
        }
        void deleteCustomer(CustomerDatabase &customerDatabase, int index)
        {
            customerDatabase.deleteCustomer(index);
        }
        void rentCar(CustomerDatabase &customerDatabase,int index, Car &car, tm return_time)
        {
            customerDatabase.rentCar(index, car, return_time);
        }
        void returnCar(CustomerDatabase &customerDatabase, int index, Car& car, int condition)
        {
            customerDatabase.returnCar(index, car, condition);
        }
        void payFine(CustomerDatabase &customerDatabase, int index, int fine)
        {
            customerDatabase.payFine(index, fine);
        }

        // employee functions
        void addEmployee(EmployeeDatabase &employeeDatabase, const Employee& employee)
        {
            employeeDatabase.addEmployee(employee);
        }
        void updateEmployee(EmployeeDatabase &employeeDatabase, int index, const Employee& employee)
        {
            employeeDatabase.updateEmployee(index, employee);
        }
        void deleteEmployee(EmployeeDatabase &employeeDatabase, int index)
        {
            employeeDatabase.deleteEmployee(index);
        }
        void rentCar(EmployeeDatabase &employeeDatabase, int index, Car &car, tm return_time)
        {
            employeeDatabase.rentCar(index, car, return_time);
        }
        void returnCar(EmployeeDatabase &employeeDatabase, int index, Car& car, int condition)
        {
            employeeDatabase.returnCar(index, car, condition);
        }
        void payFine(EmployeeDatabase &employeeDatabase, int index, int fine)
        {
            employeeDatabase.payFine(index, fine);
        }

        void updateManagerDatabase()
        {
            ofstream fout;
            fout.open("Files/managerDatabase.txt", ios::out | ios::trunc);
            for (int i = 0; i < managers.size(); ++i) {
                fout << managers[i].getID() << " " << managers[i].getName() << " " << managers[i].getPassword() << endl;
            }
            fout.close();
        }

        void showManagers() const {
            std::cout << " ID     Name     Type" << endl;
            for (int i = 0; i < managers.size(); ++i) {
                std::cout << managers[i].getID() << " " << managers[i].getName() << " Manager" <<endl;
            }
        }
};

void initialise(CustomerDatabase &CustomerDatabase, EmployeeDatabase &employeeDatabase, CarDatabase &carDatabase, ManagerDatabase &managerDatabase, RequestsDatabase &requestsDatabase)
{
    // intialise each database by reading he data from files created with thier details
    ifstream fin;
    string line;
    fin.open("Files/customerDatabase.txt");
    while (fin){
        // getline(fin, line);
        // if(line == "")
        //     break;
        string id, name, pass;
        int fine_due;
        double customer_record;
        fin >> id >> name >> pass >> fine_due >> customer_record;
        if(id == "")    break;
        // cout << id << " " << name << " " << pass << " " << fine_due << " " << customer_record << endl;
        CustomerDatabase.ReadCustomer(Customer(id, name, pass, fine_due, customer_record));
    }
    fin.close();

    fin.open("Files/employeeDatabase.txt");
    while (fin) {
        // getline(fin, line);
        // if(line == "")
        //     break;
        string id, name, pass;
        int fine_due;
        double employee_record;
        fin >> id >> name >> pass >> fine_due >> employee_record;
        if(id == "")    break;
        employeeDatabase.ReadEmployee(Employee(id, name, pass, fine_due, employee_record));
    }
    fin.close();

    fin.open("Files/carDatabase.txt");
    while (fin) {
        // getline(fin, line);
        // if(line == "")
        //     break;
        string model, rented_to;
        int condition, price, rent_cat;
        bool rented;
        int day, month, year;
        fin >> model >> condition >> price >> rented >> rented_to >> rent_cat >> day >> month >> year;
        if(model == "")    break;
        tm date = {0, 0, 0, day, month, year};
        carDatabase.ReadCar(Car(model, condition, price, rented, rented_to, rent_cat, date));

        if(rented){
            if(rent_cat == 1){
                int index = CustomerDatabase.searchCustomer(rented_to);
                if(index != -1)
                {
                    CustomerDatabase.rentCar(index, carDatabase.cars[carDatabase.cars.size()-1], carDatabase.cars[carDatabase.cars.size()-1].getreturnDate());
                }
            }
            else{
                int index = employeeDatabase.searchEmployee(rented_to);
                if(index != -1){
                    employeeDatabase.rentCar(index, carDatabase.cars[carDatabase.cars.size()-1], carDatabase.cars[carDatabase.cars.size()-1].getreturnDate());
                }
            }
        }
    }
    fin.close();

    fin.open("Files/managerDatabase.txt");
    while (fin) {
        // getline(fin, line);
        // if(line == "")
        //     break;
        string id, name, pass;
        fin >> id >> name >> pass;
        if(id == "")    break;
        managerDatabase.ReadManager(Manager(id, name, pass));
    }
    fin.close();

    fin.open("Files/request.txt");
    while (fin) {
        // getline(fin, line);
        // if(line == "")
        //     break;
        string user_id, model;
        int condition, rent_cat, request_cat;
        int day, month, year;
        fin >> user_id >> model >> condition >> rent_cat >> day >> month >> year >> request_cat;
        if(user_id == "")    break;
        request r;
        r.user_id = user_id; r.model = model; r.condition = condition; r.rent_cat = rent_cat; r.return_date.tm_mday = day;
        r.return_date.tm_mon = month; r.return_date.tm_year = year; r.request_cat = request_cat;
        requestsDatabase.ReadRequest(r);
    }
    fin.close();

}

void default_init()
{
    // create 5 users each in all files
    ofstream fout;
    fout.open("Files/customerDatabase.txt", ios::out | ios::trunc);
    for (int i = 0; i < 5; ++i) {
        fout << "user" << i << " " << "user" << i << " " << "pass" << i << " " << 0 << " " << customer_rec_avg << endl;
    }
    fout.close();

    fout.open("Files/employeeDatabase.txt", ios::out | ios::trunc);
    for (int i = 0; i < 5; ++i) {
        fout << "employee" << i << " " << "employee" << i << " " << "pass" << i << " " << 0 << " " << employee_rec_avg << endl;
    }
    fout.close();

    fout.open("Files/managerDatabase.txt", ios::out | ios::trunc);
    for (int i = 0; i < 5; ++i) {
        fout << "manager" << i << " " << "manager" << i << " " << "pass" << i << endl;
    }
    fout.close();

    fout.open("Files/carDatabase.txt", ios::out | ios::trunc);
    for (int i = 0; i < 5; ++i) {
        fout << "car" << i << " " << 5 << " " << 1000 << " " << "false" << " " << "None" << " " << -1 << " " << 1 << " " << 1 << " " << 2000 << endl;
    }
    fout.close();
}

void default_init1(CustomerDatabase &CustomerDatabase, EmployeeDatabase &employeeDatabase, CarDatabase &carDatabase, ManagerDatabase &managerDatabase)
{
    // create 5 users in all databases
    for (int i = 0; i < 5; ++i) {
        CustomerDatabase.addCustomer(Customer("user" + to_string(i), "user" + to_string(i), "pass" + to_string(i), 0, customer_rec_avg));
        employeeDatabase.addEmployee(Employee("employee" + to_string(i), "employee" + to_string(i), "pass" + to_string(i), 0, employee_rec_avg));
        managerDatabase.addManager(Manager("manager" + to_string(i), "manager" + to_string(i), "pass" + to_string(i)));
        carDatabase.addCar(Car("car" + to_string(i), 5, 3000));
    }
    for(int i = 0; i < carDatabase.carnumber(); i++)
    {
        cout << "Car model: " << carDatabase.getcar(i)->getModel() << " " << " " << carDatabase.getcar(i)->isRented() << " " << carDatabase.getcar(i)->getRentedTo() << " " << carDatabase.getcar(i)->getreturnDate().tm_mday << "/" << carDatabase.getcar(i)->getreturnDate().tm_mon+1 << "/" << carDatabase.getcar(i)->getreturnDate().tm_year +1900 << endl;
    }

    //update dtabasefile
    CustomerDatabase.updateCustomerDatabase();
    employeeDatabase.updateEmployeeDatabase();
    managerDatabase.updateManagerDatabase();
    carDatabase.updateCarDatabase();

    

}

int main()
{
    string line;
    CustomerDatabase customerDatabase;
    EmployeeDatabase employeeDatabase;
    CarDatabase carDatabase;
    ManagerDatabase managerDatabase;
    RequestsDatabase requestsDatabase;

    // default_init();
    // default_init1(customerDatabase, employeeDatabase, carDatabase, managerDatabase);
    initialise(customerDatabase, employeeDatabase, carDatabase, managerDatabase, requestsDatabase);

    while(1)
    {
        string name, pass, id;
        std::cout << "Do you want to login or register. Press" << endl;
        std::cout << "1 for Login" << endl;
        std::cout << "2 for Register" << endl;
        std::cout << "3 to exit" << endl;

        std::cin >> line;
        if(line.length() != 1)
        {
            std::cout << "Invalid Input. Please enter a number between 1 and 3" << endl;
            continue;
        }

        std::cout << endl;
        if(line == "3")
            break;
        // Register
        if(line == "2")
        {
            std::cout << "Are you a Customer, Employee or Manager. Press" << endl;
            std::cout << "1 for Customer" << endl;
            std::cout << "2 for Employee" << endl;
            std::cout << "3 for Manager" << endl;

            std::cin >> line;
            
            std::cout << "Enter your Name: ";
            std::cin >> name;
            std::cout << "Enter your ID: ";
            std::cin >> id;
            std::cout << "Enter your Password: ";
            std::cin >> pass;

            if(id == "None")
            {
                std::cout << "Invalid ID" << endl;
                continue;
            }

            if (customerDatabase.searchCustomer(id) != -1 || employeeDatabase.searchEmployee(id) != -1 || managerDatabase.searchManager(id) != -1)
            {
                std::cout << "ID already exists" << endl;
                continue;
            }

            switch (line[0])
            {
                case '1':
                    // Register Customer
                    customerDatabase.addCustomer(Customer(id, name, pass));
                    break;

                case '2':
                    // Register Employee
                    employeeDatabase.addEmployee(Employee(id, name, pass));
                    break;

                case '3':
                    // Register Manager
                    std::cout << "Can't register manager yourself" << endl;
                    continue;
                    break;

                default:
                    std::cout << "Invalid Input" << endl;
                    continue;
            }
            continue;
        }
        else if(line != "1")
        {
            std::cout << "Invalid Input" << endl;
            continue;
        }

        // Login
        else
        {
            std::cout << "Are you a Customer, Employee or Manager. Press" << endl;
            std::cout << "1 for Customer" << endl;
            std::cout << "2 for Employee" << endl;
            std::cout << "3 for Manager" << endl;

            string loggedin_as;
            std::cin >> loggedin_as;
            if(loggedin_as.length() != 1)
            {
                std::cout << "Invalid Input" << endl;
                continue;
            }

            std::cout << "Enter your ID: ";
            std::cin >> id;
            std::cout << "Enter your Password: ";
            std::cin >> pass;

            int index = -1;

            switch(loggedin_as[0])
            {
                case '1':
                    // Login Customer
                    index = customerDatabase.searchCustomer(id);
                    if(index == -1)
                    {
                        std::cout << "Invalid ID" << endl;
                        continue;
                    }
                    if(customerDatabase.checkPassword(index, pass))
                    {
                        std::cout << "Login Successful" << endl;
                    }
                    else
                    {
                        std::cout << "Invalid Password" << endl;
                        continue;
                    }
                    break;
                
                case '2':
                    // Login Employee
                    index = employeeDatabase.searchEmployee(id);
                    if(index == -1)
                    {
                        std::cout << "Invalid ID" << endl;
                        continue;
                    }
                    if(employeeDatabase.checkPassword(index, pass))
                    {
                        std::cout << "Login Successful" << endl;
                    }
                    else
                    {
                        std::cout << "Invalid Password" << endl;
                        continue;
                    }
                    break;
                
                case '3':
                    // Login Manager
                    index = managerDatabase.searchManager(id);
                    if(index == -1)
                    {
                        std::cout << "Invalid ID" << endl;
                        continue;
                    }
                    if(managerDatabase.checkPassword(index, pass))
                    {
                        std::cout << "Login Successful\n" << endl;
                    }
                    else
                    {
                        std::cout << "Invalid ID or Password" << endl;
                        continue;
                    }
                    break;
                
                default:
                    std::cout << "Invalid Input" << endl;
                    continue;
            }

            // employee or customer
            if(loggedin_as == "1" || loggedin_as == "2")
            {
                while(1)
                {
                    string option;
                    // show options
                    std::cout << "\nPress" << endl;
                    std::cout << "1 to see list of available cars" << endl;
                    std::cout << "2 to see list of rented cars" << endl;
                    std::cout << "3 to rent a car" << endl;
                    std::cout << "4 to return a car" << endl;
                    std::cout << "5 to check fine" << endl;
                    std::cout << "6 to pay fine" << endl;
                    std::cout << "7 to check record" << endl;
                    std::cout << "8 to logout" << endl;
                    std::cin >> option;
                    std::cout << endl;

                    if(option.length() != 1)
                    {
                        std::cout << "Inavalid input! Enter a number between 0 to 7" << endl;
                        continue;
                    }

                    if(option[0] == '8')
                        break;

                    string model;
                    int days_rent, car_index, flag=0, fine_due=0;
                    tm return_time;
                    switch(option[0])
                    {
                        case '1':
                            // show list of available cars
                            carDatabase.showCars((int)(loggedin_as[0]-'1'));
                            break;
                        
                        case '2':
                            // show list of rented cars
                            std::cout << "Rented Cars" << endl;
                            if(loggedin_as == "1")
                            {
                                customerDatabase.showRentedCars(index);
                            }
                            else
                            {
                                employeeDatabase.showRentedCars(index);
                            }
                            break;

                        case '3':
                            // rent a car
                            fine_due = loggedin_as == "1" ? customerDatabase.getFine(index) : employeeDatabase.getFine(index);

                            if(fine_due > 0)
                            {
                                std::cout << "You have pending dues of " << fine_due << ". Please clear your dues before renting a car." << endl;
                                break;
                            }
                            for(int i = 0; i < requestsDatabase.requestnumber(); i++)
                            {
                                if((*requestsDatabase.getrequest(i)).user_id == id && (*requestsDatabase.getrequest(i)).request_cat == 1)
                                {
                                    std::cout << "You have a pending request. Please try after getting the request cleared." << endl;
                                    flag = 1;
                                    break;
                                }
                            }

                            if(flag)
                                break;
                            std::cout << "Enter the model of car you want to rent: ";
                            std::cin >> model;
                            car_index = carDatabase.searchCar(model);
                            if(car_index == -1)
                            {
                                std::cout << "Car not found" << endl;
                                break;
                            }
                            if(carDatabase.cars[car_index].isRented())
                            {
                                std::cout << "Car is already rented" << endl;
                                break;
                            }
                            // car is available
                            std::cout << "Days you want to rent car: ";
                            std::cin >> days_rent;
                            if(days_rent <= 0)
                            {
                                std::cout << "Invalid days" << endl;
                                continue;
                            }
                            return_time = day_add(get_time(), days_rent);
                            std::cout << "Car to be returned by: " << return_time.tm_mday << "/" << return_time.tm_mon + 1 << "/" << return_time.tm_year + 1900 << endl;

                            if(loggedin_as == "1")
                                customerDatabase.rentRequest(index, carDatabase.cars[car_index], return_time, requestsDatabase);
                            else
                                employeeDatabase.rentRequest(index, carDatabase.cars[car_index], return_time, requestsDatabase);
                            break;

                        case '4':
                            // return a car
                            std::cout << "Enter the model of car you want to return: ";
                            std::cin >> model;

                            car_index = carDatabase.searchCar(model);
                            if(car_index == -1)
                            {
                                std::cout << "Car not found" << endl;
                                continue;
                            }

                            if(loggedin_as == "1")
                            {
                                if(carDatabase.cars[car_index].getRentedTo() != id)
                                {
                                    std::cout << "Car not rented by you" << endl;
                                    break;
                                }
                                customerDatabase.returnRequest(index, carDatabase.cars[car_index], requestsDatabase);
                            }
                            else
                            {
                                if(carDatabase.cars[car_index].getRentedTo() != id)
                                {
                                    std::cout << "Car not rented by you" << endl;
                                    break;
                                }
                                employeeDatabase.returnRequest(index, carDatabase.cars[car_index], requestsDatabase);
                            }
                            std::cout << "Return Request Sent to manager" << endl;
                            break;
                        
                        case '5':
                            // check fine
                            if(loggedin_as == "1")
                            {
                                std::cout << "Fine: " << customerDatabase.getFine(index) << endl;
                            }
                            else
                            {
                                std::cout << "Fine: " << employeeDatabase.getFine(index) << endl;
                            }
                            break;

                        case '6':
                            // pay fine
                            int fine;
                            std::cout << "Enter the amount you want to pay: ";
                            std::cin >> fine;
                            if(loggedin_as == "1")
                            {
                                customerDatabase.payFine(index, fine);
                            }
                            else
                            {
                                employeeDatabase.payFine(index, fine);
                            }
                            break;

                        case '7':
                            // check record
                            if(loggedin_as == "1")
                            {
                                std::cout << "Record: " << customerDatabase.getRecord(index) << endl;
                            }
                            else
                            {
                                std::cout << "Record: " << employeeDatabase.getRecord(index) << endl;
                            }
                            break;

                        default:
                            std::cout << "Invalid Input" << endl;
                    }
                }
                
            }

            else if(loggedin_as == "3")
            {
                while(1)
                {
                    string option;
                    // show options
                    std::cout << "\nPress" << endl;
                    std::cout << "1 to see list of available cars" << endl;
                    std::cout << "2 to see list of rented cars" << endl;
                    std::cout << "3 to see rent/ return requests" << endl;
                    std::cout << "4 to aprove a rent request" << endl;
                    std::cout << "5 to aprove a return request" << endl;
                    std::cout << "6 to delete a request" << endl;
                    std::cout << "7 to add a car" << endl;
                    std::cout << "8 to delete a car" << endl;
                    std::cout << "9 to update a car" << endl;
                    std::cout << "10 to add a customer" << endl;
                    std::cout << "11 to delete a customer" << endl;
                    std::cout << "12 to update a customer" << endl;
                    std::cout << "13 to add an employee" << endl;
                    std::cout << "14 to delete an employee" << endl;
                    std::cout << "15 to update an employee" << endl;
                    std::cout << "16 to view list of all users" << endl;
                    std::cout << "17 to logout" << endl;
                    std::cin >> option;
                    std::cout << endl;

                    if(option.length() > 2)
                    {
                        std::cout << "Inavalid input! Enter a number between 1 to 15" << endl;
                        continue;
                    }
                    if(option == "17")  break;

                    string model;
                    int condition, price;
                    
                    int o_val = option[0] - '0';
                    if(option.length() == 2)
                        o_val = o_val * 10 + (option[1] - '0');

                    int car_index, customer_index, employee_index, fine_due=0;
                    tm return_time = get_time();
                    request *r = NULL;
                    switch (o_val)
                    {
                        case 1:
                            // show list of available cars
                            carDatabase.showCars(0);
                            break;
                        case 2:
                            // show list of rented cars
                            std::cout << "Rented Cars" << endl;
                            carDatabase.showRentedCars();
                            break;
                        case 3:
                            // show rent/ return requests
                            std::cout << "Requests" << endl;
                            requestsDatabase.showRequests();
                            break;
                        case 4:
                            // aprove a rent request
                            std::cout << "Enter the request index you want to aprove: ";
                            int request_index;
                            std::cin >> request_index;
                            if(request_index < 0 || request_index >= requestsDatabase.requestnumber())
                            {
                                std::cout << "Invalid request index" << endl;
                                continue;
                            }
                            r = requestsDatabase.getrequest(request_index);
                            if(r->request_cat == 2)
                            {
                                std::cout << "This is a return request" << endl;
                                continue;
                            }

                            if(r->rent_cat == 1)
                            {
                                customer_index = customerDatabase.searchCustomer(r->user_id);
                                if(customer_index == -1)
                                {
                                    std::cout << "Customer not found" << endl;
                                    continue;
                                }
                                car_index = carDatabase.searchCar(r->model);
                                if(car_index == -1)
                                {
                                    std::cout << "Car not found" << endl;
                                    continue;
                                }
                                customerDatabase.rentCar(customer_index, carDatabase.cars[car_index], r->return_date);
                            }
                            else
                            {
                                employee_index = employeeDatabase.searchEmployee(r->user_id);
                                if(employee_index == -1)
                                {
                                    std::cout << "Employee not found" << endl;
                                    continue;
                                }
                                car_index = carDatabase.searchCar(r->model);
                                if(car_index == -1)
                                {
                                    std::cout << "Car not found" << endl;
                                    continue;
                                }
                                employeeDatabase.rentCar(employee_index, carDatabase.cars[car_index], r->return_date);
                            }

                            std::cout << "Request aproved" << endl;
                            requestsDatabase.deleteRequest(carDatabase, request_index);
                            carDatabase.updateCarDatabase();
                            break;
                        case 5:
                            // aprove a return request
                            std::cout << "Enter the request index you want to aprove: ";
                            std::cin >> request_index;
                            if(request_index < 0 || request_index >= requestsDatabase.requestnumber())
                            {
                                std::cout << "Invalid request index" << endl;
                                continue;
                            }
                            r = requestsDatabase.getrequest(request_index);
                            if(r->request_cat == 1)
                            {
                                std::cout << "This is a rent request" << endl;
                                continue;
                            }

                            int car_condition;
                            std::cout << "Enter car condition: ";
                            std::cin >> car_condition;
                            r->condition = car_condition;
                            if(car_condition > carDatabase.cars[car_index].getCondition())
                            {
                                cout << "Condition cannot be greater than original condition" << endl;
                                break;
                            }
                            if(r->rent_cat == 1)
                            {
                                customer_index = customerDatabase.searchCustomer(r->user_id);
                                if(customer_index == -1)
                                {
                                    std::cout << "Customer not found" << endl;
                                    continue;
                                }
                                car_index = carDatabase.searchCar(r->model);
                                if(car_index == -1)
                                {
                                    std::cout << "Car not found" << endl;
                                    continue;
                                }
                                customerDatabase.returnCar(customer_index, carDatabase.cars[car_index], r->condition);
                                customerDatabase.updateCustomerDatabase();
                            }
                            else
                            {
                                employee_index = employeeDatabase.searchEmployee(r->user_id);
                                if(employee_index == -1)
                                {
                                    std::cout << "Employee not found" << endl;
                                    continue;
                                }
                                car_index = carDatabase.searchCar(r->model);
                                if(car_index == -1)
                                {
                                    std::cout << "Car not found" << endl;
                                    continue;
                                }
                                employeeDatabase.returnCar(employee_index, carDatabase.cars[car_index], r->condition);
                                employeeDatabase.updateEmployeeDatabase();
                            }

                            std::cout << "Request aproved" << endl;
                            requestsDatabase.deleteRequest(carDatabase, request_index);
                            carDatabase.updateCarDatabase();
                            break;
                        case 6:
                            // delete a request
                            std::cout << "Enter the request index you want to delete: ";
                            std::cin >> request_index;
                            if(request_index < 0 || request_index >= requestsDatabase.requestnumber())
                            {
                                std::cout << "Invalid request index" << endl;
                                continue;
                            }
                            requestsDatabase.removeRequest(request_index);
                            std::cout << "Request deleted" << endl;
                            break;
                        case 7:
                            // add a car
                            std::cout << "Enter the model of car: ";
                            std::cin >> model;
                            std::cout << "Enter the condition of car: ";
                            std::cin >> condition;
                            std::cout << "Enter the price of car: ";
                            std::cin >> price;
                            if(condition < 0 || price < 0 || condition > 5)
                            {
                                std::cout << "Invalid Parameters of car, Kindly check again" << endl;
                                break;
                            }

                            if(carDatabase.searchCar(model) != -1)
                            {
                                std::cout << "Car already exists" << endl;
                                break;
                            }
                            managerDatabase.addCar(carDatabase, Car(model, condition, price));
                            std::cout << "Car added" << endl;
                            break;
                        case 8:
                            // delete a car
                            std::cout << "Enter the model of car you want to delete: ";
                            std::cin >> model;
                            car_index = carDatabase.searchCar(model);
                            if(car_index == -1)
                            {
                                std::cout << "Car not found" << endl;
                                continue;
                            }
                            managerDatabase.deleteCar(carDatabase, car_index);
                            std::cout << "Car deleted" << endl;
                            break;
                        case 9:
                            // update a car
                            std::cout << "Enter the model of car you want to update: ";
                            std::cin >> model;
                            car_index = carDatabase.searchCar(model);
                            if(car_index == -1)
                            {
                                std::cout << "Car not found" << endl;
                                continue;
                            }
                            std::cout << "Enter the new condition of car: ";
                            std::cin >> condition;
                            std::cout << "Enter the new price of car: ";
                            std::cin >> price;
                            if(condition < 0 || price < 0 || condition > 5)
                            {
                                std::cout << "Invalid Parameters of car, Kindly check again" << endl;
                                break;
                            }
                            managerDatabase.updateCar(carDatabase, car_index, Car(model, condition, price));
                            std::cout << "Car updated" << endl;
                            break;
                        case 10:
                            // add a customer
                            std::cout << "Enter the name of customer: ";
                            std::cin >> name;
                            std::cout << "Enter the ID of customer: ";
                            std::cin >> id;
                            std::cout << "Enter the password of customer: ";
                            std::cin >> pass;
                            if(customerDatabase.searchCustomer(id) != -1 || employeeDatabase.searchEmployee(id) != -1 || managerDatabase.searchManager(id) != -1)
                            {
                                std::cout << "ID not available" << endl;
                                break;
                            }
                            managerDatabase.addCustomer(customerDatabase, Customer(id, name, pass));
                            std::cout << "Customer added" << endl;
                            break;
                        case 11:
                            // delete a customer
                            std::cout << "Enter the ID of customer you want to delete: ";
                            std::cin >> id;
                            customer_index = customerDatabase.searchCustomer(id);
                            if(customer_index == -1)
                            {
                                std::cout << "Customer not found" << endl;
                                continue;
                            }
                            managerDatabase.deleteCustomer(customerDatabase, customer_index);
                            std::cout << "Customer deleted" << endl;
                            break;
                        case 12:
                            // update a customer
                            std::cout << "Enter the ID of customer you want to update: ";
                            std::cin >> id;
                            customer_index = customerDatabase.searchCustomer(id);
                            if(customer_index == -1)
                            {
                                std::cout << "Customer not found" << endl;
                                continue;
                            }
                            std::cout << "Enter the new name of customer: ";
                            std::cin >> name;
                            std::cout << "Enter the new password of customer: ";
                            std::cin >> pass;
                            managerDatabase.updateCustomer(customerDatabase, customer_index, Customer(id, name, pass, (*customerDatabase.getcustomer(customer_index)).getFine(), (*customerDatabase.getcustomer(customer_index)).getRecord()));
                            std::cout << "Customer updated" << endl;
                            break;
                        case 13:
                            // add an employee
                            std::cout << "Enter the name of employee: ";
                            std::cin >> name;
                            std::cout << "Enter the ID of employee: ";
                            std::cin >> id;
                            std::cout << "Enter the password of employee: ";
                            std::cin >> pass;
                            if(customerDatabase.searchCustomer(id) != -1 || employeeDatabase.searchEmployee(id) != -1 || managerDatabase.searchManager(id) != -1)
                            {
                                std::cout << "ID not available" << endl;
                                break;
                            }
                            managerDatabase.addEmployee(employeeDatabase, Employee(id, name, pass));
                            std::cout << "Employee added" << endl;
                            break;
                        case 14:  
                            // delete an employee
                            std::cout << "Enter the ID of employee you want to delete: ";
                            std::cin >> id;
                            employee_index = employeeDatabase.searchEmployee(id);
                            if(employee_index == -1)
                            {
                                std::cout << "Employee not found" << endl;
                                continue;
                            }
                            managerDatabase.deleteEmployee(employeeDatabase, employee_index);
                            std::cout << "Employee deleted" << endl;
                            break;
                        case 15:
                            // update an employee
                            std::cout << "Enter the ID of employee you want to update: ";
                            std::cin >> id;
                            employee_index = employeeDatabase.searchEmployee(id);
                            if(employee_index == -1)
                            {
                                std::cout << "Employee not found" << endl;
                                continue;
                            }
                            std::cout << "Enter the new name of employee: ";
                            std::cin >> name;
                            std::cout << "Enter the new password of employee: ";
                            std::cin >> pass;
                            managerDatabase.updateEmployee(employeeDatabase, employee_index, Employee(id, name, pass, (*employeeDatabase.getemployee(employee_index)).getFine(), (*employeeDatabase.getemployee(employee_index)).getRecord()));
                            std::cout << "Employee updated" << endl;
                            break;
                        case 16:
                            // view list of all users
                            std::cout << "Customers" << endl;
                            customerDatabase.showCustomers();
                            std::cout << "Employees" << endl;
                            employeeDatabase.showEmployees();
                            std::cout << "Managers" << endl;
                            managerDatabase.showManagers();
                            break;
                        default:
                            std::cout << "Invalid Input" << endl;
                            break;
                    }
                }
            }
        }
    }
}
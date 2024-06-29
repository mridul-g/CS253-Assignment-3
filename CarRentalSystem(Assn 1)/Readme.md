# Car Rental System

## Overview
This project is the submission of Mridul Gupta (220672) for assignment 1 of course 253.

This project implements a simple car rental system in C++. It includes classes to manage cars, users (customers, employees, managers), rental requests, and databases.

The working is defined as follows:
- There are 5 users, 5 employes, 5 cars and 2 managers int he database initially.
- New customers or employees can register using the register option or by asking the manager.
- A customer or employee can request to rent a car. He will be rented a car only if his request is aproved by the manager.
- When a customer or employee returns a car, a request will be sent to manager. He will examine the car condition and accept the return.
- The <b> customer/employee record</b> is based on user's history. The car is rented to a user if the sum of conditions of cars rented is less than or equal to his record.
- Record varies from 1 to 15. And its default value is 8.
## How to Run the Code

Follow the steps below to compile and run the `Solution.cpp` file:

1. Open a terminal window.
2. Navigate to the directory where the `Solution.cpp` file is located using the `cd` command. For example, if the file is in a folder named `src` on your desktop, you would type `cd Desktop/src`.
3. Make sure that the Files folder is in the same directory and the files `carDatabase.txt`, `customerDatabase.txt`, `employeeDatabase.txt`, `managerDatabase.txt`, and `request.txt` are present in the Files folder.
4. Once you're in the correct directory, compile the `Solution.cpp` file using the `g++` command followed by the name of your file. For example, you would type `g++ Solution.cpp -o Solution`.
5. After the file has been compiled, you can run it by typing `./Solution`.


Please note that these instructions assume you have a C++ compiler installed on your computer. If you don't, you will need to install one. The `g++` compiler is recommended.
## Default Logins

For testing purposes, the system is pre-populated with some default users. You can use these logins to test the functionality of the system.

### Customer Login (5 default logins)

- Username: user0, user1, user2, user3, user4
- Password: pass0, pass1, pass2, pass3, pass4

### Employee Login (5 default logins)

- Usernames: employee0, employee1
- Passwords: pass0, pass1

### Manager Login (2 default logins)

- Username: manager0, manager1
- Password: pass0, pass1

To login, follow these steps:

1. Run the program by typing `./Solution` in the terminal.
2. You will be presented with a menu. Choose the option to login.
3. Enter the username and password of the user you want to login as.
4. If the login is successful, you will be taken to the respective user's dashboard where you can perform various operations.

Please note that these are default logins for testing purposes. In a real-world application, you would need to register and create your own login credentials.

## Fine Calculation in Car Rental System

In the car rental system, the charges for the car are not included in the fine dues and it is assumed that the user has already paid the car charges. The users's record is also affected by late returns and damage to the cars. Fines and record for late returns and damage to cars are calculated using the following formula:
```cpp
fines = 2500 * days_delayed + (change_in_condition ^ 2) * 5000

record = record - days_delayed / 2 + 0.5 + (change_in_condition) * 2
```


Where:
- `days_delayed` is the number of days the return is delayed.
- `change_in_condition` is the difference in the car's condition at the time of return compared to the condition when it was rented. 
- Condition varies from 0 to 5 where 5 is best and 0 is worst.

This formula ensures that fines account for both late returns and any damage incurred during the rental period.

## Classes

### 1. Car

The `Car` class represents a car in the car rental system. It encapsulates data about the car, such as its model, condition, rental status, and rental details. The class also provides methods to rent the car, return the car, and update its rental status and condition.

### 2. CarDatabase

The `CarDatabase` class manages a collection of cars in the car rental system. It provides functionality to add, update, delete, and search for cars. Additionally, it can display information about the cars and update the car database file.

### 3. User

The `User` class represents a user of the car rental system. It encapsulates data about the user, such as their name, ID, and password. This class serves as a base class for specific user types like customers and employees.

### 4. Customer

The `Customer` class represents a customer of the car rental system. It inherits from the `User` class and adds functionality specific to customers, such as renting cars, returning cars, paying fines, and managing rented cars.

### 5. Employee

The `Employee` class represents an employee of the car rental system. Similar to the `Customer` class, it inherits from the `User` class and adds functionality specific to employees, such as renting cars on behalf of customers and managing rented cars.

### 6. RequestsDatabase

The `RequestsDatabase` class manages a collection of rental requests in the car rental system. It provides functionality to add, update, delete, and search for rental requests. Additionally, it can display information about the rental requests and update the request database file.

### 7. CustomerDatabase

The `CustomerDatabase` class manages a collection of customers in the car rental system. It provides functionality to add, update, delete, and search for customers. Additionally, it can handle customer-related operations such as renting cars, returning cars, and managing fines.

### 8. EmployeeDatabase

The `EmployeeDatabase` class manages a collection of employees in the car rental system. It provides functionality to add, update, delete, and search for employees. Additionally, it can handle employee-related operations such as renting cars, returning cars, and managing fines.

### 9. Manager

The `Manager` class represents a manager of the car rental system. It inherits from the `User` class and adds functionality specific to managers, such as managing users (customers, employees, managers) and the car rental system as a whole.

### 10. ManagerDatabase

The `ManagerDatabase` class manages a collection of managers in the car rental system. It provides functionality to add, update, delete, and search for managers. Additionally, it can handle manager-related operations such as managing users and updating the manager database file.

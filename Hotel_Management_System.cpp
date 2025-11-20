#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include <fstream>
using namespace std;

class Room
{
protected:
    int roomNumber;
    bool isBooked;
    double pricePerDay;
    string customerName;
    string customerPhone;

public:
    Room(int number, double price)
        : roomNumber(number), isBooked(false), pricePerDay(price), customerName(""), customerPhone("") {}

    virtual ~Room() {}

    virtual double calculateBill(int days) = 0;

    int getRoomNumber() const { return roomNumber; }
    bool getStatus() const { return isBooked; }
    double getPrice() const { return pricePerDay; }
    string getCustomerName() const { return customerName; }
    string getCustomerPhone() const { return customerPhone; }

    void bookRoom(const string &name, const string &phone)
    {
        isBooked = true;
        customerName = name;
        customerPhone = phone;
    }

    void freeRoom()
    {
        isBooked = false;
        customerName = "";
        customerPhone = "";
    }
};

class StandardRoom : public Room
{
public:
    StandardRoom(int number) : Room(number, 1500.0) {}
    double calculateBill(int days) override { return pricePerDay * days; }
};

class DeluxeRoom : public Room
{
public:
    DeluxeRoom(int number) : Room(number, 3000.0) {}
    double calculateBill(int days) override { return (pricePerDay * days) * 0.95; }
};

class SuiteRoom : public Room
{
public:
    SuiteRoom(int number) : Room(number, 5000.0) {}
    double calculateBill(int days) override { return (pricePerDay * days) * 0.9; }
};

class Hotel
{
private:
    vector<Room *> rooms;

public:
    Hotel()
    {
        for (int i = 1; i <= 5; ++i)
            rooms.push_back(new StandardRoom(100 + i));
        for (int i = 1; i <= 3; ++i)
            rooms.push_back(new DeluxeRoom(200 + i));
        for (int i = 1; i <= 2; ++i)
            rooms.push_back(new SuiteRoom(300 + i));
    }

    ~Hotel()
    {
        for (Room *r : rooms)
            delete r;
    }

    Room *findRoom(int roomNumber)
    {
        for (Room *r : rooms)
            if (r->getRoomNumber() == roomNumber)
                return r;
        return nullptr;
    }

    void showAllRooms() const
    {
        cout << "\nAll Rooms in Hotel:\n";
        cout << left << setw(12) << "Room No" << setw(12)
             << "Type" << setw(12) << "Price/day" << setw(12)
             << "Status" << setw(20) << "Customer Name" << setw(15) << "Phone" << '\n';
        cout << string(80, '-') << '\n';

        for (const Room *r : rooms)
        {
            string type;
            if (dynamic_cast<const StandardRoom *>(r))
                type = "Standard";
            else if (dynamic_cast<const DeluxeRoom *>(r))
                type = "Deluxe";
            else if (dynamic_cast<const SuiteRoom *>(r))
                type = "Suite";
            else
                type = "Unknown";

            cout << left << setw(12) << r->getRoomNumber()
                 << setw(12) << type
                 << setw(12) << r->getPrice()
                 << setw(12) << (r->getStatus() ? "Booked" : "Free")
                 << setw(20) << (r->getStatus() ? r->getCustomerName() : "-")
                 << setw(15) << (r->getStatus() ? r->getCustomerPhone() : "-")
                 << '\n';
        }
    }

    void showAvailableRooms() const
    {
        cout << "\nAvailable Rooms:\n";
        cout << left << setw(12) << "Room No" << setw(12)
             << "Type" << setw(12) << "Price/day" << '\n';
        cout << string(36, '-') << '\n';

        for (const Room *r : rooms)
        {
            if (!r->getStatus())
            {
                string type;
                if (dynamic_cast<const StandardRoom *>(r))
                    type = "Standard";
                else if (dynamic_cast<const DeluxeRoom *>(r))
                    type = "Deluxe";
                else if (dynamic_cast<const SuiteRoom *>(r))
                    type = "Suite";

                cout << left << setw(12) << r->getRoomNumber()
                     << setw(12) << type
                     << setw(12) << r->getPrice() << '\n';
            }
        }
    }

    void bookRoom(int roomNumber, int days, const string &name, const string &phone)
    {
        Room *room = findRoom(roomNumber);
        if (!room)
        {
            cout << "Room not found!\n";
            return;
        }
        if (room->getStatus())
        {
            cout << "Room is already booked.\n";
            return;
        }

        room->bookRoom(name, phone);
        double bill = room->calculateBill(days);
        cout << "\nRoom " << roomNumber << " booked successfully for " << days << " day(s).\n";
        cout << "Customer: " << name << " | Phone: " << phone << '\n';
        cout << "Total bill: Rs " << bill << "\n";
    }

    void freeRoom(int roomNumber)
    {
        Room *room = findRoom(roomNumber);
        if (!room)
        {
            cout << "Room not found!\n";
            return;
        }
        if (!room->getStatus())
        {
            cout << "Room is already free.\n";
            return;
        }

        room->freeRoom();
        cout << "Room " << roomNumber << " is now available.\n";
    }

    void saveToFile(const string &filename) const
    {
        ofstream ofs(filename);
        for (const Room *r : rooms)
        {
            ofs << r->getRoomNumber() << ' '
                << r->getPrice() << ' '
                << r->getStatus() << ' '
                << r->getCustomerName() << ' '
                << r->getCustomerPhone() << '\n';
        }
        ofs.close();
        cout << "Data saved successfully.\n";
    }

    void loadFromFile(const string &filename)
    {
        ifstream ifs(filename);
        if (!ifs)
        {
            cout << "No saved data found.\n";
            return;
        }

        int num;
        double price;
        bool status;
        string name, phone;

        while (ifs >> num >> price >> status >> name >> phone)
        {
            Room *r = findRoom(num);
            if (r)
            {
                if (status)
                    r->bookRoom(name, phone);
                else
                    r->freeRoom();
            }
        }
        ifs.close();
        cout << "Data loaded successfully.\n";
    }
};

bool loginSystem()
{
    string username, password;
    cout << "=== HOTEL LOGIN SYSTEM ===\n";
    cout << "Enter username: ";
    cin >> username;
    cout << "Enter password: ";
    cin >> password;

    if (username == "abhinikesh" && password == "1234")
    {
        cout << "\nLogin successful! Welcome Admin.\n";
        return true;
    }
    else
    {
        cout << "\nInvalid username or password. Access denied.\n";
        return false;
    }
}

int main()
{
    if (!loginSystem())
        return 0;

    Hotel hotel;
    int choice = 0;
    const string dataFile = "hotel_data.txt";

    do
    {
        cout << "\n=== HOTEL MANAGEMENT SYSTEM ===\n";
        cout << "1. Show Available Rooms\n";
        cout << "2. Book a Room\n";
        cout << "3. Free a Room\n";
        cout << "4. Show All Rooms\n";
        cout << "5. Save Data to File\n";
        cout << "6. Load Data from File\n";
        cout << "7. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice)
        {
        case 1:
            hotel.showAvailableRooms();
            break;
        case 2:
        {
            int roomNo, days;
            string name, phone;
            cout << "Enter room number: ";
            cin >> roomNo;
            cout << "Enter number of days: ";
            cin >> days;
            cout << "Enter customer name: ";
            cin >> name;
            cout << "Enter customer phone: ";
            cin >> phone;
            hotel.bookRoom(roomNo, days, name, phone);
            break;
        }
        case 3:
        {
            int roomNo;
            cout << "Enter room number to free: ";
            cin >> roomNo;
            hotel.freeRoom(roomNo);
            break;
        }
        case 4:
            hotel.showAllRooms();
            break;
        case 5:
            hotel.saveToFile(dataFile);
            break;
        case 6:
            hotel.loadFromFile(dataFile);
            break;
        case 7:
            cout << "Exiting...\n";
            break;
        default:
            cout << "Invalid choice.\n";
            break;
        }
    } while (choice != 7);

    return 0;
}

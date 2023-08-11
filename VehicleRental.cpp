#include <bits/stdc++.h>
using namespace std;

/* =========================================================== */
/* ================== Vehicle Rental Design ================== */
/* =========================================================== */

/* Represents location of User and Vehicle */
class Location {
public:
    int latitude;
    int longitude;
    int pincode;
    string city;
    string country;
    
    Location() {};

    Location(int lat, int lng, int pincode, string city, string country) {
        this->latitude = lat;
        this->longitude = lng;
        this->pincode = pincode;
        this->city = city;
        this->country = country;
    }
};

enum LicenseType
{
    MC, 
    MCWG, 
    LMV, 
    HMV
};

/* Holds driving license information of a user */
class LicenseInfo {
public:
    int licenseNo;
    string driverName;
    int issuedAt;
    int validTill;
    string address;
    LicenseType licenseType;
    
    LicenseInfo() {};

    LicenseInfo(int licenseNo, string driverName, int issuedAt, int validTill, string address, LicenseType licenseType) {
        this->licenseNo = licenseNo;
        this->driverName = driverName;
        this->validTill = validTill;
        this->issuedAt = issuedAt;
        this->address = address;
        this->licenseType = licenseType;
    }
};

/* User account information */
class User {
public:
    int userId;
    string name;
    string email;
    string phone;
    Location location;
    LicenseInfo licenseInfo;
    
    User() {};

    User(string name, string email, string phone) {
        this->userId = time(NULL);
        this->name = name;
        this->email = email;
        this->phone = phone;
    }

    void setLocation(Location location) {
        this->location = location;
    }

    void setLicenseInfo(LicenseInfo licenseInfo) {
        this->licenseInfo = licenseInfo;
    }
};

enum VehicleType {
    CAR,
    BIKE,
    VAN,
    SUV
    //...
};

/* Holds vehicle related information & attributes */
class Vehicle {
public:
    int vehicleId;
    string name;
    string model;
    string description;
    VehicleType vehicleType;
    Location location;
    int seatingCapacity;
    int rentalPrice;
    int kmsDriven;
    
    Vehicle() {};

    Vehicle(string name, string model, VehicleType vehicleType) {
        this->vehicleId = time(NULL);
        this->name = name;
        this->model = model;
        this->vehicleType = vehicleType;
    }

    void setDescription(string description) {
        this->description = description;
    }

    void setLocation(Location location) {
        this->location = location;
    }

    void setSeatingCapacity(int capacity) {
        this->seatingCapacity = capacity;
    }

    void setRentalPrice(int price) {
        this->rentalPrice = price;
    }

    void setKmsDriven(int kmsDriven) {
        this->kmsDriven = kmsDriven;
    }
};

enum ReservationStatus 
{
    PENDING_CONFIRMATION,
    CONFIRMED,
    CANCELLED,
    COMPLETE
};

class Invoice;

/* Maps user to a vehicle from startTime to endTime */
class Reservation {
public: 
    int reservationId;
    Vehicle vehicle;
    User user;
    int startTime;
    int endTime;
    Location startLocation;
    Location endLocation;
    Invoice *invoice;
    ReservationStatus status;
    
    Reservation() {};

    Reservation(User user, Vehicle vehicle, int stTime, int endTime, Location stLocation, Location endLocation) {
        this->reservationId = time(NULL);
        this->user = user;
        this->vehicle = vehicle;
        this->startTime = stTime;
        this->endTime = endTime;
        this->startLocation = stLocation;
        this->endLocation = endLocation;
        this->status = ReservationStatus::CONFIRMED;
    }

    void setInvoice(Invoice *invoice) {
        this->invoice = invoice;
    }

    void setReservationStatus(ReservationStatus status) {
        this->status = status;
    }
};

/* Invoice is associated with the reservation */
class Invoice {
public:
    int invoiceId;
    Reservation reservation;
    int charges;

    Invoice(Reservation reservation) {
        this->invoiceId = time(NULL);
        this->reservation = reservation;

        int duration = reservation.endTime - reservation.startTime;
        duration = duration / (60 * 60);

        this->charges = duration * reservation.vehicle.rentalPrice;
    }
};

enum PaymentStatus {
    PROCESSING,
    COMPLETED,
    FAILED
};

/* Handles payment for vehicle rental using invoice */
class Payment {
public:
    int paymentId;
    int paymentTime;
    int amount;
    int reservationId;
    PaymentStatus status;

    Payment(int amount, int reservationId) {
        this->paymentId = time(NULL);
        this->amount = amount;
        this->reservationId = reservationId;
        this->status = PaymentStatus::PROCESSING;
    }

    void makePayment() {
        cout << "Making Payment of Rs. " << amount << " against reservationId " << reservationId << endl;
        sleep(5);
        this->status = PaymentStatus::COMPLETED;
        this->paymentTime = time(NULL);
        cout << "Payment successful!" << endl;
    }
};

/* Core application wrapper */
class RentalSystem
{
    static RentalSystem* instance;

    RentalSystem() {}
    RentalSystem(const RentalSystem &) {}

public:
    vector<User> users;
    vector<Vehicle> vehicles;
    vector<Reservation> reservations;

    static RentalSystem* getInstance();

    void addUser(User user) {
        this->users.push_back(user);
        cout << "Created user with ID " << user.userId << " name " << user.name << endl;
    }

    void addVehicle(Vehicle vehicle) {
        this->vehicles.push_back(vehicle);
        cout << "Created vehicle with ID " << vehicle.vehicleId << " rental price " << vehicle.rentalPrice << endl;
    }

    void listAvailableVehicles(Location location, int startTime, int endTime) {
        cout << "Listing Available Vehicles: " << endl;
        for (auto vehicle: vehicles) {
            if (vehicle.location.city == location.city) {
                bool reservationFound = false;
                for (auto reservation: reservations) {
                    if (reservation.vehicle.vehicleId == vehicle.vehicleId &&
                        reservation.status != ReservationStatus::CANCELLED &&
                        reservation.status != ReservationStatus::COMPLETE && 
                        (reservation.startTime <= startTime && startTime <= reservation.endTime ||
                        reservation.startTime <= endTime && endTime <= reservation.endTime)) {
                        reservationFound = true;
                        break;
                    }
                }

                if (!reservationFound) {
                    cout << "Vehicle " << vehicle.vehicleType << " with ID " << vehicle.vehicleId << endl;
                }
            }
        }
    }

    Reservation makeReservation(User user, Vehicle vehicle, int startTime, int endTime, Location startLocation, Location endLocation) {
        Reservation reservation(user, vehicle, startTime, endTime, startLocation, endLocation);
        this->reservations.push_back(reservation);
        cout << "Reservation successful for user " << user.userId << " and vehicle " << vehicle.vehicleId << " from " << startTime << " to " << endTime << endl;
        return reservation;
    }

    Invoice completeReservation(Reservation reservation) {
        reservation.setReservationStatus(ReservationStatus::COMPLETE);
        Invoice invoice(reservation);
        reservation.setInvoice(&invoice);
        cout << "Reservation completed for reservationId " << reservation.reservationId << endl; 
        cout << "Invoice created (" << invoice.invoiceId << ") for reservationId " << reservation.reservationId << endl; 
        return invoice;
    }

    void makePayment(Invoice invoice) {
        Payment payment(invoice.charges, invoice.reservation.reservationId);
        payment.makePayment();
    }
};

RentalSystem* RentalSystem::instance = NULL;
RentalSystem* RentalSystem::getInstance() {
    if (instance == NULL)
        instance = new RentalSystem();
    return instance;
}

/* Driver function */
int main()
{
    RentalSystem *system = RentalSystem::getInstance();

    // Creating and adding user
    User user("Abhishek", "abhishek@gmail.com", "9999999999");
    Location location(28, 77, 560093, "Karnataka", "India");
    user.setLocation(location);

    LicenseInfo licenseInfo(61327, "Abhishek", time(NULL) - 360*24*60*60, time(NULL) + 360*5*24*60*60, "Karnataka, India", LicenseType::LMV);
    user.setLicenseInfo(licenseInfo);

    system->addUser(user);

    // Creating and adding vehicle
    Vehicle vehicle("WagonR", "LXI 2015", VehicleType::CAR);
    vehicle.setLocation(location);
    vehicle.setSeatingCapacity(5);
    vehicle.setRentalPrice(150);
    vehicle.setKmsDriven(20000);

    system->addVehicle(vehicle);

    // List available vehicles
    system->listAvailableVehicles(location, time(NULL) + 2*24*60*60, time(NULL) + 5*24*60*60);

    // Make reservation for user and vehicle from startTime to endTime
    Reservation reservation = system->makeReservation(user, vehicle, time(NULL) + 2*24*60*60, time(NULL) + 5*24*60*60, location, location);

    // shouldn't list any vehicles since it is reserved
    system->listAvailableVehicles(location, time(NULL) + 2*24*60*60, time(NULL) + 5*24*60*60);
    
    // Complete reservation and generate Invoice
    Invoice invoice = system->completeReservation(reservation);

    // Make payment and complete the rental process
    system->makePayment(invoice);

    return 0;
}

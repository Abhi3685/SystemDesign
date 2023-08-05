#include <bits/stdc++.h>
using namespace std;

/* =========================================================== */
/* ==================== Parking Lot Design =================== */
/* =========================================================== */

/* Different types of parking spots are available for different vehicle types */
enum ParkingSpotType
{
    SMALL,
    MEDIUM,
    LARGE,
    XLARGE
};

/* Vehicle types that are allowed in the parking lot */
enum VehicleType {
    CAR,
    MotorBike,
    TRUCK,
    BUS,
    VAN
};

/* Vehicle class to have basic vehicle information.
We can have a factory design pattern to create different type of vehicles. */
class Vehicle
{
    string licensePlateNumber;
    VehicleType vehicleType;

public:
    Vehicle(string licensePlateNumber, VehicleType vehicleType) {
        this->licensePlateNumber = licensePlateNumber;
        this->vehicleType = vehicleType;
    }
    
    VehicleType getType() {
        return vehicleType;
    }
};

/* Parking Spot is a basic entity that contains properties like whether the spot is available
    or not and the spot type to identify what kind of vehicles can be parked in that spot */
class ParkingSpot
{
    int spotId;
    int floorId;
    ParkingSpotType spotType;
    bool isAvailable;

public:
    ParkingSpot(int floorId, int spotId, ParkingSpotType spotType)
    {
        this->floorId = floorId;
        this->spotId = spotId;
        this->spotType = spotType;
        isAvailable = true;
    }
    
    void setAvailability(bool isAvailable) {
        this->isAvailable = isAvailable;
    }

    int getSpotId() {
        return spotId;
    }
    
    int getFloorId() {
        return floorId;
    }
    
    ParkingSpotType getSpotType() {
        return spotType;
    }
};

/* Parking Ticket is a composition entity that maps vehicle to a parking spot
    and is issued at the entry panel */
class ParkingTicket
{
    int ticketNumber;
    int issuedAt;
    int charges;
    ParkingSpot *allocatedSpot;
    Vehicle *vehicle;

public:
    void setTicketNumber(int ticketNumber) {
        this->ticketNumber = ticketNumber;
    }
    
    void setIssuedAt(int time) {
        this->issuedAt = time;
    }
    
    void setAllocatedSpot(ParkingSpot *parkingSpot) {
        this->allocatedSpot = parkingSpot;
    }

    void setVehicle(Vehicle *vehicle) {
        this->vehicle = vehicle;
    }

    void setCharges(int charges) {
        this->charges = charges;
    }
    
    ParkingSpot* getAllocatedSpot() {
        return allocatedSpot;
    }
    
    Vehicle* getVehicle() {
        return vehicle;
    }
    
    int getIssuedAt() {
        return issuedAt;
    }
    
    int getCharges() {
        return charges;
    }
    
    int getTicketNumber() {
        return ticketNumber;
    }
};

class EntrancePanel;
class ExitPanel;
class ParkingFloor;

/* The strategy with which the vehicles are assigned a parking spot can vary
Say, on weekends we deploy strategy that allots spots near to the elevator or
on weekdays we allot slots near to entry / exit gates. So we use Strategy Pattern here. */
class ParkingStrategy
{
public:
    virtual void addParkingSpot(int floorId, ParkingSpotType spotType) = 0;

    virtual ParkingSpot* getParkingSpot(ParkingSpotType spotType) = 0;
    virtual void vacateParkingSpot(ParkingSpot *parkingSpot) = 0;
    virtual bool isParkingSpotAvailable(ParkingSpotType spotType) = 0;
};

class Compare {
public:
    bool operator()(ParkingSpot *p1, ParkingSpot *p2)
    {
        return p1->getSpotId() > p2->getSpotId();
    }
};

/* Allots first available slot on the basis of slotNumber preferably on the lowest floor possible */
class NormalParkingStrategy: public ParkingStrategy
{
    std::map<int, std::map<ParkingSpotType,
        priority_queue<ParkingSpot*, std::vector<ParkingSpot*>, Compare>>> slots; // floor -> (spotType -> available slots mapping)

public:
    void addParkingSpot(int floorId, ParkingSpotType spotType) {
        ParkingSpot* parkingSpot = new ParkingSpot(floorId, time(NULL), spotType);
        slots[floorId][spotType].push(parkingSpot);
    }

    ParkingSpot* getParkingSpot(ParkingSpotType spotType) {
        for (auto floor : slots)
        {
            if (floor.second[spotType].size() > 0) {
                ParkingSpot* parkingSpot = floor.second[spotType].top();
                slots[floor.first][spotType].pop();

                parkingSpot->setAvailability(false);

                return parkingSpot;
            }
        }
        
        return NULL; // no available slots
    }

    void vacateParkingSpot(ParkingSpot *parkingSpot) {
        parkingSpot->setAvailability(true);
        slots[parkingSpot->getFloorId()][parkingSpot->getSpotType()].push(parkingSpot);
    }

    bool isParkingSpotAvailable(ParkingSpotType spotType)
    {
        for (auto floor: slots)
            if (floor.second[spotType].size() > 0)
                return true;

        return false;
    }
};

/* Helper method to get parking spot type from given vehicle type */
ParkingSpotType getSpotTypeFromVehicleType(VehicleType vehicleType)
{
    ParkingSpotType spotType = ParkingSpotType::SMALL;
        
    switch (vehicleType)
    {
        case VehicleType::MotorBike:
            spotType = ParkingSpotType::SMALL;
            break;
        case VehicleType::CAR:
            spotType = ParkingSpotType::MEDIUM;
            break;
        case VehicleType::TRUCK:
            spotType = ParkingSpotType::LARGE;
            break;
        default:
            spotType = ParkingSpotType::XLARGE;
            break;
    }
    
    return spotType;
}

/* Main entity for this problem statement. We are using singleton design pattern to have support of only one instance throughout the deployment.
If we want, we can alter the design to have multiple instances and would need to add corresponding functionality to handle mutiple parking lots */
class ParkingLot
{
    int id;
    string address;
    vector<ParkingFloor*> parkingFloors;
    vector<EntrancePanel*> entryPanels;
    vector<ExitPanel*> exitPanels;

    ParkingLot(int id) {
        this->id = id;
    }

    ParkingLot(ParkingLot &a) {};
    static ParkingLot* instance;

public:
    NormalParkingStrategy parkingStrategy;

    static ParkingLot* getInstance();

    void setAddress(string address) {
        this->address = address;
    }

    int addParkingFloor(ParkingFloor *floor) {
        parkingFloors.push_back(floor);
        return 0;
    }

    int addEntryPanel(EntrancePanel *entryPanel) {
        entryPanels.push_back(entryPanel);
        return 0;
    } 

    int addExitPanel(ExitPanel *exitPanel) {
        exitPanels.push_back(exitPanel);
        return 0;
    }

    bool canPark(VehicleType vehicleType) {
        return parkingStrategy.isParkingSpotAvailable(getSpotTypeFromVehicleType(vehicleType));
    }
};

ParkingLot* ParkingLot::instance = NULL;

ParkingLot* ParkingLot::getInstance()
{
    if (instance == NULL)
        instance = new ParkingLot(1);

    return instance;
}

/* Entrance Panel is where user gets the parking ticket and spot gets allocated */
class EntrancePanel
{
    int id;

public:
    EntrancePanel(int id) {
        this->id = id;
    }

    ParkingTicket* getParkingTicket(Vehicle *vehicle) {
        if (!ParkingLot::getInstance()->canPark(vehicle->getType()))
            return NULL;
        
        ParkingSpotType spotType = getSpotTypeFromVehicleType(vehicle->getType());
        ParkingSpot* parkingSpot = ParkingLot::getInstance()->parkingStrategy.getParkingSpot(spotType);
        if (parkingSpot == NULL) return NULL;
        
        /* Builder design pattern used when constructing Parking Ticket */
        ParkingTicket *parkingTicket = new ParkingTicket();
        parkingTicket->setIssuedAt(time(NULL));
        parkingTicket->setAllocatedSpot(parkingSpot);
        parkingTicket->setVehicle(vehicle);
        parkingTicket->setTicketNumber(time(NULL));
        return parkingTicket;
    }
};

/* Exit Panel scans the parking ticket, vacate the spot and calculates the parking charges */
class ExitPanel
{
    int id;
    std::unordered_map<ParkingSpotType, int> hourlyCosts;

    int calculateCost(ParkingTicket *parkingTicket) {
        int duration = time(NULL) - parkingTicket->getIssuedAt();
        
        int hours = duration / (60 * 60);
        if (hours == 0) hours = 1;
        
        return hours * hourlyCosts[parkingTicket->getAllocatedSpot()->getSpotType()];
    }

public:
    ExitPanel(int id)
    {
        this->id = id;
        hourlyCosts.insert({ParkingSpotType::SMALL, 10});
        hourlyCosts.insert({ParkingSpotType::MEDIUM, 20});
        hourlyCosts.insert({ParkingSpotType::LARGE, 30});
        hourlyCosts.insert({ParkingSpotType::XLARGE, 50});
    }

    ParkingTicket* scanAndVacate(ParkingTicket *parkingTicket) {
        parkingTicket->setCharges(calculateCost(parkingTicket));
        ParkingLot::getInstance()->parkingStrategy.vacateParkingSpot(parkingTicket->getAllocatedSpot());
        return parkingTicket;
    }
};

/* Floor keeps track of slots that are on the particular floor.
We don't need parking strategy here as the it seems more logical to be part of the parking lot. */
class ParkingFloor
{
    int floorId;
    std::unordered_map<int, ParkingSpot*> occupiedSlots; // slotNumber to ParkingSpot mapping for occupied spots

public:
    ParkingFloor(int id) {
        this->floorId = id;
    }

    void markParkingSpot(ParkingSpot* parkingSpot) {
        occupiedSlots[parkingSpot->getSpotId()] = parkingSpot;
    }

    void vacateParkingSpot(int spotId) {
        occupiedSlots.erase(spotId);
    }
};

/* Payment processor is used to pay parking charges. It can also have different payment strategies
like Credit Card, Debit Card, UPI, ... */
class Payment
{
    int id;
    int ticketId;
    int amount;
    int initiatedAt;
    int completedAt;

public:
    Payment(int id, int ticketId, int amount) {
        this->id = id;
        this->ticketId = ticketId;
        this->amount = amount;
    }

    void makePayment() {
        this->initiatedAt = time(NULL);
        cout << "Payment made successfully of Rs. " << amount << endl;
        this->completedAt = time(NULL);
    }
};

/* Driver function */
int main()
{
    ParkingLot* parkingLot = ParkingLot::getInstance();
    parkingLot->setAddress("Parking Lot, Phoenix Mall, Bengaluru, Karnataka");

    // should be able to add a parking floor
    parkingLot->addParkingFloor(new ParkingFloor(1));
    parkingLot->addParkingFloor(new ParkingFloor(2));

    // should be able to add entrance panel
    EntrancePanel *entrance = new EntrancePanel(1);
    parkingLot->addEntryPanel(entrance);

    // should be able to add exit panel
    ExitPanel *exitPanel = new ExitPanel(1);
    parkingLot->addExitPanel(exitPanel);

    int floorId = 1;
    // should be able to add parking spot of different types
    parkingLot->parkingStrategy.addParkingSpot(floorId, ParkingSpotType::SMALL);
    parkingLot->parkingStrategy.addParkingSpot(floorId, ParkingSpotType::LARGE);
    parkingLot->parkingStrategy.addParkingSpot(floorId, ParkingSpotType::MEDIUM);

    // check for availability of parking lot - TRUE
    cout << ParkingLot::getInstance()->canPark(VehicleType::CAR) << endl;
    // check for availability of parking lot - FALSE
    cout << ParkingLot::getInstance()->canPark(VehicleType::VAN) << endl;

    Vehicle *vehicle = new Vehicle("KA05MR2311", VehicleType::CAR);
    Vehicle *van = new Vehicle("KA01MR7804", VehicleType::VAN);
    
    // Should be able to get parking ticket
    ParkingTicket *parkingTicket = entrance->getParkingTicket(vehicle);
    cout << parkingTicket->getAllocatedSpot()->getSpotId() << endl;

    parkingLot->parkingStrategy.addParkingSpot(floorId, ParkingSpotType::MEDIUM);

    // Should be able to get parking ticket
    Vehicle *car = new Vehicle("KA02MR6355", VehicleType::CAR);
    ParkingTicket *parkingTicket1 = entrance->getParkingTicket(car);
    cout << parkingTicket1->getAllocatedSpot()->getSpotId() << endl;

    // Should not be able to get ticket
    ParkingTicket *tkt = entrance->getParkingTicket(new Vehicle("ka04rb8458", VehicleType::CAR));
    cout << "Parking Ticket should be NULL and the assertion is " << (NULL == tkt) << endl;

    // Should be able to get ticket
    ParkingTicket *mtrTkt = entrance->getParkingTicket(new Vehicle("ka01ee4901", VehicleType::MotorBike));
    cout << mtrTkt->getAllocatedSpot()->getSpotId() << endl;

    // vacate parking spot
    mtrTkt = exitPanel->scanAndVacate(mtrTkt);
    cout << mtrTkt->getCharges() << endl;

    // park on vacated spot
    ParkingTicket *mtrTkt1 = entrance->getParkingTicket(new Vehicle("ka01ee7791", VehicleType::MotorBike));
    cout << mtrTkt->getAllocatedSpot()->getSpotId() << endl;

    // park when spot is not available
    ParkingTicket *unavailableTkt = entrance->getParkingTicket(new Vehicle("ka01ee4455", VehicleType::MotorBike));
    cout << "Parking Ticket should be NULL and the assertion is " << (NULL == unavailableTkt) << endl;

    // vacate car
    parkingTicket = exitPanel->scanAndVacate(parkingTicket);
    cout << parkingTicket->getCharges() << endl;

    // Now should be able to park car
    cout << ParkingLot::getInstance()->canPark(VehicleType::CAR) << endl;

    // Should be able to vacate parked vehicle
    parkingTicket1 = exitPanel->scanAndVacate(parkingTicket1);
    cout << parkingTicket1->getCharges() << endl;

    // Payment
    Payment payment(time(NULL), parkingTicket1->getTicketNumber(), parkingTicket1->getCharges());
    payment.makePayment();

    // vacate motorbike spot
    mtrTkt = exitPanel->scanAndVacate(mtrTkt);
    cout << mtrTkt->getCharges() << endl;

    return 0;
}

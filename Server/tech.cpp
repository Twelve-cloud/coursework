#include "streamtable.h"
#include "tech.h"
#include "sysfunction.h"
#include <string>
#include <fstream>
#include <cstdint>
#include <ctime>

extern StreamTable kt;

// Статические элементы класса

tm* Tech::rctime = nullptr;
uint64_t Tech::count = 0;
Tech::~Tech() {}

std::string Tech::setCurrentDate()
{
    const time_t timer = time(NULL);
    rctime = localtime(&timer);
    char tmp[64];
    strftime(tmp, 64, "%d.%m.%Y %H:%M:%S, %A", rctime);

    return static_cast<std::string>(tmp);
}

// !Статические элементы класса

// Виртуальные функции класса

std::istream& Tech::setState(std::istream& in)
{
    m_recordtime = setCurrentDate();
    std::cout << "Enter serial number: ";       std::getline(std::cin, m_serialnumber);
    std::cout << "Enter manufacturer: ";        std::getline(std::cin, m_manufacturer);
    std::cout << "Enter release date: ";        std::getline(std::cin, m_releasedate);
    std::cout << "Enter model: ";               std::getline(std::cin, m_model);
    std::cout << "Enter vendor code: ";         std::getline(std::cin, m_vendor);
    std::cout << "Enter country of making: ";   std::getline(std::cin, m_countrymaker);
    getnumber(m_price, "Enter price: ");

    return in;
}

std::ostream& Tech::print(std::ostream& out) const
{
    kt << "ID"  << m_ID;
    kt << "Type" << m_type;
    kt << "Record time" << m_recordtime;
    kt << "Serial Number" << m_serialnumber;
    kt << "Manufacturer" << m_manufacturer;
    kt << "Release date" << m_releasedate;
    kt << "Model" << m_model;
    kt << "Vendor code" << m_vendor;
    kt << "Country maker" << m_countrymaker;
    kt << "Price" << m_price;

    return out;
}

void Tech::writeToFile(std::fstream& file)
{
    file << m_ID << std::endl;
    file << m_type << std::endl;
    file << m_recordtime << std::endl;
    file << m_serialnumber << std::endl;
    file << m_manufacturer << std::endl;
    file << m_releasedate << std::endl;
    file << m_model << std::endl;
    file << m_vendor << std::endl;
    file << m_countrymaker << std::endl;
    file << m_price << std::endl;
}

void Tech::readFile(std::fstream& file, Tech* obj)
{
    std::getline(file, obj -> getRecordTime());
    file >> obj -> getSerialNumber();
    file >> obj -> getManufacturer();
    file >> obj -> getReleaseData();
    file >> obj -> getModel();
    file >> obj -> getVendor();
    file >> obj -> getCountrymaker();
    file >> obj -> getPrice();
}

// !Виртуальные функции класса

// Дружественные функции и методы класса

std::ostream& operator<<(std::ostream& out, const Tech& object)
{
    return object.print(out);
}

std::istream& operator>>(std::istream& in, Tech& object)
{
    return object.setState(in);
}

// !Дружественные функции и методы класса


// Методы дочерних классов

std::istream& Computer::setState(std::istream& in)
{
    Tech::setState(in);
    std::cout << "Enter name of processor: ";
    std::getline(std::cin, m_processor);
    getnumber(m_core, "Enter amount of cores: ");
    std::cout << "Enter RAM type: ";
    std::getline(std::cin, m_ramtype);
    getnumber(m_ramsize, "Enter size of RAM: ");
    std::cout << "Enter resolution of screen: ";
    std::getline(std::cin, m_screenresol);
    getnumber(m_screendiagonal, "Enter screen diagonal: ");

    return in;
}

std::ostream& Computer::print(std::ostream& out) const
{
    Tech::print(out);
    kt << "Processor" << m_processor;
    kt << "Cores" << m_core;
    kt << "Type of RAM" << m_ramtype;
    kt << "RAM size" << m_ramsize;
    kt << "Resolution of the screen" << m_screenresol;
    kt << "Diagonal of the screen" << m_screendiagonal;

    return out;
}

void Computer::writeToFile(std::fstream& file)
{
    Tech::writeToFile(file);

    file << m_processor << std::endl;
    file << m_core << std::endl;
    file << m_ramtype << std::endl;
    file << m_ramsize << std::endl;
    file << m_screenresol << std::endl;
    file << m_screendiagonal << std::endl;
}

void Computer::readFile(std::fstream& file, Tech* obj)
{
    Tech::readFile(file, obj);

    file >> dynamic_cast<Computer*>(obj) -> getProcessor();
    file >> dynamic_cast<Computer*>(obj) -> getRamtype();
    file >> dynamic_cast<Computer*>(obj) -> getScreenResol();
    file >> dynamic_cast<Computer*>(obj) -> getRamSize();
    file >> dynamic_cast<Computer*>(obj) -> getCore();
    file >> dynamic_cast<Computer*>(obj) -> getScreenDiagonal();
}

std::istream& MobilePhone::setState(std::istream& in)
{
    Tech::setState(in);

    std::cout << "Enter name of OS: ";
    std::getline(std::cin, m_os);
    std::cout << "Enter resolution of screen: ";
    std::getline(std::cin, m_screenresol);
    getnumber(m_screendiagonal, "Enter screen diagonal: ");
    std::cout << "Enter name of processor: ";
    std::getline(std::cin, m_processor);
    getnumber(m_core, "Enter amount of cores: ");
    getnumber(m_ramsize, "Enter size of RAM: ");
    getnumber(m_simcount, "Enter amount of sim cards: ");


    return in;
}

std::ostream& MobilePhone::print(std::ostream& out) const
{
    Tech::print(out);
    kt << "Operating System" << m_os;
    kt << "Resolution of the screen" << m_screenresol;
    kt << "Screen diagonal" << m_screendiagonal;
    kt << "Processor" << m_processor;
    kt << "Cores" << m_core;
    kt << "RAM size" << m_ramsize;
    kt << "Amount of the sim" << m_simcount;

    return out;
}

void MobilePhone::writeToFile(std::fstream& file)
{
    Tech::writeToFile(file);

    file << m_os << std::endl;
    file << m_screenresol << std::endl;
    file << m_screendiagonal << std::endl;
    file << m_processor << std::endl;
    file << m_core << std::endl;
    file << m_ramsize << std::endl;
    file << m_simcount << std::endl;
}

void MobilePhone::readFile(std::fstream& file, Tech* obj)
{
    Tech::readFile(file, obj);

    file >> dynamic_cast<MobilePhone*>(obj) -> getOS();
    file >> dynamic_cast<MobilePhone*>(obj) -> getScreenResol();
    file >> dynamic_cast<MobilePhone*>(obj) -> getScreenDiagonal();
    file >> dynamic_cast<MobilePhone*>(obj) -> getProcessor();
    file >> dynamic_cast<MobilePhone*>(obj) -> getCore();
    file >> dynamic_cast<MobilePhone*>(obj) -> getRamSize();
    file >> dynamic_cast<MobilePhone*>(obj) -> getSimCount();
}

std::istream& TV::setState(std::istream& in)
{
    Tech::setState(in);
    std::cout << "Enter type of screen: ";
    std::getline(std::cin, m_typescreen);
    std::cout << "Enter resolution of screen: ";
    std::getline(std::cin, m_screenresolution);
    getnumber(m_screendiagonal, "Enter screen diagonal: ");
    std::cout << "Enter name of processor: ";
    std::getline(std::cin, m_processor);
    getnumber(m_core, "Enter amount of cores: ");
    std::cout << "3D mode? Y - yes, Any - no: ";
    m_3Dmode = getchar() == 'Y' ? true : false; getchar();

    return in;
}

std::ostream& TV::print(std::ostream& out) const
{
    Tech::print(out);

    std::cout << std::boolalpha;
    kt << "Type of screen" << m_typescreen;
    kt << "Resolution of the screen" << m_screenresolution;
    kt << "Diagonal of the screen" << m_screendiagonal;
    kt << "Processor" << m_processor;
    kt << "Cores" << m_core;
    kt << "3D" << m_3Dmode;

    return out;
}

void TV::writeToFile(std::fstream& file)
{
    Tech::writeToFile(file);

    file << m_typescreen << std::endl;
    file << m_screenresolution << std::endl;
    file << m_screendiagonal << std::endl;
    file << m_processor << std::endl;
    file << m_core << std::endl;
    file << m_3Dmode << std::endl;
}

void TV::readFile(std::fstream& file, Tech* obj)
{
    Tech::readFile(file, obj);

    file >> dynamic_cast<TV*>(obj) -> getTypescreen();
    file >> dynamic_cast<TV*>(obj) -> getScreenResol();
    file >> dynamic_cast<TV*>(obj) -> getScreenDiagonal();
    file >> dynamic_cast<TV*>(obj) -> getProcessor();
    file >> dynamic_cast<TV*>(obj) -> getCore();
    file >> dynamic_cast<TV*>(obj) -> get3DMode();
}

std::istream& Toaster::setState(std::istream& in)
{
    Tech::setState(in);
    getnumber(m_toastcount, "Enter amount of toasts: ");
    getnumber(m_power, "Enter power of toaster: ");
    std::cout << "Defrosting mode? Y - yes, Any - no: ";
    m_defrostring = getchar() == 'Y' ? true : false; getchar();
    std::cout << "Heating mode? Y - yes, Any - no: ";
    m_heating = getchar() == 'Y' ? true : false; getchar();

    return in;
}

std::ostream& Toaster::print(std::ostream& out) const
{
    Tech::print(out);

    std::cout << std::boolalpha;
    kt << "Amount of toasts" << m_toastcount;
    kt << "Power" << m_power;
    kt << "Defrosting mode" << m_defrostring;
    kt << "Heating mode" << m_heating;

    return out;
}

void Toaster::writeToFile(std::fstream& file)
{
    Tech::writeToFile(file);

    file << m_toastcount << std::endl;
    file << m_power << std::endl;
    file << m_defrostring << std::endl;
    file << m_heating << std::endl;
}

void Toaster::readFile(std::fstream& file, Tech* obj)
{
    Tech::readFile(file, obj);

    file >> dynamic_cast<Toaster*>(obj) -> getToastCount();
    file >> dynamic_cast<Toaster*>(obj) -> getPower();
    file >> dynamic_cast<Toaster*>(obj) -> getDefrosting();
    file >> dynamic_cast<Toaster*>(obj) -> getHeating();
}

std::istream& CoffeMaker::setState(std::istream& in)
{
    Tech::setState(in);
    getnumber(m_power, "Enter power: ");
    getnumber(m_pressure, "Enter pressure: ");
    std::cout << "Cappuccino maker? Y - yes, Any - no: ";
    m_cappuccinomaker = getchar() == 'Y' ? true : false; getchar();

    return in;
}

std::ostream& CoffeMaker::print(std::ostream& out) const
{
    Tech::print(out);

    std::cout << std::boolalpha;
    kt << "Power" << m_power;
    kt << "Pressure" << m_pressure;
    kt << "Cappuccino maker" << m_cappuccinomaker;

    return out;
}

void CoffeMaker::writeToFile(std::fstream& file)
{
    Tech::writeToFile(file);

    file << m_power << std::endl;
    file << m_pressure << std::endl;
    file << m_cappuccinomaker << std::endl;
}

void CoffeMaker::readFile(std::fstream& file, Tech* obj)
{
    Tech::readFile(file, obj);

    file >> dynamic_cast<CoffeMaker*>(obj) -> getPower();
    file >> dynamic_cast<CoffeMaker*>(obj) -> getPressure();
    file >> dynamic_cast<CoffeMaker*>(obj) -> getCappuccinoMaker();
}

std::istream& ElKettle::setState(std::istream& in)
{
    Tech::setState(in);
    getnumber(m_power, "Enter power: ");
    getnumber(m_volume, "Enter volume: ");
    std::cout << "Timer? Y - yes, Any - no: ";
    m_timer = getchar() == 'Y' ? true : false; getchar();

    return in;
}

std::ostream& ElKettle::print(std::ostream& out) const
{
    Tech::print(out);

    std::cout << std::boolalpha;
    kt << "Power" << m_power;
    kt << "Volume" << m_volume;
    kt << "Timer" << m_timer;

    return out;
}

void ElKettle::writeToFile(std::fstream& file)
{
    Tech::writeToFile(file);

    file << m_power << std::endl;
    file << m_volume << std::endl;
    file << m_timer << std::endl;
}

void ElKettle::readFile(std::fstream& file, Tech* obj)
{
    Tech::readFile(file, obj);

    file >> dynamic_cast<ElKettle*>(obj) -> getPower();
    file >> dynamic_cast<ElKettle*>(obj) -> getVolume();
    file >> dynamic_cast<ElKettle*>(obj) -> getTimer();
}

std::istream& Fridge::setState(std::istream& in)
{
    Tech::setState(in);
    getnumber(m_volume, "Enter volume: ");
    getnumber(m_shelfcount, "Enter amount of shelfs: ");
    getnumber(m_noiselvl, "Enter lvl of noise: ");
    std::cout << "Multizone? Y - yes, Any - no: ";
    m_multizone = getchar() == 'Y' ? true : false; getchar();

    return in;
}

std::ostream& Fridge::print(std::ostream& out) const
{
    Tech::print(out);

    std::cout << std::boolalpha;
    kt << "Volume" << m_volume;
    kt << "Amount of shelf" << m_shelfcount;
    kt << "Lvl of noise" << m_noiselvl;
    kt << "Multizone" << m_multizone;

    return out;
}

void Fridge::writeToFile(std::fstream& file)
{
    Tech::writeToFile(file);

    file << m_volume << std::endl;
    file << m_shelfcount << std::endl;
    file << m_noiselvl << std::endl;
    file << m_multizone << std::endl;
}

void Fridge::readFile(std::fstream& file, Tech* obj)
{
    Tech::readFile(file, obj);

    file >> dynamic_cast<Fridge*>(obj) -> getVolume();
    file >> dynamic_cast<Fridge*>(obj) -> getShelfCount();
    file >> dynamic_cast<Fridge*>(obj) -> getNoiseLvl();
    file >> dynamic_cast<Fridge*>(obj) -> getMultizone();
}

std::istream& Conditioner::setState(std::istream& in)
{
    Tech::setState(in);
    std::cout << "Enter working mode: ";
    std::getline(std::cin, m_workmode);
    getnumber(m_coolingpower, "Enter cooling power: ");
    getnumber(m_heatingpower, "Enter heating power: ");
    std::cout << "Remote control? Y - yes, Any - no: ";
    m_remotecontol = getchar() == 'Y' ? true : false; getchar();

    return in;
}

std::ostream& Conditioner::print(std::ostream& out) const
{
    Tech::print(out);

    std::cout << std::boolalpha;
    kt << "Working mode" << m_workmode;
    kt << "Cooling power" << m_coolingpower;
    kt << "Heating power" << m_heatingpower;
    kt << "Remote control" << m_remotecontol;

    return out;
}

void Conditioner::writeToFile(std::fstream& file)
{
    Tech::writeToFile(file);

    file << m_workmode << std::endl;
    file << m_coolingpower << std::endl;
    file << m_heatingpower << std::endl;
    file << m_remotecontol << std::endl;
}

void Conditioner::readFile(std::fstream& file, Tech* obj)
{
    Tech::readFile(file, obj);

    file >> dynamic_cast<Conditioner*>(obj) -> getWorkmode();
    file >> dynamic_cast<Conditioner*>(obj) -> getCoolingPower();
    file >> dynamic_cast<Conditioner*>(obj) -> getHeathingPower();
    file >> dynamic_cast<Conditioner*>(obj) -> getRemouteControl();
}

std::istream& Microwawe::setState(std::istream& in)
{
    Tech::setState(in);
    getnumber(m_power, "Enter power: ");
    getnumber(m_lvlpower_count, "Enter amount of lvls of power: ");
    getnumber(m_volume, "Enter volume: ");
    std::cout << "Quickstart? Y - yes, Any - no: ";
    m_quickstart = getchar() == 'Y' ? true : false; getchar();
    std::cout << "Timer? Y - yes, Any - no: ";
    m_timer = getchar() == 'Y' ? true : false; getchar();

    return in;
}

std::ostream& Microwawe::print(std::ostream& out) const
{
    Tech::print(out);

    std::cout << std::boolalpha;
    kt << "Power" << m_power;
    kt << "Lvls of power" << m_lvlpower_count;
    kt << "Volume" << m_volume;
    kt << "Quckstart" << m_quickstart;
    kt << "Timer" << m_timer;

    return out;
}

void Microwawe::writeToFile(std::fstream& file)
{
    Tech::writeToFile(file);

    file << m_power << std::endl;
    file << m_lvlpower_count << std::endl;
    file << m_volume << std::endl;
    file << m_quickstart << std::endl;
    file << m_timer << std::endl;
}

void Microwawe::readFile(std::fstream& file, Tech* obj)
{
    Tech::readFile(file, obj);

    file >> dynamic_cast<Microwawe*>(obj) -> getPower();
    file >> dynamic_cast<Microwawe*>(obj) -> getLvlPower();
    file >> dynamic_cast<Microwawe*>(obj) -> getVolume();
    file >> dynamic_cast<Microwawe*>(obj) -> getQuickstart();
    file >> dynamic_cast<Microwawe*>(obj) -> getTimer();
}

// !Методы дочерних классов

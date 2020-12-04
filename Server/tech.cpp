#include "streamtable.h"
#include "tech.h"
#include "sysfunction.h"
#include <string>
#include <fstream>
#include <cstdint>
#include <sstream>
#include <ctime>

extern StreamTable kt;
std::string getWords(int count, std::string str, ...);

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
    m_recordtime.erase(m_recordtime.find(","), 1);
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
    std::getline(file, obj -> getSerialNumber());
    std::getline(file, obj -> getManufacturer());
    std::getline(file, obj -> getReleaseData());
    std::getline(file, obj -> getModel());
    std::getline(file, obj -> getVendor());
    std::getline(file, obj -> getCountrymaker());
    file >> obj -> getPrice();
}

void Tech::getStringToSend(std::string& stringToSend)
{
    std::stringstream temp;

    std::replace(m_serialnumber.begin(), m_serialnumber.end(), ' ', '|');
    std::replace(m_manufacturer.begin(), m_manufacturer.end(), ' ', '|');
    std::replace(m_releasedate.begin(), m_releasedate.end(), ' ', '|');
    std::replace(m_model.begin(), m_model.end(), ' ', '|');
    std::replace(m_vendor.begin(), m_vendor.end(), ' ', '|');
    std::replace(m_countrymaker.begin(), m_countrymaker.end(), ' ', '|');

    temp << m_ID << " " << m_type << " "
         << m_recordtime << " " << m_serialnumber << " "
         << m_manufacturer << " " << m_releasedate << " "
         << m_model << " " << m_vendor << " "
         << m_countrymaker << " " << m_price << " ";
    stringToSend = temp.str();
}

bool Tech::replaceObject(std::string& string)
{
    char serialnumber[64], manufacturer[64], releasedate[64], model[64], vendor[64], countrymaker[64], price[64];
    string = getWords(8, string, serialnumber, manufacturer, releasedate, model, vendor,countrymaker, price);
    if (atoi(price) == 0)
        return false;

    m_recordtime = setCurrentDate();
    m_recordtime.erase(m_recordtime.find(","), 1);

    m_serialnumber = serialnumber;
    m_manufacturer = manufacturer;
    m_releasedate = releasedate;
    m_model = model;
    m_vendor = vendor;
    m_countrymaker = countrymaker;
    m_price = atoi(price);
    return true;
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
    file << m_ramtype << std::endl;
    file << m_screenresol << std::endl;
    file << m_ramsize << std::endl;
    file << m_core << std::endl;
    file << m_screendiagonal << std::endl;
}

void Computer::readFile(std::fstream& file, Tech* obj)
{
    Tech::readFile(file, obj);

    std::getline(file, dynamic_cast<Computer*>(obj) -> getProcessor());
    std::getline(file, dynamic_cast<Computer*>(obj) -> getProcessor());
    std::getline(file, dynamic_cast<Computer*>(obj) -> getRamtype());
    std::getline(file, dynamic_cast<Computer*>(obj) -> getScreenResol());
    file >> dynamic_cast<Computer*>(obj) -> getRamSize();
    file >> dynamic_cast<Computer*>(obj) -> getCore();
    file >> dynamic_cast<Computer*>(obj) -> getScreenDiagonal();
}

void Computer::getStringToSend(std::string& stringToSend)
{
    Tech::getStringToSend(stringToSend);
    std::stringstream temp;

    std::replace(m_processor.begin(), m_processor.end(), ' ', '|');
    std::replace(m_ramtype.begin(), m_ramtype.end(), ' ', '|');
    std::replace(m_screenresol.begin(), m_screenresol.end(), ' ', '|');

    temp << m_processor << " " << m_core << " " << m_ramtype << " " << m_ramsize << " " << m_screenresol << " " << m_screendiagonal;
    stringToSend += temp.str();

}

bool Computer::replaceObject(std::string& string)
{
    if (Tech::replaceObject(string) == false)
        return false;

    char processor[64], core[64], ramtype[64], ramsize[64], screenresol[64], screendiag[64];
    string = getWords(7, string, processor, core, ramtype, ramsize, screenresol, screendiag);

    if (atoi(core) && atoi(ramsize) && atoi(screendiag) == 0)
        return false;

    m_processor = processor;
    m_ramtype = ramtype;
    m_screenresol = screenresol;
    m_core = atoi(core);
    m_ramsize = atoi(ramsize);
    m_screendiagonal = atoi(screendiag);

    return true;
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

    std::getline(file, dynamic_cast<MobilePhone*>(obj) -> getOS());
    std::getline(file, dynamic_cast<MobilePhone*>(obj) -> getOS());
    std::getline(file, dynamic_cast<MobilePhone*>(obj) -> getScreenResol());
    file >> dynamic_cast<MobilePhone*>(obj) -> getScreenDiagonal();
    std::getline(file, dynamic_cast<MobilePhone*>(obj) -> getProcessor());
    std::getline(file, dynamic_cast<MobilePhone*>(obj) -> getProcessor());
    file >> dynamic_cast<MobilePhone*>(obj) -> getCore();
    file >> dynamic_cast<MobilePhone*>(obj) -> getRamSize();
    file >> dynamic_cast<MobilePhone*>(obj) -> getSimCount();
}

void MobilePhone::getStringToSend(std::string& stringToSend)
{
    Tech::getStringToSend(stringToSend);
    std::stringstream temp;

    std::replace(m_os.begin(), m_os.end(), ' ', '|');
    std::replace(m_screenresol.begin(), m_screenresol.end(), ' ', '|');
    std::replace(m_processor.begin(), m_processor.end(), ' ', '|');

    temp << m_os << " " << m_screenresol << " " << m_screendiagonal << " " << m_processor << " " << m_core << " " << m_ramsize << " " << m_simcount;
    stringToSend += temp.str();
}

bool MobilePhone::replaceObject(std::string& string)
{
    if (Tech::replaceObject(string) == false)
        return false;

    char os[64], screenresol[64], processor[64], core[64], ramsize[64], screendiag[64], simcount[64];
    string = getWords(8, string, os, screenresol, screendiag, processor, core, ramsize, simcount);

    if (atoi(core) && atoi(ramsize) && atoi(screendiag) && atoi(simcount) == 0)
        return false;


    m_os = os;
    m_processor = processor;
    m_screenresol = screenresol;
    m_core = atoi(core);
    m_ramsize = atoi(ramsize);
    m_screendiagonal = atoi(screendiag);
    m_simcount = atoi(simcount);

    return true;
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

    std::getline(file, dynamic_cast<TV*>(obj) -> getTypescreen());
    std::getline(file, dynamic_cast<TV*>(obj) -> getTypescreen());
    std::getline(file, dynamic_cast<TV*>(obj) -> getScreenResol());
    file >> dynamic_cast<TV*>(obj) -> getScreenDiagonal();
    std::getline(file, dynamic_cast<TV*>(obj) -> getProcessor());
    std::getline(file, dynamic_cast<TV*>(obj) -> getProcessor());
    file >> dynamic_cast<TV*>(obj) -> getCore();
    file >> dynamic_cast<TV*>(obj) -> get3DMode();
}

void TV::getStringToSend(std::string& stringToSend)
{
    Tech::getStringToSend(stringToSend);
    std::stringstream temp;

    std::replace(m_typescreen.begin(), m_typescreen.end(), ' ', '|');
    std::replace(m_screenresolution.begin(), m_screenresolution.end(), ' ', '|');
    std::replace(m_processor.begin(), m_processor.end(), ' ', '|');

    temp << m_typescreen << " " << m_screenresolution << " " << m_screendiagonal << " " << m_processor << " " << m_core << " " << m_3Dmode;
    stringToSend += temp.str();
}

bool TV::replaceObject(std::string& string)
{
    if (Tech::replaceObject(string) == false)
        return false;

    char core[64], _3Dmode[64];
    getWords(6, string, m_typescreen.c_str(), m_screenresolution.c_str(), m_processor.c_str(), core, _3Dmode);

    if (atoi(core))
        return false;
    m_core = atoi(core);
    m_3Dmode = atoi(_3Dmode);

    return true;
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

void Toaster::getStringToSend(std::string& stringToSend)
{
    Tech::getStringToSend(stringToSend);
    std::stringstream temp;

    temp << m_toastcount << " " << m_power << " " << m_defrostring << " " << m_heating;
    stringToSend += temp.str();
}

bool Toaster::replaceObject(std::string& string)
{
    if (Tech::replaceObject(string) == false)
        return false;

    char toastcount[64], power[64], defrosting[64], heating[64];
    getWords(5, string, toastcount, power, defrosting, heating);

    if (atoi(toastcount) && atoi(power) == 0)
        return false;

    m_toastcount = atoi(toastcount);
    m_power = atoi(power);
    m_defrostring = atoi(defrosting);
    m_heating = atoi(heating);

    return true;
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

void CoffeMaker::getStringToSend(std::string& stringToSend)
{
    Tech::getStringToSend(stringToSend);
    std::stringstream temp;
    temp << m_power << " " << m_pressure << " " << m_cappuccinomaker;
    stringToSend += temp.str();
}

bool CoffeMaker::replaceObject(std::string& string)
{
    if (Tech::replaceObject(string) == false)
        return false;

    char power[64], pressure[64], cappuchinomaker[64];
    getWords(4, string, power, pressure, cappuchinomaker);

    if (atoi(power) && atoi(pressure) == 0)
        return false;

    m_power = atoi(power);
    m_pressure = atoi(pressure);
    m_cappuccinomaker = atoi(cappuchinomaker);

    return true;
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

void ElKettle::getStringToSend(std::string& stringToSend)
{
    Tech::getStringToSend(stringToSend);
    std::stringstream temp;
    temp << m_power << " " << m_volume << " " << m_timer;
    stringToSend += temp.str();
}

bool ElKettle::replaceObject(std::string& string)
{
    if (Tech::replaceObject(string) == false)
        return false;

    char power[64], volume[64], timer[64];
    getWords(4, string, power, volume, timer);

    if (atoi(power) && atoi(volume) == 0)
        return false;

    m_power = atoi(power);
    m_volume = atoi(volume);
    m_timer = atoi(timer);

    return true;
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

void Fridge::getStringToSend(std::string& stringToSend)
{
    Tech::getStringToSend(stringToSend);
    std::stringstream temp;
    temp << m_volume << " " << m_shelfcount << " " << m_noiselvl << " " << m_multizone;
    stringToSend += temp.str();
}

bool Fridge::replaceObject(std::string& string)
{
    if (Tech::replaceObject(string) == false)
        return false;

    char volume[64], shelfcount[64], noiselvl[64], multizone[64];
    getWords(5, string, volume, shelfcount, noiselvl, multizone);

    if (atoi(volume) && atoi(shelfcount) && atoi(noiselvl) == 0)
        return false;

    m_volume = atoi(volume);
    m_shelfcount = atoi(shelfcount);
    m_noiselvl = atoi(noiselvl);
    m_multizone = atoi(multizone);

    return true;
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

    std::getline(file, dynamic_cast<Conditioner*>(obj) -> getWorkmode());
    std::getline(file, dynamic_cast<Conditioner*>(obj) -> getWorkmode());
    file >> dynamic_cast<Conditioner*>(obj) -> getCoolingPower();
    file >> dynamic_cast<Conditioner*>(obj) -> getHeathingPower();
    file >> dynamic_cast<Conditioner*>(obj) -> getRemouteControl();
}

void Conditioner::getStringToSend(std::string& stringToSend)
{
    Tech::getStringToSend(stringToSend);
    std::stringstream temp;

    std::replace(m_workmode.begin(), m_workmode.end(), ' ', '|');

    temp << m_workmode << " " << m_coolingpower << " " << m_heatingpower << " " << m_remotecontol;
    stringToSend += temp.str();

}

bool Conditioner::replaceObject(std::string& string)
{
    if (Tech::replaceObject(string) == false)
        return false;

    char workmode[64], coolingpower[64], heatingpower[64], remotecontol[64];
    getWords(5, string, workmode, coolingpower, heatingpower, remotecontol);

    if (atoi(coolingpower) && atoi(heatingpower) == 0)
        return false;

    m_workmode = workmode;
    m_coolingpower = atoi(coolingpower);
    m_heatingpower = atoi(heatingpower);
    m_remotecontol = atoi(remotecontol);

    return true;
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

void Microwawe::getStringToSend(std::string& stringToSend)
{
    Tech::getStringToSend(stringToSend);
    std::stringstream temp;
    temp << m_power << " " << m_lvlpower_count << " " << m_volume << " " << m_quickstart << " " << m_timer;
    stringToSend += temp.str();
}

bool Microwawe::replaceObject(std::string& string)
{
    if (Tech::replaceObject(string) == false)
        return false;

    char power[64], lvlpower_count[64], volume[64], quickstart[64], timer[64];
    getWords(6, string, power, lvlpower_count, volume, quickstart, timer);

    if (atoi(power) && atoi(lvlpower_count) && atoi(volume) == 0)
        return false;

    m_power = atoi(power);
    m_lvlpower_count = atoi(lvlpower_count);
    m_volume = atoi(volume);
    m_quickstart = atoi(quickstart);
    m_timer = atoi(timer);

    return true;
}

// !Методы дочерних классов

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
#define NRus(str) QString::fromLocal8Bit(str).toUtf8().data()

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
    std::cout << Rus("Введите серийный нормер: ");       std::getline(std::cin, m_serialnumber);
    std::cout << Rus("Введите производителя: ");         std::getline(std::cin, m_manufacturer);
    std::cout << Rus("Введите дату поступления: ");      std::getline(std::cin, m_releasedate);
    std::cout << Rus("Введите модель: ");                std::getline(std::cin, m_model);
    std::cout << Rus("Введите артикул: ");               std::getline(std::cin, m_vendor);
    std::cout << Rus("Введите страну изготовления: ");   std::getline(std::cin, m_countrymaker);
    getnumber(m_price, Rus("Введите цену: "));

    return in;
}

std::ostream& Tech::print(std::ostream& out) const
{
    kt << Rus("ИД")  << m_ID;
    kt << Rus("Тип") << Rus(m_type.c_str());
    kt << Rus("Время записи") << m_recordtime;
    kt << Rus("Серийный номер") << m_serialnumber;
    kt << Rus("Производитель") << m_manufacturer;
    kt << Rus("Дата поступления") << m_releasedate;
    kt << Rus("Модель") << m_model;
    kt << Rus("Артикул") << m_vendor;
    kt << Rus("Страна производства") << m_countrymaker;
    kt << Rus("Цена") << m_price;

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
         << NRus(m_recordtime.c_str()) << " " << NRus(m_serialnumber.c_str()) << " "
         << NRus(m_manufacturer.c_str()) << " " << NRus(m_releasedate.c_str()) << " "
         << NRus(m_model.c_str()) << " " << NRus(m_vendor.c_str()) << " "
         << NRus(m_countrymaker.c_str()) << " " << m_price << " ";
    stringToSend = temp.str();
}

bool Tech::replaceObject(std::string& string)
{
    char serialnumber[64], manufacturer[64], releasedate[64], model[64], vendor[64], countrymaker[64], price[64];
    string = getWords(8, string, serialnumber, manufacturer, releasedate, model, vendor,countrymaker, price);

    int32_t tmp = (atoi(price));
    if (tmp < 0) tmp = 0;
    m_price = tmp;

    m_recordtime = setCurrentDate().c_str();
    m_recordtime.erase(m_recordtime.find(","), 1);

    m_serialnumber = serialnumber;
    m_manufacturer = manufacturer;
    m_releasedate = releasedate;
    m_model = model;
    m_vendor = vendor;
    m_countrymaker = countrymaker;
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
    std::cout << Rus("Введите модель процессора: ");
    std::getline(std::cin, m_processor);
    getnumber(m_core, Rus("Введите количество ядер: "));
    std::cout << Rus("Введите тип ОП: ");
    std::getline(std::cin, m_ramtype);
    getnumber(m_ramsize, Rus("Введите размер ОП: "));
    std::cout << Rus("Введите разрешение экрана: ");
    std::getline(std::cin, m_screenresol);
    getnumber(m_screendiagonal, Rus("Введите диагональ экрана: "));

    return in;
}

std::ostream& Computer::print(std::ostream& out) const
{
    Tech::print(out);
    kt << Rus("Процессор") << m_processor;
    kt << Rus("Ядер") << m_core;
    kt << Rus("Тип ОП") << m_ramtype;
    kt << Rus("Размер ОП") << m_ramsize;
    kt << Rus("Разрешение экрана") << m_screenresol;
    kt << Rus("Диагональ экрана") << m_screendiagonal;

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

    temp << NRus(m_processor.c_str()) << " " << m_core << " " << NRus(m_ramtype.c_str()) << " " << m_ramsize << " " << NRus(m_screenresol.c_str()) << " " << m_screendiagonal;
    stringToSend += temp.str();

}

bool Computer::replaceObject(std::string& string)
{
    if (Tech::replaceObject(string) == false)
        return false;

    char processor[64], core[64], ramtype[64], ramsize[64], screenresol[64], screendiag[64];
    string = getWords(7, string, processor, core, ramtype, ramsize, screenresol, screendiag);

    int32_t tmp = (atoi(ramsize));
    if (tmp < 0) tmp = 0;
    m_ramsize = tmp;

    tmp = (atoi(screendiag));
    if (tmp < 0) tmp = 0;
    m_screendiagonal = tmp;

    tmp = (atoi(core));
    if (tmp < 0) tmp = 0;
    m_core = tmp;

    m_processor = processor;
    m_ramtype = ramtype;
    m_screenresol = screenresol;

    return true;
}

std::istream& MobilePhone::setState(std::istream& in)
{
    Tech::setState(in);

    std::cout << Rus("Введите название ОС: ");
    std::getline(std::cin, m_os);
    std::cout << Rus("Введите разрешение экрана: ");
    std::getline(std::cin, m_screenresol);
    getnumber(m_screendiagonal, Rus("Введите диагональ экрана: "));
    std::cout << Rus("Введите имя процессора: ");
    std::getline(std::cin, m_processor);
    getnumber(m_core, Rus("Введите количество ядер: "));
    getnumber(m_ramsize, Rus("Введите размер ОП: "));
    getnumber(m_simcount, Rus("Введите количество сим.карт: "));


    return in;
}

std::ostream& MobilePhone::print(std::ostream& out) const
{
    Tech::print(out);
    kt << Rus("Операционная система") << m_os;
    kt << Rus("Разрешение экрана") << m_screenresol;
    kt << Rus("Диагональ экрана") << m_screendiagonal;
    kt << Rus("Процессор") << m_processor;
    kt << Rus("Ядер") << m_core;
    kt << Rus("Размер ОП") << m_ramsize;
    kt << Rus("Количество Сим.карт") << m_simcount;

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

    temp << NRus(m_os.c_str()) << " " << NRus(m_screenresol.c_str()) << " " << m_screendiagonal << " " << NRus(m_processor.c_str()) << " " << m_core << " " << m_ramsize << " " << m_simcount;
    stringToSend += temp.str();
}

bool MobilePhone::replaceObject(std::string& string)
{
    if (Tech::replaceObject(string) == false)
        return false;

    char os[64], screenresol[64], processor[64], core[64], ramsize[64], screendiag[64], simcount[64];
    string = getWords(8, string, os, screenresol, screendiag, processor, core, ramsize, simcount);

    int32_t tmp = (atoi(ramsize));
    if (tmp < 0) tmp = 0;
    m_ramsize = tmp;

    tmp = (atoi(screendiag));
    if (tmp < 0) tmp = 0;
    m_screendiagonal = tmp;

    tmp = (atoi(core));
    if (tmp < 0) tmp = 0;
    m_core = tmp;

    tmp = (atoi(simcount));
    if (tmp < 0) tmp = 0;
    m_simcount = tmp;

    m_os = os;
    m_processor = processor;
    m_screenresol = screenresol;

    return true;
}

std::istream& TV::setState(std::istream& in)
{
    Tech::setState(in);
    std::cout << Rus("Введите тип экрана: ");
    std::getline(std::cin, m_typescreen);
    std::cout << Rus("Введите разрешение экрана: ");
    std::getline(std::cin, m_screenresolution);
    getnumber(m_screendiagonal, Rus("Введите диагональ экрана: "));
    std::cout << Rus("Введите имя процессора: ");
    std::getline(std::cin, m_processor);
    getnumber(m_core, Rus("Введите количество симкарт: "));
    std::cout << Rus("3Д режим? Y - да, Другое - нет: ");
    m_3Dmode = getchar() == 'Y' ? true : false; getchar();

    return in;
}

std::ostream& TV::print(std::ostream& out) const
{
    Tech::print(out);

    std::cout << std::boolalpha;
    kt << Rus("Тип экрана") << m_typescreen;
    kt << Rus("Разрешение экрана") << m_screenresolution;
    kt << Rus("Диагональ экрана") << m_screendiagonal;
    kt << Rus("Процессор") << m_processor;
    kt << Rus("Ядер") << m_core;
    kt << Rus("3Д режим") << m_3Dmode;

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

    temp << NRus(m_typescreen.c_str()) << " " << NRus(m_screenresolution.c_str()) << " " << m_screendiagonal << " " << NRus(m_processor.c_str()) << " " << m_core << " " << m_3Dmode;
    stringToSend += temp.str();
}

bool TV::replaceObject(std::string& string)
{
    if (Tech::replaceObject(string) == false)
        return false;

    char typescreen[64], screenresol[64], processor[64], core[64], _3Dmode[64];
    getWords(6, string, typescreen, screenresol, processor, core, _3Dmode);

    m_typescreen = typescreen;
    m_processor = processor;
    m_screenresolution = screenresol;

    int32_t tmp = (atoi(core));
    if (tmp < 0) tmp = 0;
    m_core = tmp;

    tmp = (atoi(_3Dmode));
    if (tmp < 0) tmp = 0;
    m_3Dmode = tmp;

    return true;
}

std::istream& Toaster::setState(std::istream& in)
{
    Tech::setState(in);
    getnumber(m_toastcount, Rus("Введите количество тостов: "));
    getnumber(m_power, Rus("Введите мощность тостера: "));
    std::cout << Rus("Режим разморозки? Y - да, Другое - нет: ");
    m_defrostring = getchar() == 'Y' ? true : false; getchar();
    std::cout << Rus("Режим нагрева? Y - да, Другое - нет: ");
    m_heating = getchar() == 'Y' ? true : false; getchar();

    return in;
}

std::ostream& Toaster::print(std::ostream& out) const
{
    Tech::print(out);

    std::cout << std::boolalpha;
    kt << Rus("Количество тостов") << m_toastcount;
    kt << Rus("Мощность") << m_power;
    kt << Rus("Разморозка") << m_defrostring;
    kt << Rus("Нагрев") << m_heating;

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

    int32_t tmp = (atoi(toastcount));
    if (tmp < 0) tmp = 0;
    m_toastcount = tmp;

    tmp = (atoi(power));
    if (tmp < 0) tmp = 0;
    m_power = tmp;

    tmp = (atoi(defrosting));
    if (tmp < 0) tmp = 0;
    m_defrostring = tmp;

    tmp = (atoi(heating));
    if (tmp < 0) tmp = 0;
    m_heating = tmp;

    return true;
}

std::istream& CoffeMaker::setState(std::istream& in)
{
    Tech::setState(in);
    getnumber(m_power, Rus("Введите мощность: "));
    getnumber(m_pressure, Rus("Введите давление: "));
    std::cout << Rus("Каппучино? Y - да, Другое - нет: ");
    m_cappuccinomaker = getchar() == 'Y' ? true : false; getchar();

    return in;
}

std::ostream& CoffeMaker::print(std::ostream& out) const
{
    Tech::print(out);

    std::cout << std::boolalpha;
    kt << Rus("Мощность") << m_power;
    kt << Rus("Давление") << m_pressure;
    kt << Rus("Каппучино") << m_cappuccinomaker;

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
    getnumber(m_power, Rus("Введите мощность: "));
    getnumber(m_volume, Rus("Введите объем: "));
    std::cout << Rus("Таймер? Y - да, Другое - нет: ");
    m_timer = getchar() == 'Y' ? true : false; getchar();

    return in;
}

std::ostream& ElKettle::print(std::ostream& out) const
{
    Tech::print(out);

    std::cout << std::boolalpha;
    kt << Rus("Мощность") << m_power;
    kt << Rus("Объем") << m_volume;
    kt << Rus("Таймер") << m_timer;

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
    getnumber(m_volume, Rus("Введите объем: "));
    getnumber(m_shelfcount, Rus("Введите количество полок: "));
    getnumber(m_noiselvl, Rus("Введите уровень шума: "));
    std::cout << Rus("Мультизона? Y - да, Другое - нет: ");
    m_multizone = getchar() == 'Y' ? true : false; getchar();

    return in;
}

std::ostream& Fridge::print(std::ostream& out) const
{
    Tech::print(out);

    std::cout << std::boolalpha;
    kt << Rus("Объем") << m_volume;
    kt << Rus("Кол.полок") << m_shelfcount;
    kt << Rus("Уровень шума") << m_noiselvl;
    kt << Rus("Мультизона") << m_multizone;

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
    std::cout << Rus("Введите режим работы: ");
    std::getline(std::cin, m_workmode);
    getnumber(m_coolingpower, Rus("Введите мощность охлаждения: "));
    getnumber(m_heatingpower, Rus("Введите мощность нагревания: "));
    std::cout << Rus("Удаленное управление? Y - да, Другое - нет: ");
    m_remotecontol = getchar() == 'Y' ? true : false; getchar();

    return in;
}

std::ostream& Conditioner::print(std::ostream& out) const
{
    Tech::print(out);

    std::cout << std::boolalpha;
    kt << Rus("Режим работы") << m_workmode;
    kt << Rus("Охл.мощность") << m_coolingpower;
    kt << Rus("Нагрев.мощность") << m_heatingpower;
    kt << Rus("Уд.управление") << m_remotecontol;

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

    temp << NRus(m_workmode.c_str()) << " " << m_coolingpower << " " << m_heatingpower << " " << m_remotecontol;
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
    getnumber(m_power, Rus("Введите мощность: "));
    getnumber(m_lvlpower_count, Rus("Введите кол-во уровней мощности: "));
    getnumber(m_volume, Rus("Введите объем: "));
    std::cout << Rus("Быстрый старт? Y - да, Другое - нет: ");
    m_quickstart = getchar() == 'Y' ? true : false; getchar();
    std::cout << Rus("Таймер? Y - да, Другое - нет: ");
    m_timer = getchar() == 'Y' ? true : false; getchar();

    return in;
}

std::ostream& Microwawe::print(std::ostream& out) const
{
    Tech::print(out);
    std::cout << std::boolalpha;
    kt << Rus("Мощность") << m_power;
    kt << Rus("Уровней мощности") << m_lvlpower_count;
    kt << Rus("Объем") << m_volume;
    kt << Rus("Быстр.старт") << m_quickstart;
    kt << Rus("Таймер") << m_timer;

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

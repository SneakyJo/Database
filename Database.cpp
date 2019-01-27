// Курс "белый пояс по с++" финальная задача v1.2
// Простенькая отсортированная база данных вида: дата:событие1;событие2;...
// Задача подразумевает реализацию в 1 файле

#include <string>
#include <iostream>
#include <vector>
#include <map>
#include <set>
#include <iomanip>

using namespace std;

typedef unsigned short ushort;

class Date{
public:

  Date() { };

  Date(const int& _year, const ushort& _month, const ushort& _day)
  {
    SetDate(_year, month, _day);
  }

  void SetDate(const int& _year,const ushort& _month,const ushort& _day)
  {
    year = _year; month = _month; day = _day;
  }

  int getYear() const { return year; }
  ushort getMonth() const { return month; }
  ushort getDay() const { return day; }

private:
  int year;
  ushort month;
  ushort day;
};

// OPERATORS FOR DATE     ----------------------------

bool operator< (const Date& date1, const Date& date2)           // Для контейнера map
{
  if (date1.getYear() < date2.getYear())                        // Дни и месяцы ограничены, а годы - нет
  {
    return true;
  }
  else if (date1.getYear() > date2.getYear())
  {
    return false;
  }
  else
  {
    int total1 = date1.getMonth() * 31 + date1.getDay();        // Все дни = 31
    int total2 = date2.getMonth() * 31 + date2.getDay();

    if (total1 < total2)
      return true;
  }

  return false;
}

bool operator== (const Date& date1, const Date& date2)
{
  if (date1.getYear() == date2.getYear() && date1.getMonth() == date2.getMonth() && date1.getDay() == date2.getDay())
    return true;
  return false;
}

// OPERATORS FOR DATE     ----------------------------


class Database{
public:

  Database() { }

  Database(const map<Date, set<string>>& _events)
  {
      events = _events;
  }

  Database(const map<string, set<string>>& _events)
  {
    Date _newDate;

      for (const auto& [key, value] : _events)
        if (CheckDate(key, _newDate))
          for (const string& str : value)
              events[_newDate].insert(str);
  }

  Database(const string& _date, const set<string>& _events)
  {
    Date _newDate;

    if (CheckDate(_date, _newDate))
      for (const string& str : _events)
        events[_newDate].insert(str);
  }

  Database(const string& _date, const string& _event)
  {
    Date _newDate;

    if (CheckDate(_date, _newDate))
      events[_newDate].insert(_event);
  }

  bool AddEvent(const string& _date, const string& _event)       // Добавить событие в дате
  {
    Date _newDate;

    if (CheckDate(_date, _newDate))
    {
      events[_newDate].insert(_event);
      return true;
    }

    return false;
  }

  bool DeleteEvent(const string& _date, const string& _event)    // Удалить событие за дату
  {
    Date _newDate;

    if (CheckDate(_date, _newDate))
      if (events[_newDate].count(_event) > 0)
      {
        events[_newDate].erase(events[_newDate].find(_event));
        cout << "Deleted successfully." << endl;
        return true;
      }

    cout << "Event not found." << endl;

    return false;
  }

  bool DeleteEvents(const string& _date)                    // Удалить все события за дату
  {
    Date _newDate;

    if (CheckDate(_date, _newDate))
    {
      size_t _count = events[_newDate].size();
      events[_newDate].clear();

      cout << "Deleted " << _count << " events." << endl;

      if (_count > 0)
        return true;
    }

    return false;
  }


  bool FindData(const string& _date) const                  // Вывести события за дату
  {
    Date _newDate;

    if (CheckDate(_date, _newDate))
      if (events.count(_newDate) > 0)
      {
        for (const string& ev : events.at(_newDate))    // я всего-лишь вывожу, не верит, что не буду изменять ev
          cout << ev << endl;

        return true;
      }

    cout << "No events.";
    return false;

  }

  void Print() const                        // Вывести всё
  {

    for (const auto& [key, value] : events)
    {
      // Возможный ввод 1-1-1, необходимый вывод: 0001-01-01

      cout << setw(4) << setfill('0') << key.getYear() << "/";
      cout << setw(2) << key.getMonth() << "/";
      cout << setw(2) << key.getDay() << " ";

      for (const string& ev : value)
        cout << ev << " ";                                            // Вывод событий

      cout << endl;
    }
  }

private:
map <Date, set<string>> events;


bool CheckDate(string _dateStr, Date& inputDate) const   // Проверяем строку на корректность ввода и возвращаем удобного вида Дату
{
  size_t pos1 = 0;
  size_t pos2 = 0;
  ushort i = 0;
  const char separator = '-';

  int ourDate[2];                     // 3 значение (день) сразу в параметры кинем

  const string err_copy = _dateStr;      // Ошибка, вы ввели: err_copy

  try{

   while (i < 2)                        // Считаем только год и месяц в цикле
   {
     pos2 = _dateStr.find(separator);            // 2012-*

     if (pos2 == 0)       // -1-11-11 корректная дата
     {
         pos1 = pos2;
         pos2 = _dateStr.find(separator, pos1 + 1);
     }

     int len = pos2 - pos1;
     string sub = _dateStr.substr(0, len);    // переменные для наглядности действия

     int num = stoi(sub);                     // Если что, кидает throw
     i++;

     if (i == 2)                            // На i = 1, год ограничений нет
      if (num > 11 || num < 0)
        throw runtime_error("Invalid month"s);

     ourDate[i - 1] = num;                  // Храним преобразованные год и месяц в массиве

     _dateStr.erase(_dateStr.begin(), _dateStr.begin() + len + 1);   // Удаляем отработанное
    }

    int num = stoi(_dateStr);      // формат не подразумевает '-' после дня, поэтому остаток строки и есть дни
    if (num > 31 || num < 0)
      throw runtime_error("Invalid day"s);


    inputDate.SetDate(ourDate[0], ourDate[1], num); // Числа гарантированно конвертируются в ushort, ведь были проверки.
    // Таким образом проверяем строку и сразу же преобразуем её в тип Date, доступный по ссылке
  }
  catch(exception& ex)
  {
    cout << "Wrong date format: "s << err_copy << " (" << ex.what() << ")" << endl;
    return false;
  }

  return true;

}

};


vector<string> separateInput(const string& _str)
{
  const char separator = ' ';

  if (_str.find(separator) == string::npos)     // Если ввели только print
    return {_str};

  vector<string> output;
  size_t pos1 = 0, pos2 = 0;

  while (_str.find(separator, pos1 + 1) != string::npos)
  {
    pos2 = _str.find(separator, pos1 + 1);
    output.push_back(_str.substr(pos1, pos2 - pos1));
    pos1 = pos2;
  }

  output.push_back(_str.substr(pos2 + 1, _str.size() - pos2 ));

  return output;
}




int main()
{
  // Условности задачи:
  // 1) Любая команда = 1 строка, возможны пустые строки
  // 2) События всегда содержат нужное количество аргументов
  // 3) Все команды, даты и события являются непустыми строками и не содержат пробелов и прочих пробельных символов
  // 4) Дата с отрицательным значением года должна учитываться, но выводиться не будет

  // Пример ввода:
  // add 2001-09-11 towers
  // add 2001-9-11 fbi
  // find 2001-9-11
  // del 2001-9-11 fbi
  // add 2001-9-11 terrorist
  // print
  // del 2001-9-11
  // add 2019-1-1 YOJ
  // print


  Database db;

  string command;
  while (getline(cin, command)) {
    // Считайте команды с потока ввода и обработайте каждую

    vector<string> _vecCommands = separateInput(command);         // 0 - команда, 1 - дата, 2 - событие

    if (_vecCommands[0] == "add"s)
    {
      db.AddEvent(_vecCommands[1], _vecCommands[2]);        // Добавить событие за определённую дату
    }
    else if (_vecCommands[0] == "del"s)
    {
      if (_vecCommands.size() == 2)
        db.DeleteEvents(_vecCommands[1]);                   // Если введена только дата - удаляем все события за дату
      else
        db.DeleteEvent(_vecCommands[1], _vecCommands[2]);   // Иначе определённое событие за дату

    }
    else if (_vecCommands[0] == "find"s)
    {
      db.FindData(_vecCommands[1]);        // Найти и вывести событие за дату
    }
    else if (_vecCommands[0] == "print"s)
    {
      db.Print();                                           // Вывести всю БД
    }
    else if (command == "")  {              // Пропускаем пустую строку
    }
    else
    {
      cout << "Unknown command: " << command << endl;
    }

    // Ctrl + C to exit
  }

  return 0;
}

// Ввод/Вывод
#include <iostream>
#include <locale>
#include <fstream>

// Структуры данных
#include <set>
#include <string>
#include <vector>

struct Person
{
    friend bool operator<( const Person& lhs, const Person& rhs );
    // Имя, фамилия, телефон.
    std::wstring name, surname, phone;
};

// Не оптимально по памяти.
bool operator<( const Person& lhs, const Person& rhs )
{
    std::wstring left;
    std::wstring right;

    left += lhs.name + lhs.surname + lhs.phone;
    right += rhs.name + rhs.surname + rhs.phone;

    //Сортировка сначала по имени, затем по фамилии, затем по телефону.
    return left < right;
}

void setRuLocale()
{
    // C++ locale
    std::locale::global( std::locale( "ru_RU.UTF-8" ) );
    // C locale
    setlocale( LC_ALL, "Russian" );
}

std::set<Person> readFile( const std::string& inputFile )
{
    std::wifstream data( inputFile );

    //Предполагается, что все строки в файле различны. Отсортирован по возрастанию (в алфавитном порядке).
    std::set<Person> file;

    if ( data.is_open() )
    {
        Person person;
        while ( data >> person.surname >> person.name >> person.phone )
        {
            // Убираем ":" после имени
            person.name = person.name.substr( 0, person.name.size() - 1 );
            file.insert( person );
        }
    }
    else
        std::wcout << L"Не удалось открыть файл.\n";
    return file;
}

std::wstring makeLowerCase( const std::wstring& str )
{
    std::wstring lower;
    lower.reserve( str.size() );
    for ( const auto& letter : str )
        lower += std::tolower( letter, std::locale( "Russian" ) );
    return lower;
}

std::wstring getQuery()
{
    std::wstring query;
    std::wcout << L"Введите запрос:\n";
    std::wcin >> query;
    return makeLowerCase( query );
}

std::vector<Person> searchData( const std::wstring& query, const std::set<Person>& data )
{
    std::vector<Person> found;
    for ( const auto& person : data )
        if ( makeLowerCase( person.name ).find( query ) != std::wstring::npos or
            makeLowerCase( person.surname ).find( query ) != std::wstring::npos or
            makeLowerCase( person.phone ).find( query ) != std::wstring::npos )
            found.push_back( person );
    return found;
}

void printFound( const std::vector<Person>& found )
{
    if ( found.empty() )
        std::wcout << L"Ничего не найдено.\n";
    else
    {
        std::wcout << L"Найдено:\n";
        for ( const auto& person : found )
            std::wcout << person.surname << " " << person.name << ": " << person.phone << "\n";
    }
}

int main()
{
    setRuLocale();

    // Содержимое файла
    auto data = readFile( "data.txt" );

    // 3 примера
    for ( auto n = 0; n < 3; ++n )
    {
        // Приглашение ввода + чтение запроса
        auto query = getQuery();
        // Поиск запроса в файле
        auto found = searchData( query, data );
        // Вывод в std::wcin в соответсвии с запросом
        printFound( found );
        std::cout << "\n";
    }

    return 0;
}

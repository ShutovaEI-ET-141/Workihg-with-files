// Практика 9
//Вариант 24
//Определить структурный тип, описывающий расписание полетов самолетов(пункт
//посадки, время отправления, время прибытия, время полета, стоимость билета, тип
//рейса : чартер, транзит, стыковка).Заполнить структурный массив 20 - ю записями.
//Переписать из исходного массива в другой массив, информацию только о тех рейсах
//пункт назначения, которых начинающихся на «К» и рейс транзитный.Затем новый
//массив отсортировать по времени полета(рационально переставлять все поля
//структуры разом).Вывести все данные по конкретному рейсу.Вывести 5 чартерных
//рейсов, которые вылетают ночью.Реализовать функцию изменения рейса.В
//отдельный массив поместить все стыковочные рейсы, стоимостью менее N рублей.
//Реализовать вывод отфильтрованных данных в виде оберточной функции.

#include <iostream>
#include <fstream>
#include <cstring>
#include <cstdlib>
#include <ctime>
using namespace std;
const int SIZE = 20;
enum FlightType {
    CHARTER,
    TRANSIT,
    CONNECTION
};
struct Time {
    int hours;
    int minutes;
};
struct Flight {
    char destination[50];
    Time departure;
    Time arrival;
    int flightTime;
    int price;
    FlightType type;
};
typedef Flight arr[SIZE];
void printTime(const Time& t) {
    printf("%02d:%02d", t.hours, t.minutes);
}
void print(const Flight& f) {
    printf("%-15s ", f.destination);
    printf("Отпр: ");
    printTime(f.departure);
    printf(" Приб: ");
    printTime(f.arrival);
    printf(" Время: %3d мин ", f.flightTime);
    printf("Цена: %5d руб ", f.price);
    printf("Тип: ");
    switch (f.type) {
    case CHARTER: printf("Чартер"); break;
    case TRANSIT: printf("Транзит"); break;
    case CONNECTION: printf("Стыковка"); break;
    }
    printf("\n");
}
void printFiltered(const Flight* arr, int size, const char* title) {
    printf("\n=== %s ===\n", title);
    if (size == 0) {
        printf("Нет данных\n");
        return;
    }
    for (int i = 0; i < size; i++) {
        print(arr[i]);
    }
}
void swal(Flight& f1, Flight& f2) {
    Flight temp = f1;
    f1 = f2;
    f2 = temp;
}
bool checkSortFlightTime(Flight f1, Flight f2) {
    return f1.flightTime < f2.flightTime;
}
Flight* sorted(Flight workers[], bool (*y)(Flight, Flight), int size) {
    for (int i = 0; i < size; i++) {
        for (int j = size - 1; j > i; j--) {
            if (y(workers[i], workers[j])) {
                swal(workers[i], workers[j]);
            }
        }
    }
    return workers;
}
bool checkFilterKTransit(const Flight& f) {
    return (f.destination[0] == 'К' || f.destination[0] == 'к') && f.type == TRANSIT;
}
int filterFlights(const Flight* src, int srcSize, Flight* dst) {
    int count = 0;
    for (int i = 0; i < srcSize; i++) {
        if (checkFilterKTransit(src[i])) {
            dst[count++] = src[i];
        }
    }
    return count;
}
void printFlightByIndex(const Flight* arr, int size, int index) {
    if (index >= 0 && index < size) {
        printf("\n=== Данные по рейсу %d ===\n", index);
        print(arr[index]);
    }
    else {
        printf("Неверный индекс\n");
    }
}
bool checkNightCharter(const Flight& f) {
    if (f.type != CHARTER) return false;
    int hour = f.departure.hours;
    return (hour >= 22 || hour < 6);
}
void printNightCharters(const Flight* arr, int size) {
    printf("\n=== Чартерные рейсы, вылетающие ночью ===\n");
    int count = 0;
    for (int i = 0; i < size && count < 5; i++) {
        if (checkNightCharter(arr[i])) {
            print(arr[i]);
            count++;
        }
    }
    if (count == 0) printf("Нет таких рейсов\n");
}
void modifyFlight(Flight* arr, int size, int index) {
    if (index < 0 || index >= size) {
        printf("Неверный индекс\n");
        return;
    }
    int typeChoice;
    printf("Введите новые данные для рейса %d:\n", index);
    printf("Пункт назначения: ");
    scanf_s("%s", arr[index].destination, (unsigned int)sizeof(arr[index].destination));
    printf("Время отправления (часы минуты): ");
    scanf_s("%d %d", &arr[index].departure.hours, &arr[index].departure.minutes);
    printf("Время прибытия (часы минуты): ");
    scanf_s("%d %d", &arr[index].arrival.hours, &arr[index].arrival.minutes);
    printf("Время полета (мин): ");
    scanf_s("%d", &arr[index].flightTime);
    printf("Стоимость: ");
    scanf_s("%d", &arr[index].price);
    printf("Тип рейса (0-чартер, 1-транзит, 2-стыковка): ");
    scanf_s("%d", &typeChoice);
    arr[index].type = static_cast<FlightType>(typeChoice);
    printf("Рейс изменен\n");
}
bool checkConnectionByPrice(const Flight& f, int N) {
    return f.type == CONNECTION && f.price < N;
}
int filterConnectionByPrice(const Flight* src, int srcSize, Flight* dst, int N) {
    int count = 0;
    for (int i = 0; i < srcSize; i++) {
        if (checkConnectionByPrice(src[i], N)) {
            dst[count++] = src[i];
        }
    }
    return count;
}
void saveToBinary(const char* filename, const Flight* arr, int size) {
    ofstream out(filename, ios::binary | ios::out);
    out.write((char*)arr, sizeof(Flight) * size);
    out.close();
}
int loadFromBinary(const char* filename, Flight* arr, int maxSize) {
    ifstream in(filename, ios::binary | ios::in);
    if (!in.is_open()) return 0;
    in.read((char*)arr, sizeof(Flight) * maxSize);
    int count = (int)(in.gcount() / sizeof(Flight));
    in.close();
    return count;
}
Flight* init() {
    Flight* flights = new Flight[SIZE];
    flights[0] = { "Курск", {8, 0}, {10, 30}, 150, 5000, TRANSIT };
    flights[1] = { "Краснодар", {23, 30}, {1, 0}, 90, 3000, CHARTER };
    flights[2] = { "Казань", {14, 0}, {16, 0}, 120, 4000, TRANSIT };
    flights[3] = { "Москва", {9, 0}, {11, 0}, 120, 4500, CHARTER };
    flights[4] = { "Калининград", {22, 0}, {0, 30}, 150, 6000, TRANSIT };
    flights[5] = { "Сочи", {10, 0}, {12, 0}, 120, 3500, CONNECTION };
    flights[6] = { "Краснодар", {1, 0}, {3, 0}, 120, 3200, CHARTER };
    flights[7] = { "Курск", {2, 0}, {4, 30}, 150, 4800, CHARTER };
    flights[8] = { "Волгоград", {13, 0}, {14, 30}, 90, 2500, CONNECTION };
    flights[9] = { "Краснодар", {15, 0}, {17, 0}, 120, 2800, TRANSIT };
    flights[10] = { "Казань", {23, 0}, {1, 30}, 150, 5500, CHARTER };
    flights[11] = { "Калининград", {3, 0}, {5, 0}, 120, 3100, CHARTER };
    flights[12] = { "Курск", {20, 0}, {22, 0}, 120, 4200, CONNECTION };
    flights[13] = { "Краснодар", {11, 0}, {13, 0}, 120, 3700, TRANSIT };
    flights[14] = { "Краснодар", {18, 0}, {20, 0}, 120, 2900, CONNECTION };
    flights[15] = { "Казань", {4, 0}, {6, 0}, 120, 3400, CHARTER };
    flights[16] = { "Калининград", {12, 0}, {14, 0}, 120, 4100, TRANSIT };
    flights[17] = { "Курск", {21, 0}, {23, 0}, 120, 3900, CHARTER };
    flights[18] = { "Краснодар", {7, 0}, {9, 0}, 120, 2600, CONNECTION };
    flights[19] = { "Казань", {19, 0}, {21, 0}, 120, 4400, TRANSIT };
    return flights;
}
int main() {
    setlocale(LC_ALL, "");
    Flight* flights = init();
    printf("=== Исходный массив рейсов ===\n");
    for (int i = 0; i < SIZE; i++) {
        print(flights[i]);
    }
    Flight filtered[SIZE];
    int filteredCount = filterFlights(flights, SIZE, filtered);
    sorted(filtered, checkSortFlightTime, filteredCount);
    printFiltered(filtered, filteredCount, "Рейсы с пунктом на 'К' и транзитные (отсортированы по времени полета)");
    printFlightByIndex(filtered, filteredCount, 0);
    printNightCharters(flights, SIZE);
    int modifyIndex;
    printf("\nВведите индекс рейса для изменения (0-19): ");
    scanf_s("%d", &modifyIndex);
    modifyFlight(flights, SIZE, modifyIndex);
    int N;
    printf("\nВведите N (стоимость): ");
    scanf_s("%d", &N);
    Flight connections[SIZE];
    int connCount = filterConnectionByPrice(flights, SIZE, connections, N);
    printFiltered(connections, connCount, "Стыковочные рейсы дешевле N рублей");
    saveToBinary("flights.dat", flights, SIZE);
    printf("\nМассив сохранен в flights.dat\n");
    Flight loadedFlights[SIZE];
    int loadedCount = loadFromBinary("flights.dat", loadedFlights, SIZE);
    printf("Загружено из файла %d записей\n", loadedCount);
    delete[] flights;
    return 0;
}

// Практика 10
#include <iostream>
#include <fstream>
#include <cstring>
using namespace std;
const int MAX_NAME = 80;
struct Person {
    char name[MAX_NAME];
    int pol;
    int age;
};
const int N = 5;
// ЗАДАНИЕ 1
void updateFromTextFile(Person* people, int size, const char* filename) {
    ifstream fin;
    fin.open(filename);
    if (!fin.is_open()) {
        cerr << "Ошибка открытия файла " << filename << endl;
        return;
    }
    char fam[MAX_NAME];
    int polFromFile;
    while (!fin.eof()) {
        fin >> fam >> polFromFile;
        if (fin.eof()) break;
        for (int i = 0; i < size; i++) {
            if (strcmp(people[i].name, fam) == 0) {
                people[i].pol = polFromFile;
                break;
            }
        }
    }
    fin.close();
}
//ЗАДАНИЕ 2
void writeBinary(const char* filename, Person* people, int size) {
    ofstream fout;
    fout.open(filename, ios::binary | ios::out);
    
    if (!fout.is_open()) {
        cerr << "Ошибка записи в бинарный файл" << endl;
        return;
    }
    
    fout.write((char*)people, sizeof(Person) * size);
    fout.close();
}
void readBinary(const char* filename, Person* people, int size) {
    ifstream fin;
    fin.open(filename, ios::binary | ios::in);
    
    if (!fin.is_open()) {
        cerr << "Ошибка чтения бинарного файла" << endl;
        return;
    }
    
    fin.read((char*)people, sizeof(Person) * size);
    fin.close();
}
void printPeople(Person* people, int size) {
    for (int i = 0; i < size; i++) {
        cout << people[i].name << " | пол: " << people[i].pol 
             << " | возраст: " << people[i].age << endl;
    }
}
int main() {
    setlocale(LC_ALL, "Russian");
    Person people[N] = {
        {"Ivanov", -1, 25},
        {"Petrov", -1, 30},
        {"Sidorova", -1, 22},
        {"Ivanova", -1, 28},
        {"Makarkin", -1, 35}
    };
    cout << "=== До обновления из текстового файла ===" << endl;
    printPeople(people, N);
    // ЗАДАНИЕ 1
    updateFromTextFile(people, N, "data.txt");
    cout << "\n=== После обновления из текстового файла ===" << endl;
    printPeople(people, N);
    // ЗАДАНИЕ 2
    writeBinary("people.bin", people, N);
    Person newPeople[N];
    memset(newPeople, 0, sizeof(newPeople));
    readBinary("people.bin", newPeople, N);
    cout << "\n=== После чтения из бинарного файла ===" << endl;
    printPeople(newPeople, N);
    return 0;
}

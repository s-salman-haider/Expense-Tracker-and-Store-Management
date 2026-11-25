#include <iostream>
#include <SFML/Graphics.hpp>//we used sfml 2.6
#include <fstream>

using namespace std;

const int MAX = 100;
const int SIZE = 40;

//data used for the store management system

//expenses
char expenseName[MAX][SIZE]{};
char expenseCategory[MAX][SIZE]{};
double expenseAmount[MAX]{};
int expenseCount = 0;

//products
char productName[MAX][SIZE]{};
int productQuantity[MAX]{};
double productCost[MAX]{};
double productPrice[MAX]{};
int productCount = 0;

//employees
char employeeName[MAX][SIZE]{};
char employeeRole[MAX][SIZE]{};
double employeeSalary[MAX]{};
double employeeBonus[MAX]{};
int employeeCount = 0;

//suppliers
char supplierName[MAX][SIZE]{};
char supplierPhone[MAX][SIZE]{};
char supplierItem[MAX][SIZE]{};
int supplierCount = 0;

//sales
char saleProduct[MAX][SIZE]{};
int saleQuantity[MAX]{};
double saleTotal[MAX]{};
int saleCount = 0;

//deliveries
char deliveryProduct[MAX][SIZE]{};
char deliveryDestination[MAX][SIZE]{};
int deliveryQuantity[MAX]{};
double deliveryCost[MAX]{};
int deliveryCount = 0;

//input fields
char fieldValue[4][SIZE]{};
int fieldLength[4]{};
bool fieldSelected[4]{};
int fieldCount = 0;

//sfml variables
sf::RenderWindow window;
sf::Font font;

int page = 0;

char status[100] = "Welcome to Store Management System!";

sf::Color background(18, 23, 32);
sf::Color panel(31, 40, 54);
sf::Color cyan(0, 190, 210);

//functions for string

void copyText(char dest[], const char source[])
{
    int i = 0;

    while (source[i] != '\0' && i < SIZE - 1)
    {
        dest[i] = source[i];
        i++;
    }

    dest[i] = '\0';
}

bool sameText(const char a[], const char b[])
{
    int i = 0;

    while (a[i] != '\0' && b[i] != '\0')
    {
        if (a[i] != b[i])
            return false;

        i++;
    }

    return a[i] == b[i];
}

bool toNumber(const char text[], double& result)
{
    if (text[0] == '\0')
        return false;

    int i = 0;
    double number = 0;
    double decimal = 0.1;
    bool dot = false;
    int digits = 0;

    while (text[i] != '\0')
    {
        if (text[i] == '.' && !dot)
        {
            dot = true;
        }
        else if (text[i] >= '0' && text[i] <= '9')
        {
            int digit = text[i] - '0';

            if (!dot)
                number = number * 10 + digit;
            else
            {
                number += digit * decimal;
                decimal *= 0.1;
            }

            digits++;
        }
        else
        {
            return false;
        }

        i++;
    }

    if (digits == 0)
        return false;

    result = number;
    return true;
}

bool toInteger(const char text[], int& result)
{
    double n;

    if (!toNumber(text, n))
        return false;

    if (n > 1000000 || n < 0 || n != (int)n)
        return false;

    result = (int)n;
    return true;
}

void numberText(int n, char output[])
{
    char temp[20];
    int i = 0;
    int j = 0;

    if (n == 0)
        temp[i++] = '0';

    while (n > 0)
    {
        temp[i++] = char('0' + n % 10);
        n /= 10;
    }

    while (i > 0)
        output[j++] = temp[--i];

    output[j] = '\0';
}

void moneyText(double amount, char output[])
{
    int whole = (int)amount;
    int cents = (int)((amount - whole) * 100 + 0.5);

    if (cents >= 100)
    {
        whole++;
        cents = 0;
    }

    char temp[20];
    int i = 0;
    int j = 0;

    output[j++] = 'R';
    output[j++] = 's';
    output[j++] = '.';
    output[j++] = ' ';

    if (whole == 0)
        temp[i++] = '0';

    while (whole > 0)
    {
        temp[i++] = char('0' + whole % 10);
        whole /= 10;
    }

    while (i > 0)
        output[j++] = temp[--i];

    output[j++] = '.';
    output[j++] = char('0' + cents / 10);
    output[j++] = char('0' + cents % 10);
    output[j] = '\0';
}

void setStatus(const char message[])
{
    int i = 0;

    while (message[i] != '\0' && i < 99)
    {
        status[i] = message[i];
        i++;
    }

    status[i] = '\0';
}


// ================= FILE STORAGE =================
// Records are stored in store_data.txt in the program's working folder.

bool readFileCount(ifstream& file, int& count)
{
    if (!(file >> count))
        return false;

    file.ignore(10000, '\n');

    if (count < 0 || count > MAX)
    {
        count = 0;
        return false;
    }

    return true;
}

bool readFileText(ifstream& file, char text[])
{
    file.getline(text, SIZE);
    return !file.fail();
}

template <typename T>
bool readFileValue(ifstream& file, T& value)
{
    if (!(file >> value))
        return false;

    file.ignore(10000, '\n');
    return true;
}

bool saveData()
{
    ofstream file("store_data.txt");

    if (!file)
    {
        setStatus("Could not open store_data.txt for saving!");
        return false;
    }

    file << "STORE_TRACKER_V1" << endl;

    // Expenses
    file << expenseCount << endl;
    for (int i = 0; i < expenseCount; i++)
    {
        file << expenseName[i] << endl;
        file << expenseCategory[i] << endl;
        file << expenseAmount[i] << endl;
    }

    // Products
    file << productCount << endl;
    for (int i = 0; i < productCount; i++)
    {
        file << productName[i] << endl;
        file << productQuantity[i] << endl;
        file << productCost[i] << endl;
        file << productPrice[i] << endl;
    }

    // Employees
    file << employeeCount << endl;
    for (int i = 0; i < employeeCount; i++)
    {
        file << employeeName[i] << endl;
        file << employeeRole[i] << endl;
        file << employeeSalary[i] << endl;
        file << employeeBonus[i] << endl;
    }

    // Suppliers
    file << supplierCount << endl;
    for (int i = 0; i < supplierCount; i++)
    {
        file << supplierName[i] << endl;
        file << supplierPhone[i] << endl;
        file << supplierItem[i] << endl;
    }

    // Sales
    file << saleCount << endl;
    for (int i = 0; i < saleCount; i++)
    {
        file << saleProduct[i] << endl;
        file << saleQuantity[i] << endl;
        file << saleTotal[i] << endl;
    }

    // Deliveries
    file << deliveryCount << endl;
    for (int i = 0; i < deliveryCount; i++)
    {
        file << deliveryProduct[i] << endl;
        file << deliveryDestination[i] << endl;
        file << deliveryQuantity[i] << endl;
        file << deliveryCost[i] << endl;
    }

    file.close();

    if (!file)
    {
        setStatus("Error while writing store_data.txt!");
        return false;
    }

    return true;
}

void loadData()
{
    ifstream file("store_data.txt");

    if (!file)
    {
        setStatus("No saved file found. Starting with empty records.");
        return;
    }

    string version;
    getline(file, version);

    if (version != "STORE_TRACKER_V1")
    {
        setStatus("Saved file has an unknown format.");
        return;
    }

    // Expenses
    if (!readFileCount(file, expenseCount))
    {
        setStatus("Invalid expense data in file.");
        return;
    }
    for (int i = 0; i < expenseCount; i++)
    {
        if (!readFileText(file, expenseName[i]) ||
            !readFileText(file, expenseCategory[i]) ||
            !readFileValue(file, expenseAmount[i]))
        {
            expenseCount = i;
            setStatus("Some expense data could not be loaded.");
            return;
        }
    }

    // Products
    if (!readFileCount(file, productCount))
    {
        setStatus("Invalid product data in file.");
        return;
    }
    for (int i = 0; i < productCount; i++)
    {
        if (!readFileText(file, productName[i]) ||
            !readFileValue(file, productQuantity[i]) ||
            !readFileValue(file, productCost[i]) ||
            !readFileValue(file, productPrice[i]))
        {
            productCount = i;
            setStatus("Some product data could not be loaded.");
            return;
        }
    }

    // Employees
    if (!readFileCount(file, employeeCount))
    {
        setStatus("Invalid employee data in file.");
        return;
    }
    for (int i = 0; i < employeeCount; i++)
    {
        if (!readFileText(file, employeeName[i]) ||
            !readFileText(file, employeeRole[i]) ||
            !readFileValue(file, employeeSalary[i]) ||
            !readFileValue(file, employeeBonus[i]))
        {
            employeeCount = i;
            setStatus("Some employee data could not be loaded.");
            return;
        }
    }

    // Suppliers
    if (!readFileCount(file, supplierCount))
    {
        setStatus("Invalid supplier data in file.");
        return;
    }
    for (int i = 0; i < supplierCount; i++)
    {
        if (!readFileText(file, supplierName[i]) ||
            !readFileText(file, supplierPhone[i]) ||
            !readFileText(file, supplierItem[i]))
        {
            supplierCount = i;
            setStatus("Some supplier data could not be loaded.");
            return;
        }
    }

    // Sales
    if (!readFileCount(file, saleCount))
    {
        setStatus("Invalid sales data in file.");
        return;
    }
    for (int i = 0; i < saleCount; i++)
    {
        if (!readFileText(file, saleProduct[i]) ||
            !readFileValue(file, saleQuantity[i]) ||
            !readFileValue(file, saleTotal[i]))
        {
            saleCount = i;
            setStatus("Some sales data could not be loaded.");
            return;
        }
    }

    // Deliveries
    if (!readFileCount(file, deliveryCount))
    {
        setStatus("Invalid delivery data in file.");
        return;
    }
    for (int i = 0; i < deliveryCount; i++)
    {
        if (!readFileText(file, deliveryProduct[i]) ||
            !readFileText(file, deliveryDestination[i]) ||
            !readFileValue(file, deliveryQuantity[i]) ||
            !readFileValue(file, deliveryCost[i]))
        {
            deliveryCount = i;
            setStatus("Some delivery data could not be loaded.");
            return;
        }
    }

    setStatus("Saved records loaded from store_data.txt.");
}


//functions for store management

int findProduct(const char name[])
{
    for (int i = 0; i < productCount; i++)
    {
        if (sameText(productName[i], name))
            return i;
    }

    return -1;
}

bool addExpense(const char name[],
    const char category[], double amount)
{
    if (expenseCount >= MAX || name[0] == '\0' ||
        category[0] == '\0' || amount <= 0)
        return false;

    copyText(expenseName[expenseCount], name);
    copyText(expenseCategory[expenseCount], category);

    expenseAmount[expenseCount] = amount;

    expenseCount++;

    return true;
}

bool addProduct(const char name[], int quantity,
    double cost, double price)
{
    if (productCount >= MAX || name[0] == '\0' ||
        quantity < 0 || cost < 0 || price <= 0 ||
        findProduct(name) != -1)
        return false;

    copyText(productName[productCount], name);

    productQuantity[productCount] = quantity;
    productCost[productCount] = cost;
    productPrice[productCount] = price;

    productCount++;

    return true;
}

bool addEmployee(const char name[], const char role[],
    double salary, double bonus)
{
    if (employeeCount >= MAX || name[0] == '\0' ||
        role[0] == '\0' || salary < 0 || bonus < 0)
        return false;

    copyText(employeeName[employeeCount], name);
    copyText(employeeRole[employeeCount], role);

    employeeSalary[employeeCount] = salary;
    employeeBonus[employeeCount] = bonus;

    employeeCount++;

    return true;
}

bool addSupplier(const char name[], const char phone[],
    const char item[])
{
    if (supplierCount >= MAX || name[0] == '\0' ||
        phone[0] == '\0' || item[0] == '\0')
        return false;

    copyText(supplierName[supplierCount], name);
    copyText(supplierPhone[supplierCount], phone);
    copyText(supplierItem[supplierCount], item);

    supplierCount++;

    return true;
}

bool addSale(const char name[], int quantity)
{
    int index = findProduct(name);

    if (index == -1 || quantity <= 0 ||
        quantity > productQuantity[index] ||
        saleCount >= MAX)
        return false;

    copyText(saleProduct[saleCount], name);

    saleQuantity[saleCount] = quantity;
    saleTotal[saleCount] = quantity * productPrice[index];

    productQuantity[index] -= quantity;

    saleCount++;

    return true;
}

bool addDelivery(const char name[], const char destination[],
    int quantity, double cost)
{
    int index = findProduct(name);

    if (index == -1 || destination[0] == '\0' ||
        quantity <= 0 || cost < 0 ||
        quantity > productQuantity[index] ||
        deliveryCount >= MAX)
        return false;

    copyText(deliveryProduct[deliveryCount], name);
    copyText(deliveryDestination[deliveryCount], destination);

    deliveryQuantity[deliveryCount] = quantity;
    deliveryCost[deliveryCount] = cost;

    productQuantity[index] -= quantity;

    deliveryCount++;

    return true;
}

double revenue()
{
    double total = 0;

    for (int i = 0; i < saleCount; i++)
        total += saleTotal[i];

    return total;
}

double expensesTotal()
{
    double total = 0;

    for (int i = 0; i < expenseCount; i++)
        total += expenseAmount[i];

    for (int i = 0; i < employeeCount; i++)
        total += employeeSalary[i] + employeeBonus[i];

    for (int i = 0; i < deliveryCount; i++)
        total += deliveryCost[i];

    return total;
}

double inventoryValue()
{
    double total = 0;

    for (int i = 0; i < productCount; i++)
        total += productQuantity[i] * productCost[i];

    return total;
}

//functions for inputs

void clearField(int index)
{
    fieldValue[index][0] = '\0';
    fieldLength[index] = 0;
    fieldSelected[index] = false;
}

void clearAllFields()
{
    for (int i = 0; i < 4; i++)
        clearField(i);
}

void setupFields()
{
    fieldCount = 0;

    if (page == 1 || page == 4)
        fieldCount = 3;

    if (page == 2 || page == 3 || page == 6)
        fieldCount = 4;

    if (page == 5)
        fieldCount = 2;

    clearAllFields();
}

void handleField(int index, sf::Event& event,
    sf::Vector2f mouse)
{
    float x = 270;
    float y = 170 + index * 75;

    sf::FloatRect bounds(x, y, 340, 40);

    if (event.type == sf::Event::MouseButtonPressed &&
        event.mouseButton.button == sf::Mouse::Left)
    {
        fieldSelected[index] = bounds.contains(mouse);
    }

    if (fieldSelected[index] &&
        event.type == sf::Event::TextEntered)
    {
        unsigned int key = event.text.unicode;

        if (key == 8 && fieldLength[index] > 0)
        {
            fieldLength[index]--;
            fieldValue[index][fieldLength[index]] = '\0';
        }
        else if (key >= 32 && key <= 126 &&
            fieldLength[index] < SIZE - 1)
        {
            fieldValue[index][fieldLength[index]] = (char)key;

            fieldLength[index]++;

            fieldValue[index][fieldLength[index]] = '\0';
        }
    }
}

void drawField(int index)
{
    sf::RectangleShape box(sf::Vector2f(340, 40));

    box.setPosition(270, 170 + index * 75);
    box.setFillColor(sf::Color(42, 51, 66));
    box.setOutlineThickness(1);

    if (fieldSelected[index])
        box.setOutlineColor(cyan);
    else
        box.setOutlineColor(sf::Color(90, 105, 125));

    sf::Text text;
    text.setFont(font);
    text.setCharacterSize(16);
    text.setFillColor(sf::Color::White);
    text.setPosition(280, 178 + index * 75);
    text.setString(fieldValue[index]);

    window.draw(box);
    window.draw(text);
}

//functions for gui/graphics

const char* pages[8] =
{
    "Dashboard",
    "Expenses",
    "Inventory",
    "Employees",
    "Suppliers",
    "Sales",
    "Distribution",
    "Reports"
};

const char* labels[8][4] =
{
    {"", "", "", ""},
    {"Expense Name", "Category", "Amount", ""},
    {"Product Name", "Quantity", "Cost Price", "Selling Price"},
    {"Employee Name", "Job Role", "Monthly Salary", "Bonus"},
    {"Supplier Name", "Phone Number", "Product Supplied", ""},
    {"Product Name", "Quantity", "", ""},
    {"Product Name", "Destination", "Quantity", "Delivery Cost"},
    {"", "", "", ""}
};

void drawText(const char* message, float x, float y,
    int size, sf::Color color = sf::Color::White)
{
    sf::Text t;

    t.setFont(font);
    t.setString(message);
    t.setCharacterSize(size);
    t.setPosition(x, y);
    t.setFillColor(color);

    window.draw(t);
}

void drawMoney(double amount, float x, float y)
{
    char buffer[50];

    moneyText(amount, buffer);

    drawText(buffer, x, y, 18, cyan);
}

void drawButton(float x, float y, float width,
    const char* name)
{
    sf::RectangleShape button(sf::Vector2f(width, 42));

    button.setPosition(x, y);
    button.setFillColor(cyan);

    window.draw(button);

    drawText(name, x + 15, y + 10, 15);
}

void drawSidebar()
{
    sf::RectangleShape side(sf::Vector2f(220, 700));

    side.setFillColor(sf::Color(24, 30, 41));

    window.draw(side);

    drawText("STORE MANAGER", 20, 25, 20, cyan);
    drawText("Business Management", 20, 55, 12);

    for (int i = 0; i < 8; i++)
    {
        sf::RectangleShape item(sf::Vector2f(190, 44));

        item.setPosition(15, 100 + i * 60);

        if (page == i)
            item.setFillColor(sf::Color(0, 105, 130));
        else
            item.setFillColor(panel);

        window.draw(item);

        drawText(pages[i], 30, 112 + i * 60, 15);
    }

    drawText("ESC - Exit", 20, 650, 13);
}

void drawDashboard()
{
    drawText("Business Dashboard", 250, 25, 28);

    drawText("Overview of your store activity",
        250, 65, 14, sf::Color(150, 160, 175));

    const char* names[4] =
    {
        "Total Revenue",
        "Total Expenses",
        "Inventory Value",
        "Estimated Profit"
    };

    double values[4] =
    {
        revenue(),
        expensesTotal(),
        inventoryValue(),
        revenue() - expensesTotal()
    };

    for (int i = 0; i < 4; i++)
    {
        float x = 245 + i * 220;

        sf::RectangleShape card(sf::Vector2f(205, 120));

        card.setPosition(x, 115);
        card.setFillColor(panel);

        window.draw(card);

        drawText(names[i], x + 12, 135, 14);
        drawMoney(values[i], x + 12, 175);
    }

    sf::RectangleShape summary(sf::Vector2f(865, 280));

    summary.setPosition(245, 270);
    summary.setFillColor(panel);

    window.draw(summary);

    drawText("Store Statistics", 275, 290, 21, cyan);

    int counts[6] =
    {
        expenseCount,
        productCount,
        employeeCount,
        supplierCount,
        saleCount,
        deliveryCount
    };

    const char* names2[6] =
    {
        "Expenses",
        "Products",
        "Employees",
        "Suppliers",
        "Sales",
        "Deliveries"
    };

    for (int i = 0; i < 6; i++)
    {
        char n[20];

        numberText(counts[i], n);

        drawText(names2[i], 300, 345 + i * 30, 16);
        drawText(n, 550, 345 + i * 30, 16, cyan);
    }
}

void drawRecords()
{
    sf::RectangleShape area(sf::Vector2f(420, 465));

    area.setPosition(710, 110);
    area.setFillColor(panel);

    window.draw(area);

    drawText("Saved Records", 735, 130, 20, cyan);

    float y = 180;

    if (page == 1)
    {
        for (int i = 0; i < expenseCount && i < 12; i++)
        {
            drawText(expenseName[i], 735, y, 13);
            drawText(expenseCategory[i], 885, y, 13);
            drawMoney(expenseAmount[i], 1010, y);

            y += 30;
        }
    }
    else if (page == 2)
    {
        for (int i = 0; i < productCount && i < 12; i++)
        {
            char qty[20];

            numberText(productQuantity[i], qty);

            drawText(productName[i], 735, y, 14);
            drawText("Stock:", 970, y, 13);
            drawText(qty, 1030, y, 13, cyan);

            y += 30;
        }
    }
    else if (page == 3)
    {
        for (int i = 0; i < employeeCount && i < 12; i++)
        {
            drawText(employeeName[i], 735, y, 13);
            drawText(employeeRole[i], 890, y, 13);
            drawMoney(employeeSalary[i], 1010, y);

            y += 30;
        }
    }
    else if (page == 4)
    {
        for (int i = 0; i < supplierCount && i < 12; i++)
        {
            drawText(supplierName[i], 735, y, 13);
            drawText(supplierPhone[i], 875, y, 13);
            drawText(supplierItem[i], 1000, y, 13);

            y += 30;
        }
    }
    else if (page == 5)
    {
        for (int i = 0; i < saleCount && i < 12; i++)
        {
            drawText(saleProduct[i], 735, y, 14);
            drawMoney(saleTotal[i], 1000, y);

            y += 30;
        }
    }
    else if (page == 6)
    {
        for (int i = 0; i < deliveryCount && i < 12; i++)
        {
            drawText(deliveryProduct[i], 735, y, 13);
            drawText(deliveryDestination[i], 880, y, 13);
            drawMoney(deliveryCost[i], 1010, y);

            y += 30;
        }
    }
}

void drawForm()
{
    drawText(pages[page], 250, 25, 28);

    drawText("Enter information and save your record.",
        250, 65, 14, sf::Color(150, 160, 175));

    sf::RectangleShape form(sf::Vector2f(430, 465));

    form.setPosition(245, 110);
    form.setFillColor(panel);

    window.draw(form);

    for (int i = 0; i < fieldCount; i++)
    {
        drawText(labels[page][i],
            270, 140 + i * 75, 14);

        drawField(i);
    }

    drawButton(270, 480, 160, "SAVE RECORD");

    drawRecords();
}

void drawReports()
{
    drawText("Financial Reports", 250, 25, 28);

    sf::RectangleShape card(sf::Vector2f(850, 440));

    card.setPosition(245, 110);
    card.setFillColor(panel);

    window.draw(card);

    drawText("Total Sales Revenue", 285, 165, 19);
    drawMoney(revenue(), 650, 165);

    drawText("Total Expenses", 285, 235, 19);
    drawMoney(expensesTotal(), 650, 235);

    drawText("Inventory Value", 285, 305, 19);
    drawMoney(inventoryValue(), 650, 305);

    drawText("Estimated Profit", 285, 390, 22);
    drawMoney(revenue() - expensesTotal(), 650, 390);

    drawText("Profit = Revenue - Expenses",
        285, 465, 14, sf::Color(150, 160, 175));
}

//function to save records

void saveRecord()
{
    double a = 0;
    double b = 0;
    int quantity = 0;

    bool success = false;

    if (page == 1 && toNumber(fieldValue[2], a))
    {
        success = addExpense(
            fieldValue[0],
            fieldValue[1],
            a
        );
    }
    else if (page == 2 &&
        toInteger(fieldValue[1], quantity) &&
        toNumber(fieldValue[2], a) &&
        toNumber(fieldValue[3], b))
    {
        success = addProduct(
            fieldValue[0],
            quantity,
            a,
            b
        );
    }
    else if (page == 3 &&
        toNumber(fieldValue[2], a) &&
        toNumber(fieldValue[3], b))
    {
        success = addEmployee(
            fieldValue[0],
            fieldValue[1],
            a,
            b
        );
    }
    else if (page == 4)
    {
        success = addSupplier(
            fieldValue[0],
            fieldValue[1],
            fieldValue[2]
        );
    }
    else if (page == 5 &&
        toInteger(fieldValue[1], quantity))
    {
        success = addSale(fieldValue[0], quantity);
    }
    else if (page == 6 &&
        toInteger(fieldValue[2], quantity) &&
        toNumber(fieldValue[3], a))
    {
        success = addDelivery(
            fieldValue[0],
            fieldValue[1],
            quantity,
            a
        );
    }

    if (success)
    {
        if (saveData())
            setStatus("Record saved successfully!");
        else
            setStatus("Record added, but could not save to file!");

        clearAllFields();
    }
    else
    {
        setStatus("Invalid details, duplicate product, or insufficient stock!");
    }
}

//main starts here

int main()
{
    window.create(
        sf::VideoMode(1150, 700),
        "Store Management and Expense Tracker"
    );

    window.setFramerateLimit(60);

    if (!font.loadFromFile("C:/Windows/Fonts/arial.ttf"))
    {
        cout << "Font not found. Check Windows Arial font path.\n";
        return 1;
    }

    loadData();
    setupFields();

    //main application loop
    while (window.isOpen())
    {
        sf::Event event;

        //handle events
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::KeyPressed &&
                event.key.code == sf::Keyboard::Escape)
                window.close();

            sf::Vector2f mouse = window.mapPixelToCoords(
                sf::Mouse::getPosition(window)
            );

            //handle input fields
            for (int i = 0; i < fieldCount; i++)
                handleField(i, event, mouse);

            //handle mouse clicks
            if (event.type == sf::Event::MouseButtonPressed &&
                event.mouseButton.button == sf::Mouse::Left)
            {
                //navigation
                if (mouse.x >= 15 && mouse.x <= 205 &&
                    mouse.y >= 100 && mouse.y < 580)
                {
                    int selected = (int)((mouse.y - 100) / 60);

                    if (selected >= 0 && selected < 8)
                    {
                        page = selected;

                        setupFields();

                        setStatus("Section opened.");
                    }
                }

                //save button
                if (page >= 1 && page <= 6 &&
                    mouse.x >= 270 && mouse.x <= 430 &&
                    mouse.y >= 480 && mouse.y <= 522)
                {
                    saveRecord();
                }
            }
        }

        //drawing screen
        window.clear(background);

        drawSidebar();

        if (page == 0)
            drawDashboard();
        else if (page == 7)
            drawReports();
        else
            drawForm();

        drawText(status, 245, 660, 14, cyan);

        window.display();
    }

    saveData();

    return 0;
}
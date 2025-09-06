#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <stack>
#include <algorithm>
#include <stdexcept>
#include <cctype> // Для isdigit

// using namespace std; // Избегаем использования using namespace в заголовочных файлах

class ArithmeticTranslyator {
private:
    std::vector<std::string> data;
    // Удален неиспользуемый std::stack<std::string> st;
    const std::string operators = "+-*/()";
    const std::string numbers_chars = "0123456789."; // Переименовано для ясности

    // Вспомогательная функция для проверки, является ли токен одним из определенных операторов или скобок
    bool isOperator(const std::string& str) const {
        if (str.length() != 1) return false;
        return operators.find(str[0]) != std::string::npos;
    }

    // Вспомогательная функция для проверки, является ли токен бинарным оператором (+, -, *, /)
    bool isBinaryOperator(const std::string& str) const {
        if (str.length() != 1) return false;
        return str == "+" || str == "-" || str == "*" || str == "/";
    }

    // Вспомогательная функция для проверки, является ли токен числом (на основе логики парсинга)
    // Предполагается, что токены - это либо числа, либо односимвольные операторы/скобки
    bool isNumberToken(const std::string& str) const {
        // Если это не распознанный оператор/скобка, то это должен быть токен числа
        return !isOperator(str);
    }

    // Возвращает приоритет оператора
    int imp(const std::string& op) const {
        if (op == "+" || op == "-") return 1;
        if (op == "*" || op == "/") return 2;
        return 0; // Для скобок или других не-операторов
    }

public:
    // Конструктор принимает строку и парсит/валидирует её
    ArithmeticTranslyator(const std::string& str = "") { // Принятие по константной ссылке
        std::string currentToken;

        // --- Парсинг ---
        // Улучшенный парсинг для разделения чисел и операторов без пробелов (например, "3+4")
        for (char c : str) {
            if (std::isdigit(c) || c == '.') {
                currentToken += c;
            }
            else {
                if (!currentToken.empty()) {
                    data.push_back(currentToken);
                    currentToken.clear();
                }
                if (c != ' ') { // Игнорируем пробелы
                    // Если текущий символ - оператор/скобка, добавляем его как отдельный токен
                    data.push_back(std::string(1, c));
                }
            }
        }
        if (!currentToken.empty()) {
            data.push_back(currentToken);
        }
        // --- Конец Парсинга ---


        if (str.empty() || str.find_first_not_of(" ") == std::string::npos) {
            throw std::runtime_error("Неверное выражение: Входная строка пуста или содержит только пробелы.");
        }


        // Проверка 1: Начальный/Конечный токены
        // Предполагаем, что унарные операторы не поддерживаются
        if (isBinaryOperator(data.front())) {
            throw std::runtime_error("Неверное выражение: Начинается с бинарного оператора.");
        }
        if (data.front() == ")") {
            throw std::runtime_error("Неверное выражение: Начинается с закрывающей скобки.");
        }
        if (isBinaryOperator(data.back())) {
            throw std::runtime_error("Неверное выражение: Заканчивается бинарным оператором.");
        }
        if (data.back() == "(") {
            throw std::runtime_error("Неверное выражение: Заканчивается открывающей скобкой.");
        }
        // Заканчиваться десятичной точкой также является ошибкой, проверится при валидации точки ниже

        // Проверка 2: Пустые скобки ()
        for (size_t i = 0; i + 1 < data.size(); ++i) {
            if (data[i] == "(" && data[i + 1] == ")") {
                throw std::runtime_error("Неверное выражение: Пустые скобки.");
            }
        }

        // Проверка 3: Баланс скобок
        int cnt_balance = 0;
        for (const std::string& s : data) {
            if (s == "(") cnt_balance++;
            else if (s == ")") cnt_balance--;
            if (cnt_balance < 0) throw std::runtime_error("Неверное выражение: Несоответствие скобок (лишняя закрывающая скобка).");
        }
        if (cnt_balance != 0) throw std::runtime_error("Неверное выражение: Несоответствие скобок (незакрытая открывающая скобка).");

        // Проверка 4: Недопустимый формат числа с плавающей точкой и нераспознанные токены
        for (const std::string& s : data) {
            if (isNumberToken(s)) { // Только проверяем потенциальные числа
                size_t dot_pos = s.find('.');
                if (dot_pos != std::string::npos) { // Если есть точка
                    if (s.front() == '.' || s.back() == '.' || std::count(s.begin(), s.end(), '.') > 1) {
                        throw std::runtime_error("Неверное выражение: Недопустимый формат числа с плавающей точкой '" + s + "'.");
                    }
                    // Проверка, нет ли символов, кроме цифр и точки, в токене числа
                    for (char ch : s) {
                        if (!std::isdigit(ch) && ch != '.') {
                            // Эта ошибка, вероятно, означает сбой парсинга или нераспознанный токен
                            throw std::runtime_error("Неверное выражение: Недопустимый символ в токене числа '" + s + "'.");
                        }
                    }
                }
                else { // Если точки нет, проверяем, состоят ли они только из цифр
                    for (char ch : s) {
                        if (!std::isdigit(ch)) {
                            // Эта ошибка, вероятно, означает сбой парсинга или нераспознанный токен
                            throw std::runtime_error("Неверное выражение: Недопустимый символ в токене числа '" + s + "'.");
                        }
                    }
                }
            }
            else if (!isOperator(s)) {
                // Токен не является распознанным оператором/скобкой И не является числом (по нашим правилам)
                throw std::runtime_error("Неверное выражение: Нераспознанный токен '" + s + "'.");
            }
        }

        // Проверка 5: Валидация последовательности токенов
        // Это критически важно для надежности calculate(), учитывая отсутствие унарных операторов и неявного умножения
        for (size_t i = 0; i + 1 < data.size(); ++i) {
            const std::string& current = data[i];
            const std::string& next = data[i + 1];

            // Число, за которым следует открывающая скобка (например, 3(4+5)) - неявное умножение не поддерживается
            if (isNumberToken(current) && next == "(") {
                throw std::runtime_error("Неверное выражение: Число за открывающей скобкой (неявное умножение не поддерживается).");
            }

            // Закрывающая скобка, за которой следует число (например, (3+4)5) - неявное умножение не поддерживается
            if (current == ")" && isNumberToken(next)) {
                throw std::runtime_error("Неверное выражение: Закрывающая скобка за числом (неявное умножение не поддерживается).");
            }

            // Последовательные бинарные операторы (например, 3 + * 4)
            if (isBinaryOperator(current) && isBinaryOperator(next)) {
                throw std::runtime_error("Неверное выражение: Последовательные бинарные операторы.");
            }

            // Открывающая скобка, за которой следует бинарный оператор (например, ( + 3)) - предполагаем отсутствие унарных
            if (current == "(" && isBinaryOperator(next)) {
                throw std::runtime_error("Неверное выражение: Открывающая скобка за бинарным оператором (унарные операторы не поддерживаются).");
            }

            // Бинарный оператор, за которым следует закрывающая скобка (например, (3 + ))
            if (isBinaryOperator(current) && next == ")") {
                throw std::runtime_error("Неверное выражение: Бинарный оператор за закрывающей скобкой.");
            }

            // Закрывающая скобка, за которой следует открывающая скобка (например, (a)(b)) - неявное умножение не поддерживается
            if (current == ")" && next == "(") {
                throw std::runtime_error("Неверное выражение: Закрывающая скобка за открывающей скобкой (неявное умножение не поддерживается).");
            }
            // Число, за которым следует число (например, 3 4) - пробелы должны разделять, но два числа рядом - ошибка
            // Парсер выше этого избежит, но можно добавить как доп. проверку если парсинг изменится.
            // if (isNumberToken(current) && isNumberToken(next)) {
            //     throw std::runtime_error("Неверное выражение: Последовательные числа без оператора.");
            // }
        }
        // --- Конец Валидации ---
    }


    // Вычисляет значение выражения
    double calculate() const { // Добавлена константность
        if (data.empty()) {
            // Или вернуть 0.0? Зависит от желаемого поведения для пустой строки
            // Можно добавить проверку в printResult, чтобы не пытаться вычислять пустое
            throw std::runtime_error("Ошибка вычисления: Выражение пустое.");
        }

        std::stack<double> operands; // Стек операндов
        std::stack<std::string> operators; // Стек операторов

        // Лямбда-функция для выполнения операции
        auto counter = [&]() {
            if (operands.size() < 2) {
                // Это должно быть перехвачено валидацией, но служит страховкой
                throw std::runtime_error("Ошибка вычисления: Недостаточно операндов для оператора.");
            }
            double operand2 = operands.top(); operands.pop(); // Второй операнд (сверху стека)
            double operand1 = operands.top(); operands.pop(); // Первый операнд
            if (operators.empty()) {
                // Не должно произойти при стандартной оценке Shunting-Yard, если валидация прошла
                throw std::runtime_error("Ошибка вычисления: Стек операторов пуст.");
            }
            std::string op = operators.top(); operators.pop(); // Оператор

            // Выполнение операции
            if (op == "+") operands.push(operand1 + operand2);
            else if (op == "-") operands.push(operand1 - operand2);
            else if (op == "*") operands.push(operand1 * operand2);
            else if (op == "/") {
                if (operand2 == 0) throw std::runtime_error("Деление на ноль");
                operands.push(operand1 / operand2);
            }
            else {
                // Не должно произойти при валидных входных данных
                throw std::runtime_error("Ошибка вычисления: Неизвестный оператор '" + op + "'.");
            }
            };

        // Алгоритм Shunting-Yard для вычисления инфиксного выражения
        for (const std::string& i : data) {
            if (isNumberToken(i)) { // Если токен - число
                try {
                    operands.push(std::stod(i)); // Преобразуем в double и помещаем в стек операндов
                }
                catch (const std::exception& e) {
                    // Должно быть перехвачено валидацией в конструкторе, но хорошая защита
                    throw std::runtime_error("Ошибка вычисления: Не удалось преобразовать токен '" + i + "' в число.");
                }
            }
            else if (i == "(") { // Если открывающая скобка
                operators.push(i); // Помещаем в стек операторов
            }
            else if (i == ")") { // Если закрывающая скобка
                // Выполняем операции из стека операторов до первой открывающей скобки
                while (!operators.empty() && operators.top() != "(") {
                    counter();
                }
                if (operators.empty()) {
                    // Этот случай должен быть перехвачен валидацией баланса скобок
                    throw std::runtime_error("Ошибка вычисления: Несоответствие скобок (нет соответствующей открывающей скобки).");
                }
                operators.pop(); // Удаляем открывающую скобку из стека
            }
            else if (isBinaryOperator(i)) { // Если бинарный оператор
                // Выполняем операции из стека операторов с более высоким или равным приоритетом
                while (!operators.empty() && operators.top() != "(" && imp(operators.top()) >= imp(i)) {
                    counter();
                }
                operators.push(i); // Помещаем текущий оператор в стек
            }
            else {
                // Должно быть перехвачено валидацией в конструкторе
                throw std::runtime_error("Ошибка вычисления: Неожиданный токен '" + i + "'.");
            }
        }

        // После обработки всех токенов, выполняем оставшиеся операции в стеке операторов
        while (!operators.empty()) {
            if (operators.top() == "(") {
                // Этот случай должен быть перехвачен валидацией баланса скобок
                throw std::runtime_error("Ошибка вычисления: Несоответствие скобок (незакрытая открывающая скобка осталась в стеке).");
            }
            counter();
        }

        // В конце в стеке операндов должен остаться ровно один результат
        if (operands.size() != 1 || !operators.empty()) {
            throw std::runtime_error("Ошибка вычисления: Неверный формат выражения, приведший к некорректному состоянию стеков.");
        }

        return operands.top(); // Возвращаем финальный результат
    }

    // Перегрузка оператора << для вывода токенизированного выражения
    friend std::ostream& operator<<(std::ostream& ostr, const ArithmeticTranslyator& other) {
        // Проверяем, пуст ли вектор data перед выводом
        if (other.data.empty()) {
            ostr << "[Пустое выражение]" << std::endl;
        }
        else {
            for (const auto& item : other.data) {
                ostr << item << " ";
            }
            ostr << std::endl;
        }
        return ostr;
    }

    // Метод для вывода результата вычисления с обработкой ошибок
    void printResult() const { // Добавлена константность

        try {
            std::cout << calculate() << std::endl;
        }
        catch (const std::runtime_error& error) {
            std::cerr << "Ошибка: " << error.what() << std::endl;
        }
        catch (const std::exception& error) {
            // Перехватываем другие потенциальные исключения (например, bad_alloc)
            std::cerr << "Неожиданная ошибка: " << error.what() << std::endl;
        }
    }

    // Опционально: Добавить геттер для токенизированных данных для тестирования/проверки
    const std::vector<std::string>& getTokens() const {
        return data;
    }
};



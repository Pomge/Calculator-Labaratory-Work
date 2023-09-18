//#include <QDebug>
#include <QStack>
#include "mainwindow.h"
#include "ui_mainwindow.h"

void expression_in_bracket_remover(QString &expression, int expression_length);

double memory = 0.0;

int left_brackets = 0;
int right_brackets = 0;

bool is_ready_for_next_number = false;
bool is_after_blocking = false;

QStack<Leksema> expression_stack;


//void print_stack(QStack<Leksema> &stack) {
//    if (stack.isEmpty()) {
//        return;
//    }

//    Leksema item = stack.pop();
//    print_stack(stack);

//    if (item.type == '0') {
//        qInfo() << '\t' << item.value;
//    } else {
//        qInfo() << '\t' << item.type;
//    }

//    stack.push(item);
//}

void reverse_stack(QStack<Leksema> &stack) {
    QStack<Leksema> reversed_stack;

    while (!stack.isEmpty()) {
        Leksema item = stack.pop();
        reversed_stack.push(item);
    }

    stack = reversed_stack;
}


void add_number_in_stack(QStack<Leksema> &numbers_stack, double number) {
    Leksema item = { '0', number };
    numbers_stack.push(item);
}

void add_operation_in_stack(QStack<Leksema> &operations_stack, QChar operation_character) {
    Leksema item = { operation_character, 0.0 };
    operations_stack.push(item);
}


bool is_operation(QChar character) {
    return character == '/' ||
            character == '*' ||
            character == '-' ||
            character == '+' ||
            character == "÷" ||
            character == "×" ||
            character == "−" ||
            character == "+";
}

int get_operation_rang(QChar operation_character) {
    if (operation_character == '/' || operation_character == '*') {
        return 2;
    } else if (operation_character == '-' || operation_character == '+') {
        return 1;
    } else {
        return 0;
    }
}

QChar get_operation_char(QString operation) {
    if (operation == "÷") {
        return '/';
    } else if (operation == "×") {
        return '*';
    } else if (operation == "−") {
        return '-';
    } else if (operation == "+") {
        return '+';
    }
    return '_';
}


void format_expression_stack(QStack<Leksema> &stack) {
    Leksema last_item = stack.top();
    if (is_operation(last_item.type)) {
        stack.pop();
    }

    int brackets_difference = left_brackets - right_brackets;
    if (brackets_difference != 0) {
        for (int i = 0; i < brackets_difference; i++) {
            add_operation_in_stack(stack, ')');
        }
    }
}

double get_number_from_string(int *i, QString string) {
    QString number_string = "";
    int string_length = string.length();

    bool expression;
    if (*i == 0) {
        expression = string[*i].isDigit() || string[*i] == "−" || string[*i] == ',';
    } else {
        expression = string[*i].isDigit() || string[*i] == ',';
    }

    while (expression) {
        number_string.append(string[*i]);
        if (*i + 1 < string_length) {
            (*i)++;
        } else {
            break;
        }
        expression = string[*i].isDigit() || string[*i] == ',';
    }
    number_string.replace(',', '.');
    double result = number_string.toDouble();

    return result;
}


double divide_function(double divisible, double divider) {
    return divisible / divider;
}

double multiply_function(double multiplier_1, double multiplier_2) {
    return multiplier_1 * multiplier_2;
}

double minus_function(double reduced, double subtracted) {
    return reduced - subtracted;
}

double plus_function(double summand_1, double summand_2) {
    return summand_1 + summand_2;
}


MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);

    connect(ui->pushButton_0, SIGNAL(clicked()), this, SLOT(on_pushButton_digit_clicked()));
    connect(ui->pushButton_1, SIGNAL(clicked()), this, SLOT(on_pushButton_digit_clicked()));
    connect(ui->pushButton_2, SIGNAL(clicked()), this, SLOT(on_pushButton_digit_clicked()));
    connect(ui->pushButton_3, SIGNAL(clicked()), this, SLOT(on_pushButton_digit_clicked()));
    connect(ui->pushButton_4, SIGNAL(clicked()), this, SLOT(on_pushButton_digit_clicked()));
    connect(ui->pushButton_5, SIGNAL(clicked()), this, SLOT(on_pushButton_digit_clicked()));
    connect(ui->pushButton_6, SIGNAL(clicked()), this, SLOT(on_pushButton_digit_clicked()));
    connect(ui->pushButton_7, SIGNAL(clicked()), this, SLOT(on_pushButton_digit_clicked()));
    connect(ui->pushButton_8, SIGNAL(clicked()), this, SLOT(on_pushButton_digit_clicked()));
    connect(ui->pushButton_9, SIGNAL(clicked()), this, SLOT(on_pushButton_digit_clicked()));

    connect(ui->pushButton_divide, SIGNAL(clicked()), this, SLOT(on_pushButton_math_operations_clicked()));
    connect(ui->pushButton_multiply, SIGNAL(clicked()), this, SLOT(on_pushButton_math_operations_clicked()));
    connect(ui->pushButton_minus, SIGNAL(clicked()), this, SLOT(on_pushButton_math_operations_clicked()));
    connect(ui->pushButton_plus, SIGNAL(clicked()), this, SLOT(on_pushButton_math_operations_clicked()));
}

MainWindow::~MainWindow() {
    delete ui;
}


void MainWindow::mousePressEvent(QMouseEvent *event) {
    QPointF local_position = event->localPos();
    if ((local_position.y() > 0.0 && local_position.y() < 30.0)  &&
            (local_position.x() > 0.0 && local_position.x() < 190.0)) {
        previous_position = event->globalPos();
    } else {
        previous_position = { -300, 0 };
    }
}

void MainWindow::mouseMoveEvent(QMouseEvent *event) {
    QPointF local_position = event->localPos();
    if ((local_position.y() > 0.0 && local_position.y() < 30.0)  &&
            (local_position.x() > 0.0 && local_position.x() < 190.0) &&
            previous_position.x() != -300) {
        QPoint current_position = event->globalPos();
        QPoint delta = current_position - previous_position;
        move(x() + delta.x(), y() + delta.y());
        previous_position = event->globalPos();
    }
}


void MainWindow::on_pushButton_digit_clicked() {
    if (is_after_blocking) {
        block_ui(false);
        is_after_blocking = false;
    }

    QPushButton *q_push_button = (QPushButton *) sender();

    if (is_ready_for_next_number) {
        ui->label_entry->setText("0");
        is_ready_for_next_number = false;
    }

    QString current_digit = q_push_button->text();
    QString previous_digits = ui->label_entry->text();

    int previous_digits_length = previous_digits.length();
    if (previous_digits_length + 1 < 13) {
        if (previous_digits_length == 1 && previous_digits == "0") {
            ui->label_entry->setText(current_digit);
        } else {
            QString expression = ui->label_expression->text();
            int expression_length = expression.length();
            if (expression_length > 1 && expression[expression_length - 1] == ")") {
                expression_in_bracket_remover(expression, expression_length);
                ui->label_expression->setText(expression);
                ui->label_entry->setText(current_digit);
            } else {
                QString entry = previous_digits.append(current_digit);
                ui->label_entry->setText(entry);
            }
        }
    }
}

void MainWindow::on_pushButton_math_operations_clicked() {
    QPushButton *q_push_button = (QPushButton *) sender();
    QString operation = q_push_button->text();

    QString expression = ui->label_expression->text();
    int expression_length = expression.length();

    if (expression_length > 0) {
        QChar last_character = expression[expression_length - 1];
        if (is_ready_for_next_number && is_operation(last_character)) {
            expression = expression.mid(0, expression_length - 1) + operation;

            expression_stack.pop();
            add_operation_in_stack(expression_stack, get_operation_char(operation));
        } else if (expression[expression_length - 1] != ")") {
            QString current_number = ui->label_entry->text();
            expression += current_number + operation;

            double current_number_double = current_number.replace(',', '.').toDouble();
            add_number_in_stack(expression_stack, current_number_double);
            add_operation_in_stack(expression_stack, get_operation_char(operation));
        } else {
            expression += operation;
            add_operation_in_stack(expression_stack, get_operation_char(operation));
        }
    } else {
        QString current_number = ui->label_entry->text();
        expression = current_number + operation;

        QString current_number_stack(current_number);
        current_number_stack.replace(',', '.');
        double current_number_double = current_number_stack.toDouble();
        add_number_in_stack(expression_stack, current_number_double);
        add_operation_in_stack(expression_stack, get_operation_char(operation));
    }
    ui->label_expression->setText(expression);

    //    qInfo() << "print_stack(expression_stack) {";
    //    print_stack(expression_stack);
    //    qInfo() << "}\n";
    calculate_expression();
    is_ready_for_next_number = true;
}

void MainWindow::on_pushButton_plus_minus_clicked() {
    QString minus = ui->pushButton_minus->text();
    QString current_number = ui->label_entry->text();
    int current_number_length = current_number.length();

    if (current_number != "0") {
        if (current_number[0] == minus) {
            ui->label_entry->setText(current_number.mid(1, current_number_length - 1));
        } else {
            ui->label_entry->setText(minus + current_number);
        }
    }
}

void MainWindow::on_pushButton_dot_clicked() {
    QString current_number = ui->label_entry->text();

    if (!current_number.contains(',')) {
        QString entry = current_number + ',';
        ui->label_entry->setText(entry);
    }
}

void MainWindow::on_pushButton_equally_clicked() {
    QString expression = ui->label_expression->text();
    int expression_length = expression.length();

    if (expression_length > 0) {
        QChar last_character = expression[expression_length - 1];

        if (is_operation(last_character)) {
            QString current_number = ui->label_entry->text();
            double current_number_double = current_number.replace(',', '.').toDouble();
            add_number_in_stack(expression_stack, current_number_double);
        }

//        qInfo() << "expression_stack {";
//        print_stack(expression_stack);
//        qInfo() << "}";

        calculate_expression();
        expression_stack.clear();
        ui->label_expression->setText("");
    }
}


void MainWindow::on_pushButton_clear_entry_clicked() {
    if (is_after_blocking) {
        block_ui(false);
        is_after_blocking = false;
    }
    ui->label_entry->setText("0");
}

void MainWindow::on_pushButton_clear_clicked() {
    if (is_after_blocking) {
        block_ui(false);
        is_after_blocking = false;
    }

    on_pushButton_clear_entry_clicked();
    ui->label_expression->setText("");
    left_brackets = 0;
    right_brackets = 0;
    expression_stack.clear();

    brackets_difference_counter();
}

void MainWindow::on_pushButton_erase_clicked() {
    if (is_after_blocking) {
        block_ui(false);
        is_after_blocking = false;
    }

    QString current_number = ui->label_entry->text();

    int current_number_length = current_number.length();
    if (current_number_length > 1) {
        QString entry = current_number.mid(0, current_number_length - 1);
        ui->label_entry->setText(entry);
    } else {
        ui->label_entry->setText("0");
    }
}


void MainWindow::brackets_difference_counter() {
    int brackets_difference = left_brackets - right_brackets;

    if (brackets_difference > 0) {
        QString text = QString::number(brackets_difference);
        ui->label_brackets_difference_counter->setText(text);
    } else {
        ui->label_brackets_difference_counter->setText("");
    }
}

void expression_in_bracket_remover(QString &expression, int expression_length) {
    int right_bracket_counter = 0;
    int i = expression_length - 1;
    while (expression[i] == ')') {
        right_bracket_counter++;
        expression_stack.pop();
        i--;
    }

    int k = 0;
    while (k != right_bracket_counter) {
        Leksema item = expression_stack.pop();
        if (item.type == ')') {
            right_bracket_counter++;
        }
        if (item.type == '(') {
            k++;
        }
    }

    left_brackets -= (right_bracket_counter - 1);
    right_brackets -= right_bracket_counter;
    int last_index_left_bracket = 0;
    for (int i = expression_length; i > 0; i--) {
        if (expression[i] == '(') {
            right_bracket_counter--;
            if (right_bracket_counter == 0) {
                last_index_left_bracket = i;
                break;
            }
        }
    }
    expression = expression.mid(0, last_index_left_bracket);
    left_brackets--;
}

void MainWindow::on_pushButton_left_bracket_clicked() {
    QString expression = ui->label_expression->text();
    int expression_length = expression.length();

    if (expression_length > 0 && expression[expression_length - 1] == ')') {
        expression_in_bracket_remover(expression, expression_length);
    }

    expression.append('(');
    add_operation_in_stack(expression_stack, '(');
    ui->label_expression->setText(expression);
    left_brackets++;

    brackets_difference_counter();
    //    qInfo() << "left_brackets:" << left_brackets;
    //    qInfo() << "right_brackets:" << right_brackets;
    //    qInfo() << "print_stack(expression_stack) {";
    //    print_stack(expression_stack);
    //    qInfo() << "}\n";
}

void MainWindow::on_pushButton_right_bracket_clicked() {
    if (left_brackets - right_brackets > 0) {
        QString current_number = ui->label_entry->text();
        QString current_number_stack(current_number);
        current_number_stack.replace(',', '.');
        double current_number_double = current_number_stack.toDouble();

        QString expression = ui->label_expression->text();
        int expression_length = expression.length();
        QChar last_character = expression[expression_length - 1];

        if (right_brackets == 0) {
            expression.append(current_number);
            add_number_in_stack(expression_stack, current_number_double);
        } else if (is_operation(last_character) || last_character == '(') {
            expression.append(current_number);
            add_number_in_stack(expression_stack, current_number_double);
        }

        expression.append(')');
        add_operation_in_stack(expression_stack, ')');
        ui->label_expression->setText(expression);
        right_brackets++;

//        qInfo() << "left_brackets:" << left_brackets;
//        qInfo() << "right_brackets:" << right_brackets;
//        qInfo() << "print_stack(expression_stack) {";
//        print_stack(expression_stack);
//        qInfo() << "}\n";
        calculate_expression();
    }
    brackets_difference_counter();
}


void MainWindow::block_ui(bool block) {
    QFont font = ui->label_entry->font();

    if (block) {
        is_after_blocking = true;
        font.setPointSize(11);
        ui->label_entry->setText("Division by zero is not possible");
    } else {
        font.setPointSize(24);
        ui->label_entry->setText("0");
        ui->label_expression->setText("");
    }

    ui->pushButton_divide->setEnabled(!block);
    ui->pushButton_multiply->setEnabled(!block);
    ui->pushButton_plus->setEnabled(!block);
    ui->pushButton_minus->setEnabled(!block);

    ui->pushButton_plus_minus->setEnabled(!block);
    ui->pushButton_dot->setEnabled(!block);
    ui->pushButton_equally->setEnabled(!block);

    ui->pushButton_left_bracket->setEnabled(!block);
    ui->pushButton_right_bracket->setEnabled(!block);



    ui->label_entry->setFont(font);
}

void MainWindow::division_by_zero(QStack<Leksema> &numbers_stack, QStack<Leksema> &operations_stack) {
    expression_stack.clear();
    numbers_stack.clear();
    operations_stack.clear();

    block_ui(true);
}

void MainWindow::calculate_expression(QStack<Leksema> &numbers_stack, QStack<Leksema> &operations_stack) {
    //    qInfo() << "calculate {";

    //    qInfo() << "numbers_stack {";
    //    print_stack(numbers_stack);
    //    qInfo() << "}";

    //    qInfo() << "operations_stack {";
    //    print_stack(operations_stack);
    //    qInfo() << "}";

    double number_2 = numbers_stack.pop().value;
    double number_1 = numbers_stack.pop().value;
    double result = 0;

    QChar operation_character = operations_stack.pop().type;
    if (operation_character == '/') {
        if (number_2 == 0.0) {
            division_by_zero(numbers_stack, operations_stack);
            return;
        } else {
            result = divide_function(number_1, number_2);
        }
    } else if (operation_character == '*') {
        result = multiply_function(number_1, number_2);
    } else if (operation_character == '-') {
        result = minus_function(number_1, number_2);
    } else if (operation_character == '+') {
        result = plus_function(number_1, number_2);
    }
    add_number_in_stack(numbers_stack, result);
    //    qInfo() << "result:" << result;


    //    qInfo() << "numbers_stack {";
    //    print_stack(numbers_stack);
    //    qInfo() << "}";

    //    qInfo() << "operations_stack {";
    //    print_stack(operations_stack);
    //    qInfo() << "}";

    //    qInfo() << "}";
}

void MainWindow::calculate_expression() {
    QStack<Leksema> formated_expression_stack(expression_stack);
    format_expression_stack(formated_expression_stack);
    reverse_stack(formated_expression_stack);

    QStack<Leksema> numbers_stack;
    QStack<Leksema> operations_stack;

    while (!formated_expression_stack.isEmpty()) {
        Leksema current_item = formated_expression_stack.pop();
        QChar current_item_type = current_item.type;

        if (current_item_type == '0') {
            add_number_in_stack(numbers_stack, current_item.value);
        } else {
            if (current_item_type == '(') {
                add_operation_in_stack(operations_stack, current_item_type);
            } else if (operations_stack.isEmpty()) {
                add_operation_in_stack(operations_stack, current_item_type);
            } else {
                Leksema last_operation = operations_stack.top();
                QChar last_operation_type = last_operation.type;

                int current_operation_rang = get_operation_rang(current_item_type);
                int last_operation_rang = get_operation_rang(last_operation_type);

                if (current_item_type == ')') {
                    while (last_operation_type != '(') {
                        calculate_expression(numbers_stack, operations_stack);
                        last_operation = operations_stack.top();
                        last_operation_type = last_operation.type;
                    }
                    operations_stack.pop();
                } else if (current_operation_rang > last_operation_rang) {
                    add_operation_in_stack(operations_stack, current_item_type);
                } else {
                    while (current_operation_rang <= last_operation_rang) {
                        calculate_expression(numbers_stack, operations_stack);

                        if (operations_stack.isEmpty()) {
                            break;
                        }

                        last_operation = operations_stack.top();
                        last_operation_type = last_operation.type;
                        last_operation_rang = get_operation_rang(last_operation_type);
                    }
                    add_operation_in_stack(operations_stack, current_item_type);
                }
            }
        }
    }

    while (!operations_stack.isEmpty()) {
        calculate_expression(numbers_stack, operations_stack);
    }

    if (!numbers_stack.isEmpty()) {
        double result = numbers_stack.top().value;
        QString result_string = QString::number(result, 'g', 10);
        result_string.replace('.', ',');
        if (result < 0.0) {
            result_string.replace('-', "−");
        }
        ui->label_entry->setText(result_string);
    }
}


void MainWindow::on_pushButton_memory_clear_clicked() {
    memory = 0.0;
    ui->pushButton_memory_clear->setEnabled(false);
    ui->pushButton_memory_result->setEnabled(false);
}

void MainWindow::on_pushButton_memory_result_clicked() {
    QString memory_string = QString::number(memory, 'g', 10);
    memory_string.replace('.', ',');
    memory_string.replace('-', "−");
    ui->label_entry->setText(memory_string);
}

void MainWindow::on_pushButton_memory_add_clicked() {
    QString current_number = ui->label_entry->text();

    double current_number_double = current_number.replace(',', '.').toDouble();
    memory += current_number_double;

    ui->pushButton_memory_clear->setEnabled(true);
    ui->pushButton_memory_result->setEnabled(true);
}

void MainWindow::on_pushButton_memory_subtract_clicked() {
    QString current_number = ui->label_entry->text();

    double current_number_double = current_number.replace(',', '.').toDouble();
    memory -= current_number_double;

    ui->pushButton_memory_clear->setEnabled(true);
    ui->pushButton_memory_result->setEnabled(true);
}


void MainWindow::on_pushButton_close_clicked() {
    QCoreApplication::quit();
}

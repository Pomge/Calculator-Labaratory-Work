#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMouseEvent>
#include <QPoint>

void expression_in_bracket_remover(QString *expression, int expression_length);

struct Leksema {
    QChar type;
    double value;
};

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

    QPoint previous_position = { -300, 0 };
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);

private slots:
    void on_pushButton_digit_clicked();
    void on_pushButton_math_operations_clicked();
    void on_pushButton_plus_minus_clicked();
    void on_pushButton_dot_clicked();
    void on_pushButton_equally_clicked();

    void on_pushButton_clear_entry_clicked();
    void on_pushButton_clear_clicked();
    void on_pushButton_erase_clicked();

    void brackets_difference_counter();
    void on_pushButton_left_bracket_clicked();
    void on_pushButton_right_bracket_clicked();

    void block_ui(bool block);
    void division_by_zero(QStack<Leksema> &numbers_stack, QStack<Leksema> &operations_stack);
    void calculate_expression(QStack<Leksema> &numbers_stack, QStack<Leksema> &operations_stack);
    void calculate_expression();

    void on_pushButton_memory_clear_clicked();
    void on_pushButton_memory_result_clicked();
    void on_pushButton_memory_add_clicked();
    void on_pushButton_memory_subtract_clicked();

    void on_pushButton_close_clicked();
};
#endif // MAINWINDOW_H

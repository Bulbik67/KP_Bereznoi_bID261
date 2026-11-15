/*
 * Курсовой проект. Подзадача 1. Вариант 3.
 * Студент: Бережной П. А.
 *
 * Y(x) = |sin^2(x) - (1/4)*cos^3(x)|
 *
 *        | 4*ln(2.3x - 1),    x > 2.5
 * V(x) = | 1 + 3*sin|1 - x|,   0 <= x <= 2.5
 *        | x^7,               x < 0
 */

#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <math.h>
#include <locale.h>
#include <float.h>

#define EPS         1e-9   
#define MAX_POINTS  500    
#define GRAPH_WIDTH 60     


double calc_y(double x);
double calc_v(double x);
int check_y_domain(double x);
int check_v_domain(double x);
int is_defined(int f_num, double x);
int check_y_limits(double x);
int check_v_limits(double x);
int in_limits(int f_num, double x);
int can_evaluate(int f_num, double x);
double get_result(int f_num, double x);
int count_points(double begin, double end, double h);
int get_first(int type);
int get_last(int type, double begin, double end, double h, int n);
double get_min(int f_num, double begin, double h, int first, int last);
double get_max(int f_num, double begin, double h, int first, int last);
int to_col(double y, double y_min, double y_max, int width);
int get_zero_col(double y_min, double y_max, int width);
char get_symbol(int col, int dot_col, int zero_col);

int main(void)
{
    int f_num, task, range_type;
    double x_start, x_end, step_x;
    double x_val, y_val, y_min, y_max;
    int n, i, first, last, col, dot_col, zero_col;
    int ch;

    setlocale(LC_CTYPE, "Russian");

    do
    {
        printf("\nМеню выбора функции \n");
        printf("1 - Y(x) = |sin^2(x) - 0.25*cos^3(x)|\n");
        printf("2 - V(x) - функция с условиями\n");
        printf("0 - Выход из программы\n");
        printf("Введите номер: ");

        if (scanf("%d", &f_num) != 1)
        {
            printf("Ошибка! Введите цифру.\n");
            while ((ch = getchar()) != '\n' && ch != EOF);
            if (ch == EOF) return 0;
            f_num = -1;
            continue;
        }

        switch (f_num)
        {
        case 0:
            printf("Программа завершена.\n");
            break;
        case 1:
        case 2:
            break;
        default:
            printf("Такого пункта нет.\n");
            continue;
        }

        if (f_num == 0) break;

        printf("\nВыберите действие для функции %s:\n", (f_num == 1) ? "Y(x)" : "V(x)");
        printf("1 - Найти значение в точке\n");
        printf("2 - Вывести таблицу\n");
        printf("3 - Нарисовать график\n");
        printf("Ваш выбор: ");

        if (scanf("%d", &task) != 1)
        {
            printf("Ошибка ввода.\n");
            while ((ch = getchar()) != '\n' && ch != EOF);
            if (ch == EOF) return 0;
            continue;
        }

        if (task < 1 || task > 3)
        {
            printf("Неверный пункт меню.\n");
            continue;
        }

        if (task == 1)
        {
            printf("Введите x: ");
            if (scanf("%lf", &x_val) != 1)
            {
                printf("Ошибка: нужно число.\n");
                while ((ch = getchar()) != '\n' && ch != EOF);
                if (ch == EOF) return 0;
                continue;
            }
            if (!isfinite(x_val))
            {
                printf("Ошибка: x должно быть конечным.\n");
                continue;
            }

            if (!is_defined(f_num, x_val))
                printf("Функция не существует в точке x = %g\n", x_val);
            else if (!in_limits(f_num, x_val))
                printf("Результат слишком большой для double (переполнение)\n");
            else
                printf("%s(x) = %.6f при x = %g\n",
                    (f_num == 1) ? "Y" : "V", get_result(f_num, x_val), x_val);
            continue;
        }

       
        printf("\nТип промежутка:\n");
        printf("1 - [a, b] (отрезок)\n");
        printf("2 - (a, b) (интервал)\n");
        printf("3 - [a, b) (полуинтервал)\n");
        printf("4 - (a, b] (полуинтервал)\n");
        printf("Выбор: ");
        if (scanf("%d", &range_type) != 1)
        {
            printf("Ошибка ввода.\n");
            while ((ch = getchar()) != '\n' && ch != EOF);
            if (ch == EOF) return 0;
            continue;
        }
        if (range_type < 1 || range_type > 4)
        {
            printf("Нет такого типа.\n");
            continue;
        }

        printf("Введите начало (a) и конец (b) через пробел: ");
        if (scanf("%lf %lf", &x_start, &x_end) != 2)
        {
            printf("Ошибка: нужно два числа.\n");
            while ((ch = getchar()) != '\n' && ch != EOF);
            if (ch == EOF) return 0;
            continue;
        }

        printf("Введите шаг: ");
        if (scanf("%lf", &step_x) != 1)
        {
            printf("Ошибка: нужно число.\n");
            while ((ch = getchar()) != '\n' && ch != EOF);
            if (ch == EOF) return 0;
            continue;
        }

        if (!isfinite(x_start) || !isfinite(x_end) || !isfinite(step_x))
        {
            printf("Ошибка: введены не числа.\n");
            continue;
        }
        if (x_start >= x_end)
        {
            printf("Ошибка: начало должно быть меньше конца.\n");
            continue;
        }
        if (step_x <= 0.0)
        {
            printf("Ошибка: шаг должен быть положительным.\n");
            continue;
        }

        n = count_points(x_start, x_end, step_x);
        if (n > MAX_POINTS)
        {
            printf("Слишком много точек. Увеличьте шаг.\n");
            continue;
        }

        first = get_first(range_type);
        last = get_last(range_type, x_start, x_end, step_x, n);

        if (first > last)
        {
            printf("В этом промежутке нет точек с таким шагом.\n");
            continue;
        }

        if (task == 2) 
        {
            printf("\nТАБЛИЦА ЗНАЧЕНИЙ %s \n", (f_num == 1) ? "Y(x)" : "V(x)");
            printf("-------------------------------\n");
            printf("|     x      |   Значение    |\n");
            printf("-------------------------------\n");

            for (i = first; i <= last; i++)
            {
                x_val = x_start + i * step_x;
                if (!is_defined(f_num, x_val))
                {
                    printf("| %10.4f | %13s |\n", x_val, "не определена");
                }
                else if (!in_limits(f_num, x_val))
                {
                    printf("| %10.4f | %13s |\n", x_val, "переполнение");
                }
                else
                {
                    y_val = get_result(f_num, x_val);
                    printf("| %10.4f | %13.6f |\n", x_val, y_val);
                }
            }
            printf("-------------------------------\n");
        }
        else 
        {
            y_min = get_min(f_num, x_start, step_x, first, last);
            y_max = get_max(f_num, x_start, step_x, first, last);

            if (y_min > y_max)
            {
                printf("Нельзя построить график: функция везде не определена.\n");
                continue;
            }

            zero_col = get_zero_col(y_min, y_max, GRAPH_WIDTH);

            printf("\nГРАФИК %s\n", (f_num == 1) ? "Y(x)" : "V(x)");
            printf("Y меняется от %.4f до %.4f\n", y_min, y_max);
            printf("Ось Y обозначена как '|', точки - '*'\n\n");

            for (i = first; i <= last; i++)
            {
                x_val = x_start + i * step_x;
                if (can_evaluate(f_num, x_val))
                    dot_col = to_col(get_result(f_num, x_val), y_min, y_max, GRAPH_WIDTH);
                else
                    dot_col = -1;

                printf("%10.4f :", x_val);
                for (col = 0; col < GRAPH_WIDTH; col++)
                    putchar(get_symbol(col, dot_col, zero_col));
                printf("\n");
            }
        }

    } while (f_num != 0);

    return 0;
}



double calc_y(double x)
{
    double s = sin(x);
    double c = cos(x);
    return fabs(s * s - 0.25 * c * c * c);
}

double calc_v(double x)
{
    double res;
    if (x > 2.5)
        res = 4.0 * log(2.3 * x - 1.0);
    else if (x >= 0.0 && x <= 2.5)
        res = 1.0 + 3.0 * sin(fabs(1.0 - x));
    else
        res = pow(x, 7.0);
    return res;
}


int check_y_domain(double x)
{
    return 1; 
}

int check_v_domain(double x)
{
    int ok = 1;
    if (x > 2.5)
    {
        
        if (2.3 * x - 1.0 <= EPS)
            ok = 0;
    }
    return ok;
}

int is_defined(int f_num, double x)
{
    if (f_num == 1) return check_y_domain(x);
    else return check_v_domain(x);
}



int check_y_limits(double x)
{
    return 1; 
}

int check_v_limits(double x)
{
    int ok = 1;
    if (x < 0.0)
    {
        
        if (fabs(x) > 1000.0) 
            ok = 0;
    }
    return ok;
}

int in_limits(int f_num, double x)
{
    if (f_num == 1) return check_y_limits(x);
    else return check_v_limits(x);
}

int can_evaluate(int f_num, double x)
{
    return is_defined(f_num, x) && in_limits(f_num, x);
}

double get_result(int f_num, double x)
{
    if (f_num == 1) return calc_y(x);
    else return calc_v(x);
}



int count_points(double begin, double end, double h)
{
    double steps = (end - begin) / h + EPS;
    if (!isfinite(steps) || steps >= MAX_POINTS)
        return MAX_POINTS + 1;
    return (int)steps + 1;
}

int get_first(int type)
{
    return (type == 2 || type == 4) ? 1 : 0;
}

int get_last(int type, double begin, double end, double h, int n)
{
    int last = n - 1;
    if ((type == 2 || type == 3) && fabs(begin + (n - 1) * h - end) < h * 1e-6)
        last = n - 2;
    return last;
}



double get_min(int f_num, double begin, double h, int first, int last)
{
    double min_val = DBL_MAX;
    double x, y;
    for (int i = first; i <= last; i++)
    {
        x = begin + i * h;
        if (can_evaluate(f_num, x))
        {
            y = get_result(f_num, x);
            if (y < min_val) min_val = y;
        }
    }
    return min_val;
}

double get_max(int f_num, double begin, double h, int first, int last)
{
    double max_val = -DBL_MAX;
    double x, y;
    for (int i = first; i <= last; i++)
    {
        x = begin + i * h;
        if (can_evaluate(f_num, x))
        {
            y = get_result(f_num, x);
            if (y > max_val) max_val = y;
        }
    }
    return max_val;
}



int to_col(double y, double y_min, double y_max, int width)
{
    if (y_max * 0.5 - y_min * 0.5 < EPS)
        return width / 2;
    return (int)((y * 0.5 - y_min * 0.5) / (y_max * 0.5 - y_min * 0.5) * (width - 1) + 0.5);
}

int get_zero_col(double y_min, double y_max, int width)
{
    if (y_min > 0.0 || y_max < 0.0)
        return -1;
    return to_col(0.0, y_min, y_max, width);
}

char get_symbol(int col, int dot_col, int zero_col)
{
    if (col == dot_col) return '*';
    if (col == zero_col) return '|';
    return ' ';
}
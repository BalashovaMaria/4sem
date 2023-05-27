#include <Python.h>
/*Создание дубликата массива*/
static float *dublicate(float *a, int a_size){
    /*Выделение памяти под заданный размер для хранения коэффициентов*/
    float *b;
    b = (float*)malloc(a_size * sizeof(float));
    int i;
    /*Копируем каждый коэф-т и записываем в массив*/
    for (i = 0; i<a_size; i++){
        b[i] = a[i];
    }
    /*Возвращаем полученный массив*/
    return b;
}
/*Вывод полинома в консоль, передаем массив с коэф-ами и общую длину массива*/
static void print_poly(float *a, int a_size){
    /*Выводим в цикле строку вида ax^i+*/
    int i;
    for(i = 0; i<a_size-1; i++){
        printf("%fx^%i+",a[i],a_size-i-1);
    }
    /*Выводим вне цикла чтобы в конце не написать +*/
    printf("%fx^%i",a[a_size-1],0);
}
/*Построение следующей строки таблицы по предыдущей*/
static float *horner_step(float *a, int a_size, float x){
    float *b;
    b = (float*)malloc(a_size * sizeof(float));
    b[0] = a[0];
    int i;
    for(i = 1; i<a_size; i++){
        b[i] = b[i-1]*x+a[i];
    }
    return b;
}
/*Нахождение значения полинома в точке*/
static void find_value_c(float *a, int a_size, float x){
    /*шаг метода Горнера для поиска значения полинома*/
    float *b;
    b = horner_step(a,a_size,x);
    /*Вывод результатов*/
    printf("\nPolynom \n");
    print_poly(a,a_size);
    printf("\nin point %f has value %f\n",x,b[a_size-1]);
    free(b);
    return;
}

/*Поиск целых корней полинома*/
void find_roots_c(float *aa, int a_size){
    float *a = dublicate(aa,a_size);
    printf("\nFor polynom \n");
    print_poly(a,a_size);
    printf("\n");
    /*Отдельная проверка на корень равный 0*/
    if (a[a_size-1] == 0){
        printf("0 is a root!\n");
    }
    float *b;
    /*Бежим по всем числам от 1 до модуля свободного члена*/
    int i;
    int c = abs((int)(a[a_size-1]));
    for(i = 1; i<=c; i++){
        /*Если нашли делитель свободного члена, то делаем с ним шаг метода Горнера*/
        if (c % i == 0){
            b = horner_step(a,a_size,i);
            /*Проверяем, если получили 0, то выводим информацию о корне и обновляем текущую таблицу*/
            if (b[a_size-1] == 0){
                int j;
                for (j = 0; j<a_size; j++){
                    a[i] = b[i];
                }
                printf("%i is a root!\n",i);
                a_size--;
            }
            free(b);
            /*То же самое, только для отрицательного делителя*/
            b = horner_step(a,a_size,-i);
            if (b[a_size-1] == 0){
                int j;
                for (j = 0; j<a_size; j++){
                    a[i] = b[i];
                }
                printf("%i is a root!\n",-i);
                a_size--;
            }
            free(b);
        }
    }
    free(a);
    printf("No other roots\n");
    return;
}
/*Поиск частного полинома и монома */
void divisor_c(float *a, int a_size, float n){
    //Делаем шаг, тогда в последней ячейке хранится остаток, а в остальных - частное
    float *b = horner_step(a,a_size,-n);
    //Выводим результат
    printf("\n");
    print_poly(a,a_size);
    printf(" =\n = (x+%f)(",n);
    print_poly(b,a_size-1);
    printf(") + %f\n",b[a_size-1]);
    free(b);
    return;
}
/*Поиск значения производных всех порядков в точке*/
void find_diff_c(float *aa, int a_size, float x){
    float *a = dublicate(aa,a_size);
    printf("\nFor polynom\n");
    print_poly(a,a_size);
    printf("\nThe following derivatives were found:\n");
    /*Коэффициент чтобы не считать факториал с нуля*/
    int mult = 1;
    /*Первый шаг пропускаем, так как это значение самого полинома (0 производная)*/
    a = horner_step(a,a_size,x);
    /*Каждый шаг цикла делаем шаг по методу Горнера и выдаем значение очередной производной*/
    int i;
    for(i = 1; i<a_size;i++){
        a = horner_step(a,a_size-i,x);
        /*Не забываем обновлять текущее значение факториала*/
        mult = mult*i;
        printf("the %ith derivative is %f\n",i,a[a_size-1-i]*mult);
    }
    free(a);
}

/* определение функции-модуля для Python */
static PyObject* find_value(PyObject* self, PyObject* args)
{
    float x;
    PyObject * listObj;
    /* получение аргументов из Python*/
    if(!PyArg_ParseTuple(args, "fO", &x, &listObj))
        return NULL;
    /* Преобзарование списка из питона в массив си */
    int i;
    int a_size = PyList_Size(listObj);
    float *a;
    a = (float*)malloc(a_size * sizeof(float));
    for (i = 0; i< a_size; i++){
        PyObject* temp = PyList_GetItem(listObj, i);
        a[i] = PyFloat_AsDouble(temp);
    }
    /*Запуск функции */
    find_value_c(a,a_size,x);
    free(a);
    Py_INCREF(Py_None);
    return Py_None;
}

static PyObject* find_roots(PyObject* self, PyObject* args)
{
    PyObject * listObj;
    /* получение аргументов из Python*/
    if(!PyArg_ParseTuple(args, "O", &listObj))
        return NULL;
    /* Преобзарование списка из питона в массив си */
    int i;
    int a_size = PyList_Size(listObj);
    float *a;
    a = (float*)malloc(a_size * sizeof(float));
    for (i = 0; i< a_size; i++){
        PyObject* temp = PyList_GetItem(listObj, i);
        a[i] = PyFloat_AsDouble(temp);
    }
    /*Запуск функции */
    find_roots_c(a,a_size);
    free(a);
    Py_INCREF(Py_None);
    return Py_None;
}

static PyObject* divisor(PyObject* self, PyObject* args)
{
    float n;
    PyObject * listObj;
    /* получение аргументов из Python*/
    if(!PyArg_ParseTuple(args, "fO", &n, &listObj))
        return NULL;
    /* Преобзарование списка из питона в массив си */
    int i;
    int a_size = PyList_Size(listObj);
    float *a;
    a = (float*)malloc(a_size * sizeof(float));
    for (i = 0; i< a_size; i++){
        PyObject* temp = PyList_GetItem(listObj, i);
        a[i] = PyFloat_AsDouble(temp);
    }
    /*Запуск функции */
    divisor_c(a,a_size,n);
    free(a);
    Py_INCREF(Py_None);
    return Py_None;
}
static PyObject* find_diff(PyObject* self, PyObject* args)
{
    float x;
    PyObject * listObj;
    /* получение аргументов из Python*/
    if(!PyArg_ParseTuple(args, "fO", &x, &listObj))
        return NULL;
    /* Преобзарование списка из питона в массив си */
    int i;
    int a_size = PyList_Size(listObj);
    float *a;
    a = (float*)malloc(a_size * sizeof(float));
    for (i = 0; i< a_size; i++){
        PyObject* temp = PyList_GetItem(listObj, i);
        a[i] = PyFloat_AsDouble(temp);
    }
    /*Запуск функции */
    find_diff_c(a,a_size,x);
    free(a);
    Py_INCREF(Py_None);
    return Py_None;
}

/* список функций модуля*/
static PyMethodDef horner_methods[] = {
    {"find_roots", find_roots, METH_VARARGS, "Поиск целых корней полинома"},
    {"find_value", find_value, METH_VARARGS, "Поиск значения полинома"},
    {"divisor", divisor, METH_VARARGS, "Поиск целых корней полинома"},
    {"find_diff", find_diff, METH_VARARGS, "Поиск значений производных в точке"},
    {NULL, NULL, 0, NULL}
};

/* инициализация модуля */
static struct PyModuleDef horner_module = {
    PyModuleDef_HEAD_INIT,
    "horner",
    "",
    -1,
    horner_methods
};

PyMODINIT_FUNC PyInit_horner(void)
{
    return PyModule_Create(&horner_module);
}

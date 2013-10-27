#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <memory>
#include <iostream>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    int array[25];
    std::vector<int> s;
    auto x = s.begin();

    std::iota(std::begin(array), std::end(array), 1);
    int array2[25];
    std::iota(std::begin(array2), std::end(array2), 15);

    int array3[25];
    std::transform(std::begin(array), std::end(array), std::begin(array2), std::begin(array3), [] (int a, int b) { return a+b; } );

    for(int i : array3)
    {
        std::cout << i << " ";
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}


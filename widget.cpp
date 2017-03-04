#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

    this->setWindowTitle("File Splitter");

    settings =new QSettings("LonelyYorsh","FileSplitter",this);

    //Get values from settings, second arguement is default value
    ui->lineEdit_Input->setText(settings->value("inputPath","input.json").toString());
    ui->lineEdit_Output->setText(settings->value("outputPath","output.json").toString());
    ui->spinBox->setValue(settings->value("linesPerFile",100).toInt());
}

Widget::~Widget()
{
    delete ui;
}

void Widget::on_pushButton_Input_clicked()
{
    QString file = QFileDialog::getOpenFileName(this, tr("Load ..."),ui->lineEdit_Input->text(),tr("text file (*.*)"));

    if(file.isEmpty()) {
        // Error ...
        return;
    }

    //Set the text of the line edit
    ui->lineEdit_Input->setText(file);
}

void Widget::on_pushButton_Output_clicked()
{
    QString file = QFileDialog::getSaveFileName(this, tr("Save as ..."),ui->lineEdit_Output->text(),tr("text file (*.*)"));

    if(file.isEmpty()) {
        // Error ...
        return;
    }

    //Set the text of the line edit
    ui->lineEdit_Output->setText(file);
}


void Widget::on_pushButton_Split_clicked()
{
    settings->setValue("inputPath",ui->lineEdit_Input->text());
    settings->setValue("outputPath",ui->lineEdit_Output->text());
    settings->setValue("linesPerFile",ui->spinBox->value());

    Split();
}

void Widget::Split()
{
    QFile inputFile(ui->lineEdit_Input->text());
    QString outputPath = ui->lineEdit_Output->text();

    //Split the string at each . to remove the extension
    QStringList list=outputPath.split(".",QString::SkipEmptyParts);
    if(list.length()<2)
    {
        QMessageBox::critical(this,"Error","Incorrect output path.\nMake sure the file name has an extension");
        return;
    }

    //Remake the beggining of the path
    outputPath=QString();
    for(int i=0;i<list.length()-1;i++)
    {
        if(i>0)
            outputPath+=".";
        outputPath+=list.at(i);
    }
    //Make the end of the path
    outputPath+="%1."+list.at(list.length()-1);

    if (!inputFile.open(QIODevice::ReadOnly))
    {
        QMessageBox::critical(this,"Error","Couldn't open input file.\nMake sure the path to the input file is correct.");
        return;
    }
    int linesPerFile = ui->spinBox->value();

    //If of the output file
    ulong fileId = 0;

    QTextStream in(&inputFile);
    while (!in.atEnd())
    {
        //Read a block from the input file
        QString block = QString();
        for(int i=0; i<linesPerFile;i++)
        {
            if(in.atEnd())
                break;
            if(i>0)
                block+="\n";
            block += in.readLine();
        }

        //Find this output file name
        QString currentOutputPath = outputPath.arg(fileId,3,10,QLatin1Char('0'));
        QFile outputFile(currentOutputPath);

        //Write this block to this output file
        if ( outputFile.open(QIODevice::WriteOnly) )
        {
            QTextStream stream( &outputFile );
            stream << block;
            outputFile.close();
        }

        //Go to the next output file
        fileId++;

    }
    inputFile.close();

    QMessageBox::information(this,"Done","Split in "+QString::number(fileId)+" files with success !");
}

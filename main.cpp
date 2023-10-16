#include <iostream>
#include <cstdlib>
#include <string>
#include <time.h>
#include <conio.h>
#include <fstream>
#include <windows.h>


using namespace std;

HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE); // used for gotoXY allows to print on-screen in the designed space
COORD CursorPosition; // used for goto
void gotoXY(int,int); // function defined below

void gotoXY(int x, int y)
{
	CursorPosition.X = x;
	CursorPosition.Y = y;
	SetConsoleCursorPosition(console,CursorPosition);
}

int checkingList(string array_ID [], string array_name [], double array_price[], int &tot) // check item list available in the list item file for a max of 1000 items
{
    string ID, name;   // variable used to read from item list file as strings
    double price; // variable used to read from item list file as double to calculate the price


    ifstream iFile; // open item list file
    iFile.open("ItemsList.txt");
    while(iFile >> ID >> name >> price) //reading from file
    {
        tot++;          // Counting number of items in the file
        array_ID[tot]=ID; //passing product ID
        array_name[tot]=name; // passing product Name
        array_price[tot]=price; //passing product Price

    };
    iFile.close();
    return (tot); // return number of items in the list
}

int invoiceCount(int Nrec)// allow to count the number of invoices and purchases  previously made also when the program is closed, count is kept in the rcN.txt file
{
    int nub;
    ifstream iFile;   //open the file rcN where invoice number is stored
    iFile.open("rcN.txt");
    iFile >>nub;  //reading the current invoice number and assign it to nub
    Nrec=nub+1; // after reading the current invoice number, add 1
    iFile.close();

    ofstream (output);
    output.open ("rcN.txt");
    output<<Nrec<<endl; //replace the old invoice number with the new one.
    output.close();
    return(Nrec); //return Nerc used to save the current invoice file using the current number as index

}

double Finsert(string array_ID [], string array_name [], double array_price[], int tot, int &recepit, int &screen, int &screenPointer, int &input)//after checking the list and user selected to scan items, read of input and tot price
{
    char ii='y';
    int control=0;
    string items;
    double TotalPrice=0;
    // screen increments in order to print the welcome message in the right place when users press next customer
    gotoXY(18,screen++); cout <<" *************************************************************"<<endl;
    gotoXY(18,screen++); cout <<" *        WELCOME TO HERTS SUPERMARKET CHECKOUT SYSTEM       *"<<endl;
    gotoXY(18,screen++); cout <<" *              Check out system: SCAN ITEMS                 *"<<endl;
    gotoXY(18,screen++); cout <<" *************************************************************"<<endl;

    ofstream output;
    output.open("ItemSelected.txt"); //open file  text item selected to insert the current customer items
    output<<recepit<<endl;

    while (ii!='x')
    {
        control=0;
        gotoXY(22,input++);cout<<"Please insert the product ID (Type F to exit): ";
        cin>>items;

        for (int j=0;j<=tot;j++)
        {
            if(items == array_ID[j])
            {
                TotalPrice+=array_price[j];// price calculation
                gotoXY(20,screenPointer+2);cout<<"                                                                                         "<<endl;
                gotoXY(20,screenPointer+2);cout<<" Item ID: "<<array_ID[j]<<" Item Name: "<<array_name [j]<<" Item Price: "<<array_price[j]<<endl;
                output<<array_ID[j]<<" "<<array_name [j]<<" "<<array_price[j]<<endl;
                control=1;
            }
        }

        if (control!=1)
        {
            control=0;
            if(items == "f" || items == "F" )
            {
                ii='x';
                gotoXY(22,input++); cout<<"Total Price >>: "<<TotalPrice<<endl;
            }else
            {
                gotoXY(20,screenPointer+2);cout<<"                                                         "<<endl;
                gotoXY(20,screenPointer+2);cout<<"*You have inserted the wrong ID, follow the the next step"<<endl;
                ii='y';
            }
        }
    }
    output.close();
    return(TotalPrice);
}

double Tprice(double TotalPrice, double &TotalCahIN, int &screen, int &screenPointer, int &input, int &recepit)// price input check
{
    double cashin,income,change; int i=0; bool out=true;
    TotalCahIN=0;
    gotoXY(22,input++);cout<<"Insert cash: ";
    cin>>change;
    TotalCahIN=change;
    while(cin.fail())
    {
        gotoXY(22,input++); cout << "Error you must to enter a number: ";
        cin.clear();
        cin.ignore(256,'\n');
        cin >> change;
    }


    while(i==0)
    {

        if(change>=TotalPrice)
        {
            change=change-TotalPrice;
            gotoXY(22,input++);cout<<"Change: "<<change;
            i=1;
        }else
        {

            while(change<TotalPrice)
            {
                income=change-TotalPrice;
                gotoXY(22,input++);cout<<"Cash left: "<<income<<endl;
                gotoXY(22,input++);cout<<"Insert cash in: ";
                cin>>cashin;
                TotalCahIN+=cashin;
                change+=cashin;
            }
            gotoXY(22,input++); cout<<"TOT Cash received: "<<TotalCahIN<<endl;
            i=0;
        }
    }
    return(change);
}

void invoice( double printPrice, double TotalPrice,int tot, double &TotalCahIN, string &invoN, int &recepit) // invoice processing in file folder
{
    string ID, name,RecepitN,price;   //assuming it is string
    int items=0;
    string array_ID[100],array_price[100], array_name[100];

    ifstream iFile;
    iFile.open("ItemSelected.txt");

    iFile>>RecepitN;// get the 1st line, the receipt number in the Item select text file and read it as string
    invoN="Invoice#"+RecepitN+".txt"; // assign to invoN the name of the file receipt using the current number

    while(iFile >> ID >> name >> price) //reading from file
    {
        items++;        // allows the input storage into the arrays of the items selected from the selectedItems text file.
        for (int i=0; i<items;i++)
        {
            array_ID[i]=ID; //passing product ID
            array_name[i]=name; // passing product Name
            array_price[i]=price; //passing product Price
        }
    };
    iFile.close();

    ofstream output;
    output.open(invoN.c_str()); //generate a file  using the invoN variable to name the receipt in the folder

    output <<"           ******************************"<<endl;
    output <<"           *  |WELCOME TO HERTS MARKET| *"<<endl;
    output <<"           *  |     ITEMS SELCETED    | *"<<endl;
    output <<"           ******************************"<<endl;

    output<<"                Invoice #: "<<RecepitN<<endl;
    output<<" ITEMS ID"<<"   ITEM Name  "<<"  ITME Price "<<endl;

    for (int j=0;j<=items;j++)
    {
        output<<array_ID[j]<<"  "<<array_name [j]<<"  "<<array_price[j]<<endl;
    }

    output<<" Total Price: "<<TotalPrice<<"\n Total Cash in: "<<TotalCahIN<<"\n Change: "<<printPrice<<endl;
    output<<"\n Thank your for shopping with HERTS MARKET."<<"\n HAVE A NICE DAY! "<<endl;
    output.close();
}

int NewItems(int confirm, string array_ID[], int tot)// user selected to insert a new item
{
    string name,ID;
    double price; bool in=true, check=false;

    gotoXY(18,3); cout <<" *************************************************************"<<endl;
    gotoXY(18,4); cout <<" *        WELCOME TO HERTS SUPERMARKET CHECKOUT SYSTEM       *"<<endl;
    gotoXY(18,5); cout <<" *       NEW ITEM SYSTEM : follow the instruction below      *"<<endl;
    gotoXY(18,6); cout <<" *************************************************************"<<endl;

    while(in==true)//loop to verify input with existing items
    {

        gotoXY(20,13);cout<<" Insert the item ID: ";
        getline(cin,ID);
        for (int i=0;i<tot;i++) // checking the list total items compared with ID input
        {
            if(array_ID[i]==ID)
            {
                check=true;  //exist an item with same ID
            }
        }
        if(check==true) //check existing items in the list before inserting new items
            {
                gotoXY(20,13);cout<<"                                                                       ";//clear input line
                gotoXY(20,10);cout<<"# The ID item already exist, insert the the correct ID of the product #";//clear input line
                check=false; //check existing items in the list before inserting new items
            }else
            {in=false;}
    }

    gotoXY(20,10);cout<<"                                                                       ";//  clears error input check line
    gotoXY(20,10);cout<<" Item ID: "<<ID;
    gotoXY(20,13);cout<<"                                                                       "; // clears the input for the name of the item
    gotoXY(20,13);cout<<" Insert the item Name: ";  getline(cin,name);
    gotoXY(20,10);cout<<"                                                                       ";//  clears error input check line
    gotoXY(20,10);cout<<" Item Name: "<<name;
    gotoXY(20,13);cout<<"                                                                ";//clears the line for the price input
    gotoXY(20,13);cout<<" Insert the item Price: "; cin>>price;
     while(cin.fail())  //if cin is not a number to the following
    {
        gotoXY(22,10); cout << "# Error you must to enter a number #";
        cin.clear();
        cin.ignore(256,'\n');
        gotoXY(20,13);cout<<"                                    "; //clears the line for the price input
        gotoXY(20,13);cout<<" Insert the item Price: "; cin>>price;
        gotoXY(22,10); cout << "                                   "; //clears the line for the price input
    }

    gotoXY(20,10);cout<<"                                                                            ";//  clears error input check line
    gotoXY(20,10);cout<<" Item ID: "<<ID<<" Item Name: "<<name<<" Item Price: "<<price<<endl;

    ofstream output;
    output.open("ItemsList.txt", ios::out | ios::app);
    output<<ID<<" "<<name<<" "<<price<<"\n";
    output.close();
    confirm=1;
    return(confirm);
}

int menuOption(int NewProdcut,int &screen, int &input) // 1st menu option
{
    int  x=7,check=0;
    bool running=true;

    cout<<endl;
    gotoXY(18,screen=screen++); cout <<" ****************************************************"<<endl;
    gotoXY(18,screen=screen++); cout <<" *   WELCOME TO HERTS SUPERMARKET CHECKOUT SYSTEM   *"<<endl;
    gotoXY(18,screen=screen++); cout <<" * To select an option: use keyboard arrows         *"<<endl;
    gotoXY(18,screen=screen++); cout <<" ****************************************************"<<endl;
    cout<<endl;

    gotoXY(20,7); cout << "->"; // used as design as selector

    while(running){


        gotoXY(22,7);  cout << " INSERT A NEW PRDUCT  TO THE LIST ";
        gotoXY(22,8);  cout << " SCAN ITEMS, check out system";

        system("pause>nul"); // the >nul bit causes it the print no message

        if(GetAsyncKeyState(VK_DOWN) && x != 8) //down button pressed
			{

				gotoXY(20,x); cout << "  ";
				x++;
				check=1;
				gotoXY(20,x); cout << "->";
				running=true;

			}
        if(GetAsyncKeyState(VK_UP) && x != 7) //up button pressed
			{
				gotoXY(20,x); cout << "  ";
				x--; check=0;
				gotoXY(20,x); cout << "->";
				running=true;
			}
        if(GetAsyncKeyState(VK_RETURN))
            {
                if(check==0)
                {
                    NewProdcut=0;
                    } else
                        {
                            NewProdcut=1;
                        }
                gotoXY(20,7); cout <<"                                               ";
                gotoXY(20,8); cout <<"                                               ";
                running=false;

            }

}
screen=3; //in order to print the next welcome to the correct place
return(NewProdcut);

}



int main()
{
    string invoN, array_ID[100], array_name[100];   //assuming it is string
    int numb=0, tot=0, Nrec,recepit, confirm=0,added,screen=3;
    double  array_price[100],TotalPrice, printPrice=0,TotalCahIN=0;
    char nextcustomer='y',invocieR='n';
    int NewProdcut=0, screenPointer, input;
    int nulla;
    checkingList(array_ID,array_name,array_price,tot);// scan the list of items to obtain the number of items
    NewProdcut= menuOption(NewProdcut,screen, recepit); // depending of the user choose, the result is assigned to new product
    //checkingList(array_ID,array_name,array_price,tot);// scan the list of items to obtain the number of items in case user adds new item

    if (NewProdcut==1) // if Newpruduct = 1 means the user selected the scan checkout system
    {
        while(nextcustomer=='y'||nextcustomer=='Y') // variable = to true for the 1st loop. loop for next customer. if user insert != y loops ends
        {

            recepit=invoiceCount(Nrec); // call the function to check the invoice number and assign it to receipt variable
            screenPointer=screen+4; // screen pointer is used to print on screen item selected or error item not found
            input=screenPointer+5; //input allows to print on screen the request to insert the item ID after the screen pointer
            TotalPrice= Finsert (array_ID,array_name,array_price,tot,recepit,screen,screenPointer,input); //receive items input and assign the total price to TotalPrice var
            printPrice=Tprice(TotalPrice,TotalCahIN,screen,screenPointer,input,recepit);// calculation of the amount received and assigned to print price
            screen=input+1;
            gotoXY(22,screen++);cout<<"(Purchase #: "<<recepit<<" will be saved in a file automatically)";
            gotoXY(22,screen++);cout<<" To print the receipt, type y: ";
            cin>>invocieR;

            invoice(printPrice, TotalPrice, tot, TotalCahIN,invoN,recepit); //function to generate the invoice

            if (invocieR=='y'|| invocieR=='Y')
            {
                ShellExecute(NULL, "open", invoN.c_str(), NULL, NULL, SW_SHOWDEFAULT); //allows to open the current receipt number if user press y
            }

            gotoXY(22,screen+2);cout<<" Next customer, type 'y': ";
            cin>>nextcustomer;
            gotoXY(22,screen++);cout<<"\n  ";
        }
        screen+=2;
        gotoXY(22,screen++);cout<<" Thank your for shopping with HERTS MARKET.";
        gotoXY(22,screen++);cout<<" HAVE A NICE DAY! \n"<<endl;
    }else
    {
        added=NewItems(confirm,array_ID,tot); //new item function if user selects to insert a new item  and NewProdcut!=1. the result is assign to added variable to print the following statement
        if(added==1)
        {
            gotoXY(20,13);cout<<"                                                                       "; // clears the input for the name of the item
            gotoXY(20,13);cout<<"ITEM ADDED";
            gotoXY(20,15);cout<<"THANK YOU";
        }
        else
        {
            gotoXY(20,13);cout<<"Item not ADDED";
        }

    }

    return 0;
}

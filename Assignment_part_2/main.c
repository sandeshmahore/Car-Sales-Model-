#define _CRT_SECURE_NO_WARNINGS  // so that visual studio can work with scanf 


// this eneables us to work with scanf

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// this variable will be given a number that will be given the most recent error no.
extern int errno;

// Car Sales
/* Constants */
#define DISCOUNT_PERCENTAGE 0.30f // this would refer to 30 percent discount 
#define MAX_SALES 50
#define CSV_FILE "document.csv"
#define bool unsigned char   // this can accept an unsigned char type value 
#define OPTION_BUY_A_CAR 'a'
#define OPTION_VIEW_CAR_SALES_STATISTICS 'b'
#define OPTION_VIEW_SALES_BETWEEN 'c'
#define OPTION_SHOW_CAR_TYPES 'd'
#define OPTION_SHOW_EMPLOYEE_DETAILS 'e'
#define OPTION_EXIT_CAR_STORE 'x'
#define TRUE 1
#define FALSE 0
#define FILE_OPENED 0
#define FILE_CLOSED 1
#define FILE_ERROR 2

/*variables*/

unsigned short availableCars = 50, carsRequired, carSold = 0, correct_input = FALSE, carTypeIndex, numberOfSales = 0, carAmountPerSale[MAX_SALES], typeOfCarPerSale[MAX_SALES];

float totalPrice = 0, totalTax = 0, totalSalesValue, carSales[MAX_SALES], carPrices[] = { 24000.0f ,23000.0f , 22000.0f };

bool discountGiven = 0, discountGivenPerSale[MAX_SALES];

char club_member, customerChoice, paymentMode, carTypes[][50] = { "Model Ev 3.0", "Model Ev 2.0 ","Model Ev 1.0" }, customerNames[MAX_SALES][201];



//will be holding the file stream once the opened 
FILE* file;
unsigned char fileStatus = FILE_CLOSED;


/*greeting the customer!*/

void clearScreen()
{
	system("cls");
}

struct employee_data {

	char employee_name[20], designation[20];
};


void pauseProgram(char customerChoice)
{
	if (customerChoice == OPTION_EXIT_CAR_STORE) {
		printf("press enter to exit... ");
	}
	else
	{
		printf("\n\n Press enter to return to the Menu...");
	}
	if (customerChoice != '_')
	{
		getchar();
	}

	getchar();
}

char getCharFromConsole(char message[201])
{
	char userInput;
	printf(message);
	scanf("\n%c", &userInput);
	return userInput;

}

unsigned short getUnsignedShortFromConsole(char message[201])
{
	unsigned short userInput;
	printf(message);
	scanf("\n%hd", &userInput);
	return userInput;
}


void swapUnsignedShort(unsigned short* a, unsigned short* b)
{
	unsigned short temp;
	temp = *a;
	*a = *b;
	*b = temp;

}

void swapBool(bool* a, bool* b) {
	bool temp;
	temp = *a;
	*a = *b;
	*b = temp;

}


FILE* createFile(char fileName[201])
{
	//open the file from writing 
	file = fopen(fileName, "w");
	if (file != NULL)
	{
		fclose(file);
	}
	return file;

}


void openFile(char fileName[201], char mode[4])
{
	file = fopen(fileName, mode);
	if (file == NULL)
	{
		if (createFile(fileName) == NULL)
		{
			fileStatus = FILE_ERROR;
		}
		else {
			openFile(fileName, mode);
		}
	}
	else {
		fileStatus = FILE_OPENED;
	}
}

void closeFile()
{
	if (fileStatus == FILE_OPENED) {
		fclose(file);
		fileStatus = FILE_CLOSED;
	}

}

void readDataFromFile()
{
	int lineCounter = 0;
	while (1)
	{
		unsigned short carAmountPerSaleValue = 0, typeOfCarPerSaleValue = 0, discountGivenPerSaleValue = 0;
		char customerNameValue[201] = "";
		int scanResult = fscanf(
			file,
			" %hd,%hd,%hd,%[^\n]s",
			&carAmountPerSaleValue,
			&typeOfCarPerSaleValue,
			&discountGivenPerSaleValue,
			&customerNameValue
		);


		//if we reach the end then stop the loop
		if (scanResult == EOF)
		{
			break;
		}

		carAmountPerSale[lineCounter] = carAmountPerSaleValue;
		typeOfCarPerSale[lineCounter] = typeOfCarPerSaleValue;
		discountGivenPerSale[lineCounter] = (bool)discountGivenPerSaleValue;
		// as we are working with strings we need to add string copy 
		strcpy(customerNames[lineCounter], customerNameValue);

		availableCars -= carAmountPerSaleValue;
		carSold += carAmountPerSaleValue;
		lineCounter++;

	}
	numberOfSales = lineCounter;
}

void getDataFromFile()
{
	openFile(CSV_FILE, "r");
	if (fileStatus == FILE_OPENED) {
		readDataFromFile();
	}
	else if (fileStatus == FILE_ERROR)
	{
		printf(" There was an error trying to read the file %s.", CSV_FILE);
		pauseProgram('_');
	}
	closeFile();
}

void writeDataToFile() {
	for (int i = 0; i < numberOfSales; i++) {

		char line[201];
		char data[50];
		_itoa((int)carAmountPerSale[i], data, 10);
		strcpy(line, data);
		strcat(line, ",");

		_itoa((int)typeOfCarPerSale[i], data, 50);
		strcat(line, data);
		strcat(line, ",");

		_itoa((int)discountGivenPerSale[i], data, 50);
		strcat(line, data);
		strcat(line, ",");

		strcat(line, customerNames[i]);
		fprintf(file, line);

		if (i < numberOfSales - 1) {

			fprintf(file, "\n");

		}
	}
}
void saveDataToFile() {
	openFile(CSV_FILE, "w");
	if (fileStatus == FILE_OPENED) {
		writeDataToFile();
	}
	else if (fileStatus == FILE_ERROR)
	{
		printf("There was an error trying to write to the file %s.", CSV_FILE);
		pauseProgram('_');
	}
	closeFile();
}


void menu_greet_Customer() {
	printf("Hello! Welcome to the Amazing Car Store ");
}

void menu_show_menu() {
	printf("Menu:\n\n");
	printf("a.Buy a Car/Cars\n\n");
	printf("b.View Car Sales Statistics\n\n");
	printf("c.View Sales between\n\n");
	printf("d.Show car types\n\n");
	printf("e.Show Employee Details\n\n");
	printf("x.Exit the Car Store\n\n");
}



void menu_show_car_types()
{
	{
		int numberOfCars = sizeof(carPrices) / sizeof(float);
		printf("Car Types:\n");
		for (int i = 0; i < numberOfCars; i++)
		{
			printf("%d - %s \n", i, carTypes[i]);
			printf("%d - %f \n",i,carPrices[i]);
			
		}
	}
}



float menu_applyDiscount(float totalPrice)
{
	return totalPrice * (1 - DISCOUNT_PERCENTAGE);
}

bool menu_check_If_Discount_is_needed(char paymentMode)
{
	if (paymentMode == 'c' || paymentMode == 'C')
	{
		//changing the value of discountGiven so that it can indicate that we have given a certain amount of discount
		return TRUE;
	}
	else {
		club_member = getCharFromConsole("Are you a member of Car Club?Answer 'y' or 'n' : ");

		if (club_member == 'y' || club_member == 'Y') {
			return TRUE;
		}
	}

	return FALSE;

}

void menu_print_discount_outcome(bool dicountGiven) {
	switch (discountGiven)
	{

	case TRUE:
		printf("You get a discount ");
		break;

	case FALSE:
		printf("No discount given");
		break;
	}
}

void menu_buy_cars()
{
	printf("Buy Amazing Car/Cars!");
	printf("\n");
	printf("What is your name? Name: ");
	printf("\n");

	scanf("\n%[^\n]s", &customerNames[numberOfSales]);

	/*Asking the user how many cars does he wants*/
	printf("\n");
	printf("\n");
	printf("Hello , How many cars do you want?");
	scanf("%hd", &carsRequired);
	printf("\n");
	printf("\n");


	if (availableCars > 0)
	{
		printf("There are %hd cars available", availableCars);
		printf("\n");
	}
	else
	{
		printf("We are so sorry!, No more cars are available");
		//after this statement works we can terminatew our code 
		return;
	}


	if (availableCars < carsRequired)
	{
		printf("\n\n");

		printf("Sorry We have a less amount of cars as per your requirement!");
		return;
	}

	carAmountPerSale[numberOfSales] = carsRequired;

	menu_show_car_types();

	carTypeIndex = getUnsignedShortFromConsole("\nWhat type of Model do you need ? Choose from above");

	typeOfCarPerSale[numberOfSales] = carTypeIndex;

	totalPrice = carsRequired * carPrices[carTypeIndex];

	availableCars -= carsRequired;
	carSold += carsRequired;

	clearScreen();
	printf("\n");
	paymentMode = getCharFromConsole("Are you paying by cash ? Answer(C for Cash / X for Cheque) = ");

	discountGiven = menu_check_If_Discount_is_needed(paymentMode);

	if (discountGiven == TRUE)
	{
		totalPrice = menu_applyDiscount(totalPrice);
	}


	discountGivenPerSale[numberOfSales] = (unsigned char)discountGiven;
	menu_print_discount_outcome(discountGiven);
	//present the outcome 
	printf("No. of Cars bought equals to %hd ", carsRequired);
	printf("\n");
	printf("Total amount that you need to pay is GBP %f", totalPrice);
	printf("\n");
	printf("\n\n No. of cars left to sell is %hd", availableCars);
	numberOfSales++;
}

void sortArraysByTheNumberOfCarsSoldPerSale()
{
	for (int i = 0; i < numberOfSales - 1; i++)
	{

		for (int j = i + 1; j < numberOfSales; j++)
		{
			if (carAmountPerSale[i] > carAmountPerSale[j])
			{

				swapUnsignedShort(&carAmountPerSale[i], &carAmountPerSale[j]);
				swapUnsignedShort(&carAmountPerSale[i], &carAmountPerSale[j]);
				swapBool(&discountGivenPerSale[i], &discountGivenPerSale[j]);
				char temp[201];
				strcpy(temp, customerNames[i]);
				strcpy(customerNames[i], customerNames[j]);
				strcpy(customerNames[j], temp);


			}

		}
		

	} 

}

void printSalesDataAtPosition(int position)
{
	int typeOfCar = typeOfCarPerSale[position];
	float price = carAmountPerSale[position] * carPrices[typeOfCar];
	char discountGivenText[4];
	if (discountGivenPerSale[position] == TRUE)
	{
		strcpy(discountGivenText, "Yes");
		price *= (1 - DISCOUNT_PERCENTAGE);
	}
	else
	{
		strcpy(discountGivenText, "No");
	}
	printf(" Sale Index :%d|Sale Amount: %f| Type of Car: %s|"
		"Car Price: %f| Number Of cars: %hd |"
		" DiscountGiven : %s | Customer Name : %s\n",
		position, price, carTypes[typeOfCar],
		carPrices[typeOfCar], carAmountPerSale[position],
		discountGivenText, customerNames[position]);

}

void menu_view_sales()
{

	printf("All sales data:");
	float totalSalesValue = 0;


	for (int i = 0; i < numberOfSales; i++)
	{
		printSalesDataAtPosition(i);

		float price = carAmountPerSale[i] * carPrices[i];
		totalSalesValue += price;

	}
	printf("\n\nThe total number of Cars Sold is %hd at a value of %.2f GBP.(without Discount)", carSold, totalSalesValue);
	printf("\n\n No. of cars left to sell is %hd", availableCars);

}

void salesDataBasedOnCarsSoldBetweenMinMax(unsigned short min, unsigned short max)
{
	printf("Sales Data containing between %hd and %hd cars sold: \n\n", min, max);

	for (int i = 0; i < numberOfSales; i++)
	{
		if (min <= carAmountPerSale[i] && carAmountPerSale[i] <= max)
		{
			printSalesDataAtPosition(i);

		}
	}
}

void menu_viewSalesBetween() {

	sortArraysByTheNumberOfCarsSoldPerSale();
	unsigned short minCarsSold = getUnsignedShortFromConsole("What's the minimum number of Cars sold you are interested in? Number = ");
	unsigned short maxCarsSold = getUnsignedShortFromConsole("What is the maximum number of Cars sold you are interested in? Number = ");

	salesDataBasedOnCarsSoldBetweenMinMax(minCarsSold, maxCarsSold);
}

void menu_option_employee_details() 
{

	printf("Employee list:\n\n");
	struct employee_data detail1;
	// employee information
	strcpy(detail1.employee_name,"Rajesh Singhania");
	strcpy(detail1.designation, "Sr. Sales Manager");

	printf("Name = %s",detail1.employee_name);
	printf("\n");
	printf("Designation = %s",detail1.designation);
	printf("\n");
	printf("Contact number = +44 7895623542");
	printf("\n\n");
	// employee #2
	strcpy(detail1.employee_name, "Angela Walker");
	strcpy(detail1.designation, "Sr.Executive");

	printf("Name = %s", detail1.employee_name);
	printf("\n");
	printf("Designation = %s", detail1.designation);
	printf("\n");
	printf("Contact number = +44 7584689254");
	printf("\n\n");
	//employee #3
	strcpy(detail1.employee_name, "Ian Choudhary");
	strcpy(detail1.designation, "Cashier");

	printf("Name = %s", detail1.employee_name);
	printf("\n");
	printf("Designation = %s", detail1.designation);
	printf("\n");
	printf("Contact number = +44 7458963256");
	printf("\n\n");
	//emploee #4
	strcpy(detail1.employee_name, "Jason Holder");
	strcpy(detail1.designation, "Technician");

	printf("Name = %s", detail1.employee_name);
	printf("\n");
	printf("Designation = %s", detail1.designation);
	printf("\n");
	printf("Contact number = +44 7878545963");
	printf("\n\n");

}

void menu_exit() {
	saveDataToFile();

	printf("Thankyou for Visting! Come again!\n\n ");
	printf("Thankyou for using our services!\n\n");
}

void main()
{
	getDataFromFile();

	do {

		clearScreen();
		menu_greet_Customer();
		menu_show_menu();



		customerChoice = getCharFromConsole("Please choose one option from above!");
		printf("\n");
		printf("\n");
		// showing availability of cars 
		// showing the menu to the customer 
		clearScreen();
		switch (customerChoice)
		{
		case OPTION_BUY_A_CAR:
			menu_buy_cars();
			break;

		case OPTION_VIEW_CAR_SALES_STATISTICS:
			menu_view_sales();
			break;

		case OPTION_VIEW_SALES_BETWEEN:
			menu_viewSalesBetween();
			break;

		case OPTION_SHOW_CAR_TYPES:
			menu_show_car_types();
			break;

		case OPTION_SHOW_EMPLOYEE_DETAILS:
			menu_option_employee_details();
			break;

		case OPTION_EXIT_CAR_STORE:
			menu_exit();
			break;
		}
		pauseProgram(customerChoice);

	} while (customerChoice != OPTION_EXIT_CAR_STORE);


	clearScreen();
	printf("\n\nHave a good day!");
}


#include <QApplication>
#include <QPushButton>
#include <QVBoxLayout>
#include <QWidget>
#include <QTextEdit>
#include <QInputDialog>
#include <QString>
#include <QMessageBox>
#include <QTimer>
#include <thread>
#include <chrono>
#include <vector>
#include <string>
#include <iostream>
#include <QProcess>
#include <QDesktopServices>
#include <QUrl>

using namespace std;

int quantity[] = { 23, 16, 10, 14, 20, 18, 28, 12, 14, 20, 13, 12, 16, 25, 14, 23, 30, 24, 18, 24, 27, 32, 19, 25, 50, 17, 10, 40, 29, 15, 17, 23, 37, 14, 23, 23, 15, 17, 14, 17, 15, 23, 26, 18, 24, 23, 14, 18, 20, 17, 19, 27, 12, 24, 14, 10, 17, 40, 15, 19, 15, 20, 12, 17,23 ,18, 22, 17, 12, 10, 24, 23, 11 };
int prices[] = { 200, 350, 500, 700, 400, 280, 300, 400, 100, 250, 150, 200, 400, 350, 500, 550, 270, 500, 600, 450, 700, 400, 300, 280, 70, 700, 400, 150, 450, 200, 1100, 1500, 2500, 1100, 300, 560, 1900, 700, 2400, 1000, 800, 1500, 600, 1100, 600, 1400, 1900, 2300, 800, 670, 1300, 400, 450, 500, 1200, 1400, 680, 300, 250, 550, 700, 450, 500, 1300, 1700, 2000, 1350, 900, 1400, 1000, 1600, 1900, 1200 };
string dishes[] = { "Pasteries......", "Bread..........", "Tiramisu.......", "Fritata........", "Nan_Chana......", "Halva_Puri.....", "Paye...........", "Haleem.........", "Bun............", "Porridge.......", "Soup...........", "Noodles........", "Tangulu........", "Tacos..........", "Quesadillas....", "Churos.........", "Beans..........", "Arroz..........", "Pastrima.......", "Baklava........", "Sucuk..........", "Ajvar..........", "Menemen........", "Biryani........", "Kebab..........", "Karahi.........", "Rogan_Josh.....", "Dal_Tadka......", "Pide...........", "Manti..........", "Imam_Bayidi....", "Pilav..........", "Corba..........", "Pozole.........", "Mole...........", "Tamales........", "Carnitas.......", "Chiles_Rellenos", "Bruschetta.....", "Lasagna........", "Cacio_e_Pepe...", "Pesto..........", "Risotto........", "Pate...........", "Salade-Nicoise.", "Cassoulet......", "Duck-Confit....", "Sole-Meuniere..", "Nihari.........", "Paye...........", "Keema..........", "Palak-Paneer...", "Pulao..........", "Ezme...........", "Mercimek-Corbasi", "Tarhana-Corbasi.", "Lahmacun........", "Roasted-vegetables", "Caldo-de-Pollo....", "Guacamole......", "Elote..........", "Rice-and-beans..", "Sopes..........", "Fettuchini-Alfredo", "Ravioli..........", "Penne-Arrabiata", "Gnocchi..........", "Osso-Buco.........", "Salad-Verte.......", "Coq-au-Vin........", "Ratatouille.......", "Magret-de-Canard", "Quiche-Lorraine" };

// Add these global variables for order functionality
int selected_indices[73];
int quantities[73];
int selection_count = 0;
int sum1 = 0;
int check1 = 0;
int total = 0;
int coupon_codes[20] = {1234, 5678, 4321, 8765, 3456, 1235, 6789, 0000, 1111, 2222,
                        3333, 4444, 5555, 6666, 7777, 8888, 9999, 4545, 6767, 3232};

int Quan_drinks[] = {50, 30, 100, 70};  // Initial quantity for drinks
int Dprice[] = {80, 200, 150, 200};    // Price for drinks
QString drinks[] = {"tea", "coffee", "water_bottle", "cold_drinks"};

int sum2 = 0;
int choice2, quantity2;
int check2 = 0;
int selected_indices2[10];
int quantities2[10];
int selection_count2 = 0;

void display_result(QTextEdit* textEdit, int check1, int check2) {
    int waitTime = 0;

    if (check1 == 0 && check2 >= 1)
        waitTime = 30;
    else if ((check1 + check2) >= 1 && (check1 + check2) <= 5)
        waitTime = 45;
    else if ((check1 + check2) >= 6 && (check1 + check2) <= 10)
        waitTime = 60;
    else if ((check1 + check2) >= 11 && (check1 + check2) <= 20)
        waitTime = 70;
    else if ((check1 + check2) >= 20 && (check1 + check2) <= 40)
        waitTime = 100;
    else if ((check1 + check2) >= 40 && (check1 + check2) <= 70)
        waitTime = 120;
    else
        waitTime = 180;

    textEdit->append(QString("<p><strong>Your order will be ready in %1 minutes...</strong></p>").arg(waitTime));

    // Simulate waiting using QTimer
    QTimer::singleShot(waitTime * 1000, [=]() {
        textEdit->append("<p><strong>Your order is READY!</strong></p>");
    });
}

void bill(QTextEdit* textEdit) {
    total = sum1 + sum2;

    // Display the ordered items
    textEdit->clear();
    textEdit->append("<h3>Product | Quantity | Price</h3>");
    for (int i = 0; i < selection_count; i++) {
        int product = selected_indices[i];
        textEdit->append(QString("%1 | %2 | %3")
                             .arg(QString::fromStdString(dishes[product]))
                             .arg(quantities[i])
                             .arg(prices[product]));
    }

    for (int i = 0; i < selection_count2; i++) {
        int product = selected_indices2[i];
        textEdit->append(QString("%1 | %2 | %3")
                             .arg(drinks[product])
                             .arg(quantities2[i])
                             .arg(Dprice[product]));
    }

    // Calculate total and apply initial discount if applicable
    textEdit->append(QString("<p><strong>Your Total Bill: %1</strong></p>").arg(total));
    if (total > 5000) {
        total -= total * 0.05;  // 5% discount
        textEdit->append(QString("<p><strong>Discount applied! New Total: %1</strong></p>").arg(total));
    }

    // Ask if the user has a coupon
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(nullptr, "Coupon Code", "Do you have a coupon code?",
                                  QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        // Check for coupon
        bool ok;
        int coupon = QInputDialog::getInt(nullptr, "Coupon Code", "Enter a coupon code:", 0, 0, 9999, 1, &ok);
        if (ok) {
            bool validCoupon = false;
            for (int i = 0; i < 20; i++) {
                if (coupon == coupon_codes[i]) {
                    total -= total * 0.05;  // Another 5% discount
                    validCoupon = true;
                    break;
                }
            }
            if (validCoupon) {
                textEdit->append(QString("<p><strong>Coupon Applied! Final Total: %1</strong></p>").arg(total));
            } else {
                QMessageBox::warning(nullptr, "Invalid Coupon", "The coupon code entered is invalid.");
            }
        }
    }

    // Ask for payment method with Cash or Card buttons
    QMessageBox paymentBox;
    paymentBox.setWindowTitle("Payment Method");
    paymentBox.setText("Choose your payment method:");
    QPushButton* cashButton = paymentBox.addButton("Cash", QMessageBox::AcceptRole);
    QPushButton* cardButton = paymentBox.addButton("Card", QMessageBox::RejectRole);

    paymentBox.exec();

    QString paymentMethodText;
    double taxRate;

    if (paymentBox.clickedButton() == cashButton) {
        paymentMethodText = "Cash";
        taxRate = 0.1; // 10% tax
    } else if (paymentBox.clickedButton() == cardButton) {
        paymentMethodText = "Card";
        taxRate = 0.12; // 12% tax
    } else {
        // Default to Cash if no button is selected (failsafe)
        paymentMethodText = "Cash";
        taxRate = 0.1;
    }

    double tax = total * taxRate;
    total += tax;

    // Append tax details and final total
    textEdit->append(QString("<p><strong>Payment Method: %1</strong></p>").arg(paymentMethodText));
    textEdit->append(QString("<p><strong>Tax (%1%): %2</strong></p>")
                         .arg(taxRate * 100)
                         .arg(tax));
    textEdit->append(QString("<p><strong>Total after Tax: %1</strong></p>").arg(total));

    // Display final result
    display_result(textEdit, check1, check2);
}


void drinks_tea(QTextEdit* textEdit) {
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(nullptr, "Order Drinks",
                                  "Do you want to order drinks?",
                                  QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::No) {
        return;  // Exit if the user doesn't want to order drinks
    }

    QString menuText;
    menuText.append("<h2>Drinks/Tea</h2>");
    menuText.append("<table border='0' cellspacing='0' cellpadding='5' style='width:100%;'>");

    for (int i = 0; i < 4; i++) {
        menuText.append(QString("<tr>"
                                "<td style='width:5%;'>%1.</td>"
                                "<td style='width:70%;'>%2</td>"
                                "<td style='width:25%; text-align:right;'>Price: %3</td>"
                                "</tr>")
                            .arg(i + 1)
                            .arg(drinks[i])
                            .arg(Dprice[i]));
    }
    menuText.append("</table>");

    textEdit->setHtml(menuText);

    bool ok = false;
    do {
        int choice2 = QInputDialog::getInt(nullptr, "Order Drink",
                                           "Enter the index of the drink you want (1-4):\nEnter 0 to exit",
                                           1, 0, 4, 1, &ok);
        if (!ok || choice2 == 0)
            break;

        int quantity2 = QInputDialog::getInt(nullptr, "Order Quantity",
                                             "Enter the quantity you want:",
                                             1, 1, 100, 1, &ok);
        if (!ok)
            break;

        if (quantity2 > Quan_drinks[choice2 - 1]) {
            QMessageBox::warning(nullptr, "Insufficient Quantity", "The quantity you want is not available.");
            continue;
        }

        sum2 += Dprice[choice2 - 1] * quantity2;
        check2 += quantity2;

        selected_indices2[selection_count2] = choice2 - 1;
        quantities2[selection_count2] = quantity2;
        ++selection_count2;

        Quan_drinks[choice2 - 1] -= quantity2;

        // Ask if the user wants to order more drinks
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(nullptr, "Order More Drinks",
                                      "Do you want to order more drinks?",
                                      QMessageBox::Yes | QMessageBox::No);
        if (reply == QMessageBox::No)
            break;

    } while (true);

    textEdit->append(QString("<h3>Your Total Bill for Drinks: %1</h3>").arg(sum2));
}



void order(QTextEdit* textEdit) {
    selection_count = 0;  // Reset selection count
    sum1 = 0;             // Reset total sum
    check1 = 0;           // Reset item count

    bool ok = false;
    do {
        // Get the index of the product
        int choice1 = QInputDialog::getInt(nullptr, "Order Product",
                                           "Enter the index of the product you want (1-73):\nEnter 0 to exit",
                                           1, 0, 73, 1, &ok);
        if (!ok || choice1 == 0)
            break;

        // Get the quantity of the product
        int quantity1 = QInputDialog::getInt(nullptr, "Order Quantity",
                                             "Enter the quantity you want:",
                                             1, 1, 100, 1, &ok);
        if (!ok)
            break;

        if (quantity1 > quantity[choice1 - 1]) {
            QMessageBox::warning(nullptr, "Insufficient Quantity", "The quantity you want is not available.");
            continue;
        }

        // Update the order details
        sum1 += prices[choice1 - 1] * quantity1;
        check1 += quantity1;
        selected_indices[selection_count] = choice1 - 1;
        quantities[selection_count] = quantity1;
        ++selection_count;

        // Deduct from available quantity
        quantity[choice1 - 1] -= quantity1;

        // Ask if the user wants to order more
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(nullptr, "Order More",
                                      "Do you want to order more items?",
                                      QMessageBox::Yes | QMessageBox::No);
        if (reply == QMessageBox::No)
            break;

    } while (true);

    // Display the total bill
    QString summary;
    summary.append("<h3>Your Order Summary</h3>");
    for (int i = 0; i < selection_count; ++i) {
        summary.append(QString("<p>%1 x %2 (Price: %3 each)</p>")
                           .arg(QString::fromStdString(dishes[selected_indices[i]]))
                           .arg(quantities[i])
                           .arg(prices[selected_indices[i]]));
    }
    summary.append(QString("<p><strong>Total Bill:</strong> %1</p>").arg(sum1));

    textEdit->setHtml(summary);
}


void menu_for_customer(QTextEdit* textEdit)
{
    QString menuText;

    // BREAKFAST SECTION
    menuText.append("<h2>BREAKFAST</h2>");
    menuText.append("<table border='0' cellspacing='0' cellpadding='5' style='width:100%;'>");
    for (int i = 0; i < 23; i++)
    {
        menuText.append(QString("<tr>"
                                "<td style='width:5%;'>%1.</td>"
                                "<td style='width:70%;'>%2</td>"
                                "<td style='width:25%; text-align:right;'>Price: %3</td>"
                                "</tr>")
                            .arg(i + 1)
                            .arg(QString::fromStdString(dishes[i]))
                            .arg(prices[i]));
    }
    menuText.append("</table>");

    // LUNCH SECTION
    menuText.append("<h2>LUNCH</h2>");
    menuText.append("<table border='0' cellspacing='0' cellpadding='5' style='width:100%;'>");
    for (int i = 23; i < 48; i++)
    {
        menuText.append(QString("<tr>"
                                "<td style='width:5%;'>%1.</td>"
                                "<td style='width:70%;'>%2</td>"
                                "<td style='width:25%; text-align:right;'>Price: %3</td>"
                                "</tr>")
                            .arg(i + 1)
                            .arg(QString::fromStdString(dishes[i]))
                            .arg(prices[i]));
    }
    menuText.append("</table>");

    // DINNER SECTION
    menuText.append("<h2>DINNER</h2>");
    menuText.append("<table border='0' cellspacing='0' cellpadding='5' style='width:100%;'>");
    for (int i = 48; i < 73; i++)
    {
        menuText.append(QString("<tr>"
                                "<td style='width:5%;'>%1.</td>"
                                "<td style='width:70%;'>%2</td>"
                                "<td style='width:25%; text-align:right;'>Price: %3</td>"
                                "</tr>")
                            .arg(i + 1)
                            .arg(QString::fromStdString(dishes[i]))
                            .arg(prices[i]));
    }
    menuText.append("</table>");

    textEdit->setHtml(menuText);
}


void menu_for_admin(QTextEdit* textEdit)
{
    QString menuText;

    menuText.append("<h2>BREAKFAST</h2>");
    menuText.append("<table border='0' cellspacing='0' cellpadding='5' style='width:100%;'>");
    for (int i = 0; i < 23; i++)
    {
        menuText.append(QString("<tr>"
                                "<td style='width:5%;'>%1.</td>"
                                "<td style='width:50%;'>%2</td>"
                                "<td style='width:20%; text-align:center;'>Quantity: %3</td>"
                                "<td style='width:25%; text-align:right;'>Price: %4</td>"
                                "</tr>")
                            .arg(i + 1)
                            .arg(QString::fromStdString(dishes[i]))
                            .arg(quantity[i])
                            .arg(prices[i]));
    }
    menuText.append("</table>");

    menuText.append("<h2>LUNCH</h2>");
    menuText.append("<table border='0' cellspacing='0' cellpadding='5' style='width:100%;'>");
    for (int i = 23; i < 48; i++)
    {
        menuText.append(QString("<tr>"
                                "<td style='width:5%;'>%1.</td>"
                                "<td style='width:50%;'>%2</td>"
                                "<td style='width:20%; text-align:center;'>Quantity: %3</td>"
                                "<td style='width:25%; text-align:right;'>Price: %4</td>"
                                "</tr>")
                            .arg(i + 1)
                            .arg(QString::fromStdString(dishes[i]))
                            .arg(quantity[i])
                            .arg(prices[i]));
    }
    menuText.append("</table>");

    menuText.append("<h2>DINNER</h2>");
    menuText.append("<table border='0' cellspacing='0' cellpadding='5' style='width:100%;'>");
    for (int i = 48; i < 73; i++)
    {
        menuText.append(QString("<tr>"
                                "<td style='width:5%;'>%1.</td>"
                                "<td style='width:50%;'>%2</td>"
                                "<td style='width:20%; text-align:center;'>Quantity: %3</td>"
                                "<td style='width:25%; text-align:right;'>Price: %4</td>"
                                "</tr>")
                            .arg(i + 1)
                            .arg(QString::fromStdString(dishes[i]))
                            .arg(quantity[i])
                            .arg(prices[i]));
    }
    menuText.append("</table>");

    textEdit->setHtml(menuText);
}



void change_Quantity(QTextEdit* textEdit)
{
    bool ok;
    int index = QInputDialog::getInt(nullptr, "Change Quantity", "Enter the index (1-73) to change quantity:", 1, 1, 73, 1, &ok);
    if (ok)
    {
        int newQuantity = QInputDialog::getInt(nullptr, "New Quantity", "Enter the new quantity:", 1, 1, 100, 1, &ok);
        if (ok)
        {
            quantity[index - 1] = newQuantity;
            menu_for_admin(textEdit);
        }
    }
}

void change_price(QTextEdit* textEdit)
{
    bool ok;
    int index = QInputDialog::getInt(nullptr, "Change Price", "Enter the index (1-73) to change price:", 1, 1, 73, 1, &ok);
    if (ok)
    {
        int newPrice = QInputDialog::getInt(nullptr, "New Price", "Enter the new price:", 1, 1, 10000, 1, &ok);
        if (ok)
        {
            prices[index - 1] = newPrice;
            menu_for_admin(textEdit);
        }
    }
}

void confirmation(QTextEdit* textEdit, int& sum1, int& check1) {
    // Create a confirmation message box
    QMessageBox confirmationBox;
    confirmationBox.setWindowTitle("Confirm Order");
    confirmationBox.setText("ORDER CONFIRMATION.");

    QAbstractButton* confirmButton = confirmationBox.addButton("Confirm", QMessageBox::YesRole);
    QAbstractButton* deleteButton = confirmationBox.addButton("Delete", QMessageBox::NoRole);

    confirmationBox.exec();

    if (confirmationBox.clickedButton() == confirmButton) {
        textEdit->append("<h3>Your order has been confirmed. Total Bill: " + QString::number(sum1 + sum2) + "</h3>");

    } else if (confirmationBox.clickedButton() == deleteButton) {
        // Reset the order details
        sum1 = 0;
        check1 = 0;
        textEdit->append("<h3>Your order has been deleted.</h3>");
    }
}

// Helper function to change button color
void changeButtonColor(QPushButton* button, QList<QPushButton*> buttons)
{
    for (QPushButton* btn : buttons) {
        btn->setStyleSheet("background-color: white; color: black;");  // Reset all buttons to white
    }
    button->setStyleSheet("background-color: purple; color: white;");  // Change the clicked button to purple
}

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);

    // Main window setup
    QWidget window;
    window.setWindowTitle("Restaurant Management System");
    window.resize(800, 600);
    window.setStyleSheet("background-color: #C8A2C8;");  // Light purple background

    // Main layout
    QVBoxLayout* layout = new QVBoxLayout;

    // Buttons and Widgets
    QPushButton* adminButton = new QPushButton("Admin Mode");
    QPushButton* customerButton = new QPushButton("Customer Mode");

    QTextEdit* textEdit = new QTextEdit;
    textEdit->setReadOnly(true);

    QPushButton* seeMenuAdminButton = new QPushButton("See Menu");
    QPushButton* exitMenuAdminButton = new QPushButton("Exit Menu");
    QPushButton* changeQuantityButton = new QPushButton("Change Quantity");
    QPushButton* changePriceButton = new QPushButton("Change Price");

    QPushButton* seeMenuCustomerButton = new QPushButton("See Menu");
    QPushButton* exitMenuCustomerButton = new QPushButton("Exit Menu");
    QPushButton* placeOrderButton = new QPushButton("Place Order");
    QPushButton* drinksteaButton = new QPushButton("Drinks/Tea");
    QPushButton* confirmOrderButton = new QPushButton("Confirm Order");
    QPushButton* billButton = new QPushButton("View Bill");

    // Feedback section
    QTextEdit* feedbackTextEdit = new QTextEdit;
    feedbackTextEdit->setPlaceholderText("Enter your feedback here...");
    QPushButton* feedbackButton = new QPushButton("Send Feedback");

    // Add widgets to the layout
    layout->addWidget(adminButton);
    layout->addWidget(customerButton);
    layout->addWidget(textEdit);
    layout->addWidget(seeMenuAdminButton);
    layout->addWidget(exitMenuAdminButton);
    layout->addWidget(changeQuantityButton);
    layout->addWidget(changePriceButton);
    layout->addWidget(seeMenuCustomerButton);
    layout->addWidget(exitMenuCustomerButton);
    layout->addWidget(placeOrderButton);
    layout->addWidget(drinksteaButton);
    layout->addWidget(confirmOrderButton);
    layout->addWidget(billButton);

    // Add feedback section
    layout->addWidget(feedbackTextEdit);
    layout->addWidget(feedbackButton);

    // Initially hide admin/customer menu buttons
    QList<QPushButton*> buttons = { adminButton, customerButton, seeMenuAdminButton, exitMenuAdminButton,
                                    changeQuantityButton, changePriceButton, seeMenuCustomerButton,
                                    exitMenuCustomerButton, placeOrderButton, drinksteaButton,
                                    confirmOrderButton, billButton, feedbackButton };

    for (QPushButton* btn : buttons) {
        btn->setStyleSheet("background-color: white; color: black;");  // Reset colors
    }

    seeMenuAdminButton->setVisible(false);
    exitMenuAdminButton->setVisible(false);
    changeQuantityButton->setVisible(false);
    changePriceButton->setVisible(false);
    seeMenuCustomerButton->setVisible(false);
    exitMenuCustomerButton->setVisible(false);
    placeOrderButton->setVisible(false);
    drinksteaButton->setVisible(false);
    confirmOrderButton->setVisible(false);
    billButton->setVisible(false);
    feedbackTextEdit->setVisible(false);  // Initially hide feedback section
    feedbackButton->setVisible(false);   // Initially hide feedback button

    QObject::connect(adminButton, &QPushButton::clicked, [&]() {
        // Ask for the admin password
        bool ok;
        QString password = QInputDialog::getText(nullptr, "Admin Login",
                                                 "Enter Admin Password:",
                                                 QLineEdit::Password, "", &ok);

        if (ok && password == "admin123") {  // Replace "admin123" with your actual password
            // Password is correct, proceed to admin mode
            changeButtonColor(adminButton, { adminButton, customerButton });

            // Show admin menu options
            seeMenuAdminButton->setVisible(true);
            exitMenuAdminButton->setVisible(true);
            changeQuantityButton->setVisible(true);
            changePriceButton->setVisible(true);

            adminButton->setEnabled(false);
            customerButton->setEnabled(false);
        } else if (ok) {
            // Password is incorrect, show an error message
            QMessageBox::warning(nullptr, "Invalid Password", "The password you entered is incorrect.");
        }
    });


    // Connect Customer Mode button click to change color
    QObject::connect(customerButton, &QPushButton::clicked, [&]() {
        // Call color change function to highlight the selected button
        changeButtonColor(customerButton, { adminButton, customerButton });

        // Your customer mode code here
        seeMenuCustomerButton->setVisible(true);
        exitMenuCustomerButton->setVisible(true);
        placeOrderButton->setVisible(true);
        drinksteaButton->setVisible(true);
        confirmOrderButton->setVisible(true);
        billButton->setVisible(true);
        adminButton->setEnabled(false);
        customerButton->setEnabled(false);
    });

    // Connect See Menu Admin button click to change color
    QObject::connect(seeMenuAdminButton, &QPushButton::clicked, [&]() {
        // Call color change function to highlight the selected button
        changeButtonColor(seeMenuAdminButton, { seeMenuAdminButton, exitMenuAdminButton, changeQuantityButton, changePriceButton });

        // Call admin menu function
        menu_for_admin(textEdit);  // Placeholder function
    });

    // Connect Exit Menu Admin button click to change color
    QObject::connect(exitMenuAdminButton, &QPushButton::clicked, [&]() {
        // Call color change function to highlight the selected button
        changeButtonColor(exitMenuAdminButton, { seeMenuAdminButton, exitMenuAdminButton, changeQuantityButton, changePriceButton });

        // Hide admin menu options
        textEdit->clear();
        seeMenuAdminButton->setVisible(false);
        exitMenuAdminButton->setVisible(false);
        changeQuantityButton->setVisible(false);
        changePriceButton->setVisible(false);
        adminButton->setEnabled(true);
        customerButton->setEnabled(true);
    });

    // Connect Change Quantity button click to change color
    QObject::connect(changeQuantityButton, &QPushButton::clicked, [&]() {
        // Call color change function to highlight the selected button
        changeButtonColor(changeQuantityButton, { seeMenuAdminButton, exitMenuAdminButton, changeQuantityButton, changePriceButton });

        // Placeholder function to change quantity
        change_Quantity(textEdit);  // Placeholder function
    });

    // Connect Change Price button click to change color
    QObject::connect(changePriceButton, &QPushButton::clicked, [&]() {
        // Call color change function to highlight the selected button
        changeButtonColor(changePriceButton, { seeMenuAdminButton, exitMenuAdminButton, changeQuantityButton, changePriceButton });

        // Placeholder function to change price
        change_price(textEdit);  // Placeholder function
    });

    // Connect See Menu Customer button click to change color
    QObject::connect(seeMenuCustomerButton, &QPushButton::clicked, [&]() {
        // Call color change function to highlight the selected button
        changeButtonColor(seeMenuCustomerButton, { seeMenuCustomerButton, exitMenuCustomerButton, placeOrderButton, drinksteaButton, confirmOrderButton, billButton });

        // Call customer menu function
        menu_for_customer(textEdit);  // Placeholder function
    });

    // Connect Exit Menu Customer button click to change color
    QObject::connect(exitMenuCustomerButton, &QPushButton::clicked, [&]() {
        // Call color change function to highlight the selected button
        changeButtonColor(exitMenuCustomerButton, { seeMenuCustomerButton, exitMenuCustomerButton, placeOrderButton, drinksteaButton, confirmOrderButton, billButton });

        // Hide customer menu options
        textEdit->clear();
        seeMenuCustomerButton->setVisible(false);
        exitMenuCustomerButton->setVisible(false);
        placeOrderButton->setVisible(false);
        drinksteaButton->setVisible(false);
        confirmOrderButton->setVisible(false);
        billButton->setVisible(false);
        adminButton->setEnabled(true);
        customerButton->setEnabled(true);
    });

    // Connect Place Order button click to change color
    QObject::connect(placeOrderButton, &QPushButton::clicked, [&]() {
        // Call color change function to highlight the selected button
        changeButtonColor(placeOrderButton, { seeMenuCustomerButton, exitMenuCustomerButton, placeOrderButton, drinksteaButton, confirmOrderButton, billButton });

        // Placeholder function to place order
        order(textEdit);  // Placeholder function
        feedbackTextEdit->setVisible(true);  // Show feedback section after order
        feedbackButton->setVisible(true);
    });

    // Connect Drinks/Tea button click to change color
    QObject::connect(drinksteaButton, &QPushButton::clicked, [&]() {
        // Call color change function to highlight the selected button
        changeButtonColor(drinksteaButton, { seeMenuCustomerButton, exitMenuCustomerButton, placeOrderButton, drinksteaButton, confirmOrderButton, billButton });

        // Placeholder function for drinks/tea menu
        drinks_tea(textEdit);  // Placeholder function
    });

    // Connect Confirm Order button click to change color
    QObject::connect(confirmOrderButton, &QPushButton::clicked, [&]() {
        // Call color change function to highlight the selected button
        changeButtonColor(confirmOrderButton, { seeMenuCustomerButton, exitMenuCustomerButton, placeOrderButton, drinksteaButton, confirmOrderButton, billButton });

        // Placeholder function for confirming order
        confirmation(textEdit, sum1, check1);  // Placeholder function
    });

    // Connect Bill button click to change color
    QObject::connect(billButton, &QPushButton::clicked, [&]() {
        // Call color change function to highlight the selected button
        changeButtonColor(billButton, { seeMenuCustomerButton, exitMenuCustomerButton, placeOrderButton, drinksteaButton, confirmOrderButton, billButton });

        // Placeholder function to show bill
        bill(textEdit);  // Placeholder function
    });

    // Connect Feedback button click to change color
    QObject::connect(feedbackButton, &QPushButton::clicked, [&]() {
        // Call color change function to highlight the selected button
        changeButtonColor(feedbackButton, { feedbackButton });

        QString feedback = feedbackTextEdit->toPlainText();
        if (!feedback.isEmpty()) {
            // Encode the feedback to handle special characters
            QString encodedFeedback = QUrl::toPercentEncoding(feedback);

            // Construct the mailto URL
            QString mailto = "mailto:thetasker45@gmail.com?subject=User Feedback&body=" + encodedFeedback;

            // Open the default email client using QDesktopServices
            if (!QDesktopServices::openUrl(QUrl(mailto))) {
                QMessageBox::warning(&window, "Error", "Failed to open mail client.");
            }
        } else {
            QMessageBox::warning(&window, "No Feedback", "Please enter your feedback before sending.");
        }
    });

    window.setLayout(layout);
    window.show();

    return app.exec();
}

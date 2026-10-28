#include <iostream>
#include <string>
using namespace std;

class Book {
private:
    int id;
    string title;
    string author;
    bool issued;
    static int count;

public:
    Book() {
        id = 0;
        title = "";
        author = "";
        issued = false;
        count++;
    }

    void input() {
        cout << "Enter Book ID: ";
        cin >> id;
        cin.ignore();

        cout << "Enter Title: ";
        getline(cin, title);

        cout << "Enter Author: ";
        getline(cin, author);

        if (id <= 0) {
            cout << "Invalid ID! ID must be positive.\n";
            id = 0;
        }
    }

    void setIssued(bool status) {
        issued = status;
    }

    int getId() {
        return id;
    }

    bool isIssued() {
        return issued;
    }

    static void showCount() {
        cout << "Total book objects: " << count << endl;
    }

    friend void showBook(const Book& b);

    virtual float calculateFine(int daysLate) {
        return 0;
    }

    virtual void display() {
        cout << "Book ID: " << id << endl;
        cout << "Title: " << title << endl;
        cout << "Author: " << author << endl;
    }

    virtual ~Book() {
        count--;
    }
};

int Book::count = 0;

void showBook(const Book& b) {
    cout << "Title: " << b.title << endl;
    cout << "Author: " << b.author << endl;
}

class LibraryBook : public Book {
public:
    float calculateFine(int daysLate) override {
        if (daysLate <= 0)
            return 0;
        return daysLate * 2.0f;   // Rs. 2 per late day
    }

    void display() override {
        Book::display();

        if (isIssued())
            cout << "Status: Issued\n";
        else
            cout << "Status: Available\n";
    }
};

int main() {
    const int MAX = 100;
    Book* books[MAX];
    int total = 0;
    int choice;

    do {
        cout << "\n===== LIBRARY MANAGEMENT SYSTEM =====\n";
        cout << "1. Add Book\n";
        cout << "2. Display Books\n";
        cout << "3. Search Book\n";
        cout << "4. Issue Book\n";
        cout << "5. Return Book\n";
        cout << "6. Delete Book\n";
        cout << "7. Show Total Objects\n";
        cout << "0. Exit\n";
        cout << "Enter choice: ";
        cin >> choice;

        switch (choice) {
            case 1: {
                if (total == MAX) {
                    cout << "Library is full!\n";
                    break;
                }

                Book* b = new LibraryBook();
                b->input();

                bool duplicate = false;

                for (int i = 0; i < total; i++) {
                    if (books[i]->getId() == b->getId()) {
                        duplicate = true;
                        break;
                    }
                }

                if (duplicate) {
                    cout << "Book ID already exists!\n";
                    delete b;
                } else {
                    books[total] = b;
                    total++;
                    cout << "Book added successfully!\n";
                }
                break;
            }

            case 2:
                if (total == 0) {
                    cout << "No book records found!\n";
                }

                for (int i = 0; i < total; i++) {
                    cout << "\nBook " << i + 1 << endl;
                    books[i]->display();
                }
                break;

            case 3: {
                int id, found = -1;
                cout << "Enter Book ID to search: ";
                cin >> id;

                for (int i = 0; i < total; i++) {
                    if (books[i]->getId() == id) {
                        found = i;
                        break;
                    }
                }

                if (found == -1)
                    cout << "Book not found!\n";
                else
                    books[found]->display();

                break;
            }

            case 4: {
                int id, found = -1;
                cout << "Enter Book ID to issue: ";
                cin >> id;

                for (int i = 0; i < total; i++) {
                    if (books[i]->getId() == id) {
                        found = i;
                        break;
                    }
                }

                if (found == -1) {
                    cout << "Book not found!\n";
                } else if (books[found]->isIssued()) {
                    cout << "Book is already issued!\n";
                } else {
                    books[found]->setIssued(true);
                    cout << "Book issued successfully!\n";
                    showBook(*books[found]);
                }
                break;
            }

            case 5: {
                int id, found = -1, daysLate;
                cout << "Enter Book ID to return: ";
                cin >> id;

                for (int i = 0; i < total; i++) {
                    if (books[i]->getId() == id) {
                        found = i;
                        break;
                    }
                }

                if (found == -1) {
                    cout << "Book not found!\n";
                } else if (!books[found]->isIssued()) {
                    cout << "This book was not issued!\n";
                } else {
                    cout << "Enter number of days late (0 if on time): ";
                    cin >> daysLate;

                    books[found]->setIssued(false);
                    cout << "Book returned successfully!\n";
                    cout << "Fine: Rs. " << books[found]->calculateFine(daysLate) << endl;
                }
                break;
            }

            case 6: {
                int id, found = -1;
                cout << "Enter Book ID to delete: ";
                cin >> id;

                for (int i = 0; i < total; i++) {
                    if (books[i]->getId() == id) {
                        found = i;
                        break;
                    }
                }

                if (found == -1) {
                    cout << "Book not found!\n";
                } else {
                    delete books[found];

                    for (int i = found; i < total - 1; i++) {
                        books[i] = books[i + 1];
                    }

                    total--;
                    cout << "Book object deleted successfully!\n";
                }
                break;
            }

            case 7:
                Book::showCount();
                break;

            case 0:
                cout << "Exiting program...\n";
                break;

            default:
                cout << "Invalid choice!\n";
        }

    } while (choice != 0);

    for (int i = 0; i < total; i++) {
        delete books[i];
    }

    return 0;
}
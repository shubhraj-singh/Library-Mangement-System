#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Book {
    int id;
    char title[100];
    char author[100];
    int quantity;
};

void addBook();
void displayBooks();
void searchBook();
void deleteBook();

int main() {
    int choice;

    while (1) {
        printf("\n====== Library Management System ======\n");
        printf("1. Add Book\n");
        printf("2. Display All Books\n");
        printf("3. Search Book\n");
        printf("4. Delete Book\n");
        printf("5. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        getchar(); // to consume newline character

        switch (choice) {
            case 1: addBook(); break;
            case 2: displayBooks(); break;
            case 3: searchBook(); break;
            case 4: deleteBook(); break;
            case 5: exit(0);
            default: printf("Invalid choice. Please try again.\n");
        }
    }

    return 0;
}

void addBook() {
    FILE *fp;
    struct Book b;

    fp = fopen("books.dat", "ab");
    if (fp == NULL) {
        printf("Error opening file.\n");
        return;
    }

    printf("Enter Book ID: ");
    scanf("%d", &b.id);
    getchar();
    printf("Enter Book Title: ");
    fgets(b.title, sizeof(b.title), stdin);
    b.title[strcspn(b.title, "\n")] = '\0';  // remove newline
    printf("Enter Author Name: ");
    fgets(b.author, sizeof(b.author), stdin);
    b.author[strcspn(b.author, "\n")] = '\0';  // remove newline
    printf("Enter Quantity: ");
    scanf("%d", &b.quantity);

    fwrite(&b, sizeof(b), 1, fp);
    fclose(fp);

    printf("Book added successfully!\n");
}

void displayBooks() {
    FILE *fp;
    struct Book b;

    fp = fopen("books.dat", "rb");
    if (fp == NULL) {
        printf("No books to display.\n");
        return;
    }

    printf("\nList of Books:\n");
    printf("--------------------------------------------\n");
    while (fread(&b, sizeof(b), 1, fp)) {
        printf("ID: %d\nTitle: %s\nAuthor: %s\nQuantity: %d\n\n",
               b.id, b.title, b.author, b.quantity);
    }
    fclose(fp);
}

void searchBook() {
    FILE *fp;
    struct Book b;
    int id, found = 0;

    printf("Enter Book ID to search: ");
    scanf("%d", &id);

    fp = fopen("books.dat", "rb");
    if (fp == NULL) {
        printf("Error opening file.\n");
        return;
    }

    while (fread(&b, sizeof(b), 1, fp)) {
        if (b.id == id) {
            printf("\nBook Found:\n");
            printf("ID: %d\nTitle: %s\nAuthor: %s\nQuantity: %d\n",
                   b.id, b.title, b.author, b.quantity);
            found = 1;
            break;
        }
    }
    fclose(fp);

    if (!found) {
        printf("Book with ID %d not found.\n", id);
    }
}

void deleteBook() {
    FILE *fp, *temp;
    struct Book b;
    int id, found = 0;

    printf("Enter Book ID to delete: ");
    scanf("%d", &id);

    fp = fopen("books.dat", "rb");
    temp = fopen("temp.dat", "wb");

    if (fp == NULL || temp == NULL) {
        printf("Error opening file.\n");
        return;
    }

    while (fread(&b, sizeof(b), 1, fp)) {
        if (b.id == id) {
            found = 1;
            continue;
        }
        fwrite(&b, sizeof(b), 1, temp);
    }

    fclose(fp);
    fclose(temp);

    remove("books.dat");
    rename("temp.dat", "books.dat");

    if (found)
        printf("Book deleted successfully.\n");
    else
        printf("Book with ID %d not found.\n", id);
}
